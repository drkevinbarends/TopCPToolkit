<!---
## Make-methods

!!! warning
    No such method exist yet for truth-related algorithms!
--->

## Config blocks

### [truthConfig](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/python/truthConfig.py)
Name in YAML: **PartonHistory**

`histories`
:   parton-level interpretations of the MC truth record. Multiple histories can be provided as `history1.history2.etc`. See [possible values](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/python/truthConfig.py) (keys of the `truth_branches` dictionary). The following list of histories is supported:

| **History**     | **Description/Analysis** |
|----------------------|----------------|
| `Ttbar`             | Top-antitop (\( t\bar{t} \)) production. |
| `Ttbarbbbar`        | Top-antitop with additional bottom-anti-bottom (\( t\bar{t}+b\bar{b}\)) production. |
| `Ttz`               | Top-antitop pair with an additional Z boson (\( t\bar{t}Z \)). |
| `Ttw`               | Top-antitop pair with an additional W boson (\( t\bar{t}W \)). |
| `Tth`               | Top-antitop pair with a Higgs boson (\( t\bar{t}H \)). |
| `Tzq`               | Single top-quark production with an associated Z boson and a quark (\( tZq \)). |
| `Thq`               | Single top-quark production with a Higgs boson and a light quark (\( tHq \)). |
| `Tqgamma`          | Single top-quark production with a photon and a light quark (\( tq\gamma \)). |
| `Ttgamma`          | Top-antitop pair production with a photon (\( t\bar{t}\gamma \)). |
| `FourTop`          | Four-top-quark production (\( t\bar{t}t\bar{t} \)). |
| `HWW`              | Higgs boson decaying into W bosons (\( H \to WW^* \)). |
| `HWW_nonresonant`  | Higgs boson decaying into W bosons(\( H \to WW^* \)), aimed at Sherpa Samples. |
| `HZZ`              | Higgs boson decaying into Z bosons (\( H \to ZZ^* \)). |
| `Zb`               | Z boson production associated with a bottom quark (\( Z+b \)). |

!!! success "Registers history-dependent variables:"
    - in the format `history_MC_resonance_variable` (no systematics)

### [ParticleLevelElectronsConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/TruthParticleLevelAnalysisAlgorithms/python/ParticleLevelElectronsConfig.py)
Name in YAML: **PL_Electrons**

`containerName`
:   the name of the input truth electrons container. The default is `'TruthElectrons'`.

`selectionName`
:   the name of the selection to create. The default is `''` (empty string), which applies the selection to all truth electrons.

`isolated`
:   select only truth electrons that are isolated. The default is `True`.

`notFromTau`
:   select only truth electrons that did not originate from a tau-lepton decay.

!!! success "Registers the following variables:"
    - `pt`: dressed $p_\mathrm{T}$ (no systematics)
    - `eta`: dressed $\eta$ (no systematics)
    - `phi`: dressed $\phi$ (no systematics)
    - `e`: dressed $E$ (no systematics)
    - `charge`: charge (no systematics)
    - `type`: particle type according to the [`MCTruthClassifier`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/MCTruthClassifier/MCTruthClassifier/MCTruthClassifierDefs.h) (no systematics)
    - `origin`: particle origin according to the [`MCTruthClassifier`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/MCTruthClassifier/MCTruthClassifier/MCTruthClassifierDefs.h) (no systematics)

### [ParticleLevelMuonsConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/TruthParticleLevelAnalysisAlgorithms/python/ParticleLevelMuonsConfig.py)
Name in YAML: **PL_Muons**

`containerName`
:   the name of the input truth muons container. The default is `'TruthMuons'`.

`selectionName`
:   the name of the selection to create. The default is `''` (empty string), which applies the selection to all truth muons.

`isolated`
:   select only truth muons that are isolated. The default is `True`.

`notFromTau`
:   select only truth muons that did not originate from a tau-lepton decay.

!!! success "Registers the following variables:"
    - `pt`: dressed $p_\mathrm{T}$ (no systematics)
    - `eta`: dressed $\eta$ (no systematics)
    - `phi`: dressed $\phi$ (no systematics)
    - `e`: dressed $E$ (no systematics)
    - `charge`: charge (no systematics)
    - `type`: particle type according to the [`MCTruthClassifier`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/MCTruthClassifier/MCTruthClassifier/MCTruthClassifierDefs.h) (no systematics)
    - `origin`: particle origin according to the [`MCTruthClassifier`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/MCTruthClassifier/MCTruthClassifier/MCTruthClassifierDefs.h) (no systematics)

### [ParticleLevelPhotonsConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/TruthParticleLevelAnalysisAlgorithms/python/ParticleLevelPhotonsConfig.py)
Name in YAML: **PL_Photons**

`containerName`
:   the name of the input truth photons container. The default is `'TruthPhotons'`.

`selectionName`
:   the name of the selection to create. The default is `''` (empty string), which applies the selection to all truth photons.

`isolated`
:   select only truth photons that are isolated. The default is `True`.

`isolationVariable`
:   variable to use in isolation cuts of the form `var/pT < cut`. The default is `''` (empty string).

`isolationCut`
:   threshold to use in isolation cuts of the form `var/pT < cut`. The default is `-1`.

!!! success "Registers the following variables:"
    - `pt`: $p_\mathrm{T}$ (no systematics)
    - `eta`: $\eta$ (no systematics)
    - `phi`: $\phi$ (no systematics)
    - `e`: $E$ (no systematics)
    - `type`: particle type according to the [`MCTruthClassifier`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/MCTruthClassifier/MCTruthClassifier/MCTruthClassifierDefs.h) (no systematics)
    - `origin`: particle origin according to the [`MCTruthClassifier`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/MCTruthClassifier/MCTruthClassifier/MCTruthClassifierDefs.h) (no systematics)

### [ParticleLevelTausConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/TruthParticleLevelAnalysisAlgorithms/python/ParticleLevelTausConfig.py)
Name in YAML: **PL_Taus**

`containerName`
:   the name of the input truth taus container. The default is `'TruthTaus'`.

`selectionName`
:   the name of the selection to create. The default is `''` (empty string), which applies the selection to all truth taus.

`isolated`
:   select only truth taus that are isolated. The default is `True`.

`isolationVariable`
:   variable to use in isolation cuts of the form `var/pT < cut`. The default is `''` (empty string).

`isolationCut`
:   threshold to use in isolation cuts of the form `var/pT < cut`. The default is `-1`.

!!! success "Registers the following variables:"
    - `pt`: $p_\mathrm{T}$ (no systematics)
    - `eta`: $\eta$ (no systematics)
    - `phi`: $\phi$ (no systematics)
    - `e`: $E$ (no systematics)
    - `charge`: charge (no systematics)
    - `IsHadronicTau`: whether the tau decays hadronically (no systematics)
    - `type`: particle type according to the [`MCTruthClassifier`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/MCTruthClassifier/MCTruthClassifier/MCTruthClassifierDefs.h) (no systematics)
    - `origin`: particle origin according to the [`MCTruthClassifier`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/MCTruthClassifier/MCTruthClassifier/MCTruthClassifierDefs.h) (no systematics)

### [ParticleLevelNeutrinosConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/TruthParticleLevelAnalysisAlgorithms/python/ParticleLevelNeutrinosConfig.py)
Name in YAML: **PL_Neutrinos**

`containerName`
:   the name of the input truth neutrinos container. The default is `'TruthNeutrinos'`.

`selectionName`
:   the name of the selection to create. The default is `''` (empty string), which applies the selection to all truth neutrinos.

`isolated`
:   select only truth neutrinos that are isolated. The default is `True`.

`notFromTau`
:   select only truth neutrinos that did not originate from a tau-lepton decay.

!!! success "Registers the following variables:"
    - `pt`: $p_\mathrm{T}$ (no systematics)
    - `eta`: $\eta$ (no systematics)
    - `phi`: $\phi$ (no systematics)
    - `e`: $E$ (no systematics)

### [ParticleLevelJetsConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/TruthParticleLevelAnalysisAlgorithms/python/ParticleLevelJetsConfig.py)
Name in YAML: **PL_Jets**

`containerName`
:   the name of the input truth jets container. The default is `'AntiKt4TruthDressedWZJets'`.

!!! note
     For large-R jets, `AntiKt10TruthTrimmedPtFrac5SmallR20Jets` is available in `DAOD_PHYS`, and `AntiKt10TruthSoftDropBeta100Zcut10Jets` in `DAOD_PHYSLITE`.

!!! success "Registers the following variables:"
    - `pt`: $p_\mathrm{T}$ (no systematics)
    - `eta`: $\eta$ (no systematics)
    - `phi`: $\phi$ (no systematics)
    - `e`: $E$ (no systematics)
    - `nGhosts_bHadron`: number of ghost-matched b-hadrons in the jet (no systematics)
    - `nGhosts_cHadron`: number of ghost-matched c-hadrons in the jet (no systematics)
    - `num_truth_bjets_nocuts`: number of truth small-R b-jets according to [`HadronConeExclTruthLabelID`](https://ftag.docs.cern.ch/algorithms/labelling/jet_labels/#delta-r-matching-default-scheme-in-ftag) without any cuts per event (no systematics)
    - `num_truth_bjets_nocuts`: number of truth small-R c-jets according to [`HadronConeExclTruthLabelID`](https://ftag.docs.cern.ch/algorithms/labelling/jet_labels/#delta-r-matching-default-scheme-in-ftag) without any cuts per event (no systematics)

### [ParticleLevelMissingETConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/TruthParticleLevelAnalysisAlgorithms/python/ParticleLevelMissingETConfig.py)
Name in YAML: **PL_MET**

`containerName`
:   the name of the input truth MET container. The default is `'MET_Truth'`.

`outputContainerName`
:   the name of the output truth MET container. The default is `'TruthMET'`.

!!! success "Registers the following variables:"
    - `met`: the transverse missing energy $E_\mathrm{T}$ (no systematics)
    - `phi`: the transverse missing energy $\phi$ (no systematics)

!!! warning
    The MET container you should manipulate in your config (e.g. for the `Output` block) is `Truth_MET`, not `MET_Truth`!

### [ParticleLevelOverlapRemovalConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/TruthParticleLevelAnalysisAlgorithms/python/ParticleLevelOverlapRemovalConfig.py)
Name in YAML: **PL_OverlapRemoval**

`jets`
:   the name of the input truth jets container, in the format `container` or `container.selection`. The default is `''` (empty string).

`electrons`
:   the name of the input truth electrons container, in the format `container` or `container.selection`. The default is `''` (empty string).

`muons`
:   the name of the input truth muons container, in the format `container` or `container.selection`. The default is `''` (empty string).

`photons`
:   the name of the input truth photons container, in the format `container` or `container.selection`. The default is `''` (empty string).

`label`
:   the name of the decoration to apply to all particles passing OR. The default is `'passesOR'`.

`useDressedProperties`
:   whether to use dressed electron and muon kinematics rather than simple P4 kinematics. The default is `True`.

`useRapidityForDeltaR`
:   whether to use rapidity instead of pseudo-rapidity for the calculation of $\Delta R$. The default is `True`.

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

### [PartonMatchConfig](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/python/PartonMatchConfig.py)
Name in YAML: **PartonMatch**

The algorithm matches reco-level jets with partons (from different topologies). The output of the algorithm is a vector representing the topology-dependent indices of the reco-level jets matched to parton-level objects. E.g. for the $t\bar{t}$-topology, the first entry of the vector would correspond to the reco-level jet that is matched to the $b$ quark. If the jet is not matched to a parton the index will be `-1` A variable to count the number of leptons in the event is also added.
The matching algorithm recursively matches reco-level jets to parton-level objects given a selected delta R. Due to the recursive nature the objects closest in delta R will be matched first. Afterwards these objects are removed, leading to unique matches.

`jets`
:   jet collection name (string) for matching, can be reco jets or truth jets. The default is `AntiKt4TruthDressedWZJets`.

`electrons`
:   electron collection name (string) for matching, can be reco electrons or truth electrons..

`muons`
:   muon collection name (string) for matching, can be reco muons or truth muons..

`eventSelection`
:   event selection (string) for matching. The default is `pass_ejets_%SYS%,as_char||pass_mujets_%SYS%,as_char`.

`criticalJetDR`
:   maximum $\Delta R$ (float) used for matching jets. The default is `0.3`.

`criticalLeptonDR`
:   maximum $\Delta R$ (float) used for matching leptons. The default is `0.3`.

`acceptDoubleMatches`
:   whether to accept double matches within the critical $\DeltaR$. The default is `False`.

`partonContainerName`
:   name of the parton container (string) used for matching. The default is `PartonHistoryTtbar_NOSYS`.

`partonTopology`
:   Topology of the truth events. Default is `Ttbar`. Allowed options are `Ttbar`, `FourTop`.

!!! success "Registers the following variables:"
    - `event_jet_truth_idx`: vector of int describing the detector-level jet index corresponding to a topology-dependent parton-level object
    - `event_jet_truth_candidates`: vector of int describing the number of detector-level jets being matched to a topology-dependent parton-level object
    - `event_electron_truth_idx`: vector of int describing the detector-level electron index corresponding to a topology-dependent parton-level object
    - `event_electron_truth_candidates`: vector of int describing the number of detector-level electrons being matched to a topology-dependent parton-level object
    - `event_muon_truth_idx`: vector of int describing the detector-level muon index corresponding to a topology-dependent parton-level object
    - `event_muon_truth_candidates`: vector of int describing the number of detector-level muons being matched to a topology-dependent parton-level object
    - `event_nLeptons`: int describing the number of (truth) leptons in the event

    | Topology | Jet-Matching |
    | ---- | ---- |
    | $tt$ | 0: $b$, 1: $W^+_{\text{up}}$, 2: $W^+_{\text{down}}$, 3: $\bar{b}$, 4: $W^-_{\text{up}}$, 5: $W^-_{\text{down}}$ |
    | $ttZ$ | 0: $b$, 1: $W^+_{\text{up}}$, 2: $W^+_{\text{down}}$, 3: $\bar{b}$, 4: $W^-_{\text{up}}$, 5: $W^-_{\text{down}}$, 6: $Z^_{1}$, 7: $Z^_{2}$ |
    | $tttt$ | 0: $b_1$, 1: $W^+_{1,\text{up}}$, 2: $W^+_{1,\text{down}}$, 3: $\bar{b}_1$, 4: $W^-_{1,\text{up}}$, 5: $W^-_{1,\text{down}}$, 6: $b_2$, 7: $W^+_{2,\text{up}}$, 8: $W^+_{2,\text{down}}$, 9: $\bar{b}_2$, 10: $W^-_{2,\text{up}}$, 11: $W^-_{2,\text{down}}$ |

    | Topology | Lepton-Matching |
    | ---- | ---- |
    | $tt$ | 0: $t$, 1: $\bar{t}$ |
    | $tttt$ | 0: $t_1$, 1: $\bar{t}_2$, 2: $t_2$, 3: $\bar{t}_2$ |

    Note: Here $W^\pm_{\text{down}/\text{up}}$ describes the down-type/up-type quark from the $W^\pm$ boson. In the case of $t\bar{t}t\bar{t}$ these are further labeled with indeces. For the lepton cases, the corresponding parent top quark is referenced.

### [JetMatchingConfig](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/python/JetMatchingConfig.py)
Name in YAML: **JetMatching**

The algorithm adds three variables for reco jets: index of matched truth jet, $\Delta R$ to the closest reco jet and $\Delta R$ of the matched truth jet and its closest truth jet. The matching is based on looking for minimal $\Delta R$. The reco jets with no matched truth jet (no truth jet found with $\Delta R$ within the critical $\Delta R$) have the index set to -1. If multiple reco jets are matched to the same truth jet their index is also set to -1. The algorithm also labels reco jets that have a truth lepton within $\Delta R$ < 0.4 from them. If they do, it also adds the $p_\mathrm{T}$ value of that overlapping truth lepton (otherwise the value will be -1). The algorithm considers only truth leptons thats do not come from hadron decays. In order for the tool to work properly you need to set the MC truth classifier block e.g "MCTCClassification: {}" in the truth electron ("PL_Electrons") and truth muons ("PL_Muons") blocks in the config.

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

`recoIsolationMinPt`
:   minimum reco jet pT to be considered for the dR isolation for the reco jets. The default is `-1` {no additional selection}.

`truthIsolationMinPt`
:   minimum truth jet pT to be considered for the dR isolation for the truth jets. The default is `-1` {no additional selection}.

!!! success "Registers the following variables:"
    - `truth_jet_paired_index`: the index of the truth-level jets matched to the detector-level jets
    - `reco_to_reco_jet_closest_dR`: the minimum $\Delta R$ with respect to detector-level jets
    - `truth_to_truth_jet_closest_dR`: the minimum $\Delta R$ of the matched truth-level jet with respect to truth-level jets
    - `has_truth_lepton`: label for reco jets if they have an overlapping truth lepton
    - `overlapping_truth_lepton_pt`: $p_\mathrm{T}$ of the truth lepton that overlaps with the reco jet

### [IFFLeptonDecorationBlock](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AsgAnalysisAlgorithms/python/AsgAnalysisConfig.py)
Name in YAML: **X.IFFClassification**, with X amongst: Electrons, Muons, PL_Electrons, PL_Muons

`containerName`
:   the name of the input electron or muon container.

`separateChargeFlipElectrons`
:   whether to consider charged-flip electrons as a separate class. The default is `True` (recommended).

`decoration`
:   the name (str) of the decoration set by the [IFF `TruthClassificationTool`](https://gitlab.cern.ch/atlas/athena/-/tree/21.2/PhysicsAnalysis/AnalysisCommon/TruthClassification). The default is `'IFFClass_%SYS%'`.

!!! success "Registers the following variables:"
    - `IFFClass`: the decision of the [IFF `TruthClassificationTool`](https://gitlab.cern.ch/atlas/athena/-/tree/21.2/PhysicsAnalysis/AnalysisCommon/TruthClassification)  (no systematics)

### [MCTCLeptonDecorationBlock](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AsgAnalysisAlgorithms/python/AsgAnalysisConfig.py)
Name in YAML: **X.MCTCClassification**, with X amongst: Electrons, Muons, TauJets, PL_Electrons, PL_Muons, PL_Taus

`containerName`
:   the name of the input lepton container, with a possible selection, in the format `container` or `container.selection`.

`prefix`
:   the prefix (str) of the decorations based on the MCTC classification. The default is `'MCTC_'`.

!!! success "Registers the following variables:"
    - `MCTC_isPrompt`: whether the particle is classified as prompt  (no systematics)
    - `MCTC_fromHadron`: whether the particle originated from a hadron decay  (no systematics)
    - `MCTC_fromBSM`: whether the particle originated from a BSM decay  (no systematics)
    - `MCTC_fromTau`: whether the particle originated from a tau-lepton decay  (no systematics)

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

### [ParticleLevelSoftMuonsConfig](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/python/ParticleLevelSoftMuonsConfig.py)
Name in YAML: **PL_SoftMuons**

Adds soft muons at particle level. It requires some proper configuration of the PL_Muons, PL_Jets, and PL_OverlapRemoval blocks; an example of usage can be found [here](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/share/config/exampleTtbarLjets_SoftMuons/particle.yaml).
It also adds an event flag that can be use in selections to only select events with at least one particle level soft muon, by adding ```EVENTFLAG passPLSoftMuonSelection``` in the selection.
The algorithm stores basic quantities for particle level soft muons in the output, and also adds information (pdgID and 4-momenta) relative to B-hadrons, C-hadrons in the ancestor chain of the soft muon. The possible presence of a Tau in the ancestor chain of the soft muons is also stored.

`jets`
:   the name of the input truth jets container, in the format `container` or `container.selection`. The default is `''` (empty string).

`muons`
:   the name of the input truth muons container, in the format `container` or `container.selection`. The default is `''` (empty string).

`useDressedProperties`
:   whether to use dressed muon kinematics rather than simple P4 kinematics. The default is `False`.

`useRapidityForDeltaR`
:   whether to use rapidity instead of pseudo-rapidity for the calculation of $\Delta R$. The default is `True`.
