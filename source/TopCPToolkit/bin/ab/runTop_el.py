#!/usr/bin/env python

from AthenaConfiguration.AllConfigFlags import initConfigFlags
from AnalysisAlgorithmsConfig.ConfigAccumulator import DataType
from argparse import ArgumentParser
from TopCPToolkit import metaConfig, bishop
import ROOT
import os, shutil

def parse_arguments():
    p = ArgumentParser('Run EventLoop with CP algorithms in local input')
    p.add_argument('-i', '--input-list', required=True,
                   help='Path to text file with input DAOD files to process.')
    p.add_argument('-o', '--output-name', required=True,
                   help='Name of the output file (without .root)')
    p.add_argument('-e', '--max-events', type=int, required=False, default=-1,
                   help='Number of events to run over. Set to -1 to run over all events.')
    p.add_argument('-a', '--analysis', type=str,
                   default='TopCPToolkit.TtbarCPalgoConfigBlocksAnalysis',
                   help='Name of reco analysis to run. Should be package.moduleName,'
                   'where module implements makeRecoConfiguration method')
    p.add_argument('--parton', action='store_true',
                   help='Whether to run parton level')
    p.add_argument('--particle', action='store_true',
                   help='Whether to run particle level')
    p.add_argument('--no-systematics', action='store_true',
                   help='Configure the job to run with no systematics.')
    p.add_argument('--no-filter', action='store_true',
                   help='Skip filtering of events due to event selection (selection flags are still stored.)')
    p.add_argument('--no-reco', action='store_true',
                   help='Skip running the detector-level analysis. Useful for running on TRUTH derivations.')
    p.add_argument('--direct-driver', action='store_true',
                   help='Use EL::DirectDriver instead of EL::ExecDriver. Temporary solution to fix some issues on the Grid.')
    p.add_argument('-t', '--text-config', type=str,
                   default=None,
                   help='Name of the analysis to run. Should be a directory containing reco.yaml,'
                   'and optionally particle.yaml and parton.yaml.')
    args = p.parse_args()
    bishop.warn(args)

    return args

def move_with_symlink_handling(inputfile, outputfile):
    """
    this function allows to move a file that may also be
    just a symlink to a hard output (avoids overwriting symlinks)
    """
    real_source = os.path.realpath(inputfile)
    try:
        shutil.move(real_source, outputfile)
    except Exception as e:
        print(f"Error moving file: {e}")
    return

def check_output(file_path):
    """
    check if the given file exists
    """
    if not os.path.exists(file_path):
        raise FileNotFoundError(f"The file '{file_path}' was not successfully created, aborting.")
    else:
        return

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

def run_job(sample_handler, output_stream_name, level_name, args, flags):
    job = ROOT.EL.Job()
    job.sampleHandler(sample_handler)
    job.options().setBool(ROOT.EL.Job.optAlgorithmMemoryMonitor, False)
    job.options().setDouble(ROOT.EL.Job.optMaxEvents, args.max_events)
    job.options().setString(ROOT.EL.Job.optSubmitDirMode, 'overwrite')
    if not args.direct_driver:
        job.options().setString(ROOT.EL.Job.optFactoryPreload, 'libTopCPToolkitDict.so,top::preloadComponentFactories' )
        job.options().setDouble(ROOT.EL.Job.optFilesPerWorker, 10000)
    if args.text_config:
        from TopCPToolkit.commonAlgoConfig import makeTextBasedSequence
        algSeq = makeTextBasedSequence(args.text_config, level_name, flags,
                                       noSystematics=args.no_systematics)
    else:
        if level_name == 'reco':
            from TopCPToolkit.commonAlgoConfig import makeRecoSequence
            algSeq = makeRecoSequence(args.analysis, flags,
                                      noSystematics=args.no_systematics,
                                      noFilter=args.no_filter)
        elif level_name == 'particle':
            from TopCPToolkit.commonAlgoConfig import makeParticleLevelSequence
            algSeq = makeParticleLevelSequence(args.analysis, flags,
                                               noSystematics=args.no_systematics,
                                               noFilter=args.no_filter)
        elif level_name == 'parton':
            from TopCPToolkit.commonAlgoConfig import makeTruthSequence
            algSeq = makeTruthSequence(args.analysis, flags,
                                       noSystematics=args.no_systematics)
    print(algSeq)
    for alg in algSeq:
        job.algsAdd(alg)
    job.outputAdd(ROOT.EL.OutputStream(output_stream_name))
    driver.submit(job, args.output_name)

    return

if __name__ == '__main__':

    args = parse_arguments()

    ROOT.xAOD.Init().ignore()
    sh = ROOT.SH.SampleHandler()
    sh.setMetaString('nc_tree', 'CollectionTree')

    sample_name = 'output' # this thing is used by SampleHandler

    sample = ROOT.SH.SampleLocal(sample_name)
    files = metaConfig.parse_input_filelist(args.input_list)
    for f in files:
        sample.add(f)
    sh.add(sample)

    driver = None
    if args.direct_driver:
        driver = ROOT.EL.DirectDriver()
    else:
        driver = ROOT.EL.ExecDriver()
    driver.options().setBool(ROOT.EL.Job.optGridReporting, True)

    # read FileMetadata
    flags = initConfigFlags()
    flags.Input.Files = files
    metaConfig.populate_config_flags(flags)
    flags.lock()
    metaConfig.pretty_print(flags)

    # don't run particle/parton-level analysis on data
    if flags.Input.DataType == DataType.Data:
        args.parton = False
        args.particle = False

    #output stream name in EventLoop
    outputStreamName = 'ANALYSIS'
    # outfile is the standard output file from EventLoop
    outfile = f'{args.output_name}/data-{outputStreamName}/{sample_name}.root'
    # these are temporary files to merge outputs from the reco, particle-level and parton sequences
    recofile = f'{sample_name}_reco.root'
    partonfile = f'{sample_name}_truth.root'
    particlefile = f'{sample_name}_pl.root'
    histofile = f'{args.output_name}/hist-{sample_name}.root'
    # the final file to keep
    finalfile = f'{args.output_name}.root'

    if not args.direct_driver:
        if not args.no_reco:
            pid = os.fork()
            if pid:
                os.wait()
                check_output(outfile)
                remove_empty_trees(outfile)
                check_output(histofile)
                move_with_symlink_handling(outfile, recofile)
                move_with_symlink_handling(histofile, 'only_histograms.root')
            else:
                run_job(sh, outputStreamName, 'reco', args, flags)
        if args.particle:
            pid = os.fork()
            if pid:
                os.wait()
                check_output(outfile)
                remove_empty_trees(outfile)
                move_with_symlink_handling(outfile, particlefile)
                if args.no_reco:
                    check_output(histofile)
                    move_with_symlink_handling(histofile, 'only_histograms.root')
            else:
                run_job(sh, outputStreamName, 'particle', args, flags)
        if args.parton:
            pid = os.fork()
            if pid:
                os.wait()
                check_output(outfile)
                remove_empty_trees(outfile)
                move_with_symlink_handling(outfile, partonfile)
                if args.no_reco and not args.particle:
                    check_output(histofile)
                    move_with_symlink_handling(histofile, 'only_histograms.root')
            else:
                run_job(sh, outputStreamName, 'parton', args, flags)
    else:
        if not args.no_reco:
            run_job(sh, outputStreamName, 'reco', args, flags)
            check_output(outfile)
            remove_empty_trees(outfile)
            check_output(histofile)
            move_with_symlink_handling(outfile, recofile)
            move_with_symlink_handling(histofile, 'only_histograms.root')
        if args.particle:
            run_job(sh, outputStreamName, 'particle', args, flags)
            check_output(outfile)
            remove_empty_trees(outfile)
            move_with_symlink_handling(outfile, particlefile)
            if args.no_reco:
                check_output(histofile)
                move_with_symlink_handling(histofile, 'only_histograms.root')
        if args.parton:
            run_job(sh, outputStreamName, 'parton', args, flags)
            check_output(outfile)
            remove_empty_trees(outfile)
            move_with_symlink_handling(outfile, partonfile)
            if args.no_reco and not args.particle:
                check_output(histofile)
                move_with_symlink_handling(histofile, 'only_histograms.root')

    files_to_merge = ['only_histograms.root']
    if not args.no_reco:
        files_to_merge.append(f'{recofile}')
    if args.particle:
        files_to_merge.append(f'{particlefile}')
    if args.parton:
        files_to_merge.append(f'{partonfile}')

    # run the hadd operation, but filter out harmless warnings
    tmp_stderr_file = 'hadd_stderr.txt'
    stderr_output = ""
    os.system(f'hadd -f {finalfile} ' + ' '.join(files_to_merge) + f' 2>{tmp_stderr_file}')
    os.system('rm ' + ' '.join(files_to_merge))
    with open(tmp_stderr_file, 'r') as ferr:
        stderr_output = ferr.read()
    filtered_stderr_output = stderr_output.replace("Warning in <TList::Merge>: input list is empty - nothing to merge with", "")
    if filtered_stderr_output.strip():
        print(filtered_stderr_output)
    else:
        print("hadd was successful.")
    os.system(f'rm {tmp_stderr_file}')
    os.system(f'rm -rf {args.output_name}')

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
