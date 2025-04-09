<!---
## Make-methods

### [makePhotonCalibrationConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/EgammaAnalysisAlgorithms/python/PhotonAnalysisConfig.py)

`seq`
:   the config sequence.

`containerName`
:   the name of the output container after calibration.

`postfix`
:   a postfix to apply to decorations and algorithm names. Typically not needed here since the calibration is common to all photons.

`crackVeto`
:   whether to perform LAr crack veto, i.e. remove photons within $1.37<\vert\eta\vert<1.52$. The default is `False`.

`enableCleaning`
:   whether to enable photon cleaning (`DFCommonPhotonsCleaning`). The default is `True`.

`cleaningAllowLate`
:   whether to ignore timing information in cleaning (`DFCommonPhotonsCleaningNoTime`). The default is `False`.

`recomputeIsEM`
:   whether to recompute the photon shower shape fudge corrections (sets up an instance of [`CP::PhotonShowerShapeFudgeAlg`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/EgammaAnalysisAlgorithms/Root/PhotonShowerShapeFudgeAlg.cxx)). The default is `False`, i.e. to use derivation variables.

`ptSelectionOutput`
:  whether or not to apply the default $p_\mathrm{T} > 10$ GeV cut to calibrated photons. The default is `False`.

`forceFullSimConfig`
:   whether to force the tool to use the configuration meant for full simulation samples. Only for testing purposes. The default is `False`.

!!! success "Registers the following variables:"
    - `pt`: photon $p_\mathrm{T}$
    - `eta`: photon $\eta$ (no systematics)
    - `phi`: photon $\phi$ (no systematics)
    - `e`: photon $E$


### [makePhotonWorkingPointConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/EgammaAnalysisAlgorithms/python/PhotonAnalysisConfig.py)

`seq`
:   the config sequence.

`containerName`
:   the name of the input container.

`selectionName`
:   the name of the photon selection to define (e.g. `tight` or `loose`).

`postfix`
:   a postfix to apply to decorations and algorithm names. Typically not needed here as `selectionName` is used internally.

`workingPoint`
:   the ID and isolation WPs to use. Pass them in the format `quality.isolation`. Supported ID WPs: `Tight`, `Loose`. Supported isolation WPs: `FixedCutLoose`, `FixedCutTight`, `TightCaloOnly`, `NonIso`.

`recomputeIsEM`
:   whether to rerun the cut-based selection. The default is `False`, i.e. to use derivation flags.

`noEffSF`
:   disables the calculation of efficiencies and scale factors. Experimental! only useful to test a new WP for which scale factors are not available. The default is `False`.

`forceFullSimConfig`
:   whether to force the tool to use the configuration meant for full simulation samples. Only for testing purposes. The default is `False`.

!!! success "Registers the following variables:"
    - `select`: whether the photon passes the ID and isolation cuts
    - `id_effSF`: the per-photon ID SF
    - `isol_effSF`: the per-photon isolation SF
--->

## Config blocks

### [PhotonCalibrationConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/EgammaAnalysisAlgorithms/python/PhotonAnalysisConfig.py)
Name in YAML: **Photons**

`containerName`
:   the name of the output container after calibration.

`ESModel`
:   flag for Egamma calibration. The default is `''` (empty string), which uses the current recommendations.

`decorrelationModel`
:   decorrelation model for the EGamma energy scale. Supported choices are: `FULL_v1`, `1NP_v1` (default).

`postfix`
:   a postfix to apply to decorations and algorithm names. Typically not needed here since the calibration is common to all photons.

`crackVeto`
:   whether to perform LAr crack veto based on the cluster $\eta$, i.e. remove photons within $1.37<\vert\eta\vert<1.52$. The default is `False`.

`enableCleaning`
:   whether to enable photon cleaning (`DFCommonPhotonsCleaning`). The default is `True`.

`cleaningAllowLate`
:   whether to ignore timing information in cleaning (`DFCommonPhotonsCleaningNoTime`). The default is `False`.

`recomputeIsEM`
:   whether to recompute the photon shower shape fudge corrections (sets up an instance of [`CP::PhotonShowerShapeFudgeAlg`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/EgammaAnalysisAlgorithms/Root/PhotonShowerShapeFudgeAlg.cxx)). The default is `False`, i.e. to use derivation variables.

`minPt`
:   the minimum $p_\mathrm{T}$ cut to apply to calibrated photons. The default is 10 GeV.

`recalibratePhyslite`
:   whether to run the [`CP::EgammaCalibrationAndSmearingAlg`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/EgammaAnalysisAlgorithms/Root/EgammaCalibrationAndSmearingAlg.cxx) on PHYSLITE derivations. The default is `True`.

`forceFullSimConfig`
:   whether to force the tool to use the configuration meant for full simulation samples. Only for testing purposes. The default is `False`.

`splitCalibrationAndSmearing`
:   EXPERIMENTAL: This splits the EgammaCalibrationAndSmearingTool into two steps. The first step applies a baseline calibration that is not affected by systematics. The second step then applies the systematics dependent corrections.  The net effect is that the slower first step only has to be run once, while the second is run once per systematic. ATLASG-2358. The default is `False`.

!!! success "Registers the following variables:"
    - `pt`: photon $p_\mathrm{T}$
    - `eta`: photon $\eta$ (no systematics)
    - `phi`: photon $\phi$ (no systematics)
    - `e`: photon $E$: (*since AnalysisBase 25.2.13*)

### [PhotonWorkingPointConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/EgammaAnalysisAlgorithms/python/PhotonAnalysisConfig.py)
Name in YAML: **Photons.WorkingPoint**

`containerName`
:   the name of the input container.

`selectionName`
:   the name of the photon selection to define (e.g. `tight` or `loose`).

`postfix`
:   a postfix to apply to decorations and algorithm names. Typically not needed here as `selectionName` is used internally.

`qualityWP`
:   the ID WP (string) to use. Supported ID WPs: `Tight`, `Loose`.

`isolationWP`
:   the ID WP (string) to use. Supported isolation WPs: `FixedCutLoose`, `FixedCutTight`, `TightCaloOnly`, `NonIso`.

`closeByCorrection`
:   whether to use close-by-corrected isolation working points. The default is `False`.

`recomputeIsEM`
:   whether to rerun the cut-based selection. The default is `False`, i.e. to use derivation flags.

`noEffSF`
:   disables the calculation of efficiencies and scale factors. Experimental! only useful to test a new WP for which scale factors are not available. The default is `False`.

`doFSRSelection`
:   whether to accept additional photons close to muons for the purpose of FSR corrections to these muons. Expert feature requested by the H4l analysis running on PHYSLITE. The default is `False`.

`forceFullSimConfig`
:   whether to force the tool to use the configuration meant for full simulation samples. Only for testing purposes. The default is `False`.

!!! success "Registers the following variables:"
    - `select`: whether the photon passes the ID and isolation cuts
    - `id_effSF`: the per-photon ID SF
    - `isol_effSF`: the per-photon isolation SF

### [PhotonExtraVariablesBlock](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/EgammaAnalysisAlgorithms/python/PhotonExtraVariablesConfig.py)
Name in YAML: **Photons.ExtraVariables** (*since AnalysisBase 25.2.13*)

Decorates the output photons with the conversion type and calorimeter $\eta$.

`containerName`
:   the input photon container.

!!! success "Registers the following variables:"
    - `conversionType`: photon conversion type (no systematics)
    - `caloEta2`: calorimeter $\eta$ from 2nd sampling layer (no systematics)