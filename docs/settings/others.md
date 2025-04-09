<!---
## Make-methods

### [makeCommonServicesConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AsgAnalysisAlgorithms/python/AsgAnalysisConfig.py)
Name in YAML: **CommonServices**

`seq`
:   the config sequence.

_Returns an instance of [`CommonServicesConfig`](#commonservicesconfig), see the available options below._


### [makeEventCleaningConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AsgAnalysisAlgorithms/python/EventCleaningConfig.py)

`seq`
:   the config sequence.

`runPrimaryVertexSelection`
:   whether to run primary vertex selection. The default is `True`.

`runEventCleaning`
:   whether to run event cleaning (sets up an instance of [`CP::EventFlagSelectionAlg`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AsgAnalysisAlgorithms/Root/EventFlagSelectionAlg.cxx)). The default is `False`.

`userGRLFiles`
:   a list of GRL files (list of strings) to select data from. The default is `[]` (empty list).

### [makeGeneratorAnalysisConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AsgAnalysisAlgorithms/python/AsgAnalysisConfig.py)
Name in YAML: **GeneratorLevelAnalysis**

`seq`
:   the config sequence.

`saveCutBookkeepers`
:   whether to save the cut bookkeepers information into the output file. The default is `False`.

`runNumber`
:   the MC runNumber (int). The default is `None` (autoconfigure from metadata).

`cutBookkeepersSystematics`
:   whether to also save the cut bookkeepers systematics. The default is `False`.

!!! success "Registers the following variables:"
    - `generatorWeight`: the event-level PMG generator weight

### [makePileupReweightingConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AsgAnalysisAlgorithms/python/AsgAnalysisConfig.py)

`seq`
:   the config sequence.

`campaign`
:   the MC campaign for the PRW auto-configuration.

`files`
:   the input files being processed (list of strings). Alternative to auto-configuration.

`useDefaultConfig`
:   whether to use the central PRW files. The default is `True`.

`userLumicalFiles`
:   user-provided lumicalc files (list of strings). Alternative to auto-configuration.

`userPileupConfigs`
:   user-provided PRW files. Alternative to auto-configuration.

!!! success "Registers the following variables:"
    - `eventNumber`: the event number (no systematics)
    - `runNumber`: the run number (no systematics)
    - `mcChannelNumber`: the MC channel number or DSID (no systematics)
    - `weight_pileup`: the weight for pileup reweighting
    - `weight_beamspot`: the weight to correct the beamspot size in Run 2 (no systematics)

### [makeBootstrapGeneratorConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AsgAnalysisAlgorithms/python/BootstrapGeneratorConfig.py)

`seq`
:   the config sequence.

`nReplicas`
:   the number (int) of bootstrap replicas to generate. The default is 1000.

`decoration`
:   the name of the output vector branch containing the bootstrapped weights. The default is `bootstrapWeights`.

`runOnMC`
:   toggle to force running on MC samples. The default is `False`, i.e. run only on data.
--->

## Config blocks

### [addNewConfigBlocks](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AnalysisAlgorithmsConfig/python/ConfigText.py)
Name in YAML: **AddConfigBlocks**

See [Registering new blocks](../starting/analysis.md#registering-new-blocks-directly-in-the-yaml-config-since-analysisbase-24240) for how to use this tool.

`modulePath`
:   the pythonic location of the module, i.e. `Package.Module`.

`functionName`
:   the name of the function or config block to call.

`algName`
:   the name to give the block in the YAML config file.

`pos`
:   where to insert the new block amongst the list of registered config blocks, i.e. an already registered `algName`. Recommended: `Output`.

### [CommonServicesConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AsgAnalysisAlgorithms/python/AsgAnalysisConfig.py)

`runSystematics`
:   whether to turn on the computation of systematic variations. The default is to run them on MC.

`filterSystematics`
:   a regexp string against which the systematics names will be matched. Only positive matches are retained and used in the evaluation of the various algorithms.

`onlySystematicsCategories`
:   a list of strings defining categories of systematics to enable (only recommended for studies / partial ntuple productions). Choose amongst: `jets`, `electrons`, `muons`, `photons`, `taus`, `met`, `ftag`, `event`.
This option is overridden by `filterSystematics`.

    !!! tip
        See the [guide](../starting/running_local.md#filtering-systematics) on how to use the systematics filter.

`systematicsHistogram`
:   the name (string) of the histogram to which a list of executed systematics will be printed. The default is `None` (don't write out the histogram).

    !!! tip
        We recommend using the string `'listOfSystematics'` for interfacing with [`FastFrames`](https://gitlab.cern.ch/atlas-amglab/fastframes).

### [EventCleaningBlock](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AsgAnalysisAlgorithms/python/EventCleaningConfig.py)
Name in YAML: **EventCleaning**

`runPrimaryVertexSelection`
:   whether to run primary vertex selection. The default is `True`.

`runEventCleaning`
:   whether to run event cleaning (sets up an instance of [`CP::EventFlagSelectionAlg`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AsgAnalysisAlgorithms/Root/EventFlagSelectionAlg.cxx)). The default is `False`.

`userGRLFiles`
:   a list of GRL files (list of strings) to select data from. The default is `[]` (empty list).

`minTracksPerVertex`
:   minimum number (integer) of tracks per vertex. The default is 2.

`selectionFlags`
:   flags (list of strings) to use for jet cleaning. The default is `['DFCommonJets_eventClean_LooseBad']`.

`invertFlags`
:   list of booleans determining whether to invert the cut of the above `selectionFlags`. The default is `[0]`.

    !!! warning
        In AnalysisBase, vectors of bools cannot be handled properly. Instead, use 0/1 values.

`GRLDict`
:   a custom GRL dictionary with key some name and value a GRL file. The default is `{}` (empty dictionary), i.e. use recommended values.

`noFilter`
:   whether to toggle off event filtering. The default is `False`, i.e. apply filtering.

### [GeneratorAnalysisBlock](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AsgAnalysisAlgorithms/python/AsgAnalysisConfig.py)
Name in YAML: **GeneratorLevelAnalysis**

`saveCutBookkeepers`
:   whether to save the cut bookkeepers information into the output file. The default is `True`.

`runNumber`
:   the MC runNumber (int). The default is `None` (autoconfigure from metadata).

`cutBookkeepersSystematics`
:   whether to also save the cut bookkeepers systematics. The default is `None` (follows the global systematics flag). Set to `False` or `True` to override.

!!! success "Registers the following variables:"
    - `generatorWeight`: the event-level PMG generator weight

### [PileupReweightingBlock](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AsgAnalysisAlgorithms/python/AsgAnalysisConfig.py)
Name in YAML: **PileupReweighting**

`campaign`
:   the MC campaign for the PRW auto-configuration.

`files`
:   the input files being processed (list of strings). Alternative to auto-configuration.

`useDefaultConfig`
:   whether to use the central PRW files. The default is `True`.

`userLumicalFiles`
:   user-provided lumicalc files (list of strings). Alternative to auto-configuration.

`userLumicalcFilesPerCampaign`
:   user-provided lumicalc files (dictionary of list of strings, with MC campaigns as the keys). Alternative to auto-configuration.

`userPileupConfigs`
:   user-provided PRW files (list of strings). Alternative to auto-configuration. Alternative to auto-configuration.

`userPileupConfigsPerCampaign`
:   user-provided PRW files (dictionary of list of strings, with MC campaigns as the keys)

!!! success "Registers the following variables:"
    - `eventNumber`: the event number (no systematics)
    - `runNumber`: the run number (no systematics)
    - `mcChannelNumber`: the MC channel number or DSID (no systematics)
    - `weight_pileup`: the weight for pileup reweighting
    - `weight_beamspot`: the weight to correct the beamspot size in Run 2 (no systematics)

### [BootstrapGeneratorConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AsgAnalysisAlgorithms/python/BootstrapGeneratorConfig.py)
Name in YAML: **Bootstraps**

`nReplicas`
:   the number (int) of bootstrap replicas to generate. The default is 1000.

`decoration`
:   the name of the output vector branch containing the bootstrapped weights. The default is `bootstrapWeights`.

## TopCPToolkit modules

### [metaConfig](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/python/metaConfig.py)

This module is responsible for loading additional metadata and should not be called by the end user. It appends additional metadata
information into configuration flags. The configuration flags are documented in the [Metadata-based autoconfiguration flags](configflags.md) section.

### [commonAlgoConfig](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/python/commonAlgoConfig.py)

This module contains the master sequencing functions `makeRecoSequence` / `makeTruthSequence` / `makeParticleLevelSequence` / `makeTextBasedSequence`, as [explained in the guide](../starting/analysis.md#analysis-modules).