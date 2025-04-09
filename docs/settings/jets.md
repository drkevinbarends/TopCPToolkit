## Make-methods

### [makeJetAnalysisConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/JetAnalysisAlgorithms/python/JetAnalysisConfig.py)
Name in YAML: **Jets**

`seq`
:   the config sequence.

`containerName`
:   the name of the output container after calibration.

`jetCollection`
:   the jet container to run on. It is interpreted to determine the correct config blocks to call for small- or large-R jets.

`runOriginalObjectLink`
:   sets up an instance of [`CP::AsgOriginalObjectLinkAlg`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AsgAnalysisAlgorithms/Root/AsgOriginalObjectLinkAlg.cxx) to link systematically-varied containers to the base one. The default is `False`.

`runGhostMuonAssociation`
:   whether to set up the jet-ghost-muon association algorithm [`CP::JetGhostMuonAssociationAlg`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/JetAnalysisAlgorithms/Root/JetGhostMuonAssociationAlg.cxx). The default is `False`.

`runTruthJetTagging`
:   whether to set up the jet truth tagging algorithm [`CP::JetTruthTagAlg`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/JetAnalysisAlgorithms/Root/JetTruthTagAlg.cxx). The default is `True`.

!!! success "Registers the following variables:"
    - `pt`: jet $p_\mathrm{T}$
    - `m`: jet mass (for large-R jets)
    - `eta`: jet $\eta$ (no systematics)
    - `phi`: jet $\phi$ (no systematics)
    - `e`: jet $E$ (*since AnalysisBase 25.2.13*)

#### Additional options for small-R jets

`runJvtUpdate`
:   whether to update the JVT. The default is `False`.

`runNNJvtUpdate`
:   whether to update the NN-JVT. The default is `False`.

`runFJvtUpdate`
:   whether to update the forward JVT. The default is `False`.

`runJvtSelection`
:   whether to run JVT selection. The default is `True`.

`runFJvtSelection`
:   whether to run forward JVT selection. The default is `False`.

`jvtWP`
:   which Jvt WP to apply. The default is `FixedEffPt`.

`fJvtWP`
:   which fJvt WP to apply. The default is `Loose`.

`runJvtEfficiency`
:   whether to calculate the JVT efficiency. The default is `True`.

`runFJvtEfficiency`
:   whether to calculate the forward JVT efficiency. The default is `False`.

`systematicsModelJES`
:   the NP reduction scheme to use for JES: `All`, `Global`, `Category`, `Scenario`. The default is `Category`.

`systematicsModelJER`
:   the NP reduction scheme to use for JER: `All`, `Full`, `Simple`. The default is `Full`.

    !!! note
        Not all combinations of `systematicsModelJES` and `systematicsModelJER` are valid! Please refer to the [JetETmiss twiki](https://twiki.cern.ch/twiki/bin/view/AtlasProtected/JetUncertaintiesRel22).

`recalibratePhyslite`
:   whether to run the [`CP::JetCalibrationAlg`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/JetAnalysisAlgorithms/Root/JetCalibrationAlg.cxx) on PHYSLITE derivations. The default is `True`.

`calibToolConfigFile`
:   name (str) of the config file to use for the jet calibration tool. Expert option to override JetETmiss recommendations. The default is `None`.

`calibToolCalibArea`
:   name (str) of the CVMFS area to use for the jet calibration tool. Expert option to override JetETmiss recommendations. The default is `None`.

`calibToolCalibSeq`
:   name (str) of the sequence to use for the jet calibration tool (e.g. `'JetArea_Residual_EtaJES_GSC'`). Expert option to override JetETmiss recommendations. The default is `None`.

`uncertToolConfigPath`
:   name (str) of the config file to use for the jet uncertainty tool. Expert option to override JetETmiss recommendations. The default is `None`.

`uncertToolCalibArea`
:   name (str) of the CVMFS area to use for the jet uncertainty tool. Expert option to override JetETmiss recommendations. The default is `None`.

`uncertToolMCType`
:   data type (str) to use for the jet uncertainty tool (e.g. `'AFII'` or `'MC16'`). Expert option to override JetETmiss recommendations. The default is `None`.

!!! success "Registers the following variables:"
    - `jvtEfficiency`: the per-jet JVT efficiency SF (also for fJVT!)
    - `select_baselineJvt`: the per-jet selection flag for baseline selection and JVT
    - `select_baselineFJvt`: the per-jet selection flag for baseline selection and forward JVT

#### Additional options for large-R jets

`largeRMass`
:   which large-R mass definition to use: `Comb`, `Calo`, `TA`. The default is `Comb`.

`configFileOverride`
:   name (string) of the config file to use for the calibration tool, instead of the recommended one. The default is `''` (empty string).

## Config blocks

### [JetJvtAnalysisConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/JetAnalysisAlgorithms/python/JetJvtAnalysisConfig.py)
Name in YAML: **Jets.JVT**

`containerName`
:   the name of the input container.

`postfix`
:   a postfix to apply to decorations and algorithm names. Typically not needed here.

`enableFJvt`
:   whether to enable forward JVT calculations. The default is `False`.

!!! success "Registers the following variables:"
    - `weight_jvt_effSF`: the event-level JVT efficiency SF
    - `weight_fjvt_effSF`: the event-level forward JVT efficiency SF

### [FTagConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/FTagAnalysisAlgorithms/python/FTagAnalysisConfig.py)
Name in YAML: **Jets.FlavourTagging**

`containerName`
:   the name of the input container.

`selectionName`
:   a postfix to apply to decorations and algorithm names. Typically not needed here as internally the string `f"{btagger}_{btagWP}"` is used.

`btagger`
:   the flavour tagging algorithm: `DL1dv01`, `GN2v00`. The default is `DL1r`.

`btagWP`
:   the flavour tagging WP. The default is `FixedCutBEff_77`.

`generator`
:   MC generator setup, for MC/MC SFs. The default is `"autoconfig"` (relies on the sample metadata). To override, a generator string is expected, see [the lists of recognized strings for Run2 and Run3](https://gitlab.cern.ch/atlas/athena/-/blob/release/25.2.7/PhysicsAnalysis/Algorithms/FTagAnalysisAlgorithms/python/FTagAnalysisConfig.py?ref_type=tags#L198-203).

    !!! note
        The link above is for release `25.2.7`. The list of supported generators could be updated between releases. The latest ones can be found by switching to the `main` branch.

    Also see [MC/MC Scale Factors using Top Samples](https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/PmgTopProcesses#FTAG_MC_MC_Scale_Factors_using_T).

`noEffSF`
:   disables the calculation of efficiencies and scale factors. Experimental! only useful to test a new WP for which scale factors are not available. The default is `False`.

`bTagCalibFile`
:   path (string) to a custom b-tagging CDI file. The default is `None`, which uses the latest available recommendations.

`systematicsStrategy`
:   name of systematics model; presently choose between 'SFEigen' (default) and 'Envelope'.

`eigenvectorReductionB`
:   b-jet scale factor Eigenvector reduction strategy; choose between `Loose` (default), `Medium`, `Tight`.

`eigenvectorReductionC`
:   b-jet scale factor Eigenvector reduction strategy; choose between `Loose` (default), `Medium`, `Tight`.

`eigenvectorReductionLight`
:   b-jet scale factor Eigenvector reduction strategy; choose between `Loose` (default), `Medium`, `Tight`.

`excludeFromEigenVectorTreatment`
:   (semicolon-separated) names of uncertainties to be excluded from all eigenvector decompositions (if used). The default is `''` (empty string).

`excludeFromEigenVectorBTreatment`
:   (semicolon-separated) names of uncertainties to be excluded from b-jet eigenvector decompositions (if used). The default is `''` (empty string).

`excludeFromEigenVectorCTreatment`
:   (semicolon-separated) names of uncertainties to be excluded from c-jet eigenvector decompositions (if used). The default is `''` (empty string).

`excludeFromEigenVectorLightTreatment`
:   (semicolon-separated) names of uncertainties to be excluded from light-flavour-jet eigenvector decompositions (if used). The default is `''` (empty string).

`excludeRecommendedFromEigenVectorTreatment`
:   whether or not to add recommended lists to the user specified eigenvector decomposition exclusion lists. The default is `False`.

`saveScores`
:   whether or not to save the scores from the tagger. Set to `'True'` to save only the overall score, or to `'All'` to save also the per-flavour probabilities. The default is `''` (emptry string), i.e. don't save the scores.

`saveCustomVariables`
:   [Expert mode] additional variables (list of strings) to save from the b-tagging object associated to each jet. E.g. `['pb','pc','pu', 'ptau']` to replicate `saveScores='All'`. The default is `[]` (empty list).

!!! success "Registers the following variables (all names preceded by the tagger + WP combination):"
    - `select`: the per-jet tagging decision (no systematics)
    - `quantile`: only for pseudo-continuous b-tagging, the per-jet PCBT bin (no systematics)
    - `eff`: the per-jet b-tagging efficiency SF

!!! success "Additional variables toggled by `saveScores`"
    - `{btagger}`: the per-jet discriminant from tagger `{btagger}` (no systematics)
    - `{btagger}_pu`: the per-jet light-flavour probability from tagger	`{btagger}` (no systematics)
    - `{btagger}_pc`: the per-jet charm-flavour probability from tagger	`{btagger}` (no systematics)
    - `{btagger}_pb`: the per-jet bottom-flavour probability from tagger `{btagger}` (no systematics)
    - `{btagger}_ptau`: the per-jet tau-jet-flavour probability from tagger `{btagger}` (no systematics)

### [FTagEventSFConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/FTagAnalysisAlgorithms/python/FTagEventSFnalysisConfig.py)
Name in YAML: **Jets.FlavourTaggingEventSF**

Computes the per-event b-tagging SF, i.e. a product of b-tagging efficiency/inefficiency SFs over all jets in the specified jet container, which are within the region of validity of the FTAG calibrations. See the `containerName` argument below for passing jets with specific selection. The per-event scale factor `weight_ftag_effSF_<selectionName>` is decorated to EventInfo object (see `selectionName` below).

`containerName`
:   the input jet container with a possible selection, in the format `container` or `container.selection`. The default recommendation is to pass `container.baselineJvt` selection, e.g. if the calibrated jets container is `AnaJets`, the recommendation is to pass `AnaJet.baselineJvt`.

`selectionName`
:   a postfix to apply to decorations and algorithm names. Typically not needed here as internally the string `f"{btagger}_{btagWP}"` is used.

`btagger`
:   the flavour tagging algorithm: `DL1dv01`, `GN2v00`.

`btagWP`
:   the flavour tagging WP, e.g. `FixedCutBEff_77`, `Continuous`, etc.

!!! success "Registers the following variables:"
    - `weight_ftag_effSF_{btagger}_{btagWP}`: the per-event b-tagging efficiency SF

### [JetReclusteringConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/JetAnalysisAlgorithms/JetReclusteringConfig.py)
Name in YAML: **JetReclustering**

The algorithm to run FastJet with small-R jets as an input. The output of the algorithm is a new container containing the reclustered jets.

`containerName`
:   name of the output reclustered jets container.

`jets`
:   the input jet collection to recluster, with a possible selection, in the format `container` or `container.selection`.

`clusteringAlgorithm`
:   algorithm (string) to use to recluster the jets: `AntiKt`, `Kt`, `CamKt`.

`reclusteredJetsRadius`
:   radius parameter of the reclustering algorithm. The default is 1.0.

`minPt`
:   minimum $p_\mathrm{T}$ requirement (in MeV) on the reclustered jets, creating the selection `passed_pt`. The default is 20 GeV.

!!! success "Registers the following variables:"
    - `pt`: reclustered jet $p_\mathrm{T}$
    - `eta`: reclustered jet $\eta$
    - `phi`: reclustered jet $\phi$
    - `e`: reclustered jet $E$
    - `small_r_jet_indices`: vector of indices of the small-R jets clustered into this large-R jet
