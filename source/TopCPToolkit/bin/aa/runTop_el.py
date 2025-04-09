#!/usr/bin/env python

from AthenaConfiguration.AllConfigFlags import initConfigFlags
from AnalysisAlgorithmsConfig.ConfigAccumulator import DataType
from TopCPToolkit import metaConfig
import ROOT
import os, shutil, sys, argparse

def remove_empty_trees(file_path):
    """
    remove empty TTrees (no events passing selection)
    to avoid issues later on with offline processing
    """
    # Open the ROOT file in update mode
    file = ROOT.TFile(file_path, "UPDATE")

    # List of keys to remove
    keys_to_remove = []

    # Loop over all keys in the file
    for key in file.GetListOfKeys():
        obj = key.ReadObj()

        # Check if the object is a TTree and if it has entries
        if isinstance(obj, ROOT.TTree):
            if obj.GetEntries() == 0:
                keys_to_remove.append(key.GetName())

    if len(keys_to_remove) > 0 :
        # Remove empty TTrees
        for key_name in keys_to_remove:
            print(f"Removing empty TTree {key_name}")
            file.Delete(f"{key_name};*")

        # Write changes and close the file
        file.Write()
        file.Close()
    else:
        file.Close()
    return


flags = initConfigFlags()
p = flags.getArgumentParser(description="Run Athena with CP algorithms in local input")

# modify some of the default arguments
for a in p._actions:
    if '--evtMax' in a.option_strings:
        a.option_strings = ['-e','--evtMax','--max-events'] # adds the "-e" and "--max-events" options
        p._option_string_actions['-e'] = a
        p._option_string_actions['--max-events'] = a
    if '--interactive' in a.option_strings:
        a.option_strings = ['--interactive']    # remove the "-i" option
    if '--filesInput' in a.option_strings:
        a.option_strings = ['-i','--filesInput','--input-list'] # adds the "-i" option and '--input-list' option
        p._option_string_actions['-i'] = a
        p._option_string_actions['--input-list'] = a
    if any([x in a.option_strings for x in ['--mtes','--mtes-channel','--nprocs','--threads','--concurrent-events','--CA','--debugWorker']]):
        a.help = argparse.SUPPRESS # hide all these options, b/c not yet supported

g = p._action_groups[2] # should be the main options group, adding to that so that appear in same block in help msg
g.add_argument('-o', '--output-name', required=True,
               help='Name of the output file (without .root)')
g.add_argument('-a', '--analysis', type=str,
               default='TopCPToolkit.TtbarCPalgoConfigBlocksAnalysis',
               help='Name of reco analysis to run. Should be package.moduleName,'
                    'where module implements makeRecoConfiguration method')
g.add_argument('--parton', action='store_true',
               help='Wheter to run parton level')
g.add_argument('--particle', action='store_true',
               help='Whether to run particle level')
g.add_argument('--no-systematics', action='store_true',
               help='Configure the job to run with no systematics.')
g.add_argument('--no-filter', action='store_true',
               help='Skip filtering of events due to event selection (selection flags are still stored.)')
g.add_argument('--no-reco', action='store_true',
               help='Skip running the detector-level analysis. Useful for running on TRUTH derivations.')
g.add_argument('-t', '--text-config', type=str,
               default=None,
               help='Name of the analysis to run. Should be a directory containing reco.yaml,'
                    'and optionally particle.yaml and parton.yaml.')

# changes to default flag values (done before fillFromArgs so appears in the --help flag system
from AthenaCommon import Constants
flags.Exec.OutputLevel = Constants.WARNING
flags.Exec.PrintAlgsSequence = True
flags.Exec.InfoMessageComponents = ["AthenaEventLoopMgr","THistSvc","PerfMonMTSvc","ApplicationMgr"] # don't suppress info from these

args = flags.fillFromArgs(parser=p)

# allow user to specify input file as a txt file that contains a list of ds files
# if any txt files given as input, open them and get filenames
files = []
for file in flags.Input.Files:
    if file.endswith(".txt"):
        with open(file, 'r') as _files:
            for f in _files:
                files.append(f.strip())
    else:
        files.append(file)
flags.Input.Files = files
# read FileMetadata
metaConfig.populate_config_flags(flags)
flags.lock()

metaConfig.pretty_print(flags)

# don't run particle/parton-level analysis on data
if flags.Input.DataType == DataType.Data:
    args.parton = False
    args.particle = False

from pathlib import Path
Path(args.output_name).mkdir(exist_ok=True)
# the final file to produce
finalfile = f'{args.output_name}.root'

from TopCPToolkit.commonAlgoConfig import makeRecoSequence,makeTruthSequence,makeParticleLevelSequence
from functools import partial
jobs = []
if not args.no_reco: jobs += [('reco',partial(makeRecoSequence,noSystematics=args.no_systematics,noFilter=args.no_filter))]
if args.parton: jobs += [('parton',partial(makeTruthSequence,noSystematics=args.no_systematics))]
if args.particle: jobs += [('particle',partial(makeParticleLevelSequence,noSystematics=args.no_systematics,noFilter=args.no_filter))]

from AthenaConfiguration.MainServicesConfig import MainServicesCfg
cfg = MainServicesCfg(flags)
from AthenaPoolCnvSvc.PoolReadConfig import PoolReadCfg
cfg.merge(PoolReadCfg(flags))
from EventBookkeeperTools.EventBookkeeperToolsConfig import CutFlowSvcCfg
cfg.merge(CutFlowSvcCfg(flags))
from AthenaServices.MetaDataSvcConfig import MetaDataSvcCfg
cfg.merge(MetaDataSvcCfg(flags))
cfg.getService("AthenaEventLoopMgr").IntervalInSeconds = 5 # enable processing rate reporting every 5s

files_to_merge = []

for jobLabel,jobSequence in jobs:
    histoFile = f"{args.output_name}/{jobLabel}_histograms.root"
    treeFile =  f"{args.output_name}/{jobLabel}_trees.root"
    pid = os.fork() # even though we are running the subjobs sequentially, must do each in a subprocess
    if pid==0:
        if args.text_config:
            from TopCPToolkit.commonAlgoConfig import makeTextBasedSequence
            cfg.merge( makeTextBasedSequence(args.text_config, jobLabel, flags,
                                       noSystematics=args.no_systematics) )
        else:
            cfg.merge( jobSequence(args.analysis, flags) )
        from AthenaConfiguration.ComponentFactory import CompFactory
        cfg.addService(CompFactory.THistSvc(Output=[ f"ANALYSIS DATAFILE='{histoFile}' OPT='RECREATE'", f"NTUPLES DATAFILE='{treeFile}' OPT='RECREATE'" ]))
        for algo in ["TreeMaker","NTupleMaker","MetNTupleMaker","TreeFiller"]:
            if algo in cfg._algorithms: cfg.getEventAlgo(algo).RootStreamName = "NTUPLES" # redirect ntuple to this file
        from AthenaConfiguration.Utils import setupLoggingLevels
        setupLoggingLevels(flags,cfg) # adjust logging levels according to Exec.XXXMessageComponents flags
        cfg.printConfig() # For debugging
        sys.exit(cfg.run().isFailure())
    else:
        # this is the parent process .. we could parallelize the subjobs but for now we will do sequentially
        # by having the parent wait for the child procress
        status = os.waitid(os.P_PID,pid,os.WEXITED) # parent waits for child
        print(jobLabel,"subjob returned status ",status.si_status)
        if status.si_status != 0:
            sys.exit(status.si_status)
        files_to_merge.insert(0,treeFile)
        remove_empty_trees(treeFile)
        if jobLabel == jobs[0][0]: files_to_merge += [histoFile] # takes just histograms from the first subjob

if len(files_to_merge) > 0:
    # run the hadd operation, but filter out harmless warnings
    tmp_stderr_file = 'hadd_stderr.txt'
    stderr_output = ""
    os.system(f'hadd -f {finalfile} ' + ' '.join(files_to_merge) + f' 2>{tmp_stderr_file}')
    with open(tmp_stderr_file, 'r') as ferr:
        stderr_output = ferr.read()
    filtered_stderr_output = stderr_output.replace("Warning in <TList::Merge>: input list is empty - nothing to merge with", "")
    if filtered_stderr_output.strip():
        print(filtered_stderr_output)
    else:
        print("hadd was successful.")
        os.system('rm ' + ' '.join(files_to_merge))
        os.system(f'rm {tmp_stderr_file}')
        os.system(f'rm -rf {args.output_name}') # delete the output directory

    ##############
    ## METADATA ##
    ##############

    f = ROOT.TFile(f'{finalfile}', "UPDATE")
    h_metadata = ROOT.TH1I('metadata', 'Sample metadata:Look at the bin labels:Number of files in this sample', 4, 0, 4)
    h_metadata.GetXaxis().SetBinLabel( 1, str(flags.Input.DataType.value) )
    if flags.Input.DataType != DataType.Data:
        campaign = str(flags.Input.MCCampaign.value)
    else:
        campaign = str(flags.Input.DataYear)
    h_metadata.GetXaxis().SetBinLabel( 2, campaign )
    h_metadata.GetXaxis().SetBinLabel( 3, str(flags.Input.MCChannelNumber) )
    h_metadata.GetXaxis().SetBinLabel( 4, flags.Input.eTag )
    h_metadata.Write()
    f.Close()
