<!---
## Make-methods

### [makeElectronCalibrationConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/EgammaAnalysisAlgorithms/python/ElectronAnalysisConfig.py)

`seq`
:   the config sequence.

`containerName`
:   the name of the output container after calibration.

`postfix`
:   a postfix to apply to decorations and algorithm names. Typically not needed here since the calibration is common to all electrons.

`crackVeto`
:   whether to perform LAr crack veto, i.e. remove electrons within $1.37<\vert\eta\vert<1.52$. The default is `False`.

`ptSelectionOutput`
:   whether or not to apply the default $p_\mathrm{T} > 4.5$ GeV cut to calibrated electrons. The default is `False`.

`isolationCorrection`
:   whether or not to perform isolation corrections (leakage corrections), i.e. set up an instance of [`CP::EgammaIsolationCorrectionAlg`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/EgammaAnalysisAlgorithms/Root/EgammaIsolationCorrectionAlg.cxx).

`forceFullSimConfig`
:   whether to force the tool to use the configuration meant for full simulation samples. Only for testing purposes. The default is `False`.

!!! success "Registers the following variables:"
    - `pt`: electron $p_\mathrm{T}$
    - `eta`: electron $\eta$ (no systematics)
    - `phi`: electron $\phi$ (no systematics)
    - `e`: electron $E$
    - `charge`: electron charge (no systematics)

### [makeElectronWorkingPointConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/EgammaAnalysisAlgorithms/python/ElectronAnalysisConfig.py)

`seq`
:   the config sequence.

`containerName`
:   the name of the input container.

`selectionName`
:   the name of the electron selection to define (e.g. `tight` or `loose`).

`postfix`
:   a postfix to apply to decorations and algorithm names. Typically not needed here as `selectionName` is used internally.

`workingPoint`
:   the ID and isolation WPs to use. Pass them in the format `likelihood.isolation`. Supported ID WPs: `TightLH`, `MediumLH`, `LooseBLayerLH`. Supported isolation WPs: `HighPtCaloOnly`, `Loose_VarRad`, `Tight_VarRad`, `TightTrackOnly_VarRad`, `TightTrackOnly_FixedRad`, `NonIso`.

`recomputeLikelihood`
:   whether to rerun the LH. The default is `False`, i.e. to use derivation flags.

    !!! warning
        The only way to get the DNN ID at the moment is to recompute the likelihood, as it's not stored in derivations.

`chargeIDSelection`
:   whether to run the [ECIDS tool](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/ElectronPhotonID/ElectronPhotonSelectorTools/Root/AsgElectronChargeIDSelectorTool.cxx?v=21.2). The default is `False`.

    !!! warning
        ECIDS is actually not supported in release 24. It will likely become part of the DNN ID.

`noEffSF`
:   disables the calculation of efficiencies and scale factors. Experimental! only useful to test a new WP for which scale factors are not available. The default is `False`.

`forceFullSimConfig`
:   whether to force the tool to use the configuration meant for full simulation samples. Only for testing purposes. The default is `False`.

!!! success "Registers the following variables:"
    - `select`: whether the electron passes the ID and isolation cuts
    - `reco_effSF`: the per-electron reconstruction SF
    - `id_effSF`: the per-electron ID SF
    - `isol_effSF`: the per-electron isolation SF
--->

## Config blocks

### [ElectronCalibrationConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/EgammaAnalysisAlgorithms/python/ElectronAnalysisConfig.py)
Name in YAML: **Electrons**

`containerName`
:   the name of the output container after calibration.

`ESModel`
:   flag for Egamma calibration. The default is `''` (empty string), which uses the current recommendations.

`decorrelationModel`
:   decorrelation model for the EGamma energy scale. Supported choices are: `FULL_v1`, `1NP_v1` (default).

`postfix`
:   a postfix to apply to decorations and algorithm names. Typically not needed here since the calibration is common to all electrons.

`crackVeto`
:   whether to perform LAr crack veto based on the cluster $\eta$, i.e. remove electrons within $1.37<\vert\eta\vert<1.52$. The default is `False`.

`minPt`
:   the minimum $p_\mathrm{T}$ cut to apply to calibrated electrons. The default is 4.5 GeV.

`isolationCorrection`
:   whether or not to perform isolation corrections (leakage corrections), i.e. set up an instance of [`CP::EgammaIsolationCorrectionAlg`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/EgammaAnalysisAlgorithms/Root/EgammaIsolationCorrectionAlg.cxx). The default is `False`.

`recalibratePhyslite`
:   whether to run the [`CP::EgammaCalibrationAndSmearingAlg`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/EgammaAnalysisAlgorithms/Root/EgammaCalibrationAndSmearingAlg.cxx) on PHYSLITE derivations. The default is `True`.

`forceFullSimConfig`
:   whether to force the tool to use the configuration meant for full simulation samples. Only for testing purposes. The default is `False`.

`splitCalibrationAndSmearing`
:   EXPERIMENTAL: This splits the EgammaCalibrationAndSmearingTool into two steps. The first step applies a baseline calibration that is not affected by systematics. The second step then applies the systematics dependent corrections.  The net effect is that the slower first step only has to be run once, while the second is run once per systematic. ATLASG-2358. The default is `False`.

!!! success "Registers the following variables:"
    - `pt`: electron $p_\mathrm{T}$
    - `eta`: electron $\eta$ (no systematics)
    - `phi`: electron $\phi$ (no systematics)
    - `charge`: electron charge (no systematics)
    - `e`: electron $E$ (*since AnalysisBase 25.2.13*)

### [ElectronWorkingPointConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/EgammaAnalysisAlgorithms/python/ElectronAnalysisConfig.py)
Name in YAML: **Electrons.WorkingPoint**

`containerName`
:   the name of the input container.

`selectionName`
:   the name of the electron selection to define (e.g. `tight` or `loose`).

`postfix`
:   a postfix to apply to decorations and algorithm names. Typically not needed here as `selectionName` is used internally.

`trackSelection`
:   whether or not to set up an instance of [`CP::AsgLeptonTrackSelectionAlg`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AsgAnalysisAlgorithms/Root/AsgLeptonTrackSelectionAlg.cxx), with the recommended $d_0$ and $z_0\sin\theta$ cuts. The default is `True`.

`maxD0Significance`
:   maximum d0 significance used for the trackSelection. The default is 5.

`maxDeltaZ0SinTheta`
:   maximum z0sinTheta in mm used for the trackSelection. The default is 0.5 mm.

`writeTrackD0Z0`
:   save the d0 significance and z0sinTheta variables so they can be written out. The default is `False`.

`identificationWP`
:   the ID WP (string) to use. Supported ID WPs: `TightLH`, `MediumLH`, `LooseBLayerLH`, `TightDNN`, `MediumDNN`, `LooseDNN`, `TightDNNnoCF`, `MediumDNNnoCF`, `LooseDNNnoCF`.

`isolationWP`
:   the isolation WP (string) to use. Supported isolation WPs: `HighPtCaloOnly`, `Loose_VarRad`, `Tight_VarRad`, `TightTrackOnly_VarRad`, `TightTrackOnly_FixedRad`, `NonIso`.

`closeByCorrection`
:  whether to use close-by-corrected isolation working points. The default is `False`.

`recomputeLikelihood`
:   whether to rerun the LH. The default is `False`, i.e. to use derivation flags.

    !!! warning
        The only way to get the DNN ID at the moment is to recompute the likelihood, as it's not stored in derivations.

`chargeIDSelectionRun2`
:   whether to run the [ECIDS tool](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/ElectronPhotonID/ElectronPhotonSelectorTools/Root/AsgElectronChargeIDSelectorTool.cxx?v=21.2) (only available for Run 2). The default is `False`.

    !!! warning
        ECIDS is actually not supported in release 24. It will likely become part of the DNN ID.

`noEffSF`
:   disables the calculation of efficiencies and scale factors. Experimental! only useful to test a new WP for which scale factors are not available. The default is `False`.

`doFSRSelection`
:   whether to accept additional electrons close to muons for the purpose of FSR corrections to these muons. Expert feature requested by the H4l analysis running on PHYSLITE. The default is `False`.

`forceFullSimConfig`
:   whether to force the tool to use the configuration meant for full simulation samples. Only for testing purposes. The default is `False`.

`correlationModelId`
:   the correlation model (string) to use for ID scale factors. Supported models: `SIMPLIFIED` (default), `FULL`, `TOTAL`, `TOYS`.

`correlationModelIso`
:   the correlation model (string) to use for isolation scale factors. Supported models: `SIMPLIFIED` (default), `FULL`, `TOTAL`, `TOYS`.

`correlationModelReco`
:   the correlation model (string) to use for reconstruction scale factors. Supported models: `SIMPLIFIED` (default), `FULL`, `TOTAL`, `TOYS`.


!!! success "Registers the following variables:"
    - `select`: whether the electron passes the ID and isolation cuts
    - `reco_effSF`: the per-electron reconstruction SF
    - `id_effSF`: the per-electron ID SF
    - `isol_effSF`: the per-electron isolation SF

!!! success "Additional variables toggled by `writeTrackD0Z0`"
    - `d0sig`: the $d_0$ significance (no systematics)
    - `z0sintheta`: the $z_0\sin\theta$ parameter (no systematics)