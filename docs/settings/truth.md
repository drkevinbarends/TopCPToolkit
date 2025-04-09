<!---
## Make-methods

!!! warning
    No such method exist yet for truth-related algorithms!
--->

## Config blocks

### [truthConfig](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/python/truthConfig.py)
Name in YAML: **PartonHistory**

`histories`
:   parton-level interpretations of the MC truth record. Multiple histories can be provided as `history1.history2.etc`. See [possible values](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/python/truthConfig.py) (keys of the `truth_branches` dictionary).

!!! success "Registers history-dependent variables:"
    - in the format `history_MC_resonance_variable` (no systematics)

### [particleLevelConfig](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/python/particleLevelConfig.py)
Name in YAML: **ParticleLevel**

`useTruthElectrons`
:   whether to use electrons. The default is `True`.

`elPtMin`
:   minimum electron $p_\mathrm{T}$, in MeV. The default is 25 GeV.

`elEtaMax`
:   maximum electron $\vert\eta\vert$. The default is 2.5.

`elNotFromHadron`
:   whether to reject electrons originating from hadronic decays. The default is `True` (reject them).

`elTauIsHadron`
:   whether to consider tauons as hadrons in the above rejection. The default is `False` (don't consider them as hadrons).

`useTruthMuons`
:   whether to use muons. The default is `True`.

`muPtMin`
:   minimum muon $p_\mathrm{T}$, in MeV. The default is 25 GeV.

`muEtaMax`
:   maximum muon $\vert\eta\vert$. The default is 2.5.

`muNotFromHadron`
:   whether to reject muons originating from hadronic decays. The default is `True` (reject them).

`muTauIsHadron`
:   whether to consider tauons as hadrons in the above rejection. The default is `False` (don't consider them as hadrons).

`useTruthPhotons`
:   whether to use photons. The default is `False`.

`phPtMin`
:   minimum photon $p_\mathrm{T}$, in MeV. The default is 25 GeV.

`phEtaMax`
:   maximum photon $\vert\eta\vert$. The default is 2.5.

`phOrigin`
:   comma-separated list (string) of particle origin values as given by the `MCTruthClassifier`. See [possible values](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/MCTruthClassifier/MCTruthClassifier/MCTruthClassifierDefs.h). The default is `''` (empty string).

`phIsolation`
:   space-separated isolation variable and isolation cut (string), i.e. in the format `var cut` where `var` is an isolation variable and `cut` is applied as `var / pt < cut`. The default is `''` (empty string).

`useTruthTaus`
:   whether to use taus. The default is `False.`

`tauPtMin`
:   minimum tau $p_\mathrm{T}$, in MeV. The default is 25 GeV.

`tauEtaMax`
:   maximum tau $\vert\eta\vert$. The default is 2.5.

`useTruthJets`
:   whether to use jets. The default is `True`.

`jetPtMin`
:   minimum jet $p_\mathrm{T}$, in MeV. The default is 25 GeV.

`jetEtaMax`
:   maximum jet $\vert\eta\vert$. The default is 2.5.

`useTruthLargeRJets`
:   whether to use large-R jets. The default is `True`.

`ljetPtMin`
:   minimum large-R jet $p_\mathrm{T}$, in MeV. The default is 25 GeV.

`ljetEtaMax`
:   maximum large-R jet $\vert\eta\vert$. The default is 2.5.

`ljetCollection`
:   the large-R jet collection to run on. The default is `AntiKt10TruthTrimmedPtFrac5SmallR20Jets` for `DAOD_PHYS`, and `AntiKt10TruthSoftDropBeta100Zcut10Jets` for `DAOD_PHYSLITE`.

`useTruthNeutrinos`
:   whether to use neutrinos. The default is `False`.

`nuPtMin`
:   minimum neutrino $p_\mathrm{T}$, in MeV. The default is 0 GeV.

`nuEtaMax`
:   maximum neutrino $\vert\eta\vert$. The default is 10.

`useTruthMET`
:   whether to use MET. The default is `True`.

`doOverlapRemoval`
:   whether to perform overlap removal. The default is `True`.

!!! success "Registers the following variables, depending on the object:"
    - `pt`: $p_\mathrm{T}$ (no systematics)
    - `eta`: $\eta$ (no systematics)
    - `phi`: $\phi$ (no systematics)
    - `e`: $E$ (no systematics)
    - `type`: particle type according to the [`MCTruthClassifier`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/MCTruthClassifier/MCTruthClassifier/MCTruthClassifierDefs.h) (no systematics)
    - `origin`: particle origin according to the [`MCTruthClassifier`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/MCTruthClassifier/MCTruthClassifier/MCTruthClassifierDefs.h) (no systematics)
    - `charge`: charge (no systematics)
    - `IsHadronicTau`: whether the tau decays hadronically (no systematics)
    - `nGhosts_bHadron`: number of ghost-matched b-hadrons in the jet (no systematics)
    - `nGhosts_cHadron`: number of ghost-matched c-hadrons in the jet (no systematics)
    - `num_truth_bjets_nocuts`: number of truth b-jets according to [`HadronConeExclTruthLabelID`](https://ftag.docs.cern.ch/algorithms/labelling/jet_labels/#delta-r-matching-default-scheme-in-ftag) without any cuts per event (no systematics)
    - `num_truth_bjets_nocuts`: number of truth c-jets according to [`HadronConeExclTruthLabelID`](https://ftag.docs.cern.ch/algorithms/labelling/jet_labels/#delta-r-matching-default-scheme-in-ftag) without any cuts per event (no systematics)

### [TtbarNNLORecursiveRewConfig](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/python/TtbarNNLORecursiveRewConfig.py)
Name in YAML: **TtbarNNLO**

`sampleID`
:   force the DSID (string) to be used to configure the reweighting. The default is `AutoConfig`, i.e. use the file metadata.

`reweightType`
:   the kind of reweighting to perform (string). Supported options are: `2D`, `3D`. The default is `2D`.

`reweightPath`
:   the path (string) to the directory holding the histograms for reweighting. The default is `dev/AnalysisTop/TTbarNNLOReweighter`.

`reweightSuffix`
:   the suffix (string) to use when looking for the histograms. The default is `_3iter_2023_LUX`.

`nominalOnly`
:   whether to disable systematics and run only the nominal NNLO reweighting. The default is `True` (PMG recommendation: compare only the nominal NNLO weight).

!!! success "Registers the following variables:"
    - `NNLO_type_weight`: NNLO weight (overall normalised to 1), where `type` reflects the `reweightType` option

### [PartonToJetsMatchConfig](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/python/PartonToJetsMatchConfig.py)
Name in YAML: **PartonToJetsMatch**

The algorithm matches jets with partons from ttbar lepton+jets topology. The output of the algorithm are four variables representing the indices (for the jets) of the b-jets from hadronically and semileptonically decaying tops, jets matched to the up-type and down-type quarks from the hadronic W decay. Finally, a flag is added for dilepton events.
The matching algorithm looks jets that are within the selected delta R. If more than one jet passed the criterium, the index is set to -1. For multiple objects matching the same index (or for dilepton events) the multi-matched objects have index set to -1.

`jets`
:   jet collection name (string) for matching, can be reco jets or truth jets. The default is `AntiKt4TruthDressedWZJets`.

`eventSelection`
:   event selection (string) for matching. The default is `pass_ejets_%SYS%,as_char||pass_mujets_%SYS%,as_char`.

`criticalDR`
:   maximum $\Delta R$ (float) used for matching. The default is `0.3`.

`partonContainerName`
:   name of the parton container (string) used for matching. The default is `TopPartonHistoryTtbar_NOSYS`.

!!! success "Registers the following variables:"
    - `parton_truth_lep_b_index`: the detector-level jet index corresponding to the parton-level leptonic b-quark
    - `parton_truth_had_b_index`: the detector-level jet index corresponding to the parton-level hadronic b-quark
    - `parton_truth_up_index`: the detector-level jet index corresponding to the parton-level up-type-quark from the hadronic-W decay
    - `parton_truth_down_index`: the detector-level jet index corresponding to the parton-level down-type-quark from the hadronic-W decay
    - `event_is_dilepton`: flags whether the parton-level event is dileptonic

### [JetMatchingConfig](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/python/JetMatchingConfig.py)
Name in YAML: **JetMatching**

The algorithm adds three variables for reco jets: index of matched truth jet, $\Delta R$ to the closest reco jet and $\Delta R$ of the matched truth jet and its closest truth jet. The matching is based on looking for minimal $\Delta R$. The reco jets with no matched truth jet (no truth jet found with $\Delta R$ within the critical $\Delta R$) have the index set to -1. If multiple reco jets are matched to the same truth jet their index is also set to -1. The algorithm also labels reco jets that have a truth lepton within $\Delta R$ < 0.4 from them. If they do, it also adds the $p_\mathrm{T}$ value of that overlapping truth lepton (otherwise the value will be -1).

`criticalDR`
:   maximum $\Delta R$ (float) used for matching. The default is `0.3`.

`criticalDR_leptons`
:   min $\Delta R$ (float) required between reco jets and truth leptons. The default is `0.4`.

`jets`
:   reco jets container name (string). The default is `AnaJets`.

`truthJets`
:   truth jets collection name (string). The default is `AntiKt4TruthDressedWZJets`.

`truthElectrons`
:   truth electrons collection name (string). The default is None.

`truthMuons`
:   truth muons collection name (string). The default is None.

`eventSelection`
:   event selection (string) to run the algorithm on. The default is ` ` (run over all events).

!!! success "Registers the following variables:"
    - `truth_jet_paired_index`: the index of the truth-level jets matched to the detector-level jets
    - `reco_to_reco_jet_closest_dR`: the minimum $\Delta R$ with respect to detector-level jets
    - `truth_to_truth_jet_closest_dR`: the minimum $\Delta R$ of the matched truth-level jet with respect to truth-level jets
    - `has_truth_lepton`: label for reco jets if they have an overlapping truth lepton
    - `overlapping_truth_lepton_pt`: $p_\mathrm{T}$ of the truth lepton that overlaps with the reco jet

### [IFFLeptonDecorationBlock](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AsgAnalysisAlgorithms/python/AsgAnalysisConfig.py)
Name in YAML: **X.IFFClassification**, with X amongst: Electrons, Muons

`containerName`
:   the name of the input electron or muon container.

`separateChargeFlipElectrons`
:   whether to consider charged-flip electrons as a separate class. The default is `True` (recommended).

`decoration`
:   the name (str) of the decoration set by the [IFF `TruthClassificationTool`](https://gitlab.cern.ch/atlas/athena/-/tree/21.2/PhysicsAnalysis/AnalysisCommon/TruthClassification). The default is `'IFFClass_%SYS%'`.

!!! success "Registers the following variables:"
    - `IFFClass`: the decision of the [IFF `TruthClassificationTool`](https://gitlab.cern.ch/atlas/athena/-/tree/21.2/PhysicsAnalysis/AnalysisCommon/TruthClassification)  (no systematics)

### [PartonHistoryToSpinInputConfig](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/python/PartonHistoryToSpinInputConfig.py)
Name in YAML: **SpinInputs**

Takes a parton history as input and prepares the 4-vectors of the top/anti-top and their respective spin analysers.

`history`
:   the top parton history (string) to read. The default is `None`.

`top`
:   the name (string) to give the 4-vector for the top quark. The default is `'truth_top_4vect_%SYS%'`.

`tbar`
:   the name (string) to give the 4-vector for the anti-top quark. The default is `'truth_tbar_4vect_%SYS%'`.

`top_decay`
:   the name (string) to give the 4-vector for the decay product of the top quark. The default is `'truth_top_decay_4vect_%SYS%'`.

`tbar_decay`
:   the name (string) to give the 4-vector for the decay product of the anti-top quark. The default is `'truth_tbar_decay_4vect_%SYS%'`.

### [CARConfig](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/python/CARConfig.py)
Name in YAML: **CAR**

Performs the "custom angle replacement" (CAR) [algorithm](https://journals.aps.org/prd/pdf/10.1103/PhysRevD.106.115021). An example can be found [here](https://journals.aps.org/prd/pdf/10.1103/PhysRevD.107.076016).

`history`
:   the parton history (string) to read. Possible options are `HWW` and `HZZ`.

!!! success "Registers history-dependent variables:"
    - in the format `history_MC_resonance_CAR_variable` (no systematics)
    - e.g. `HWW_MC_Hdecay1_decay1_CAR_pt`