#!/usr/bin/env python

# Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
import GridSubmission.grid
import GridSubmission.ami
import MC20_PHYS

config = GridSubmission.grid.Config()
config.code          = 'runTop_el.py --no-systematics -t tWZ'
config.outputName    = 'output' # output directory from EventLoop
config.gridUsername  = 'kebarend' # use e.g. phys-top or phys-higgs for group production
config.suffix        = 'tWZ_19June25.v0'
config.excludedSites = 'BNL-OSG2_DATADISK'
config.noSubmit      = False # set to True if you just want to test the submission
config.mergeType     = 'Default' #'None', 'Default' or 'xAOD'
config.destSE        = '' #This is the default (anywhere), or try e.g. 'UKI-SOUTHGRID-BHAM-HEP_LOCALGROUPDISK'
# by default the requested memory is set to 2GB, if you need to increase this, see the option below
# config.memory = '4000' # NOTE grid sites with 4GB for single-core user jobs are rare
# config.otherOptions = '' # use this to pass any other arguments to prun

names = ["PHYS_tWZ_MC_Background_Main_ttZ_Run2"]
samples = GridSubmission.grid.Samples(names)
GridSubmission.ami.check_sample_status(samples)  # Call with (samples, True) to halt on error
GridSubmission.grid.submit(config, samples)
