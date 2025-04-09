## Configuration flags

The configuration flags are a new unified approach in R24 for configuration based on various metadata. For a general introduction see the [ATLAS Athena flags documentation](https://atlassoftwaredocs.web.cern.ch/guides/ca_configuration/flags/).

The flags are a nested hierarchical structure containing information, such as the inputs processed, data type (data/simulation), simulation type, campaign, etc. In AnalysisBase, they are primarily filled from peeking into DxAOD files and from reading the xAOD `FileMetaData`.

As an example, assuming the flags are loaded into the variable `flags`, to determine the campaign of a MC sample one can check the value of `flags.Input.MCCampaign`. `Input` is a category in the config hierarchy, encompassing all flags related to the properties of the input file, and `MCCampaign` stores the value of the campaign (using the [`Campaign` enum](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/Tools/Campaigns/python/Utils.py)).

Below is a (non-exhaustive) list of the most common flags, which are used in TopCPToolkit:

- `Input`
    - `DataType` -- [DataType](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AnalysisAlgorithmsConfig/python/ConfigAccumulator.py#0015) enum, possible values are `Data`, `FullSim`, `FastSim`
    - `LHCPeriod` -- [LHCPeriod](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/Control/AthenaConfiguration/python/Enums.py#0065) enum, possible values are `Run2` and `Run3`
    - `RunNumber` -- list of run numbers associated with the file
    - `MCChannelNumber` -- DSID of a MC sample (N/A for data samples)
    - `RunNumberAsInt` -- run number corresponding to the file, expressed as an integer. We assume that there is only one run number associated with the input file. For data, this is the actual run number, while for MC this corresponds to one of the run numbers per campaign.
    - `AMITag` -- full AMI tags for the dataset, expressed as a single string, e.g.: `e8472_e8475_s3873_s3874_r13829_r13831_p5855`
    - `isRun3` -- boolean, true if file corresponds to Run-3 data/MC, false for Run-2
    - `isPHYSLITE` -- boolean, true if the file is in PHYSLITE derivation format
    - `MCCampaign` -- [Campaign](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/Tools/Campaigns/python/Utils.py#0003) enum, for example mc23a, mc20a, etc.
    - `GeneratorsInfo` -- a dictionary of the MC generator names and versions

You can get a printout of this information by calling

```python
metaConfig.pretty_print(flags)
```

which is done in [runTop_el.py](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/bin/runTop_el.py), at the start of the job.