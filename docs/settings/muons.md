<!---
## Make-methods

### [makeMuonCalibrationConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/MuonAnalysisAlgorithms/python/MuonAnalysisConfig.py)

`seq`
:   the config sequence.

`containerName`
:   the name of the output container after calibration.

`postfix`
:   a postfix to apply to decorations and algorithm names. Typically not needed here since the calibration is common to all muons.

`ptSelectionOutput`
:   $p_\mathrm{T}$ cut to apply to calibrated muons, in MeV. The default is 3.0 GeV.

!!! success "Registers the following variables:"
    - `pt`: muon $p_\mathrm{T}$
    - `eta`: muon $\eta$ (no systematics)
    - `phi`: muon $\phi$ (no systematics)
    - `charge`: muon charge (no systematics)

### [makeMuonWorkingPointConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/MuonAnalysisAlgorithms/python/MuonAnalysisConfig.py)

`seq`
:   the config sequence.

`containerName`
:   the name of the input container.

`selectionName`
:   the name of the muon selection to define (e.g. `tight` or `loose`).

`postfix`
:   a postfix to apply to decorations and algorithm names. Typically not needed here as `selectionName` is used internally.

`workingPoint`
:   the ID and isolation WPs to use. Pass them in the format `quality.isolation`. Supported ID WPs: `Tight`, `Medium`, `Loose`, `LowPt`, `HighPt`. Supported isolation WPs: `PflowLoose_VarRad`, `PflowTight_VarRad`, `Loose_VarRad`, `Tight_VarRad`, `NonIso`.

`qualitySelectionOutput`
:   whether to retain only muons satisfying the quality requirements (bad muon veto). The default is `True`.

`systematicBreakdown`
:   enables the full breakdown of efficiency SF systematics (1 NP per uncertainty source, instead of 1 NP in total). The default is `False`.

`noEffSF`
:   disables the calculation of efficiencies and scale factors. Experimental! only useful to test a new WP for which scale factors are not available. The default is `False`.

`onlyRecoEffSF`
:   same as `noEffSF`, but retains the ID scale factor. Experimental! only useful for CI tests. The default is `False`.

!!! success "Registers the following variables:"
    - `select`: whether the muon passes the quality and isolation cuts
    - `reco_effSF`: the per-muon reconstruction SF
    - `BadMuonVeto_effSF`: the dedicated "bad muon veto" SF, only with quality `HighPt`
    - `isol_effSF`: the per-muon isolation SF
    - `TTVA_effSF`: the per-muon track-to-vertex-association SF
--->

## Config blocks

### [MuonCalibrationConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/MuonAnalysisAlgorithms/python/MuonAnalysisConfig.py)
Name in YAML: **Muons**

`containerName`
:   the name of the output container after calibration.

`postfix`
:   a postfix to apply to decorations and algorithm names. Typically not needed here since the calibration is common to all muons.

`minPt`
:   pT cut to apply to calibrated muons, in MeV. The default is 3.0 GeV.

`recalibratePhyslite`
:   whether to run the [`CP::EgammaCalibrationAndSmearingAlg`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/EgammaAnalysisAlgorithms/Root/EgammaCalibrationAndSmearingAlg.cxx) on PHYSLITE derivations. The default is `True`.

`maxEta`
:   maximum muon $\vert\eta\vert$ (float). The default is 2.7.

`excludeNSWFromPrecisionLayers`
:   whether to ignore NSW hits. Expert option, for testing purposes or to fix a crash with older derivations (p-tag < p5834). The default is `False`.

`calibMode`
:   calibration mode of the MuonCalibTool needed to turn on the sagitta bias corrections and to select the muon track calibration type (CB or ID+MS). The default is `correctData_CB`.

!!! success "Registers the following variables:"
    - `pt`: muon $p_\mathrm{T}$
    - `eta`: muon $\eta$ (no systematics)
    - `phi`: muon $\phi$ (no systematics)
    - `charge`: muon charge (no systematics)
    - `e`: muon $E$ (*since AnalysisBase 25.2.13*)

### [MuonWorkingPointConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/MuonAnalysisAlgorithms/python/MuonAnalysisConfig.py)
Name in YAML: **Muons.WorkingPoint**

`containerName`
:   the name of the input container.

`selectionName`
:   the name of the muon selection to define (e.g. `tight` or `loose`).

`postfix`
:   a postfix to apply to decorations and algorithm names. Typically not needed here as `selectionName` is used internally.

`trackSelection`
:   whether or not to set up an instance of [`CP::AsgLeptonTrackSelectionAlg`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AsgAnalysisAlgorithms/Root/AsgLeptonTrackSelectionAlg.cxx), with the recommended $d_0$ and $z_0\sin\theta$ cuts. The default is `True`.

`maxD0Significance`
:   maximum d0 significance used for the trackSelection. The default is 3.

`maxDeltaZ0SinTheta`
:   maximum Delta z0sinTheta in mm used for the trackSelection. The default is 0.5 mm.

`writeTrackD0Z0`
:   save the d0 significance and z0sinTheta variables so they can be written out. The default is `False`.

`quality`
:   the ID WP (string) to use. Supported ID WPs: `Tight`, `Medium`, `Loose`, `LowPt`, `HighPt`.

`isolation`
:   the isolation WP (string) to use. Supported isolation WPs: `PflowLoose_VarRad`, `PflowTight_VarRad`, `Loose_VarRad`, `Tight_VarRad`, `NonIso`.

`qualitySelectionOutput`
:   whether to retain only muons satisfying the quality requirements (bad muon veto). The default is `True`.

`closeByCorrection`
:   whether to use close-by-corrected isolation working points. The default is `False`.

`systematicBreakdown`
:   enables the full breakdown of efficiency SF systematics (1 NP per uncertainty source, instead of 1 NP in total). The default is `False`.

`noEffSF`
:   disables the calculation of efficiencies and scale factors. Experimental! only useful to test a new WP for which scale factors are not available. The default is `False`.

`onlyRecoEffSF`
:   same as `noEffSF`, but retains the ID scale factor. Experimental! only useful for CI tests. The default is `False`.

`excludeNSWFromPrecisionLayers`
:   whether to ignore NSW hits. Expert option, for testing purposes or to fix a crash with older derivations (p-tag < p5834). The default is `False`.

!!! success "Registers the following variables:"
    - `select`: whether the muon passes the quality and isolation cuts
    - `reco_effSF`: the per-muon reconstruction SF
    - `BadMuonVeto_effSF`: the dedicated "bad muon veto" SF, only with quality `HighPt`
    - `isol_effSF`: the per-muon isolation SF
    - `TTVA_effSF`: the per-muon track-to-vertex-association SF

!!! success "Additional variables toggled by `writeTrackD0Z0`"
    - `d0sig`: the $d_0$ significance (no systematics)
    - `z0sintheta`: the $z_0\sin\theta$ parameter (no systematics)

### [SoftMuonSelectorConfig](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/python/SoftMuonSelectorConfig.py?ref_type=heads)
Name in YAML: **SoftMuonSelector**

This package is designed to select muons that are within a specific distance ($\Delta R$) from a jet. It is intended for selecting soft muons contained within jets, which is useful for identifying events with muons originating from the decay of heavy flavor hadrons.

`softmuons`
:   The input muon container, with an optional selection, in the format container or container.selection. The default is an empty string (''). Do not include this muon collection in the Overlap Removal algorithm.

`jets`
:   The input jet container, with an optional selection, in the format container or container.selection. The default is an empty string ('').

`softMuonDRJet`
:   The maximum $\Delta R$ between the soft muon and the nearest selected jet. Can be set to 999. to keep all soft muons. Default is 0.4.

`softMuonDRJetUseRapidity`
:   Determines the method for calculating the $\Delta R$(soft muon, jet) for the softMuonDRJet cut. Setting this parameter to True will use rapidity, while setting it to False will use pseudorapidity. The default value is `False`.

`saveSoftMuonAdditionalInfo`
:   Specifies whether to save additional information associated with the soft muon, such as track hits, track quality chi2, among others. The default is `False` (do not save additional information about the soft muon track).

`saveSoftMuonNearestJetInfo`
:   Specifies whether to save additional information associated with the nearest jet to the soft muon, such as the jet charge, EM fraction, or number of constituents. The default is `False` (do not save additional information about the nearest jet).

Events containing at least one soft muon selected by this algorithm can be identified by requiring the event to pass the `EVENTFLAG pass_SoftMuonPassDRJetcut_%SYS%` in the `Event Selection` block.

!!! success "Registers the following variables for each event:"
    - `pass_SoftMuonPassDRJetcut`: whether the muon event contains a muon that passes the selection criteria in the SoftMuonSelector tool
    - `SoftMuonJetDRmin`: per-muon angular distance between the muon and its closest jet
    - `SoftMuonPassDRJetcut`: whether the muon passes the $\Delta R$ (per-muon)
