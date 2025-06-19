<!---
## Make-methods

!!! warning
    No such method exist yet for event reconstruction algorithms!
--->

## Config blocks

### [DiTauMassBlock](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/TauAnalysisAlgorithms/python/DiTauMassConfig.py)
Name in YAML: **DiTauMMC**

`algName`
:   optional name to distinguish between multiple instances of the algorithm. The default is `''` (empty string).'

`electrons`
:   the input electron container, with a possible selection, in the format `container` or `container.selection`. The default is `''` (empty string).

`muons`
:   the input muon container, with a possible selection, in the format `container` or `container.selection`. The default is `''` (empty string).

`taus`
:   the input tau-jet container, with a possible selection, in the format `container` or `container.selection`. The default is `''` (empty string).

`jets`
:   the input jet container, with a possible selection, in the format `container` or `container.selection`. The default is `''` (empty string).

`met`
:   the input MET container. The default is `''` (empty string).

`eventSelection`
:   optional event filter to run on. The default is `''` (empty string), i.e. all events.

`saveExtraVariables`
:   whether to save additional output information from the MMC. The default is `False`.

`floatStopCriterion`
:   whether to activate the floating stopping criterion. The default is `True`.

`calibration`
:   the calibration set (string) to use. The default is `'2019'` (recommended).

`nSigmaMet`
:   the number (int) of sigmas for the MET resolution scan. The default is `-1` (no scan).

`useTailCleanup`
:   whether to activate the tail cleanup feature. The default is `-1`.

`niterFit2`
:   the number of iterations for each MET scan loop. The default is `-1`.

`niterFit3`
:   the number of iterations for each Mnu loop. The default is `-1`.

`useTauProbability`
:   whether to apply tau probability (additional PDF term corresponding to the ratio of the neutrino momentum to the reconstructed tau momentum). The default is `1`.

`useMnuProbability`
:   whether to apply $m_\nu$ probability (additional PDF term corresponding to the mass of the neutrino system per tau decay, only applied to leptonic tau decays). The default is `False`.

`useDefaultSettings`
:   whether to take all default options from the tool itself. The default is `-1`.

`useEfficiencyRecovery`
:   whether to enable refitting for failed events, to improve efficiency. The default is `-1`.

`useMETdphiLL`
:   whether to parameterise the MET resolution using sumET and dphiLL (only for the lep-lep case). The default is `False`.

`paramFilePath`
:   path (string) to the ROOT file used with `calibSet` ≥ 2019, containing the PDFs for the likelihood. The default is `'MMC_params_v1_fixed.root'` (recommended).

`doMLNU3P`
:   save information about the reconstruction with the best-fit neutrino kinematics. The default is `False`.

`doMAXW`
:   save information about the reconstruction with the maximum-weight estimator. The default is `False`.

`saveLlhHisto`
:   save likelihood histograms (`MMC_likelihoods.root`) for debugging purposes. If enabled, it can slow down the MMC running time! The default is `False`.

!!! success "Registers the following variables:"
    - `mmc_fit_status`: the returned status of the MMC fit
    - `mmc_mlm_mass`: the mass of the di-tau resonance using the maximum-likelihood estimator
    - `mmc_maxw_mass`: the mass of the di-tau resonance using the maximum-weight estimator (if `doMAXW`)
    - `mmc_mlnu3p_mass`: the mass of the di-tau resonance using the best-fit neutrino kinematics (if `doMLNU3P`)

!!! success "Additional variables toggled by `saveExtraVariables`"
    - `mmc_mlnu3p_res_4vect`: four-vector of the di-tau resonance using the best-fit neutrino kinematics
    - `mmc_mlnu3p_nu1_4vect`: four-vector of the leading-$p_\mathrm{T}$ neutrino using the best-fit neutrino kinematics
    - `mmc_mlnu3p_nu2_4vect`: four-vector of the subleading-$p_\mathrm{T}$ neutrino using the best-fit neutrino kinematics
    - `mmc_mlnu3p_tau1_4vect`: four-vector of the leading-$p_\mathrm{T}$ tau-lepton using the best-fit neutrino kinematics
    - `mmc_mlnu3p_tau2_4vect`: four-vector of the subleading-$p_\mathrm{T}$ tau-lepton using the best-fit neutrino kinematics
    - `mmc_maxw_res_4vect`: four-vector of the di-tau resonance using the maximum-weight estimator
    - `mmc_maxw_nu1_4vect`: four-vector of the leading-$p_\mathrm{T}$ neutrino using the maximum-weight estimator
    - `mmc_maxw_nu2_4vect`: four-vector of the subleading-$p_\mathrm{T}$ neutrino using the maximum-weight estimator
    - `mmc_maxw_tau1_4vect`: four-vector of the leading-$p_\mathrm{T}$ tau-lepton using the maximum-weight estimator
    - `mmc_maxw_tau2_4vect`: four-vector of the subleading-$p_\mathrm{T}$ tau-lepton using the maximum-weight estimator

!!! abstract "Documentation"
    Further notes on the tool are available at [DiTauMassTools](https://gitlab.cern.ch/atlas/athena/-/tree/main/PhysicsAnalysis/TauID/DiTauMassTools). Usage in the ATLAS Run 2 $H\to\tau\tau$ analysis is documented in Section 10 of [ATL-COM-PHYS-2020-721](https://cds.cern.ch/record/2741326). A detailed description of the Missing Mass Calculator (MMC) method and its alternatives is given in Chapter 4 of [Michael Hübner's PhD thesis](https://bonndoc.ulb.uni-bonn.de/xmlui/bitstream/handle/20.500.11811/9734/6567.pdf).

!!! note
    The MMC method can be applied to had-had, had-lep and lep-lep di-tau decays. Based on the input collections given to the algorithm, the following priority ordering is made internally:

    1. $\tau$-had + $\tau$-had
    1. $\tau$-had + $\mu$
    1. $\tau$-had + e
    1. e + $\mu$
    1. $\mu$ + $\mu$
    1. e + e

    This means that if your event has 2 hadronic tau-jets and 1 electron, the MMC fit will be run under the assumption of a had-had event. To force the MMC fit to consider the 1 electron in a had-lep topology, you'd need to edit the C++ code. Alternatively, if you have determined that some objects should not be used as inputs (e.g. hadronic tau-jet already assigned to top reconstruction, pair of leptons assigned to a Z boson), you should decorate these objects with a flag and use the relevant `container.selection` options of the algorithm. In that way, the MMC fit will only be run on the "left-over" objects.

!!! warning
    The MMC method assumes that the MET in a given event originates mostly from the neutrinos associated to the decay of the di-tau system. If your topology has additional sources of MET (e.g. $t\bar{t}H(\to\tau\tau)$, $W(\to\ell\nu)H(\to\tau\tau)$), the MMC method is not recommended and will give nonsensical answers. See e.g. the ATLAS Run 2 search for BSM $VH(\to\tau\tau)$ in [ATL-COM-PHYS-2022-022](https://cds.cern.ch/record/2799543) where the MMC method is combined with alternatives. Additional neutrinos from the decay of B-hadrons typically do not lead to significant enough MET to be a problem, i.e. $t\bar{t}(\to\text{jets})H(\to\tau\tau)$ should be safe.

!!! warning "User actions required for updating to version 2.10.0 or higher"

    This algorithm has been migrated to Athena since v2.10.0 ([issue #150](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/issues/150)). Its declaration under `AddConfigBlocks` should be removed if applicable. Using this block in YAML configs is unchanged.

### [KLFitterConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/KLFitterAnalysisAlgorithms/python/KLFitterConfig.py)
Name in YAML: **KLFitter** (needs to be imported manually!)

`electrons`
:   the input electron container, with a possible selection, in the format `container` or `container.selection`. The default is `''` (empty string).

`muons`
:   the input muon container, with a possible selection, in the format `container` or `container.selection`. The default is `''` (empty string).

`jets`
:   the input jet container, with a possible selection, in the format `container` or `container.selection`. The default is `''` (empty string).

`met`
:   the input MET container. The default is `''` (empty string).

`likelihoodType`
:   KLFitter likelihood, if only one is needed. See [possible values](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/TopCPToolkit/KLFitterEnums.h). The default is `''` (empty string).

`leptonType`
:   type of lepton to use (only relevant to certain likelihood types), if only one is needed. See [possible values](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/TopCPToolkit/KLFitterEnums.h). The default is `''` (empty string).

`jetSelectionMode`
:   jet selection mode to use, if only one is needed. See [possible values](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/TopCPToolkit/KLFitterEnums.h). The default is `''` (empty string).

`btaggingMethod`
:   strategy to handle b-jets, if only one is needed. See [possible values](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/TopCPToolkit/KLFitterEnums.h). The default is `''` (empty string).

`bTagCDIFile`
:   CDI file to pass to the b-tagging efficiency tool. The default is `xAODBTaggingEfficiency/13TeV/2022-22-13TeV-MC20-CDI-2022-07-28_v1.root`.

`btagger`
:   b-tagging algorithm to use, if only one is needed. The default is `DL1dv00`.

`btagWP`
:   b-tagging efficiency WP to use, if only one is needed. The default is `FixedCutBEff_77`.

`btagIgnoreOutOfValidityRange`
:   whether or not the b-tagger should ignore (and not fail) when a jet is outside the calibration range. The default is `False`.

`selectionRegionsConfig`
:   string of the form `selectionName: sel1, optionA: opA, optionB: opB; selectionName: sel2, ...` where options can be `likelihoodType`, `leptonType`, `jetSelectionMode`, `btaggingMethod`, `btagger` or `btagWP`. The default is `''` (empty string).

`saveAllPermutations`
:   whether to save all permutations, or just the best one. The default is `False` (only save the best one).

!!! success "Registers the following variables:"
    - `eventProbability`: the event probability
    - `logLikelihood`: the log of the likelihood

!!! abstract "Documentation"
    The source code and documentation for KLFitter are available from [github](https://github.com/KLFitter/KLFitter).

### [TopSpaNetConfig](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/python/TopSpaNetConfig.py)
Name in YAML: **SpaNet**

`btagger`
:   b-tagging algorithm to use, if only one is needed. The default is `DL1dv01`.

`electrons`
:   the input electron container, with a possible selection, in the format `container` or `container.selection`. The default is `''` (empty string).

`muons`
:   the input muon container, with a possible selection, in the format `container` or `container.selection`. The default is `''` (empty string).

`jets`
:   the input jet container, with a possible selection, in the format `container` or `container.selection`. The default is `''` (empty string).

`met`
:   the input MET container. The default is `''` (empty string).

`eventSelection`
:   optional event filter to run on. The default is `''` (empty string), i.e. all events.

`topology`
:   the SPA-Net model to run. See [possible values](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/TopSpaNet/SpaNetEnums.h).

!!! success "Registers up to 4 types of topology-dependent variables:"
    - `index`: index within a given container of the relevant particle candidate
    - `assignment`: quality of the system reconstruction
    - `detection`: likelihood of the event containing all necessary decay products to reconstruct the system
    - `reg`: regressed kinematic properties

!!! note "Documentation"
    See [arXiv:2106.03898](https://arxiv.org/abs/2106.03898) and [arXiv:2309.01886](https://arxiv.org/abs/2309.01886).

### [NuFlowsConfig](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/python/NuFlowsConfig.py)
Name in YAML: **NuFlows**

`btagger`
:   b-tagging algorithm to use, if only one is needed. The default is `DL1dv01`.

`electrons`
:   the input electron container, with a possible selection, in the format `container` or `container.selection`. The default is `''` (empty string).

`muons`
:   the input muon container, with a possible selection, in the format `container` or `container.selection`. The default is `''` (empty string).

`jets`
:   the input jet container, with a possible selection, in the format `container` or `container.selection`. The default is `''` (empty string).

`met`
:   the input MET container. The default is `''` (empty string).

`eventSelection`
:   optional event filter to run on. The default is `''` (empty string), i.e. all events.

`onnxfilepath`
:   the path to the folder containing the ONNX files (even.onnx, odd.onnx). Nominal ($m_{top} =$ 172.5 GeV) network is at `TopCPToolkit/nuflows/nominal_v01_270824`

`write_inputs`
:   boolean, if `True` registers the network input features. Contains four vectors `input_lep, input_jet, input_met, input_misc`. The default is `False`.

!!! success "Registers 2 outputs:"
    - `nu_out`: vector containing the sampled neutrino momenta $(p_x^{\nu}, p_y^{\nu}, p_z^{\nu}, p_x^{\bar{\nu}}, p_y^{\bar{\nu}}, p_z^{\bar{\nu}})$
    - `loglik`: single float containing the log-likelihood of the sampled solution

### [FakeBkgConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AsgAnalysisAlgorithms/python/FakeBkgConfig.py)
Name in YAML: **FakeBkgCalculator**

`setupName`
:   unique name given to the underlying algorithm estimating the fake background.

`electrons`
:   the input electron container, with a possible selection, in the format `container` or `container.selection`. The default is `''` (empty string).

`electronsTarget`
:   the input electron tight selection in the format `selection`.

`muons`
:   the input muon container, with a possible selection, in the format `container` or `container.selection`. The default is `''` (empty string).

`muonsTarget`
:   the input muon tight selection in the format `selection`.

`eventPreselection`
:   the event selection flag to estimate the fake background. The default is '' (empty string).

`fakeTool`
:   the tool for fake lepton estimate among the different [FakeBkgTools](https://gitlab.cern.ch/atlas/athena/-/tree/main/PhysicsAnalysis/AnalysisCommon/FakeBkgTools). The currently available one is `CP::AsymptMatrixTool`.

`config`
:   the input to the tool (ROOT or XML file). Some test files (deprecated and computed in Release 21) can be found in `dev/AnalysisTop/FakeBkgToolsData/`.

`definition`
:   this argument allows the user to specify the definition of the region of interest, in terms of how many (tight) leptons are selected, and how extra leptons are treated (vetoed or not). This must describe the way events are selected in the main analysis. More details [here](https://gitlab.cern.ch/atlas/athena/-/blob/main/PhysicsAnalysis/AnalysisCommon/FakeBkgTools/doc/arg_selection.md).

`process`
:   this argument allows one to set what kind of processes, in terms of number of real and fake/non-prompt leptons in the final state, are estimated by the fake lepton background yields or weights computed by the tools. More details [here](https://gitlab.cern.ch/atlas/athena/-/blob/main/PhysicsAnalysis/AnalysisCommon/FakeBkgTools/doc/arg_process.md).

!!! success "Registers the following variable:"
    - `weight_fake_`+`algoName`: the fake lepton weight

!!! abstract "Documentation"
    The source code and documentation for FakeBkgTools are available from [athena](https://gitlab.cern.ch/atlas/athena/-/tree/main/PhysicsAnalysis/AnalysisCommon/FakeBkgTools).

### [TopSpinDensityMatrixConfig](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/python/TopSpinDensityMatrixConfig.py)
Name in YAML: **SpinMatrix**

Constructs a series of observables used to measure spin correlations, polarisations, and entanglement in top-anti-top quark pairs.

`setup`
:   a name (string) for this particular instance (useful to distinguish between different scenarios). The default is `''` (empty string).

`eventSelection`
:   optional event filter to run on. The default is `''` (empty string), i.e. all events.

`top`
:   the name (string) of the 4-vector for the top quark. The default is `''` (empty string).

`tbar`
:   the name (string) of the 4-vector for the anti-top quark. The default is `''` (empty string).

`top_decay`
:   the name (string) of the 4-vector for the decay product of the top quark. The default is `''` (empty string).

`tbar_decay`
:   the name (string) of the 4-vector for the decay product of the anti-top quark. The default is `''` (empty string).

`doHelicityBasis`
:   whether to compute the observables in the helicity basis. The default is `False`.

`doBeamBasis`
:   whether to compute the observables in the beam basis. The default is `False`.

`doEntanglement`
:   whether to compute observables related to spin entanglement. The default is `False`.

!!! note
    No variable is saved by default, one must use the boolean switches!

!!! success "Additional variables toggled by `doHelicityBasis`"
    - `cos_theta_helicity_p`: cosine of the $\theta$ angle for the spin analyser of the top quark along the helicity axis
    - `cos_theta_helicity_m`: cosine of the $\theta$ angle for the spin analyser of the anti-top quark along the helicity axis
    - `cos_theta_transverse_p`: cosine of the $\theta$ angle for the spin analyser of the top quark along the transverse axis
    - `cos_theta_transverse_m`: cosine of the $\theta$ angle for the spin analyser of the anti-top quark along the transverse axis
    - `cos_theta_raxis_p`: cosine of the $\theta$ angle for the spin analyser of the top quark along the third axis
    - `cos_theta_raxis_m`: cosine of the $\theta$ angle for the spin analyser of the anti-top quark along the third axis

!!! success "Additional variables toggled by `doBeamBasis`"
    - `cos_theta_x_p`: cosine of the $\theta$ angle for the spin analyser of the top quark along the x axis
    - `cos_theta_x_m`: cosine of the $\theta$ angle for the spin analyser of the anti-top quark along the x axis
    - `cos_theta_y_p`: cosine of the $\theta$ angle for the spin analyser of the top quark along the y axis
    - `cos_theta_y_m`: cosine of the $\theta$ angle for the spin analyser of the anti-top quark along the y axis
    - `cos_theta_z_p`: cosine of the $\theta$ angle for the spin analyser of the top quark along the beam axis
    - `cos_theta_z_m`: cosine of the $\theta$ angle for the spin analyser of the anti-top quark along the beam axis

!!! success "Additional variables toggled by `doEntanglement`"
    - `cos_phi`: cosine of the opening angle between the spin analysers of top and the anti-top quarks in their respective rest frames (related to the quantum entanglement marker $D$)

### [SinglelepHadronicChi2RecoConfig](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/python/SinglelepHadronicChi2RecoConfig.py)
Name in YAML: **Chi2Reco**

A simple $\chi^2$ reconstruction algorithm to match reco jets to the decay products of the hadronically decaying top in the single lepton selection. The $\chi^2$ is defined as:

$$
\chi^2 = \dfrac{(m_W^{\text{reco}} - m_W)^2}{\sigma_W^2} + \dfrac{(m_t^{\text{reco}} - m_t)^2}{\sigma_t^2}
$$

Permutations of jets are considered and the one with the best $\chi^2$ value is considered to be the correct one.
Only b-tagged jets are considerted on the position of the b-jet and only light-jets (non-b tagged) jets are considered for the position of the light jets. So make sure to have at least one b-jet in the selection

`jets`
:   the jet collection to run on. The default is `''` (empty string).

`eventSelection`
:   optional event filter to run on. The default is `''` (empty string), i.e. all events.

`topMass`
:   the mass of the top quark used in the $\chi^2$ formula in GeV. The default is `'172.5'`.

`maxBJets`
:   the maxium number of b-jets to consider (ordered in pT). The default is `'2'`.

`maxLightJets`
:   the maxium number of light-jets (non-b) to consider (ordered in pT). The default is `'4'`.

`topResolution`
:   the $\sigma_t$ value in the $\chi^2$ (in GeV). The default is `'35'`.

`wResolution`
:   the $\sigma_W$ value in the $\chi^2$ (in GeV). The default is `'25'`.

`btagger`
:   the name of the b-tagger used. Needs to be set. The default is `''` (empty string).

`btagWP`
:   the name of the b-tag WP. Needs to be set. The default is `''` (empty string).

`storeAllPermutations`
:   the flag to control whether all considered permutations should be stored or only the best one. The default is `False`.

`postfix`
:   a postfix to apply to decorations and algorithm names. The default is `''` (empty string).

### [HyPERConfig](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/python/HyPERConfig.py)
Name in YAML: **HyPER**

`topology`
:   The HyPER model to run. Currently 'TtbarAllHadronic', 'TtbarLJetsNoBTag' and 'TtbarLJets' are supported.

`btagger`
:   The b-tagging algorithm used to determine the PCBT quantile. The default is `GN2v01_Continuous`. Irrelevant for the 'TtbarLJetsNoBTag' topology. The available models have been trained using GN2.

`electrons`
:   The input electron container, with a possible selection, in the format `container` or `container.selection`. The default is `''` (empty string).

`muons`
:   The input muon container, with a possible selection, in the format `container` or `container.selection`. The default is `''` (empty string).

`jets`
:   The input jet container, with a possible selection, in the format `container` or `container.selection`. The default is `''` (empty string).

`met`
:   The input MET container. The default is `''` (empty string).

`eventSelection`
:   An optional event filter to run on. The default is `''` (empty string), i.e. all events. Make sure your selection make sense in order to build the HyPER graph. For instance, a pre-selection of events containing three jets will not allow you to reconstruct two tops.

`OutputLevel`
:   This flag controls how verbose is the output of the algorithm. This is for debugging purposes. The default is '3' (INFO) - this is the setting that should be used for ntuple production. Options: 3: INFO (DEFAULT) 2: DEBUG 1: VERBOSE.

HyPER outputs the indices of the particles belonging to each hyper-edge and edge (in addition to the hyper-edge and edge scores). This is necessary for the reconstruction. These indices indicate the positions of the particles within the containers in the ntuples. Different particle types can exist in the same hyperedge (for example a leptonic top decay would have a hyperedge containing jet and lepton types). These particles could be located at the same position in their respective containers: the HyPER output also includes an `IDs` vector (when relevant). This vector denotes the type of each particle in the hyper-edge

The adopted convention is `jet=1 e=2 mu=3 met=4 tau=5`. So, to give an example, if for an event you see
`TopLep_Indices = [0,0,0]` and `TopLep_IDs = [1,3,4]` you will interpret this as the following particles decaying from TopLep:
- The jet in position zero in the jet container.
- The muon in position zero in the muon container.
- The MET. (For convention, this will always report position zero).


!!! success "Registers topology-dependent variables:"
    For `TtbarAllHadronic`
    Top1 and Top2 correspond to the two highest score hyperdges.
    - `hyper_TtbarAllHadronic_Top1/2_Indices`: The three jet indices matched to top1/2.
    - `hyper_TtbarAllHadronic_Top1/2_Score`: The score assigned to the top1/2 hyperedge.
    - `hyper_TtbarAllHadronic_W1/2_Indices`: The two jet indices matched to W1/2.
    - `hyper_TtbarAllHadronic_W1/2_Score`: The score assigned to the W1/2 edge.

    For `TtbarLJets` and `TtbarLJetsNoBTag`
    TopHad and TopLep correspond to the two highest score hyperdges for a hadronically decaying top and semi-leptonic decaying top.
    - `hyper_TtbarLJets_TopHad/Lep_Indices`: The three particle indices matched to topHad/Lep.
    - `hyper_TtbarLJets_TopHad/Lep_Score`: The score assigned to the topHad/Lep hyperedge.
    - `hyper_TtbarLJets_TopHad/Lep_IDs`: The three particle IDs matched to topHad/Lep.
    - `hyper_TtbarLJets_W1Had/Lep_Indices`: The two particle indices matched to WHad/Lep.
    - `hyper_TtbarLJets_WHad/Lep_Score`: The score assigned to the WHad/Lep edge.

!!! note "Documentation"
    To read the model paper see [Reconstructing short-lived particles using hypergraph representation learning](https://journals.aps.org/prd/abstract/10.1103/PhysRevD.111.032004).
    The HyPER GitHub repository is [here](https://github.com/tzuhanchang/HyPER).

### [ToponiumDileptonReconstructionConfig](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/python/ToponiumDileptonReconstructionConfig.py)
Name in YAML: **ToponiumDileptonReconstruction**

Reconstruction algorithm for toponium decay in the dileptonic channel $\eta_t\to t\bar{t}\to \ell^+\nu b\ell^-\bar{\nu}\bar{b}$.
The $W$ bosons are assumed to be on-shell, but the two top quarks are not.
Instead, we assume the top and antitop are collinear: this is a rough approximation valid only close to threshold, $M_{t\bar{t}}\lesssim 390$ GeV!
Additionally, the MET is assumed to come almost entirely from the neutrinos.
The algorithm proceeds by computing solutions for the heavier top and the lighter top (likely below $m_t=172.5$ GeV), but these are converted to "top" and "antitop" based on the lepton charges.

`electrons`
:   the input electron container, with a possible selection, in the format `container` or `container.selection`. The default is `''` (empty string).

`muons`
:   the input muon container, with a possible selection, in the format `container` or `container.selection`. The default is `''` (empty string).

`jets`
:   the input jet container, with a possible selection, in the format `container` or `container.selection`. The default is `''` (empty string).

`met`
:   the input MET container. The default is `''` (empty string).

`metTerm`
:   the MET term name to use. The default is `'Final'` (suited for detector-level analysis; at particle-level, you need `'NonInt'`).

`selection`
:   a possible event selection to apply. The default is `''` (empty string).

`btagDecor`
:   the b-tagging selection string. The default is `''` (empty string).

!!! success "Registers the following variable:"
    - `toponium_top_4vect`: the 4-vector of the reconstructed top quark
    - `toponium_tbar_4vect`: the 4-vector of the reconstructed antitop quark
    - `toponium_top_decay_4vect`: the 4-vector of the spin analyser (lepton) from the reconstructed top quark
    - `toponium_tbar_decay_4vect`: the 4-vector of the spin analyser (lepton) from the reconstructed antitop quark
