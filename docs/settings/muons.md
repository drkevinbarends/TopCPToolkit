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

`decorateTruth`
:   whether to decorate truth particle information on the reconstructed one. The default is `False`.

`writeTrackD0Z0`
:   save the d0 significance and z0sinTheta variables so they can be written out. The default is `False`.

!!! success "Registers the following variables:"
    - `pt`: muon $p_\mathrm{T}$
    - `eta`: muon $\eta$ (no systematics)
    - `phi`: muon $\phi$ (no systematics)
    - `charge`: muon charge (no systematics)
    - `e`: muon $E$ (*since AnalysisBase 25.2.13*)

!!! success "Additional variables toggled by `decorateTruth`:"
    - `truth_type`: type of the corresponding truth particle (no systematics)
    - `truth_origin`: origin of the corresponding truth particle (no systematics)

!!! success "Additional variables toggled by `writeTrackD0Z0`"
    - `d0sig`: the $d_0$ significance (no systematics)
    - `z0sintheta`: the $z_0\sin\theta$ parameter (no systematics)

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

`addResolutionCategory`
:   whether to add a decoration to the muon container with the resolution category. The default is `False`.

`quality`
:   the ID WP (string) to use. Supported ID WPs: `Tight`, `Medium`, `Loose`, `LowPt`, `HighPt`.

`isolation`
:   the isolation WP (string) to use. Supported isolation WPs: `PflowLoose_VarRad`, `PflowTight_VarRad`, `Loose_VarRad`, `Tight_VarRad`, `NonIso`.

`addSelectionToPreselection`
:   whether to retain only muons satisfying the working point requirements. The default is `True`.

`closeByCorrection`
:   whether to use close-by-corrected isolation working points. The default is `False`.

`systematicBreakdown`
:   enables the full breakdown of efficiency SF systematics (1 NP per uncertainty source, instead of 1 NP in total). The default is `False`.

`noEffSF`
:   disables the calculation of efficiencies and scale factors. Experimental! only useful to test a new WP for which scale factors are not available. The default is `False`.

`onlyRecoEffSF`
:   same as `noEffSF`, but retains the ID scale factor. Experimental! only useful for CI tests. The default is `False`.

`saveDetailedSF`
:   whether to save all	the independent	and detailed object scale factors. The default is `True`.

`saveCombinedSF`
:   whether to save the	combined object	scale factor. The default is `False`.

`excludeNSWFromPrecisionLayers`
:   whether to ignore NSW hits. Expert option, for testing purposes or to fix a crash with older derivations (p-tag < p5834). The default is `False`.

!!! success "Registers the following variables:"
    - `select`: whether the muon passes the quality and isolation cuts
    - `reco_effSF`: the per-muon reconstruction SF
    - `BadMuonVeto_effSF`: the dedicated "bad muon veto" SF, only with quality `HighPt`
    - `isol_effSF`: the per-muon isolation SF
    - `TTVA_effSF`: the per-muon track-to-vertex-association SF

!!! success "Additional variables toggled by `addResolutionCategory`"
    - `resolutionCategory`: the resolution category (no systematics)

### [SoftMuonSelectorConfig](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/python/SoftMuonSelectorConfig.py?ref_type=heads)
Name in YAML: **SoftMuonSelector**

This package is designed to select muons that are within a specific distance ($\Delta R$) from a jet. It is intended for selecting soft muons contained within jets, which is useful for identifying events with muons originating from the decay of heavy flavor hadrons. An example can be found [here](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/share/config/exampleTtbarLjets_SoftMuons/reco.yaml).

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

`saveSoftMuonTruthInfo`
:   Specifies whether to save the truth level information regarding the truth ancestry of the soft muons. When enabled, variables such as `softmu_truthBAncestor_pdgId` and `softmu_truthCAncestor_pdgId` are saved, which allow for the classification of sofr muons based on their truth origin (e.g. $b \rightarrow \mu$, $b \rightarrow c \rightarrow \mu$, $c \rightarrow \mu$, $\text{light} \rightarrow \mu$, etc).

Events containing at least one soft muon selected by this algorithm can be identified by requiring the event to pass the `EVENTFLAG pass_SoftMuonPassDRJetcut_%SYS%` in the `Event Selection` block.

!!! success "Registers the following variables for each event ; all the following branches are `softmu_VARNAME` with the exception of the first one "
    - `pass_SoftMuonPassDRJetcut`: event flag whether the muon event contains at least one muon that passes the selection criteria in the SoftMuonSelector tool
    - `softmu_IFFClass` : for MC, IFF class of the soft muon as defined  (no systematics)[here](https://gitlab.cern.ch/atlas/athena/-/tree/main/PhysicsAnalysis/AnalysisCommon/TruthClassification#4-details-about-the-lepton-categories)
    - `pt`: muon $p_\mathrm{T}$
    - `eta`: muon $\eta$ (no systematics)
    - `phi`: muon $\phi$ (no systematics)
    - `charge`: muon charge (no systematics)
    - `e`: muon $E$
    - `SoftMuonJetDRmin`: per-muon angular distance between the muon and its closest jet
    - `SoftMuonPassDRJetcut`: whether the muon passes the $\Delta R$ (per-muon)
    - `TTVA_effSF`: TTVA SFs (see [link](https://atlas-mcp.docs.cern.ch/guidelines/index.html)). Note that TTVA shouldn't be applied to soft muons for most use cases (turn off using `trackSelection: False` in the WP definition).
    - `reco_effSF`: reconstruction+ID SFs (see [link](https://atlas-mcp.docs.cern.ch/guidelines/index.html)).

!!! success "Additional variables toggled by `writeTrackD0Z0`; all the following branches are `softmu_VARNAME` "
    - `d0sig`: the $d_0$ significance (no systematics)
    - `z0sintheta`: the $z_0\sin\theta$ parameter (no systematics)

!!! success "Additional variables toggled by `saveSoftMuonAdditionalInfo`; all the following branches are `softmu_VARNAME` "
    - `author`: the algorithm author of the muons, see [here](https://gitlab.cern.ch/atlas/athena/-/blob/main/Event/xAOD/xAODMuon/xAODMuon/versions/MuonEnums.def#L5) (no systematics)
    - `allAuthors`: 16-bit bitmask reporting all algorithms authors of the muons as defined here [here](https://gitlab.cern.ch/atlas/athena/-/blob/main/Event/xAOD/xAODMuon/xAODMuon/versions/MuonEnums.def#L5) (no systematics)
    - `resolutionCategory`: the resolution category as defined [here](https://gitlab.cern.ch/atlas/athena/-/blob/main/PhysicsAnalysis/Interfaces/MuonAnalysisInterfaces/MuonAnalysisInterfaces/IMuonSelectionTool.h#L31)
    - `passedIDCuts`: standard MCP ID cuts, see [here](https://gitlab.cern.ch/atlas/athena/-/blob/main/PhysicsAnalysis/MuonID/MuonSelectorTools/Root/MuonSelectionTool.cxx#L594) (no systematics)
    - `passedHighPtCuts`: check if the muon passes the HighPt WP cuts, see [docs](https://atlas-mcp.docs.cern.ch/guidelines/muonselectiontool/index.html) and [code](https://gitlab.cern.ch/atlas/athena/-/blob/main/PhysicsAnalysis/MuonID/MuonSelectorTools/Root/MuonSelectionTool.cxx#L917)
    - `passedLowPtCuts`: check if the muon passes the cut-based LowPt WP cuts, see [docs](https://atlas-mcp.docs.cern.ch/guidelines/muonselectiontool/index.html) and  [code](https://gitlab.cern.ch/atlas/athena/-/blob/main/PhysicsAnalysis/MuonID/MuonSelectorTools/Root/MuonSelectionTool.cxx#L675)
    - `isBadMuon`: check if the muon is a BadMuon, see [docs](https://atlas-mcp.docs.cern.ch/guidelines/muonselectiontool/index.html#isbadmuon-flag-event-veto) and [code](https://gitlab.cern.ch/atlas/athena/-/blob/main/PhysicsAnalysis/MuonID/MuonSelectorTools/Root/MuonSelectionTool.cxx#L629).
    - `pt_id`: muon ID track $p_\mathrm{T}$
    - `eta_id`: muon ID track $\eta$ (no systematics)
    - `phi_id`: muon ID track $\phi$ (no systematics)
    - `e_id`: muon ID track $E$
    - `pt_me`: muon ME track $p_\mathrm{T}$; ME is the muon spectrometer (MS) track extrapolated to the beamspot
    - `eta_me`: muon ME track $\eta$ (no systematics); ME is the muon spectrometer (MS) track extrapolated to the beamspot
    - `phi_me`: muon ME track $\phi$ (no systematics); ME is the muon spectrometer (MS) track extrapolated to the beamspot
    - `e_me`: muon ME track $E$; ME is the muon spectrometer (MS) track extrapolated to the beamspot
    - `qOverP`: muon $q/p$
    - `qOverP_ID`: muon ID track $q/p$
    - `qOverP_ME`: muon ME track $q/p$; ME is the muon spectrometer (MS) track extrapolated to the beamspot
    - `qOverPerr`: muon uncertainty on $q/p$
    - `qOverPerr_ID`: muon ID track uncertainty on $q/p$
    - `qOverPerr_ME`: muon ME track uncertainty on $q/p$; ME is the muon spectrometer (MS) track extrapolated to the beamspot
    - `d0`: transversal impact parameter (no systematics)
    - `reducedChi2`: $\chi^{2}/NDOF$ for the muon combined track (no systematics)
    - `reducedChi2_ID`: $\chi^{2}/NDOF$ for the muon ID track (no systematics)
    - `reducedChi2_ME`: $\chi^{2}/NDOF$ for the muon ME track; ME is the muon spectrometer (MS) track extrapolated to the beamspot (no systematics)
    - `reducedSegmentChi2`: $\chi^{2}/NDOF$ for the muon segments (no systematics)
    - `momentumBalanceSignificance`: momentum difference between the ID and MS standalone measurements with respect to the uncertainty sigma on energy lost in the calorimeter system, see [docs](https://cds.cern.ch/record/2665711)
    - `scatteringCurvatureSignificance`: a variable estimating the significance of a change in trajectory (kink) along the track, computed considering pairs of adjacent hits along the track, and evaluating the significance of the angular distance in the bending plane between the two half tracks ending/starting at each of them, see [docs](https://cds.cern.ch/record/2665711) (no systematics)
    - `scatteringNeighbourSignificance`: computed as the normalized integral of the scattering angle significances, corrected for single kinks along the trajectory that diminish the absolute value of the integral significance, see [docs](https://cds.cern.ch/record/2665711)
    - `qOverPSignificance`: absolute difference between the ID and the MS q/p measurements divided by the sum in quadrature of the corresponding uncertainties, see [docs](https://cds.cern.ch/record/2665711) (no systematics)
    - `rhoPrime`: absolute difference between transverse momentum measurements in the ID and MS divided by the combined track measurement, see [docs](https://cds.cern.ch/record/2665711)
    - `segmentDeltaEta`: the angular difference between the extrapolated ID track to the MS and the innermost MS segment in $\eta$, see [docs](https://cds.cern.ch/record/2665711) (no systematics)
    - `spectrometerFieldIntegral`: integral field encoutered by the muon in the MS (no systematics)
    - `nprecisionLayers`: number of precision stations where the muons has segments (no systematics)
    - `nprecisionHoleLayers`: number of holes in precision stations for the muons (no systematics)
    - `nGoodPrecLayers`: number of good precision stations for the muons (no systematics)
    - `innerSmallHits`: number of hits in small sectors of the inner station of the MS (no systematics)
    - `innerLargeHits`: number of hits in large sectors of the inner station of the MS (no systematics)
    - `innerSmallHoles`: number of holes in small sectors of the inner station of the MS (no systematics)
    - `innerLargeHoles`: number of holes in large sectors of the inner station of the MS (no systematics)
    - `middleSmallHits`: number of hits in small sectors of the middle station of the MS (no systematics)
    - `middleLargeHits`: number of hits in large sectors of the middle station of the MS (no systematics)
    - `middleSmallHoles`: number of holes in small sectors of the middle station of the MS (no systematics)
    - `middleLargeHoles`: number of holes in large sectors of the middle station of the MS (no systematics)
    - `outerSmallHits`: number of hits in small sectors of the outer station of the MS (no systematics)
    - `outerLargeHits`: number of hits in large sectors of the outer station of the MS (no systematics)
    - `outerSmallHoles`: number of holes in small sectors of the outer station of the MS (no systematics)
    - `outerLargeHoles`: number of holes in large sectors of the outer station of the MS (no systematics)
    - `extendedSmallHits`: number of hits in small sectors of the extended station of the MS (no systematics)
    - `extendedLargeHits`: number of hits in large sectors of the extended station of the MS (no systematics)
    - `extendedSmallHoles`: number of holes in small sectors of the extended station of the MS (no systematics)
    - `extendedLargeHoles`: number of holes in large sectors of the extended station of the MS (no systematics)
    - `innerClosePrecisionHits`: number of close precision hits in the inner station of the MS (no systematics)
    - `middleClosePrecisionHits`: number of close precision hits in the middle station of the MS (no systematics)
    - `outerClosePrecisionHits`: number of close precision hits in the outer station of the MS (no systematics)
    - `extendedClosePrecisionHits`: number of close precision hits in the extended station of the MS (no systematics)
    - `innerOutBoundsPrecisionHits`: number of outbounds precision hits in the inner station of the MS (no systematics)
    - `middleOutBoundsPrecisionHits`: number of outbounds precision hits in the middle station of the MS (no systematics)
    - `outerOutBoundsPrecisionHits`: number of outbounds precision hits in the outer station of the MS (no systematics)
    - `extendedOutBoundsPrecisionHits`: number of outbounds precision hits in the extended station of the MS (no systematics)
    - `combinedTrackOutBoundsPrecisionHits`: number of outbounds precision hits on the combined track (no systematics)
    - `isSmallGoodSectors`: bool to check if the muon is in a good small sector of the MS (no systematics)
    - `isEndcapGoodLayers`: bool to check if the muon is in a good endcap layer of the MS (no systematics)
    - `cscUnspoiledEtaHits`: number of CSC unspoiled hits, run-2 only (no systematics)
    - `etaLayer1STGCHits`: number of hits in first eta-layer of sTGCs, run-3 only (no systematics)
    - `etaLayer2STGCHits`: number of hits in second eta-layer of sTGCs, run-3 only (no systematics)
    - `phiLayer1STGCHits`: number of hits in first phi-layer of sTGCs, run-3 only (no systematics)
    - `phiLayer2STGCHits`: number of hits in second phi-layer of sTGCs, run-3 only (no systematics)
    - `MMHits`: number of hits in Micromegas, run-3 only (no systematics)
    - `numberOfPixelHits`: number of Pixel hits in the ID (no systematics)
    - `numberOfPixelDeadSensors`: number of Pixel dead sensors corred by the muon in the ID (no systematics)
    - `numberOfPixelHoles`: number of Pixel holes in the ID (no systematics)
    - `numberOfSCTHits`: number of SCT hits in the ID (no systematics)
    - `numberOfSCTDeadSensors`: number of SCT dead sensors corred by the muon in the ID (no systematics)
    - `numberOfSCTHoles`: number of SCT holes in the ID (no systematics)
    - `numberOfTRTHits`: number of TRT hits in the ID (no systematics)
    - `numberOfTRTOutliers`: number of TRT holes in the ID (no systematics)
    - `isBIS78`: is the muon in the BIS78 region? (no systematics)
    - `isBEE`: is the muon in the BEE region? (no systematics)
    - `isBMG`: is the muon in the BMG region? (no systematics)
    - `ptcone20_Nonprompt_All_MaxWeightTTVA_pt500`: isolation variable built in a 0.2 cone with tracks with $p_\mathrm{T}>500$ MeV (no systematics)
    - `ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt500`: isolation variable built in a 0.3 variable cone with tracks with $p_\mathrm{T}>500$ MeV (no systematics)
    - `ptcone20_Nonprompt_All_MaxWeightTTVA_pt1000`: isolation variable built in a 0.2 cone with tracks with $p_\mathrm{T}>1000$ MeV (no systematics)
    - `ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt1000`: isolation variable built in a 0.3 variable cone with tracks with $p_\mathrm{T}>1000$ MeV (no systematics)
    - `neflowisol20`: isolation varibale built in a 0.2 cone using the neutral pflow component (no systematics)
    - `EnergyLoss`: energy loss of the muon in the calorimeter (no systematics)
    - `EnergyLossSigma`: uncertainty on energy loss of the muon in the calorimeter (no systematics)
    - `EnergyLossType`: type of energy loss of the muon in the calorimeter, see [code](https://gitlab.cern.ch/atlas/athena/-/blob/main/Event/xAOD/xAODMuon/xAODMuon/versions/MuonEnums.def#L97) (no systematics)
    - `ParamEnergyLoss`: parametric energy loss of the muon in the calorimeter (no systematics)
    - `MeasEnergyLoss`: measured energy loss of the muon in the calorimeter (no systematics)
    - `ParamEnergyLossSigmaPlus`: up uncertainty on the parametric energy loss of the muon in the calorimeter (no systematics)
    - `ParamEnergyLossSigmaMinus`: down uncertainty on the parametric energy loss of the muon in the calorimeter (no systematics)
    - `MeasEnergyLossSigma`: uncertainty on measured energy loss of the muon in the calorimeter (no systematics)
    - `nSegments`: number of segments in the muon MS track (no systematics)
    - `Seg1ChamberId`: chamber index of the first segment of the muon, see details in [docs](https://cds.cern.ch/record/2665711)
    - `Seg2ChamberId`: chamber index of the second segment of the muon, see details in [docs](https://cds.cern.ch/record/2665711)

!!! success "Additional variables toggled by `saveSoftMuonNearestJetInfo`; all the following branches are `softmu_VARNAME` "
    - `nearestJet_Index`: index of the jet nearest to the muon
    - `nearestJet_pt`: $p_\mathrm{T}$ of the jet nearest to the muon
    - `nearestJet_eta`: $\eta$ of the jet nearest to the muon
    - `nearestJet_phi`: $\phi$ of the jet nearest to the muon
    - `nearestJet_e`: energy of the jet nearest to the muon
    - `nearestJet_dR`: DeltaR between soft muon and nearest jet (calculated using $\Delta\eta$ or $\Delta y$ depending on `softMuonDRJetUseRapidity` flag)
    - `nearestJet_EMFrac`: fraction of EM energy in the jet
    - `nearestJet_NumTrkPt500`: number of tracks with $p_\mathrm{T}>500$ MeV within the jet
    - `nearestJet_SumPtTrkPt500`: sum of $p_\mathrm{T}$ of tracks with $p_\mathrm{T}>500$ MeV within the jet
    - `nearestJet_NumTrkPt1000`: number of tracks with $p_\mathrm{T}>1000$ MeV within the jet
    - `nearestJet_SumPtTrkPt1000`: sum of $p_\mathrm{T}$ of tracks with $p_\mathrm{T}>1000$ MeV within the jet
    - `nearestJet_N90Constituents`:  minimum number of constituentswhich carry 90% of the jet $p_\mathrm{T}$, see [paper](https://link.springer.com/article/10.1140/epjc/s10052-023-11837-9)
    - `nearestJet_TrackWidthPt500`: average  $p_\mathrm{T}$-weighted transverse distance in the $\eta$-$\phi$ plane between jet axis and all tracks with $p_\mathrm{T}>500$ MeV ghost-associated with the jet, see [paper](https://link.springer.com/article/10.1140/epjc/s10052-023-11837-9)
    - `nearestJet_TrackWidthPt1000`: average $p_\mathrm{T}$-weighted transverse distance in the $\eta$-$\phi$ plane between jet axis and all tracks with $p_\mathrm{T}>1000$ MeV ghost-associated with the jet, see [paper](https://link.springer.com/article/10.1140/epjc/s10052-023-11837-9)
    - `nearestJet_Width`: average $p_\mathrm{T}$-weighted transverse distance in the $\eta$-$\phi$ plane between jet axis and all jet constituents, see [paper](https://journals.aps.org/prd/abstract/10.1103/PhysRevD.86.072006)
    - `nearestJet_Charge`: $p_\mathrm{T}$ -weighted sum of charge of tracks ghost-associated with the jet, see [note](https://cds.cern.ch/record/1572980/files/ATLAS-CONF-2013-086.pdf)
    - `nearestJet_ChargedPFOWidthPt500`: Sum of $p_\mathrm{T}$ of charged Particle Flow Objects with $p_\mathrm{T}>500$ MeV; this is stored for each vertex, but in the algorithm we only store the value relative to the PV.
    - `nearestJet_ChargedPFOWidthPt1000`: Sum of $p_\mathrm{T}$ of charged Particle Flow Objects with $p_\mathrm{T}>1000$ MeV; this is stored for each vertex, but in the algorithm we only store the value relative to the PV.

!!! success "Additional variables toggled by `saveSoftMuonTruthInfo`; all the following branches are `softmu_VARNAME` "
    - `truthmuon_barcode`: barcode of the truth muon associated with the reco-level soft muon
    - `truthBAncestor_pdgId`: pdgId of the last b-hadron in the ancestor chain of the truth muon associated with the reco-level soft muon
    - `truthBAncestor_pt`: $p_\mathrm{T}$  of the last b-hadron in the ancestor chain of the truth muon associated with the reco-level soft muon
    - `truthCAncestor_pdgId`: pdgId of the last c-hadron in the ancestor chain of the truth muon associated with the reco-level soft muon
    - `truthCAncestor_pt`: $p_\mathrm{T}$  of the last c-hadron in the ancestor chain of the truth muon associated with the reco-level soft muon
    - `truthFirstBAncestor_pdgId`: pdgId of the first b-hadron in the ancestor chain of the truth muon associated with the reco-level soft muon
    - `truthFirstCAncestor_pdgId`: pdgId of the first c-hadron in the ancestor chain of the truth muon associated with the reco-level soft muon
    - `truthTauAncestor_pdgId`: pdgId of a possible tau ancestor of the truth muon associated with the reco-level soft muon (this is in practice a flag which is 15 or -15 for muons originated from a Tau decay, 0 otherwise)
