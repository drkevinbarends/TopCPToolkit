<!---
## Make-methods

### [makeOverlapAnalysisConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AsgAnalysisAlgorithms/python/OverlapAnalysisConfig.py)

`seq`
:   the config sequence.

`postfix`
:   a postfix to apply to decorations and algorithm names. Typically not needed here.

`inputLabel`
:   any possible label used to pick up the selected objects with. This should not be a label already used elsewhere, e.g. `preselectOR`.

`outputLabel`
:   decoration applied (internally) to the output objects, e.g. `passesOR`.

`linkOverlapObjects`
:   whether to set up an element link between overlapping objects. The default is `False`.

`doEleEleOR`
:   whether to set up the electron-electron overlap removal (instance of [`ORUtils::EleEleOverlapTool`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/AnalysisCommon/AssociationUtils/Root/EleEleOverlapTool.cxx)). The default is `False`.

`electrons`
:   the input electron container, with a possible selection, in the format `container` or `container.selection`.

`muons`
:   the input muon container, with a possible selection, in the format `container` or `container.selection`.

`jets`
:   the input jet container, with a possible selection, in the format `container` or `container.selection`.

`taus`
:   the input tau-jet container, with a possible selection, in the format `container` or `container.selection`.

`photons`
:   the input photon container, with a possible selection, in the format `container` or `container.selection`.

`fatJets`
:   the input large-R jet container, with a possible selection, in the format `container` or `container.selection`.

`enableUserPriority`
:   whether to use the user's custom priority ranking, instead of the recommended one. If set to `True`, will respect the priorities set with `inputLabel` (e.g. in SUSYTools, every object gets priority 2, but pre-selected jets get priority 1). The default is `False`.

`bJetLabel`
:   flag to select b-jets with. If left empty, no b-jets are used in the overlap removal. The default is `''` (empty string).

`doTauAntiTauJetOR`
:   whether to set up an instance of [`ORUtils::TauAntiTauJetOverlapTool`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/AnalysisCommon/AssociationUtils/Root/TauAntiTauJetOverlapTool.cxx). The default is `False`.

`antiTauIDTauLabel`
:   flag to select the ID tau-jet for the tau-antitau-jet overlap removal. The default is `''` (empty string).

`antiTauLabel`
:   flag to select the anti-tau-jet for the tau-antitau-jet overlap removal. The default is `''` (empty string).

`antiTauBJetLabel`
:   flag to select b-jets for the tau-antitau-jet overlap removal. The default is `''` (empty string).

`boostedLeptons`
:   whether to enable boosted lepton overlap removal (toggles on the property `UseSlidingDR` of the [`ORUtils::EleJetOverlapTool`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/AnalysisCommon/AssociationUtils/Root/EleJetOverlapTool.cxx) and [`ORUtils::MuJetOverlapTool`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/AnalysisCommon/AssociationUtils/Root/MuJetOverlapTool.cxx) tools). The default is `False`.

`configName`
:   name of the block config, useful to distinguish the various blocks if different overlap removal strategies are set up. The default is `OverlapRemoval`.

!!! success "Registers the following variables:"
    - `select_or`: the per-object overlap removal decision

!!! tip
    It is also possible to pass a number of systematics-independent selections (event-level or per-object) to the overlap removal procedure. These are however not available from the `makeOverlapAnalysisConfig` method, and instead users must modify the config sequence directly. More details in [athena!65523](https://gitlab.cern.ch/atlas/athena/-/merge_requests/65523).
--->

## Config blocks

### [OverlapAnalysisConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AsgAnalysisAlgorithms/python/OverlapAnalysisConfig.py)
Name in YAML: **OverlapRemoval**

`selectionName`
:   name of the block config, useful to distinguish the various blocks if different overlap removal strategies are set up. The default is `''` (empty string).

`inputLabel`
:   any possible label used to pick up the selected objects with. This should not be a label already used elsewhere, e.g. `preselectOR`. The default is `''` (emptry string).

`outputLabel`
:   decoration applied (internally) to the output objects. The default is `'passesOR'`.

`linkOverlapObjects`
:   whether to set up an element link between overlapping objects. The default is `False`.

`electrons`
:   the input electron container.

`electronsSelectionName`
:   a possible selection on the electron container.

`muons`
:   the input muon container.

`muonsSelectionName`
:   a possible selection on the muon container.

`jets`
:   the input jet container.

`jetsSelectionName`
:   a possible selection on the jet container.

`taus`
:   the input tau-jet container.

`tausSelectionName`
:   a possible selection on the tau-jet container.

`photons`
:   the input photon container.

`photonsSelectionName`
:   a possible selection on the photon container.

`fatJets`
:   the input large-R jet container.

`fatJetsSelectionName`
:   a possible selection on the large-R jet container.

`enableUserPriority`
:   whether to use the user's custom priority ranking, instead of the recommended one. If set to `True`, will respect the priorities set with `inputLabel` (e.g. in SUSYTools, every object gets priority 2, but pre-selected jets get priority 1). The default is `False`.

`bJetLabel`
:   flag to select b-jets with. If left empty, no b-jets are used in the overlap removal. The default is `''` (empty string).

`antiTauIDTauLabel`
:   flag to select the ID tau-jet for the tau-antitau-jet overlap removal. The default is `''` (empty string).

`antiTauLabel`
:   flag to select the anti-tau-jet for the tau-antitau-jet overlap removal. The default is `''` (empty string).

`antiTauBJetLabel`
:   flag to select b-jets for the tau-antitau-jet overlap removal. The default is `''` (empty string).

`boostedLeptons`
:   whether to enable boosted lepton overlap removal (toggles on the property `UseSlidingDR` of the [`ORUtils::EleJetOverlapTool`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/AnalysisCommon/AssociationUtils/Root/EleJetOverlapTool.cxx) and [`ORUtils::MuJetOverlapTool`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/AnalysisCommon/AssociationUtils/Root/MuJetOverlapTool.cxx) tools). The default is `False`.

`addPreselection`
:   whether to add preselection decorations without systematics. The default is `False`.

`preselectLabel`
:   label to use when adding preselection decorations. The default is `None`, which uses the `outputLabel` instead.

`doEleEleOR`
:   whether to perform the overlap removal amongst electrons. The default is `False.`

`doEleMuOR`
:   whether to perform the overlap removal between electrons and muons. The default is `True.`

`doEleJetOR`
:   whether to perform the overlap removal between electrons and jets. The default is `True.`

`doMuJetOR`
:   whether to perform the overlap removal between muons and jets. The default is `True.`

`doTauEleOR`
:   whether to perform the overlap removal between tau-jets and electrons. The default is `True.`

`doTauMuOR`
:   whether to perform the overlap removal between tau-jets and muons. The default is `True.`

`doTauJetOR`
:   whether to perform the overlap removal between tau-jets and jets. The default is `True.`

`doTauAntiTauJetOR`
:   whether to perform the overlap removal between tau-jets and anti-tau-jets. The default is `False.`

`doPhEleOR`
:   whether to perform the overlap removal between photons and electrons. The default is `True.`

`doPhMuOR`
:   whether to perform the overlap removal between photons and muons. The default is `True.`

`doPhJetOR`
:   whether to perform the overlap removal between photons and jets. The default is `True.`

`doEleFatJetOR`
:   whether to perform the overlap removal between electrons and large-R jets. The default is `True.`

`doJetFatJetOR`
:   whether to perform the overlap removal between jets and large-R jets. The default is `True.`

`addToAllSelections`
:   add OR selection decision into all object selections. The default is `True`.

`nominalOnly`
:   (experimental) toggle off the running of overlap removal on systematically-varied objects (instead, copy from nominal). The default is `False`.

`nominalOnlyUnifiedSelection`
:   (experimental) toggle off the running of overlap removal on systematically-varied objects (instead, copy from nominal), but consider the union of all systematically-varied object selections (not just nominal). The default is `False`.

!!! success "Registers the following variables:"
    - `select_or`: the per-object overlap removal decision
