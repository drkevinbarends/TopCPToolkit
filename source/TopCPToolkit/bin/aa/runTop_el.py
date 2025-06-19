#!/usr/bin/env python

from AthenaConfiguration.AllConfigFlags import initConfigFlags
from AthenaConfiguration.MainServicesConfig import MainServicesCfg
from AthenaPoolCnvSvc.PoolReadConfig import PoolReadCfg
from AthenaServices.MetaDataSvcConfig import MetaDataSvcCfg
from AthenaCommon import Constants
from AnalysisAlgorithmsConfig.ConfigAccumulator import DataType
from EventBookkeeperTools.EventBookkeeperToolsConfig import CutFlowSvcCfg
from TopCPToolkit import metaConfig
from TopCPToolkit.commonAlgoConfig import (
    makeRecoSequence,
    makeTruthSequence,
    makeParticleLevelSequence,
)
from functools import partial
from pathlib import Path
import ROOT
import os
import sys
import argparse

# Get the TMPDIR from the environment, defaulting to '/tmp' if not set
TMPDIR = os.getenv("TMPDIR", "/tmp")


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

    if len(keys_to_remove) > 0:
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


def copy_systematics_histogram(source_file, target_file):
    # Open the source file in read mode
    src = ROOT.TFile.Open(source_file, "READ")
    if not src or src.IsZombie():
        print(f"Error opening source file: {source_file}")
        return

    # Look for the histogram whose name starts with "listOfSystematics"
    keys = src.GetListOfKeys()
    hist_to_copy = None

    for key in keys:
        obj = key.ReadObj()
        if isinstance(obj, ROOT.TH1) and obj.GetName().startswith("listOfSystematics"):
            hist_to_copy = obj
            break

    if hist_to_copy is None:
        print("No histogram starting with 'listOfSystematics' found in source file.")
        src.Close()
        return

    hist_name = hist_to_copy.GetName()

    # Open the target file in UPDATE mode (to allow writing)
    tgt = ROOT.TFile.Open(target_file, "UPDATE")
    if not tgt or tgt.IsZombie():
        src.Close()
        raise RuntimeError(f"Error opening target file: {target_file}")

    # Check if a histogram with the same name already exists in the target file
    if tgt.Get(hist_name):
        src.Close()
        tgt.Close()
        raise RuntimeError(f"Histogram '{hist_name}' already exists in target file.")

    # Copy the histogram and write it to the target file
    tgt.cd()
    hist_to_copy.Write()
    print(f"Histogram '{hist_name}' copied successfully.")

    # Close both files
    src.Close()
    tgt.Close()
    return


flags = initConfigFlags()
p = flags.getArgumentParser(description="Run Athena with CP algorithms in local input")

# modify some of the default arguments
for a in p._actions:
    if "--evtMax" in a.option_strings:
        a.option_strings = [
            "-e",
            "--evtMax",
            "--max-events",
        ]  # adds the "-e" and "--max-events" options
        p._option_string_actions["-e"] = a
        p._option_string_actions["--max-events"] = a
    if "--interactive" in a.option_strings:
        a.option_strings = ["--interactive"]  # remove the "-i" option
    if "--filesInput" in a.option_strings:
        a.option_strings = [
            "-i",
            "--filesInput",
            "--input-list",
        ]  # adds the "-i" option and '--input-list' option
        p._option_string_actions["-i"] = a
        p._option_string_actions["--input-list"] = a
    if any(
        [
            x in a.option_strings
            for x in [
                "--mtes",
                "--mtes-channel",
                "--nprocs",
                "--threads",
                "--concurrent-events",
                "--CA",
                "--debugWorker",
            ]
        ]
    ):
        a.help = argparse.SUPPRESS  # hide all these options, b/c not yet supported

g = p._action_groups[
    2
]  # should be the main options group, adding to that so that appear in same block in help msg
g.add_argument(
    "-o", "--output-name", required=True, help="Name of the output file (without .root)"
)
g.add_argument(
    "-a",
    "--analysis",
    type=str,
    default="TopCPToolkit.TtbarCPalgoConfigBlocksAnalysis",
    help="Name of reco analysis to run. Should be package.moduleName,"
    "where module implements makeRecoConfiguration method",
)
g.add_argument("--parton", action="store_true", help="Wheter to run parton level")
g.add_argument("--particle", action="store_true", help="Whether to run particle level")
g.add_argument(
    "--no-systematics",
    action="store_true",
    help="Configure the job to run with no systematics.",
)
g.add_argument(
    "--no-filter",
    action="store_true",
    help="Skip filtering of events due to event selection (selection flags are still stored.)",
)
g.add_argument(
    "--no-reco",
    action="store_true",
    help="Skip running the detector-level analysis. Useful for running on TRUTH derivations.",
)
g.add_argument(
    "-t",
    "--text-config",
    type=str,
    default=None,
    help="Name of the analysis to run. Should be a directory containing reco.yaml,"
    "and optionally particle.yaml and parton.yaml.",
)
g.add_argument(
    "--split-output",
    action="store_true",
    help="Do not merge metadata histograms and main physics trees.",
)

# changes to default flag values (done before fillFromArgs so appears in the --help flag system
flags.Exec.OutputLevel = Constants.WARNING
flags.Exec.PrintAlgsSequence = True
flags.Exec.InfoMessageComponents = [
    "AthenaEventLoopMgr",
    "THistSvc",
    "PerfMonMTSvc",
    "ApplicationMgr",
]  # don't suppress info from these

args = flags.fillFromArgs(parser=p)

# allow user to specify input file as a txt file that contains a list of ds files
# if any txt files given as input, open them and get filenames
files = []
for file in flags.Input.Files:
    if file.endswith(".txt"):
        with open(file, "r") as _files:
            for f in _files:
                if len(f.split(",")) > 1:  # if there are multiple files in the line
                    files += [sub_file.strip() for sub_file in f.split(",")]
                else:
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

Path(os.path.join(TMPDIR, args.output_name)).mkdir(exist_ok=True)
# the final file to produce
finalfile = f"{args.output_name}.root"
metadatafile = finalfile.replace(".root", "_metadata.root")

jobs = []
if not args.no_reco:
    jobs += [
        (
            "reco",
            partial(
                makeRecoSequence,
                noSystematics=args.no_systematics,
                noFilter=args.no_filter,
            ),
        )
    ]
if args.parton:
    jobs += [("parton", partial(makeTruthSequence, noSystematics=args.no_systematics))]
if args.particle:
    jobs += [
        (
            "particle",
            partial(
                makeParticleLevelSequence,
                noSystematics=args.no_systematics,
                noFilter=args.no_filter,
            ),
        )
    ]

cfg = MainServicesCfg(flags)
cfg.merge(PoolReadCfg(flags))
cfg.merge(CutFlowSvcCfg(flags))
cfg.merge(MetaDataSvcCfg(flags))
cfg.getService("AthenaEventLoopMgr").IntervalInSeconds = (
    5  # enable processing rate reporting every 5s
)

files_to_merge = []

for jobLabel, jobSequence in jobs:
    histoFile = os.path.join(TMPDIR, f"{args.output_name}/{jobLabel}_histograms.root")
    treeFile = os.path.join(TMPDIR, f"{args.output_name}/{jobLabel}_trees.root")
    pid = (
        os.fork()
    )  # even though we are running the subjobs sequentially, must do each in a subprocess
    if pid == 0:
        if args.text_config:
            from TopCPToolkit.commonAlgoConfig import makeTextBasedSequence

            cfg.merge(
                makeTextBasedSequence(
                    args.text_config, jobLabel, flags, noSystematics=args.no_systematics
                )
            )
        else:
            cfg.merge(jobSequence(args.analysis, flags))
        from AthenaConfiguration.ComponentFactory import CompFactory

        cfg.addService(
            CompFactory.THistSvc(
                Output=[
                    f"ANALYSIS DATAFILE='{histoFile}' OPT='RECREATE'",
                    f"NTUPLES DATAFILE='{treeFile}' OPT='RECREATE'",
                ]
            )
        )
        for (
            algo
        ) in (
            cfg._algorithms.values()
        ):  # loop over the actual algorithms (not just the names)
            if algo.getType() in [
                "CP::TreeMakerAlg",
                "CP::AsgxAODMetNTupleMakerAlg",
                "CP::TreeFillerAlg",
                "CP::AsgxAODNTupleMakerAlg",
            ]:
                cfg.getEventAlgo(algo.getName()).RootStreamName = (
                    "NTUPLES"  # redirect ntuple to this file
                )
        from AthenaConfiguration.Utils import setupLoggingLevels

        setupLoggingLevels(
            flags, cfg
        )  # adjust logging levels according to Exec.XXXMessageComponents flags
        cfg.printConfig()  # For debugging
        sys.exit(cfg.run().isFailure())
    else:
        # this is the parent process .. we could parallelize the subjobs but for now we will do sequentially
        # by having the parent wait for the child procress
        status = os.waitid(os.P_PID, pid, os.WEXITED)  # parent waits for child
        print(jobLabel, "subjob returned status ", status.si_status)
        if status.si_status != 0:
            sys.exit(status.si_status)
        files_to_merge.insert(0, treeFile)
        remove_empty_trees(treeFile)
        if jobLabel == jobs[0][0]:  # takes just histograms from the first subjob
            os.system(f"mv {histoFile} {metadatafile}")
            if not args.split_output:
                files_to_merge += [metadatafile]
        else:  # add only the list of systematics
            copy_systematics_histogram(histoFile, metadatafile)


if len(files_to_merge) > 0:
    # run the hadd operation, but filter out harmless warnings
    tmp_stderr_file = "hadd_stderr.txt"
    stderr_output = ""
    os.system(
        f"hadd -f {finalfile} " + " ".join(files_to_merge) + f" 2>{tmp_stderr_file}"
    )
    with open(tmp_stderr_file, "r") as ferr:
        stderr_output = ferr.read()
    filtered_stderr_output = stderr_output.replace(
        "Warning in <TList::Merge>: input list is empty - nothing to merge with", ""
    )
    if filtered_stderr_output.strip():
        print(filtered_stderr_output)
    else:
        print("hadd was successful.")
        os.system("rm " + " ".join(files_to_merge))
        os.system(f"rm {tmp_stderr_file}")
        os.system(
            f"rm -rf {os.path.join(TMPDIR, args.output_name)}"
        )  # delete the output directory

    ############
    # METADATA #
    ############
    f = ROOT.TFile(
        f"{finalfile}" if not args.split_output else f"{metadatafile}", "UPDATE"
    )
    h_metadata = ROOT.TH1I(
        "metadata",
        "Sample metadata:Look at the bin labels:Number of files in this sample",
        4,
        0,
        4,
    )
    h_metadata.GetXaxis().SetBinLabel(1, str(flags.Input.DataType.value))
    if flags.Input.DataType != DataType.Data:
        campaign = str(flags.Input.MCCampaign.value)
    else:
        campaign = str(flags.Input.DataYear)
    h_metadata.GetXaxis().SetBinLabel(2, campaign)
    h_metadata.GetXaxis().SetBinLabel(3, str(flags.Input.MCChannelNumber))
    h_metadata.GetXaxis().SetBinLabel(4, flags.Input.eTag)
    h_metadata.Write()
    f.Close()
