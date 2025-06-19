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

`runJERsystematicsOnData`
:   whether to run the `All`/`Full` JER model variations also on data samples. The default is `False`. Expert option!

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

`minPt`
:   the minimum $p_\mathrm{T}$ cut to apply to calibrated large-R jets. The default is 200 GeV.

`maxPt`
:   the maximum $p_\mathrm{T}$ cut to apply to calibrated large-R jets. The default is 3000 GeV.

`maxEta`
:   the maximum $\vert\eta\vert$ cut to apply to calibrated large-R jets. The default is 2.

`minMass`
:   the minimum mass cut to apply to calibrated large-R jets. The default is 40 GeV.

`maxMass`
:   the maximum mass cut to apply to calibrated large-R jets. The default is 600 GeV.

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
:   the flavour tagging algorithm: `DL1dv01`, `GN2v01`. The default is `GN2v01`.

`btagWP`
:   the flavour tagging WP: `Continuous`, `FixedCutBEff_XX` (for $XX\%$ b-tagging efficiency). The default is `Continuous`.

`bTagCalibFile`
:   path (string) to a custom b-tagging CDI file. The default is `None`, which uses the latest available recommendations.

`saveScores`
:   whether or not to save the scores from the tagger. Set to `'True'` to save only the overall score, or to `'All'` to save also the per-flavour probabilities. The default is `''` (emptry string), i.e. don't save the scores.

`saveCustomVariables`
:   [Expert mode] additional variables (list of strings) to save from the b-tagging object associated to each jet. E.g. `['pb','pc','pu', 'ptau']` to replicate `saveScores='All'`. The default is `[]` (empty list).

!!! success "Registers the following variables (all names preceded by the tagger + WP combination):"
    - `select`: the per-jet tagging decision (no systematics)
    - `quantile`: only for pseudo-continuous b-tagging, the per-jet PCBT bin (no systematics)

!!! success "Additional variables toggled by `saveScores`:"
    - `{btagger}`: the per-jet discriminant from tagger `{btagger}` (no systematics)
    - `{btagger}_pu`: the per-jet light-flavour probability from tagger	`{btagger}` (no systematics)
    - `{btagger}_pc`: the per-jet charm-flavour probability from tagger	`{btagger}` (no systematics)
    - `{btagger}_pb`: the per-jet bottom-flavour probability from tagger `{btagger}` (no systematics)
    - `{btagger}_ptau`: the per-jet tau-jet-flavour probability from tagger `{btagger}` (no systematics)

### [FTagJetSFBlock](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/FTagAnalysisAlgorithms/python/FTagSFAnalysisConfig.py)
Name in YAML: **Jets.FlavourTaggingEventSF**

Computes the per-jet b-tagging SF in the specified jet container, within the region of validity of the FTAG calibrations. See the `containerName` argument below for passing jets with specific selection. In YAML, also sets up the per-event b-tagging SF config block (see below).

`containerName`
:   the input jet container with a possible selection, in the format `container` or `container.selection`. The default recommendation is to pass `container.baselineJvt` selection, e.g. if the calibrated jets container is `AnaJets`, the recommendation is to pass `AnaJet.baselineJvt`.

`selectionName`
:   a postfix to apply to decorations and algorithm names. Typically not needed here as internally the string `f"{btagger}_{btagWP}"` is used.

`btagger`
:   the flavour tagging algorithm: `DL1dv01`, `GN2v01`. The default is `GN2v01`.

`btagWP`
:   the flavour tagging WP, e.g. `FixedCutBEff_77`, `Continuous`, etc. The default is `Continuous`.

`generator`
:   MC generator setup, for MC/MC SFs. The default is `"autoconfig"` (relies on the sample metadata). To override, a generator string is expected, see [the lists of recognized strings for Run2 and Run3](https://gitlab.cern.ch/atlas/athena/-/blob/release/25.2.41/PhysicsAnalysis/JetTagging/JetTagPerformanceCalibration/CalibrationDataInterface/python/MCMCGeneratorHelper.py?ref_type=tags#L29-50).

    !!! note
        The link above is for release `25.2.7`. The list of supported generators could be updated between releases. The latest ones can be found by switching to the `main` branch.

    Also see [MC/MC Scale Factors using Top Samples](https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/PmgTopProcesses#FTAG_MC_MC_Scale_Factors_using_T).

`noEffSF`
:   disables the calculation of efficiencies and scale factors. Experimental! only useful to test a new WP for which scale factors are not available. The default is `False`.

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

`savePerJetSF`
:   whether or not to save the per-jet b-tagging efficiency SF as output variable. The default is `False`.

!!! success "Registers the following variables:"
    - `weight_ftag_effSF_{btagger}_{btagWP}`: the per-event b-tagging efficiency SF

!!! success "Additional variables toggled by `savePerJetSF`:"
    - `{btagger}_{btagWP}_eff`: the per-jet b-tagging efficiency SF

### [FTagEventSFBlock](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/FTagAnalysisAlgorithms/python/FTagSFAnalysisConfig.py)
Name in YAML: **Jets.FlavourTaggingEventSF**

Computes the per-event b-tagging SF, i.e. a product of b-tagging efficiency/inefficiency SFs over all jets in the specified jet container, which are within the region of validity of the FTAG calibrations. See the `containerName` argument below for passing jets with specific selection. The per-event scale factor `weight_ftag_effSF_<selectionName>` is decorated to `EventInfo` (see `selectionName` below).

`containerName`
:   the input jet container with a possible selection, in the format `container` or `container.selection`. The default recommendation is to pass `container.baselineJvt` selection, e.g. if the calibrated jets container is `AnaJets`, the recommendation is to pass `AnaJet.baselineJvt`.

`selectionName`
:   a postfix to apply to decorations and algorithm names. Typically not needed here as internally the string `f"{btagger}_{btagWP}"` is used.

`btagger`
:   the flavour tagging algorithm: `DL1dv01`, `GN2v01`. The default is `GN2v01`.

`btagWP`
:   the flavour tagging WP, e.g. `FixedCutBEff_77`, `Continuous`, etc. The default is `Continuous`.

### [BJetCalibAnalysisConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/JetAnalysisAlgorithms/python/BJetCalibAnalysisConfig.py)
Name in YAML: **Jets.BJetCalib**

Corrections to the b-jet momentum to account also for muon-in-jet calibration, as developped in the $VHbb$ and $HH$ analyses. See the [BJetCalibrationTool](https://gitlab.cern.ch/r3hh-public/b-jet-energy-corrections) and [this presentation](https://indico.cern.ch/event/1401273/contributions/5890523/attachments/2834123/4952335/B-jet%20calibration%20in%20HH%20muon-in-jet%20and%20PtReco-1.pdf) for further information.

`containerName`
:   the name of the input jet container.

`muonContainerName`
:   the name of the input muon container.

`jetPreselection`
:   the jet preselection.

`muonPreselection`
:   the muon preselection.

`doPtCorr`
:   whether to run the b-jet $p_\mathrm{T}$ correction on top of the muon-in-jet one. The default is `True`.

### [JetReclusteringConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/JetAnalysisAlgorithms/python/JetReclusteringConfig.py)
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
:   minimum $p_\mathrm{T}$ requirement (in MeV) on the reclustered jets, creating the selection `passed_sel`. The default is 20 GeV.

`maxEta`
:   maximum $\eta$ requirement on the reclustered jets, creading the selection `passed_sel`. The default is 2.5.

!!! success "Registers the following variables:"
    - `pt`: reclustered jet $p_\mathrm{T}$
    - `eta`: reclustered jet $\eta$
    - `phi`: reclustered jet $\phi$
    - `e`: reclustered jet $E$
    - `small_r_jet_indices`: vector of indices of the small-R jets clustered into this large-R jet


### [XbbTaggingConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/FTagAnalysisAlgorithms/python/XbbAnalysisConfig.py)
Name in YAML: **Jets.XbbTagging**

The algorithm tags large-R jets using the [Xbb taggers](https://xbb-docs.docs.cern.ch/Xbb/taggers/).
It writes out the Xbb tagging decision on a jet and its SF factor.

`containerName`
:   the name of the input container.

`Xbbtagger`
:   the Xbb tagging algorithm, the default is `GN2Xv01`

`XbbWP`
:   the Xbb tagging WP. The default is `FlatMassQCDEff_0p25`.

`calibFile`
:   path (string) to a Xbb JSON calibration file.

!!! success "Registers the following variables:"
    - `jet_Xbbtagger_XbbWP_select`: tagger decision on the jet
    - `jet_Xbbtagger_XbbWP_effSF`: SF per jet


### [BoostedJetTaggersConfig](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/python/BoostedJetTaggersConfig.py)
Name in YAML: **BoostedJetTaggers**

The algorithm tags large-R jets using the [BoostedJetTaggers](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/Reconstruction/Jet/BoostedJetTaggers/BoostedJetTaggers) package. More details about taggers can be found on [JetTaggingAndScaleFactors twiki](https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/JetTaggingAndScaleFactors).
The boosted jet taggers are enabled by adding a module into AddConfigBlocks block:
```
AddConfigBlocks:
    - modulePath: 'TopCPToolkit.BoostedJetTaggersConfig'
          functionName: 'BoostedJetTaggersConfig'
          algName: 'BoostedJetTaggers'
          pos: 'Output'
```

It is possible to define multiple different taggers.

`ljets`
:   The input large-R jet collection to tag jets. No default value.

`taggerType`
: Selected tagger type. Avaiable options: `JSSWTopTaggerDNN`, `JSSWTopTaggerANN`. Default value: `JSSWTopTaggerDNN`.

`taggerCalibArea`
:  Directory containing taggers with calibrations released at the same time. Default value: `Winter2024_R22_PreRecs/JSSWTopTaggerDNN/`.

`taggerConfigFile`
: Selected tagger config file from the calib area. Default value: `DNNTagger_AntiKt10UFOSD_TopContained80_Oct30.dat`.

`taggerDecoration`
: Tagger decoration is defined in the selected `taggerConfigFile`. Open the selected config file and copy the `DecorationName`. The file can be found under path relative to [atlas-groupdata/BoostedJetTaggers](https://atlas-groupdata.web.cern.ch/atlas-groupdata/BoostedJetTaggers/) or `/cvmfs/atlas.cern.ch/repo/sw/database/GroupData/BoostedJetTaggers/`. Default value: `DNNTaggerTopQuarkContained80`.

!!! success "Registers the following variables:"
- `ljet_<taggerDecoration>_tagged`
- `ljet_<taggerDecoration>_score`
- `ljet_<taggerDecoration>_passMass`
- `ljet_<taggerDecoration>_validKinRange`
- `ljet_<taggerDecoration>_SF`
- `ljet_<taggerDecoration>_efficiency`
- `ljet_<taggerDecoration>_efficiencySF`

### [EventShapeConfig](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/python/EventShapeConfig.py)
Name in YAML: **EventShape**

The algorithm stores the variable `Density` , which is the average event density for energy flow/pileup subtraction, from the EventShape class and decorates EventInfo with it.
The algorithm is enabled by adding a module into AddConfigBlocks block:
```
AddConfigBlocks:
    - modulePath: 'TopCPToolkit.EventShapeConfig'
          functionName: 'EventShapeConfig'
          algName: 'EventShape'
          pos: 'Output'
```

`eventShape`
:  Event shape variable to restore. Available options are `Kt4EMPFlowEventShape`,`Kt4EMPFlowPUSBEventShape`,`Kt4EMTopoOriginEventShape` .The default is `Kt4EMPFlowNeutEventShape` which restores the variable `Kt4EMPFlowNeutEventShape.Density`.


!!! success "Registers the following variables:"
- `event_shape_density`
