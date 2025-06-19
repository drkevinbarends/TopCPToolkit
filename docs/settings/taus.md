<!---
## Make-methods

### [makeTauCalibrationConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/TauAnalysisAlgorithms/python/TauAnalysisConfig.py)

`seq`
:   the config sequence.

`containerName`
:   the name of the output container after calibration.

`postfix`
:   a postfix to apply to decorations and algorithm names. Typically not needed here since the calibration is common to all taus.

`rerunTruthMatching`
:   whether to rerun truth matching (sets up an instance of [`CP::TauTruthMatchingAlg`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/TauAnalysisAlgorithms/Root/TauTruthMatchingAlg.cxx)). The default is `True`.

!!! success "Registers the following variables:"
    - `pt`: tau-jet $p_\mathrm{T}$
    - `eta`: tau-jet $\eta$ (no systematics)
    - `phi`: tau-jet $\phi$ (no systematics)
    - `e`: tau-jet $E$
    - `charge`: tau-jet charge (no systematics)

### [makeTauWorkingPointConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/TauAnalysisAlgorithms/python/TauAnalysisConfig.py)

`seq`
:   the config sequence.

`containerName`
:   the name of the input container.

`selectionName`
:   the name of the tau-jet selection to define (e.g. `tight` or `loose`).

`postfix`
:   a postfix to apply to decorations and algorithm names. Typically not needed here as `selectionName` is used internally.

`workingPoint`
:   the ID WP to use (single string). Supported ID WPs: `Tight`, `Medium`, `Loose`, `VeryLoose`, `NoID`.

`legacyRecommendations`
:   whether to use legacy tau-jet BDT and electron veto recommendations. The default is `False`.

`noEffSF`
:   disables the calculation of efficiencies and scale factors. Experimental! only useful to test a new WP for which scale factors are not available. The default is `False`.

`addSelectionToPreselection`
:   whether to retain only tau-jets satisfying the working pointrequirements. The default is `True`.

!!! success "Registers the following variables:"
    - `select`: whether the tau-jet passes the ID cuts
    - `effSF`: the per-tau-jet reconstruction SF
--->

## Config blocks

### [TauCalibrationConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/TauAnalysisAlgorithms/python/TauAnalysisConfig.py)
Name in YAML: **TauJets**

`inputContainer`
:   the name of the input tau-jet container. The deault is `TauJets`.

`containerName`
:   the name of the output container after calibration.

`postfix`
:   a postfix to apply to decorations and algorithm names. Typically not needed here since the calibration is common to all tau-jets. The default is `''` (empty string).

`rerunTruthMatching`
:   whether to rerun truth matching (sets up an instance of [`CP::TauTruthMatchingAlg`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/TauAnalysisAlgorithms/Root/TauTruthMatchingAlg.cxx)). The default is `True`.

`decorateTruth`
:   whether to decorate truth particle information on the reconstructed one. The default is `False`.

!!! success "Registers the following variables:"
    - `pt`: tau-jet $p_\mathrm{T}$
    - `eta`: tau-jet $\eta$ (no systematics)
    - `phi`: tau-jet $\phi$ (no systematics)
    - `charge`: tau-jet charge (no systematics)
    - `NNDecayMode`: decay mode classification (no systematics)
    - `e`: tau-jet $E$ (*since AnalysisBase 25.2.13*)
    - `nTracks`: the number of tau-tracks (no systematics)

!!! success "Additional variables toggled by `decorateTruth`"
    - `truth_pt_vis`: visible $p_\mathrm{T}$ of the linked truth-tau (no systematics)
    - `truth_eta_vis`: visible $\eta$ of the linked truth-tau (no systematics)
    - `truth_phi_vis`: visible $\phi$ of the linked truth-tau (no systematics)
    - `truth_m_vis`: visible mass of the linked truth-tau (no systematics)
    - `truth_pdgId`: pdgId of the linked truth-tau (no systematics)
    - `truth_IsHadronicTau`: whether the linked truth-tau decays hadronically (no systematics)
    - `truth_DecayMode`: decay mode classification of the linked truth-tau  (no systematics)
    - `truth_ParticleType`: particle type of the linked truth-tau (no systematics)
    - `truth_type`: type of the corresponding truth particle (no systematics)
    - `truth_origin`: origin of the corresponding truth particle (no systematics)
    - `truth_PartonTruthLabelID`: the PDG ID of the highest-energy ghost parton (no systematics)

### [TauWorkingPointConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/TauAnalysisAlgorithms/python/TauAnalysisConfig.py)
Name in YAML: **TauJets.WorkingPoint**

`containerName`
:   the name of the input container.

`selectionName`
:   the name of the tau-jet selection to define (e.g. `tight` or `loose`).

`postfix`
:   a postfix to apply to decorations and algorithm names. Typically not needed here as `selectionName` is used internally.

`quality`
:   the ID WP (string) to use. Supported ID WPs: `Tight`, `Medium`, `Loose`, `VeryLoose`, `Baseline`.

`use_eVeto`
:   use selection with or without eVeto combined with tauID recommendations: set it to `True` if electron mis-reconstructed as tau is a large background for your analysis. The default is `False`.

`useGNTau`
:   use GNTau based ID instead of RNNTau ID recommendations: new experimental feature and might become default soon. The default is `False`.

`noEffSF`
:   disables the calculation of efficiencies and scale factors. Experimental! only useful to test a new WP for which scale factors are not available. The default is `False`.

`saveDetailedSF`
:   whether to save all	the independent	and detailed object scale factors. The default is `True`.

`saveCombinedSF`
:   whether to save the	combined object	scale factor. The default is `False`.

!!! success "Registers the following variables:"
    - `select`: whether the tau-jet passes the ID cuts
    - `effSF`: the per-tau-jet reconstruction SF
