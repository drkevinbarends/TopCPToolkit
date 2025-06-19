#include "TopCPToolkit/SoftMuonSelectorAlg.h"

#include "TopCPToolkit/ParticleLevelSoftMuonOrigin.h"

namespace top {

SoftMuonSelectorAlg::SoftMuonSelectorAlg(const std::string& name,
                                         ISvcLocator* pSvcLocator)
    : EL::AnaAlgorithm(name, pSvcLocator), m_softmuonDRJetcut(0.4) {
  declareProperty("SoftMuonDRJet", m_softmuonDRJetcut = 0.4,
                  "Soft Muon maximum dR wrt nearest selected jet. Can be set "
                  "to 999. to keep all soft muons. Default 0.4");
  declareProperty(
      "SaveSoftMuonAdditionalInfo", m_saveSoftMuonAdditionalInfo = false,
      "Whether to store additional information associated with the soft muon.");
  declareProperty("SaveSoftMuonNearestJetInfo",
                  m_saveSoftMuonNearestJetInfo = false,
                  "Whether to store additional information associated with the "
                  "nearest jet associated to the soft muon.");
  declareProperty(
      "SoftMuonDRJetUseRapidity", m_softMuonDRJetUseRapidity = false,
      "Whether to use the rapidity instead of the pseudo-rapidity when "
      "calculating the Delta R between the soft muon and the closest jet.");
  declareProperty(
      "SaveSoftMuonTruthInfo", m_saveSoftMuonTruthInfo = false,
      "Whether to store truth information associated with the soft muon.");
  declareProperty("isRun3", m_isRun3 = false, "do we have CSCs or NSW?");
}

StatusCode SoftMuonSelectorAlg::initialize() {
  ANA_MSG_INFO("Initialising the SoftMuonSelector algorithm for TopCPToolkit");

  ANA_MSG_INFO("setting up muon selection tool for tight muons");
  ANA_CHECK(m_selTool_Tight.retrieve());

  ANA_CHECK(m_jetsHandle.initialize(m_systematicsList));
  ANA_CHECK(m_softmuonsHandle.initialize(m_systematicsList));
  ANA_CHECK(m_eventInfoHandle.initialize(m_systematicsList));

  //    ANA_CHECK(m_selection.initialize(m_systematicsList, m_eventInfoHandle));

  ANA_CHECK(m_softmuonSelection.initialize(m_systematicsList, m_softmuonsHandle,
                                           SG::AllowEmpty));
  ANA_CHECK(m_jetSelection.initialize(m_systematicsList, m_jetsHandle,
                                      SG::AllowEmpty));

  // Initialize output variables
  ANA_CHECK(m_SoftMuonJetDRminHandle.initialize(m_systematicsList,
                                                m_softmuonsHandle));
  ANA_CHECK(
      m_SoftMuonPassDRJetcut.initialize(m_systematicsList, m_softmuonsHandle));

  ANA_CHECK(m_pass_SoftMuonPassDRJetcut.initialize(m_systematicsList,
                                                   m_eventInfoHandle));

  // ------------------------------------------------
  // Additional information of the soft muon
  ANA_CHECK(m_softmu_author_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_allAuthors_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_resolutionCategory_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_passedIDCuts_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_passedHighPtCuts_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_passedLowPtCuts_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_isBadMuon_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));

  ANA_CHECK(m_softmu_pt_id_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_eta_id_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_phi_id_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_e_id_Handle.initialize(m_systematicsList,
                                            m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_pt_me_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_eta_me_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_phi_me_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_e_me_Handle.initialize(m_systematicsList,
                                            m_softmuonsHandle, SG::AllowEmpty));

  ANA_CHECK(m_softmu_qOverP_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_qOverP_ID_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_qOverP_ME_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_qOverPerr_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_qOverPerr_ID_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_qOverPerr_ME_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_d0_Handle.initialize(m_systematicsList, m_softmuonsHandle,
                                          SG::AllowEmpty));
  ANA_CHECK(m_softmu_reducedChi2_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_reducedChi2_ID_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_reducedChi2_ME_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_reducedSegmentChi2_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_momentumBalanceSignificance_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_scatteringCurvatureSignificance_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_scatteringNeighbourSignificance_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_qOverPSignificance_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_rhoPrime_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_segmentDeltaEta_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_spectrometerFieldIntegral_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));

  ANA_CHECK(m_softmu_nprecisionLayers_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_nprecisionHoleLayers_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_nGoodPrecLayers_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_innerSmallHits_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_innerLargeHits_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_innerSmallHoles_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_innerLargeHoles_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_middleSmallHits_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_middleLargeHits_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_middleSmallHoles_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_middleLargeHoles_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_outerSmallHits_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_outerLargeHits_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_outerSmallHoles_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_outerLargeHoles_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_extendedSmallHits_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_extendedLargeHits_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_extendedSmallHoles_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_extendedLargeHoles_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_innerClosePrecisionHits_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_middleClosePrecisionHits_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_outerClosePrecisionHits_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_extendedClosePrecisionHits_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_innerOutBoundsPrecisionHits_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_middleOutBoundsPrecisionHits_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_outerOutBoundsPrecisionHits_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_extendedOutBoundsPrecisionHits_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_combinedTrackOutBoundsPrecisionHits_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_isSmallGoodSectors_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_isEndcapGoodLayers_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_cscUnspoiledEtaHits_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_etaLayer1STGCHits_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_etaLayer2STGCHits_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_phiLayer1STGCHits_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_phiLayer2STGCHits_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_MMHits_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_numberOfPixelHits_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_numberOfPixelDeadSensors_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_numberOfPixelHoles_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_numberOfSCTHits_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_numberOfSCTDeadSensors_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_numberOfSCTHoles_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_numberOfTRTHits_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_numberOfTRTOutliers_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_numberOfTRTHoles_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));

  ANA_CHECK(m_softmu_isBIS78_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_isBEE_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_isBMG_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));

  ANA_CHECK(
      m_softmu_ptcone20_Nonprompt_All_MaxWeightTTVA_pt500_Handle.initialize(
          m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(
      m_softmu_ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt500_Handle.initialize(
          m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(
      m_softmu_ptcone20_Nonprompt_All_MaxWeightTTVA_pt1000_Handle.initialize(
          m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(
      m_softmu_ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt1000_Handle.initialize(
          m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_neflowisol20_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_EnergyLoss_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_EnergyLossSigma_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_EnergyLossType_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_ParamEnergyLoss_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_MeasEnergyLoss_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_ParamEnergyLossSigmaPlus_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_ParamEnergyLossSigmaMinus_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_MeasEnergyLossSigma_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));

  ANA_CHECK(m_softmu_nSegments_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_Seg1ChamberId_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_Seg2ChamberId_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));

  ANA_CHECK(m_softmu_nearestJet_Index_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_nearestJet_pt_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_nearestJet_eta_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_nearestJet_phi_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_nearestJet_e_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_nearestJet_dR_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_nearestJet_EMFrac_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_nearestJet_NumTrkPt500_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_nearestJet_SumPtTrkPt500_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_nearestJet_NumTrkPt1000_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_nearestJet_SumPtTrkPt1000_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_nearestJet_N90Constituents_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_nearestJet_TrackWidthPt500_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_nearestJet_TrackWidthPt1000_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_nearestJet_Width_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_nearestJet_Charge_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_nearestJet_ChargedPFOWidthPt500_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_nearestJet_ChargedPFOWidthPt1000_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));

  ANA_CHECK(m_softmu_truthmuon_barcode_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_truthBAncestor_pdgId_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_truthBAncestor_pt_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_truthBAncestor_eta_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_truthBAncestor_phi_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_truthBAncestor_e_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_truthCAncestor_pdgId_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_truthCAncestor_pt_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_truthCAncestor_eta_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_truthCAncestor_phi_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_truthCAncestor_e_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_truthTauAncestor_pdgId_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_truthFirstBAncestor_pdgId_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
  ANA_CHECK(m_softmu_truthFirstCAncestor_pdgId_Handle.initialize(
      m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));

  // ------------------------------------------------

  // Initialize the systematic list
  ANA_CHECK(m_systematicsList.initialize());

  return StatusCode::SUCCESS;
}

StatusCode SoftMuonSelectorAlg::execute() {

  static const SG::AuxElement::Decorator<float> dec_softmu_nearestJet_dR(
      "softmu_nearestJet_dR");

  for (const auto& sys : m_systematicsList.systematicsVector()) {

    const xAOD::EventInfo* evtInfo = nullptr;
    const xAOD::JetContainer* jets = nullptr;
    const xAOD::MuonContainer* softmuons = nullptr;

    ANA_CHECK(m_eventInfoHandle.retrieve(evtInfo, sys));

    ANA_CHECK(m_jetsHandle.retrieve(jets, sys));
    ANA_CHECK(m_softmuonsHandle.retrieve(softmuons, sys));

    // Fill the output variables with a default value:
    for (const xAOD::Muon* softmuon : *softmuons) {
      m_SoftMuonJetDRminHandle.set(*softmuon, -99, sys);
      m_SoftMuonPassDRJetcut.setBool(*softmuon, 0, sys);
    }
    m_pass_SoftMuonPassDRJetcut.setBool(*evtInfo, 0, sys);

    // --------------------------------------------------------

    for (const xAOD::Muon* softmuon : *softmuons) {

      if (m_saveSoftMuonAdditionalInfo) {
        m_softmu_author_Handle.set(*softmuon, -99, sys);
        m_softmu_allAuthors_Handle.set(*softmuon, 0, sys);
        m_softmu_resolutionCategory_Handle.set(*softmuon, -99, sys);
        m_softmu_passedIDCuts_Handle.set(*softmuon, false, sys);
        m_softmu_passedHighPtCuts_Handle.set(*softmuon, false, sys);
        m_softmu_passedLowPtCuts_Handle.set(*softmuon, false, sys);
        m_softmu_isBadMuon_Handle.set(*softmuon, false, sys);

        m_softmu_pt_id_Handle.set(*softmuon, -99, sys);
        m_softmu_eta_id_Handle.set(*softmuon, -99, sys);
        m_softmu_phi_id_Handle.set(*softmuon, -99, sys);
        m_softmu_e_id_Handle.set(*softmuon, -99, sys);
        m_softmu_pt_me_Handle.set(*softmuon, -99, sys);
        m_softmu_eta_me_Handle.set(*softmuon, -99, sys);
        m_softmu_phi_me_Handle.set(*softmuon, -99, sys);
        m_softmu_e_me_Handle.set(*softmuon, -99, sys);

        m_softmu_qOverP_Handle.set(*softmuon, -99, sys);
        m_softmu_qOverP_ID_Handle.set(*softmuon, -99, sys);
        m_softmu_qOverP_ME_Handle.set(*softmuon, -99, sys);
        m_softmu_qOverPerr_Handle.set(*softmuon, -99, sys);
        m_softmu_qOverPerr_ID_Handle.set(*softmuon, -99, sys);
        m_softmu_qOverPerr_ME_Handle.set(*softmuon, -99, sys);
        m_softmu_d0_Handle.set(*softmuon, -99, sys);
        m_softmu_reducedChi2_Handle.set(*softmuon, -99, sys);
        m_softmu_reducedChi2_ID_Handle.set(*softmuon, -99, sys);
        m_softmu_reducedChi2_ME_Handle.set(*softmuon, -99, sys);
        m_softmu_reducedSegmentChi2_Handle.set(*softmuon, -99, sys);
        m_softmu_momentumBalanceSignificance_Handle.set(*softmuon, -99, sys);
        m_softmu_scatteringCurvatureSignificance_Handle.set(*softmuon, -99,
                                                            sys);
        m_softmu_scatteringNeighbourSignificance_Handle.set(*softmuon, -99,
                                                            sys);
        m_softmu_qOverPSignificance_Handle.set(*softmuon, -99, sys);
        m_softmu_rhoPrime_Handle.set(*softmuon, -99, sys);
        m_softmu_segmentDeltaEta_Handle.set(*softmuon, -99, sys);
        m_softmu_spectrometerFieldIntegral_Handle.set(*softmuon, -99, sys);

        m_softmu_nprecisionLayers_Handle.set(*softmuon, -99, sys);
        m_softmu_nprecisionHoleLayers_Handle.set(*softmuon, -99, sys);
        m_softmu_nGoodPrecLayers_Handle.set(*softmuon, -99, sys);
        m_softmu_innerSmallHits_Handle.set(*softmuon, -99, sys);
        m_softmu_innerLargeHits_Handle.set(*softmuon, -99, sys);
        m_softmu_innerSmallHoles_Handle.set(*softmuon, -99, sys);
        m_softmu_innerLargeHoles_Handle.set(*softmuon, -99, sys);
        m_softmu_middleSmallHits_Handle.set(*softmuon, -99, sys);
        m_softmu_middleLargeHits_Handle.set(*softmuon, -99, sys);
        m_softmu_middleSmallHoles_Handle.set(*softmuon, -99, sys);
        m_softmu_middleLargeHoles_Handle.set(*softmuon, -99, sys);
        m_softmu_outerSmallHits_Handle.set(*softmuon, -99, sys);
        m_softmu_outerLargeHits_Handle.set(*softmuon, -99, sys);
        m_softmu_outerSmallHoles_Handle.set(*softmuon, -99, sys);
        m_softmu_outerLargeHoles_Handle.set(*softmuon, -99, sys);
        m_softmu_extendedSmallHits_Handle.set(*softmuon, -99, sys);
        m_softmu_extendedLargeHits_Handle.set(*softmuon, -99, sys);
        m_softmu_extendedSmallHoles_Handle.set(*softmuon, -99, sys);
        m_softmu_extendedLargeHoles_Handle.set(*softmuon, -99, sys);
        m_softmu_innerClosePrecisionHits_Handle.set(*softmuon, -99, sys);
        m_softmu_middleClosePrecisionHits_Handle.set(*softmuon, -99, sys);
        m_softmu_outerClosePrecisionHits_Handle.set(*softmuon, -99, sys);
        m_softmu_extendedClosePrecisionHits_Handle.set(*softmuon, -99, sys);
        m_softmu_innerOutBoundsPrecisionHits_Handle.set(*softmuon, -99, sys);
        m_softmu_middleOutBoundsPrecisionHits_Handle.set(*softmuon, -99, sys);
        m_softmu_outerOutBoundsPrecisionHits_Handle.set(*softmuon, -99, sys);
        m_softmu_extendedOutBoundsPrecisionHits_Handle.set(*softmuon, -99, sys);
        m_softmu_combinedTrackOutBoundsPrecisionHits_Handle.set(*softmuon, -99,
                                                                sys);
        m_softmu_isSmallGoodSectors_Handle.set(*softmuon, -99, sys);
        m_softmu_isEndcapGoodLayers_Handle.set(*softmuon, -99, sys);
        m_softmu_cscUnspoiledEtaHits_Handle.set(*softmuon, -99, sys);
        m_softmu_etaLayer1STGCHits_Handle.set(*softmuon, -99, sys);
        m_softmu_etaLayer2STGCHits_Handle.set(*softmuon, -99, sys);
        m_softmu_phiLayer1STGCHits_Handle.set(*softmuon, -99, sys);
        m_softmu_phiLayer2STGCHits_Handle.set(*softmuon, -99, sys);
        m_softmu_MMHits_Handle.set(*softmuon, -99, sys);
        m_softmu_numberOfPixelHits_Handle.set(*softmuon, -99, sys);
        m_softmu_numberOfPixelDeadSensors_Handle.set(*softmuon, -99, sys);
        m_softmu_numberOfPixelHoles_Handle.set(*softmuon, -99, sys);
        m_softmu_numberOfSCTHits_Handle.set(*softmuon, -99, sys);
        m_softmu_numberOfSCTDeadSensors_Handle.set(*softmuon, -99, sys);
        m_softmu_numberOfSCTHoles_Handle.set(*softmuon, -99, sys);
        m_softmu_numberOfTRTHits_Handle.set(*softmuon, -99, sys);
        m_softmu_numberOfTRTOutliers_Handle.set(*softmuon, -99, sys);
        m_softmu_numberOfTRTHoles_Handle.set(*softmuon, -99, sys);

        m_softmu_isBIS78_Handle.set(*softmuon, false, sys);
        m_softmu_isBEE_Handle.set(*softmuon, false, sys);
        m_softmu_isBMG_Handle.set(*softmuon, false, sys);

        m_softmu_ptcone20_Nonprompt_All_MaxWeightTTVA_pt500_Handle.set(
            *softmuon, -99, sys);
        m_softmu_ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt500_Handle.set(
            *softmuon, -99, sys);
        m_softmu_ptcone20_Nonprompt_All_MaxWeightTTVA_pt1000_Handle.set(
            *softmuon, -99, sys);
        m_softmu_ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt1000_Handle.set(
            *softmuon, -99, sys);
        m_softmu_neflowisol20_Handle.set(*softmuon, -99, sys);
        m_softmu_EnergyLoss_Handle.set(*softmuon, -99, sys);
        m_softmu_EnergyLossSigma_Handle.set(*softmuon, -99, sys);
        m_softmu_EnergyLossType_Handle.set(*softmuon, -99, sys);
        m_softmu_ParamEnergyLoss_Handle.set(*softmuon, -99, sys);
        m_softmu_MeasEnergyLoss_Handle.set(*softmuon, -99, sys);
        m_softmu_ParamEnergyLossSigmaPlus_Handle.set(*softmuon, -99, sys);
        m_softmu_ParamEnergyLossSigmaMinus_Handle.set(*softmuon, -99, sys);
        m_softmu_MeasEnergyLossSigma_Handle.set(*softmuon, -99, sys);

        m_softmu_nSegments_Handle.set(*softmuon, -99, sys);
        m_softmu_Seg1ChamberId_Handle.set(*softmuon, -99, sys);
        m_softmu_Seg2ChamberId_Handle.set(*softmuon, -99, sys);
      }

      if (m_saveSoftMuonNearestJetInfo) {

        m_softmu_nearestJet_Index_Handle.set(*softmuon, -99, sys);
        m_softmu_nearestJet_pt_Handle.set(*softmuon, -99, sys);
        m_softmu_nearestJet_eta_Handle.set(*softmuon, -99, sys);
        m_softmu_nearestJet_phi_Handle.set(*softmuon, -99, sys);
        m_softmu_nearestJet_e_Handle.set(*softmuon, -99, sys);
        m_softmu_nearestJet_dR_Handle.set(*softmuon, -99, sys);
        m_softmu_nearestJet_EMFrac_Handle.set(*softmuon, -99, sys);
        m_softmu_nearestJet_NumTrkPt500_Handle.set(*softmuon, -99, sys);
        m_softmu_nearestJet_SumPtTrkPt500_Handle.set(*softmuon, -99, sys);
        m_softmu_nearestJet_NumTrkPt1000_Handle.set(*softmuon, -99, sys);
        m_softmu_nearestJet_SumPtTrkPt1000_Handle.set(*softmuon, -99, sys);
        m_softmu_nearestJet_N90Constituents_Handle.set(*softmuon, -99, sys);
        m_softmu_nearestJet_TrackWidthPt500_Handle.set(*softmuon, -99, sys);
        m_softmu_nearestJet_TrackWidthPt1000_Handle.set(*softmuon, -99, sys);
        m_softmu_nearestJet_Width_Handle.set(*softmuon, -99, sys);
        m_softmu_nearestJet_Charge_Handle.set(*softmuon, -99, sys);
        m_softmu_nearestJet_ChargedPFOWidthPt500_Handle.set(*softmuon, -99,
                                                            sys);
        m_softmu_nearestJet_ChargedPFOWidthPt1000_Handle.set(*softmuon, -99,
                                                             sys);
      }

      if (m_saveSoftMuonTruthInfo) {
        m_softmu_truthmuon_barcode_Handle.set(*softmuon, -99, sys);
        m_softmu_truthBAncestor_pdgId_Handle.set(*softmuon, 0, sys);
        m_softmu_truthBAncestor_pt_Handle.set(*softmuon, -99, sys);
        m_softmu_truthBAncestor_eta_Handle.set(*softmuon, -99, sys);
        m_softmu_truthBAncestor_phi_Handle.set(*softmuon, -99, sys);
        m_softmu_truthBAncestor_e_Handle.set(*softmuon, -99, sys);
        m_softmu_truthCAncestor_pdgId_Handle.set(*softmuon, 0, sys);
        m_softmu_truthCAncestor_pt_Handle.set(*softmuon, -99, sys);
        m_softmu_truthCAncestor_eta_Handle.set(*softmuon, -99, sys);
        m_softmu_truthCAncestor_phi_Handle.set(*softmuon, -99, sys);
        m_softmu_truthCAncestor_e_Handle.set(*softmuon, -99, sys);
        m_softmu_truthTauAncestor_pdgId_Handle.set(*softmuon, 0, sys);
        m_softmu_truthFirstBAncestor_pdgId_Handle.set(*softmuon, 0, sys);
        m_softmu_truthFirstCAncestor_pdgId_Handle.set(*softmuon, 0, sys);
      }
    }

    // --------------------------------------------------------

    // first figure out if this event even passes the selection in which we are
    // to run this KLFitter instance

    //      if (!m_selection.getBool(*evtInfo, sys))
    //      return StatusCode::SUCCESS;

    // perform selection of objects
    ConstDataVector<xAOD::MuonContainer> selected_softmuons(SG::VIEW_ELEMENTS);
    ConstDataVector<xAOD::JetContainer> selected_jets(SG::VIEW_ELEMENTS);

    for (const xAOD::Muon* softmu : *softmuons) {
      if (m_softmuonSelection.getBool(*softmu, sys))
        selected_softmuons.push_back(softmu);
    }

    for (const xAOD::Jet* jet : *jets) {
      if (m_jetSelection.getBool(*jet, sys))
        selected_jets.push_back(jet);
    }

    if (selected_softmuons.size() > 0) {

      // Calculate SoftMuonJetDRmin for each soft muon
      bool PassSoftMuonDRJetCut = false;

      for (const xAOD::Muon* softmuon : selected_softmuons) {

        auto [nearestJetIndex, dRMin] =
            SoftMuonSelectorAlg::calculateMinDRSoftMuonJet(softmuon,
                                                           selected_jets);
        dec_softmu_nearestJet_dR(*softmuon) = dRMin;

        if (dRMin < m_softmuonDRJetcut) {

          m_softmu_nearestJet_Index_Handle.set(*softmuon, nearestJetIndex, sys);
          m_SoftMuonJetDRminHandle.set(*softmuon, dRMin, sys);
          m_SoftMuonPassDRJetcut.setBool(*softmuon, true, sys);

          PassSoftMuonDRJetCut = true;
        }
      }

      if (PassSoftMuonDRJetCut) {
        m_pass_SoftMuonPassDRJetcut.setBool(*evtInfo, 1, sys);
      } else {
        m_pass_SoftMuonPassDRJetcut.setBool(*evtInfo, 0, sys);
      }

      const SG::AuxElement::ConstAccessor<float> acc_SoftMuonJetDRminHandle(
          m_SoftMuonJetDRminHandle.getName(sys));
      // If the event has any soft muons, let's fill additional variables
      if (PassSoftMuonDRJetCut) {
        // If the option is set, save additional variables related to the soft
        // muon

        for (const xAOD::Muon* softmuon : selected_softmuons) {

          // Does this muon pass the DRmin cut?
          if (acc_SoftMuonJetDRminHandle(*softmuon) < m_softmuonDRJetcut &&
              acc_SoftMuonJetDRminHandle(*softmuon) > 0) {

            if (m_saveSoftMuonAdditionalInfo) {
              SoftMuonSelectorAlg::SaveAdditionalSoftMuonVariables(
                  softmuon, /*evtInfo,*/ sys);
            }

            if (m_saveSoftMuonNearestJetInfo) {
              SoftMuonSelectorAlg::SaveAdditionalInformationFromNearestJet(
                  softmuon, selected_jets, sys);
            }
            if (m_saveSoftMuonTruthInfo) {
              ANA_CHECK(
                  SoftMuonSelectorAlg::SaveAdditionalSoftMuonTruthVariables(
                      softmuon, sys));
            }
          }
        }
      }
    }
  }

  return StatusCode::SUCCESS;
}

// save additional truth muon information
StatusCode SoftMuonSelectorAlg::SaveAdditionalSoftMuonTruthVariables(
    const xAOD::Muon* softmuon, const CP::SystematicSet& sys) {

  const xAOD::TruthParticle* truthmu =
      xAOD::TruthHelpers::getTruthParticle(*softmuon);
  if (!truthmu)
    return StatusCode::SUCCESS;

  const xAOD::TruthParticle* lastBAncestor = nullptr;
  const xAOD::TruthParticle* lastCAncestor = nullptr;
  const xAOD::TruthParticle* lastTauAncestor = nullptr;
  const xAOD::TruthParticle* firstBAncestor = nullptr;
  const xAOD::TruthParticle* firstCAncestor = nullptr;
  const xAOD::TruthParticleContainer* contHF = nullptr;
  ANA_CHECK(evtStore()->retrieve(
      contHF,
      "TruthHFWithDecayParticles"));  // we need this container, available in
                                      // DAOD_PHYS, to be able to navigate back
                                      // HF ancestors of the muon
  if (!contHF) {
    ANA_MSG_ERROR(
        "Cannot retrieve TruthHFWithDecayParticles, you cannot save "
        "additional soft muon variables without that");
    return StatusCode::FAILURE;
  }
  top::getSoftMuonAncestorInformation(truthmu, lastBAncestor, lastCAncestor,
                                      lastTauAncestor, firstBAncestor,
                                      firstCAncestor, contHF);

  // filling output variables
  m_softmu_truthmuon_barcode_Handle.set(*softmuon, truthmu->barcode(), sys);
  m_softmu_truthBAncestor_pdgId_Handle.set(
      *softmuon, lastBAncestor ? lastBAncestor->pdgId() : 0, sys);
  m_softmu_truthBAncestor_pt_Handle.set(
      *softmuon, lastBAncestor ? lastBAncestor->pt() : -99, sys);
  m_softmu_truthBAncestor_eta_Handle.set(
      *softmuon, lastBAncestor ? lastBAncestor->eta() : -99, sys);
  m_softmu_truthBAncestor_phi_Handle.set(
      *softmuon, lastBAncestor ? lastBAncestor->phi() : -99, sys);
  m_softmu_truthBAncestor_e_Handle.set(
      *softmuon, lastBAncestor ? lastBAncestor->e() : -99, sys);
  m_softmu_truthCAncestor_pdgId_Handle.set(
      *softmuon, lastCAncestor ? lastCAncestor->pdgId() : 0, sys);
  m_softmu_truthCAncestor_pt_Handle.set(
      *softmuon, lastCAncestor ? lastCAncestor->pt() : -99, sys);
  m_softmu_truthCAncestor_eta_Handle.set(
      *softmuon, lastCAncestor ? lastCAncestor->eta() : -99, sys);
  m_softmu_truthCAncestor_phi_Handle.set(
      *softmuon, lastCAncestor ? lastCAncestor->phi() : -99, sys);
  m_softmu_truthCAncestor_e_Handle.set(
      *softmuon, lastCAncestor ? lastCAncestor->e() : -99, sys);
  m_softmu_truthTauAncestor_pdgId_Handle.set(
      *softmuon, lastTauAncestor ? lastTauAncestor->pdgId() : 0, sys);
  m_softmu_truthFirstBAncestor_pdgId_Handle.set(
      *softmuon, firstBAncestor ? firstBAncestor->pdgId() : 0, sys);
  m_softmu_truthFirstCAncestor_pdgId_Handle.set(
      *softmuon, firstCAncestor ? firstCAncestor->pdgId() : 0, sys);

  return StatusCode::SUCCESS;
}  // end of SoftMuonSelectorAlg::SaveAdditionalSoftMuonTruthVariables

std::pair<int, float> SoftMuonSelectorAlg::calculateMinDRSoftMuonJet(
    const xAOD::Muon* softmuon,
    const ConstDataVector<xAOD::JetContainer>& selected_jets) {

  int nearestJetIndex = -1;
  float dRMin = 100.0;

  // Loop over jets, calculate dR and record smallest value
  int ijet = 0;
  for (const xAOD::Jet* jet : selected_jets) {

    float dr = softmuon->p4().DeltaR(jet->p4(), m_softMuonDRJetUseRapidity);

    if (dr < dRMin) {
      dRMin = dr;
      nearestJetIndex = ijet;
    }
    ijet++;
  }

  //    ANA_MSG_INFO("  DR min " << dRMin);

  return std::make_pair(nearestJetIndex, dRMin);
}

// Safe addigional information closes jet

// Save additional soft muon related variables.
void SoftMuonSelectorAlg::SaveAdditionalSoftMuonVariables(
    const xAOD::Muon* softmuon,
    /*const xAOD::EventInfo *evtInfo,*/ const CP::SystematicSet& sys) {

  auto [idPt, mePt] = SoftMuonSelectorAlg::IdMsPt(*softmuon);

  m_softmu_pt_id_Handle.set(*softmuon, idPt, sys);
  m_softmu_pt_me_Handle.set(*softmuon, mePt, sys);

  const xAOD::TrackParticle* idtrack =
      softmuon->trackParticle(xAOD::Muon::InnerDetectorTrackParticle);
  const xAOD::TrackParticle* metrack = softmuon->trackParticle(
      xAOD::Muon::ExtrapolatedMuonSpectrometerTrackParticle);
  const xAOD::TrackParticle* cbtrack =
      softmuon->trackParticle(xAOD::Muon::CombinedTrackParticle);

  if (idtrack) {
    m_softmu_eta_id_Handle.set(*softmuon, idtrack->eta(), sys);
    m_softmu_phi_id_Handle.set(*softmuon, idtrack->phi(), sys);
    m_softmu_e_id_Handle.set(*softmuon, idtrack->e(), sys);
    m_softmu_qOverP_ID_Handle.set(
        *softmuon, idtrack->charge() * sin(idtrack->theta()) / idPt, sys);
  } else {
    m_softmu_eta_id_Handle.set(*softmuon, -999., sys);
    m_softmu_phi_id_Handle.set(*softmuon, -999., sys);
    m_softmu_e_id_Handle.set(*softmuon, -999., sys);
    m_softmu_qOverP_ID_Handle.set(*softmuon, -999., sys);
  }
  if (metrack) {
    m_softmu_eta_me_Handle.set(*softmuon, metrack->eta(), sys);
    m_softmu_phi_me_Handle.set(*softmuon, metrack->phi(), sys);
    m_softmu_e_me_Handle.set(*softmuon, metrack->e(), sys);
    m_softmu_qOverP_ME_Handle.set(
        *softmuon, metrack->charge() * sin(metrack->theta()) / mePt, sys);
  } else {
    m_softmu_eta_me_Handle.set(*softmuon, -999., sys);
    m_softmu_phi_me_Handle.set(*softmuon, -999., sys);
    m_softmu_e_me_Handle.set(*softmuon, -999., sys);
    m_softmu_qOverP_ME_Handle.set(*softmuon, -999., sys);
  }
  if (cbtrack) {
    m_softmu_qOverP_Handle.set(
        *softmuon, cbtrack->charge() * sin(cbtrack->theta()) / softmuon->pt(),
        sys);
  } else {
    m_softmu_qOverP_Handle.set(*softmuon, -999., sys);
  }
  m_softmu_author_Handle.set(*softmuon, softmuon->author(), sys);
  m_softmu_allAuthors_Handle.set(*softmuon, softmuon->allAuthors(), sys);
  m_softmu_resolutionCategory_Handle.set(
      *softmuon, m_selTool_Tight->getResolutionCategory(*softmuon), sys);
  m_softmu_passedIDCuts_Handle.set(
      *softmuon, m_selTool_Tight->passedIDCuts(*softmuon), sys);
  m_softmu_passedHighPtCuts_Handle.set(
      *softmuon, m_selTool_Tight->passedHighPtCuts(*softmuon), sys);
  m_softmu_passedLowPtCuts_Handle.set(
      *softmuon, m_selTool_Tight->passedLowPtEfficiencyCuts(*softmuon), sys);
  m_softmu_isBadMuon_Handle.set(*softmuon,
                                m_selTool_Tight->isBadMuon(*softmuon), sys);

  float qOverPerr = -999;
  float qOverPerr_ID = -999;
  float qOverPerr_ME = -999;
  std::vector<float> CovMatrixVec;
  if (cbtrack && cbtrack->definingParametersCovMatrixVec().size() == 15)
    qOverPerr = sqrt(cbtrack->definingParametersCovMatrix()(4, 4));
  if (idtrack && idtrack->definingParametersCovMatrixVec().size() == 15)
    qOverPerr_ID = sqrt(idtrack->definingParametersCovMatrix()(4, 4));
  if (metrack && metrack->definingParametersCovMatrixVec().size() == 15)
    qOverPerr_ME = sqrt(metrack->definingParametersCovMatrix()(4, 4));
  m_softmu_qOverPerr_Handle.set(*softmuon, qOverPerr, sys);
  m_softmu_qOverPerr_ID_Handle.set(*softmuon, qOverPerr_ID, sys);
  m_softmu_qOverPerr_ME_Handle.set(*softmuon, qOverPerr_ME, sys);

  // Impact parameter variables. d0sig and z0sintetha are automatically witten
  // in output when setting writeTrackD0Z0: True in the WP part of the
  // configuration file. We only need to add d0.
  float d0 = softmuon->primaryTrackParticle()->d0();
  m_softmu_d0_Handle.set(*softmuon, d0, sys);

  static const SG::AuxElement::ConstAccessor<float> acc_numberDoF("numberDoF");

  m_softmu_reducedChi2_Handle.set(
      *softmuon,
      (cbtrack && acc_numberDoF.isAvailable(*cbtrack) &&
               cbtrack->numberDoF() > 0
           ? cbtrack->chiSquared() / cbtrack->numberDoF()
           : -999),
      sys);
  m_softmu_reducedChi2_ID_Handle.set(
      *softmuon,
      (idtrack && acc_numberDoF.isAvailable(*idtrack) &&
               idtrack->numberDoF() > 0
           ? idtrack->chiSquared() / idtrack->numberDoF()
           : -999),
      sys);
  m_softmu_reducedChi2_ME_Handle.set(
      *softmuon,
      (metrack && acc_numberDoF.isAvailable(*metrack) &&
               metrack->numberDoF() > 0
           ? metrack->chiSquared() / metrack->numberDoF()
           : -999),
      sys);
  float reducedSegmentChi2 = -999, reducedSegmentChi2Val = -999;
  if (softmuon->parameter(reducedSegmentChi2Val,
                          xAOD::Muon::segmentChi2OverDoF))
    reducedSegmentChi2 = reducedSegmentChi2Val;
  m_softmu_reducedSegmentChi2_Handle.set(*softmuon, reducedSegmentChi2, sys);

  float momentumBalanceSignificance = -999;
  softmuon->parameter(momentumBalanceSignificance,
                      xAOD::Muon::momentumBalanceSignificance);
  m_softmu_momentumBalanceSignificance_Handle.set(
      *softmuon, momentumBalanceSignificance, sys);
  float scatteringCurvatureSignificance = -999;
  softmuon->parameter(scatteringCurvatureSignificance,
                      xAOD::Muon::scatteringCurvatureSignificance);
  m_softmu_scatteringCurvatureSignificance_Handle.set(
      *softmuon, scatteringCurvatureSignificance, sys);
  float scatteringNeighbourSignificance = -999;
  softmuon->parameter(scatteringNeighbourSignificance,
                      xAOD::Muon::scatteringNeighbourSignificance);
  m_softmu_scatteringNeighbourSignificance_Handle.set(
      *softmuon, scatteringNeighbourSignificance, sys);

  float qOverPSignificance = m_selTool_Tight->qOverPsignificance(*softmuon);
  m_softmu_qOverPSignificance_Handle.set(*softmuon, qOverPSignificance, sys);

  float rhoP = m_selTool_Tight->rhoPrime(*softmuon);
  m_softmu_rhoPrime_Handle.set(*softmuon, rhoP, sys);
  static const SG::AuxElement::ConstAccessor<float> segmentDeltaEta_acc(
      "segmentDeltaEta");
  float segmentDeltaEta = segmentDeltaEta_acc(*softmuon);

  m_softmu_segmentDeltaEta_Handle.set(*softmuon, segmentDeltaEta, sys);

  float value = -999;
  m_softmu_spectrometerFieldIntegral_Handle.set(
      *softmuon,
      (softmuon->parameter(value, xAOD::Muon::spectrometerFieldIntegral)
           ? value
           : -999),
      sys);

  hitSummary summary{};
  SoftMuonSelectorAlg::fillSummary(*softmuon, summary);
  m_softmu_nprecisionLayers_Handle.set(*softmuon, summary.nprecisionLayers,
                                       sys);
  m_softmu_nprecisionHoleLayers_Handle.set(*softmuon,
                                           summary.nprecisionHoleLayers, sys);
  m_softmu_nGoodPrecLayers_Handle.set(*softmuon, summary.nGoodPrecLayers, sys);
  m_softmu_innerSmallHits_Handle.set(*softmuon, summary.innerSmallHits, sys);
  m_softmu_innerLargeHits_Handle.set(*softmuon, summary.innerLargeHits, sys);
  m_softmu_innerSmallHoles_Handle.set(*softmuon, summary.innerSmallHoles, sys);
  m_softmu_innerLargeHoles_Handle.set(*softmuon, summary.innerLargeHoles, sys);
  m_softmu_middleSmallHits_Handle.set(*softmuon, summary.middleSmallHits, sys);
  m_softmu_middleLargeHits_Handle.set(*softmuon, summary.middleLargeHits, sys);
  m_softmu_middleSmallHoles_Handle.set(*softmuon, summary.middleSmallHoles,
                                       sys);
  m_softmu_middleLargeHoles_Handle.set(*softmuon, summary.middleLargeHoles,
                                       sys);
  m_softmu_outerSmallHits_Handle.set(*softmuon, summary.outerSmallHits, sys);
  m_softmu_outerLargeHits_Handle.set(*softmuon, summary.outerLargeHits, sys);
  m_softmu_outerSmallHoles_Handle.set(*softmuon, summary.outerSmallHoles, sys);
  m_softmu_outerLargeHoles_Handle.set(*softmuon, summary.outerLargeHoles, sys);
  m_softmu_extendedSmallHits_Handle.set(*softmuon, summary.extendedSmallHits,
                                        sys);
  m_softmu_extendedLargeHits_Handle.set(*softmuon, summary.extendedLargeHits,
                                        sys);
  m_softmu_extendedSmallHoles_Handle.set(*softmuon, summary.extendedSmallHoles,
                                         sys);
  m_softmu_extendedLargeHoles_Handle.set(*softmuon, summary.extendedLargeHoles,
                                         sys);
  m_softmu_innerClosePrecisionHits_Handle.set(
      *softmuon, summary.innerClosePrecisionHits, sys);
  m_softmu_middleClosePrecisionHits_Handle.set(
      *softmuon, summary.middleClosePrecisionHits, sys);
  m_softmu_outerClosePrecisionHits_Handle.set(
      *softmuon, summary.outerClosePrecisionHits, sys);
  m_softmu_extendedClosePrecisionHits_Handle.set(
      *softmuon, summary.extendedClosePrecisionHits, sys);
  m_softmu_innerOutBoundsPrecisionHits_Handle.set(
      *softmuon, summary.innerOutBoundsPrecisionHits, sys);
  m_softmu_middleOutBoundsPrecisionHits_Handle.set(
      *softmuon, summary.middleOutBoundsPrecisionHits, sys);
  m_softmu_outerOutBoundsPrecisionHits_Handle.set(
      *softmuon, summary.outerOutBoundsPrecisionHits, sys);
  m_softmu_extendedOutBoundsPrecisionHits_Handle.set(
      *softmuon, summary.extendedOutBoundsPrecisionHits, sys);
  m_softmu_combinedTrackOutBoundsPrecisionHits_Handle.set(
      *softmuon, summary.combinedTrackOutBoundsPrecisionHits, sys);
  m_softmu_isSmallGoodSectors_Handle.set(*softmuon, summary.isSmallGoodSectors,
                                         sys);
  m_softmu_isEndcapGoodLayers_Handle.set(*softmuon, summary.isEndcapGoodLayers,
                                         sys);
  m_softmu_cscUnspoiledEtaHits_Handle.set(*softmuon,
                                          summary.cscUnspoiledEtaHits, sys);
  m_softmu_etaLayer1STGCHits_Handle.set(*softmuon, summary.etaLayer1STGCHits,
                                        sys);
  m_softmu_etaLayer2STGCHits_Handle.set(*softmuon, summary.etaLayer2STGCHits,
                                        sys);
  m_softmu_phiLayer1STGCHits_Handle.set(*softmuon, summary.phiLayer1STGCHits,
                                        sys);
  m_softmu_phiLayer2STGCHits_Handle.set(*softmuon, summary.phiLayer2STGCHits,
                                        sys);
  m_softmu_MMHits_Handle.set(*softmuon, summary.MMHits, sys);
  m_softmu_numberOfPixelHits_Handle.set(*softmuon, summary.numberOfPixelHits,
                                        sys);
  m_softmu_numberOfPixelDeadSensors_Handle.set(
      *softmuon, summary.numberOfPixelDeadSensors, sys);
  m_softmu_numberOfPixelHoles_Handle.set(*softmuon, summary.numberOfPixelHoles,
                                         sys);
  m_softmu_numberOfSCTHits_Handle.set(*softmuon, summary.numberOfSCTHits, sys);
  m_softmu_numberOfSCTDeadSensors_Handle.set(
      *softmuon, summary.numberOfSCTDeadSensors, sys);
  m_softmu_numberOfSCTHoles_Handle.set(*softmuon, summary.numberOfSCTHoles,
                                       sys);
  m_softmu_numberOfTRTHits_Handle.set(*softmuon, summary.numberOfTRTHits, sys);
  m_softmu_numberOfTRTOutliers_Handle.set(*softmuon,
                                          summary.numberOfTRTOutliers, sys);
  m_softmu_numberOfTRTHoles_Handle.set(*softmuon, summary.numberOfTRTHoles,
                                       sys);

  bool isBIS78 = m_selTool_Tight->isBIS78(softmuon->eta(), softmuon->phi());
  m_softmu_isBIS78_Handle.set(*softmuon, isBIS78, sys);
  bool isBEE = m_selTool_Tight->isBEE(softmuon->eta(), softmuon->phi());
  m_softmu_isBEE_Handle.set(*softmuon, isBEE, sys);
  bool isBMG = m_selTool_Tight->isBMG(softmuon->eta(), softmuon->phi());
  m_softmu_isBMG_Handle.set(*softmuon, isBMG, sys);

  static const SG::AuxElement::ConstAccessor<float>
      ptcone20_Nonprompt_All_MaxWeightTTVA_pt500_acc(
          "ptcone20_Nonprompt_All_MaxWeightTTVA_pt500");
  float mu_ptcone20_Nonprompt_All_MaxWeightTTVA_pt500 =
      ptcone20_Nonprompt_All_MaxWeightTTVA_pt500_acc(*softmuon);
  static const SG::AuxElement::ConstAccessor<float>
      ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt500_acc(
          "ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt500");
  float mu_ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt500 =
      ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt500_acc(*softmuon);
  static const SG::AuxElement::ConstAccessor<float>
      ptcone20_Nonprompt_All_MaxWeightTTVA_pt1000_acc(
          "ptcone20_Nonprompt_All_MaxWeightTTVA_pt1000");
  float mu_ptcone20_Nonprompt_All_MaxWeightTTVA_pt1000 =
      ptcone20_Nonprompt_All_MaxWeightTTVA_pt1000_acc(*softmuon);
  static const SG::AuxElement::ConstAccessor<float>
      ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt1000_acc(
          "ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt1000");
  float mu_ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt1000 =
      ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt1000_acc(*softmuon);
  static const SG::AuxElement::ConstAccessor<float> neflowisol20_acc(
      "neflowisol20");
  float mu_neflowisol20 = neflowisol20_acc(*softmuon);

  m_softmu_ptcone20_Nonprompt_All_MaxWeightTTVA_pt500_Handle.set(
      *softmuon, mu_ptcone20_Nonprompt_All_MaxWeightTTVA_pt500, sys);
  m_softmu_ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt500_Handle.set(
      *softmuon, mu_ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt500, sys);
  m_softmu_ptcone20_Nonprompt_All_MaxWeightTTVA_pt1000_Handle.set(
      *softmuon, mu_ptcone20_Nonprompt_All_MaxWeightTTVA_pt1000, sys);
  m_softmu_ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt1000_Handle.set(
      *softmuon, mu_ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt1000, sys);
  m_softmu_neflowisol20_Handle.set(*softmuon, mu_neflowisol20, sys);

  static const SG::AuxElement::ConstAccessor<float> EnergyLoss("EnergyLoss");
  static const SG::AuxElement::ConstAccessor<float> EnergyLossSigma(
      "EnergyLossSigma");
  static const SG::AuxElement::ConstAccessor<unsigned char> energyLossType(
      "energyLossType");
  static const SG::AuxElement::ConstAccessor<float> ParamEnergyLoss(
      "ParamEnergyLoss");
  static const SG::AuxElement::ConstAccessor<float> MeasEnergyLoss(
      "MeasEnergyLoss");
  static const SG::AuxElement::ConstAccessor<float> ParamEnergyLossSigmaPlus(
      "ParamEnergyLossSigmaPlus");
  static const SG::AuxElement::ConstAccessor<float> ParamEnergyLossSigmaMinus(
      "ParamEnergyLossSigmaMinus");
  static const SG::AuxElement::ConstAccessor<float> MeasEnergyLossSigma(
      "MeasEnergyLossSigma");

  m_softmu_EnergyLoss_Handle.set(
      *softmuon,
      (EnergyLoss.isAvailable(*softmuon) ? EnergyLoss(*softmuon) : -999), sys);
  m_softmu_EnergyLossSigma_Handle.set(
      *softmuon,
      (EnergyLossSigma.isAvailable(*softmuon) ? EnergyLossSigma(*softmuon)
                                              : -999),
      sys);
  m_softmu_EnergyLossType_Handle.set(
      *softmuon,
      (energyLossType.isAvailable(*softmuon) ? energyLossType(*softmuon)
                                             : -999),
      sys);
  m_softmu_ParamEnergyLoss_Handle.set(
      *softmuon,
      (ParamEnergyLoss.isAvailable(*softmuon) ? ParamEnergyLoss(*softmuon)
                                              : -999),
      sys);
  m_softmu_MeasEnergyLoss_Handle.set(
      *softmuon,
      (MeasEnergyLoss.isAvailable(*softmuon) ? MeasEnergyLoss(*softmuon)
                                             : -999),
      sys);
  m_softmu_ParamEnergyLossSigmaPlus_Handle.set(
      *softmuon,
      (ParamEnergyLossSigmaPlus.isAvailable(*softmuon)
           ? ParamEnergyLossSigmaPlus(*softmuon)
           : -999),
      sys);
  m_softmu_ParamEnergyLossSigmaMinus_Handle.set(
      *softmuon,
      (ParamEnergyLossSigmaMinus.isAvailable(*softmuon)
           ? ParamEnergyLossSigmaMinus(*softmuon)
           : -999),
      sys);
  m_softmu_MeasEnergyLossSigma_Handle.set(
      *softmuon,
      (MeasEnergyLossSigma.isAvailable(*softmuon)
           ? MeasEnergyLossSigma(*softmuon)
           : -999),
      sys);

  std::vector<const xAOD::MuonSegment*> segments =
      m_selTool_Tight->getSegmentsSorted(*softmuon);

  m_softmu_nSegments_Handle.set(*softmuon, segments.size(), sys);
  int muonSeg1ChamberId =
      (segments.size() > 0) ? segments[0]->chamberIndex() : -99;
  m_softmu_Seg1ChamberId_Handle.set(*softmuon, muonSeg1ChamberId, sys);
  int muonSeg2ChamberId =
      (segments.size() > 1) ? segments[1]->chamberIndex() : -99;
  m_softmu_Seg2ChamberId_Handle.set(*softmuon, muonSeg2ChamberId, sys);
  return;
}

void SoftMuonSelectorAlg::SaveAdditionalInformationFromNearestJet(
    const xAOD::Muon* softmuon,
    const ConstDataVector<xAOD::JetContainer>& selected_jets,
    const CP::SystematicSet& sys) {

  static const SG::AuxElement::ConstAccessor<float> acc_softmu_nearestJet_dR(
      "softmu_nearestJet_dR");
  // Get the index of the nearest jet
  int nearestJetIndex = -1;

  const std::string& decorName = m_softmu_nearestJet_Index_Handle.getName(sys);
  const SG::AuxElement::ConstAccessor<int> acc_decorName(decorName);

  if (acc_decorName.isAvailable(*softmuon)) {
    nearestJetIndex = acc_decorName(*softmuon);
  }

  int ijet = 0;
  for (const xAOD::Jet* jet : selected_jets) {

    if (ijet == nearestJetIndex) {

      m_softmu_nearestJet_dR_Handle.set(
          *softmuon, acc_softmu_nearestJet_dR(*softmuon), sys);
      m_softmu_nearestJet_pt_Handle.set(
          *softmuon, (nearestJetIndex >= 0 ? jet->pt() : -999), sys);
      m_softmu_nearestJet_eta_Handle.set(
          *softmuon, (nearestJetIndex >= 0 ? jet->eta() : -999), sys);
      m_softmu_nearestJet_phi_Handle.set(
          *softmuon, (nearestJetIndex >= 0 ? jet->phi() : -999), sys);
      m_softmu_nearestJet_e_Handle.set(
          *softmuon, (nearestJetIndex >= 0 ? jet->e() : -999), sys);
      m_softmu_nearestJet_EMFrac_Handle.set(
          *softmuon,
          (nearestJetIndex >= 0 ? retrieveFloatJetMoment(jet, "EMFrac") : -999),
          sys);
      m_softmu_nearestJet_NumTrkPt500_Handle.set(
          *softmuon,
          (nearestJetIndex >= 0
               ? retrieveIntJetMomentFromVector(jet, "NumTrkPt500", 0)
               : -999),
          sys);
      m_softmu_nearestJet_SumPtTrkPt500_Handle.set(
          *softmuon,
          (nearestJetIndex >= 0
               ? retrieveFloatJetMomentFromVector(jet, "SumPtTrkPt500", 0)
               : -999),
          sys);
      m_softmu_nearestJet_NumTrkPt1000_Handle.set(
          *softmuon,
          (nearestJetIndex >= 0
               ? retrieveIntJetMomentFromVector(jet, "NumTrkPt1000", 0)
               : -999),
          sys);
      m_softmu_nearestJet_SumPtTrkPt1000_Handle.set(
          *softmuon,
          (nearestJetIndex >= 0
               ? retrieveFloatJetMomentFromVector(jet, "SumPtTrkPt1000", 0)
               : -999),
          sys);
      m_softmu_nearestJet_N90Constituents_Handle.set(
          *softmuon,
          (nearestJetIndex >= 0 ? retrieveFloatJetMoment(jet, "N90Constituents")
                                : -999),
          sys);
      m_softmu_nearestJet_TrackWidthPt500_Handle.set(
          *softmuon,
          (nearestJetIndex >= 0
               ? retrieveFloatJetMomentFromVector(jet, "TrackWidthPt500", 0)
               : -999),
          sys);
      m_softmu_nearestJet_TrackWidthPt1000_Handle.set(
          *softmuon,
          (nearestJetIndex >= 0
               ? retrieveFloatJetMomentFromVector(jet, "TrackWidthPt1000", 0)
               : -999),
          sys);
      m_softmu_nearestJet_Width_Handle.set(
          *softmuon,
          (nearestJetIndex >= 0 ? retrieveFloatJetMoment(jet, "Width") : -999),
          sys);
      m_softmu_nearestJet_Charge_Handle.set(
          *softmuon,
          (nearestJetIndex >= 0 ? retrieveFloatJetMoment(jet, "Charge") : -999),
          sys);
      m_softmu_nearestJet_ChargedPFOWidthPt1000_Handle.set(
          *softmuon,
          (nearestJetIndex >= 0 ? retrieveFloatJetMomentFromVector(
                                      jet, "ChargedPFOWidthPt1000", 0)
                                : -999),
          sys);
      m_softmu_nearestJet_ChargedPFOWidthPt500_Handle.set(
          *softmuon,
          (nearestJetIndex >= 0 ? retrieveFloatJetMomentFromVector(
                                      jet, "ChargedPFOWidthPt500", 0)
                                : -999),
          sys);

      break;  // Exit the loop once the nearest jet is found
    }
    ijet++;
  }

  return;
}

// -----------------------------------------------------------------------------------------------------
// Auxiliary functions to hel in the calculation of some of the additional
// variables
void SoftMuonSelectorAlg::fillSummary(const xAOD::Muon& muon,
                                      hitSummary& summary) const {
  retrieveSummaryValue(muon, summary.nprecisionLayers,
                       xAOD::SummaryType::numberOfPrecisionLayers);
  retrieveSummaryValue(muon, summary.nprecisionHoleLayers,
                       xAOD::SummaryType::numberOfPrecisionHoleLayers);
  retrieveSummaryValue(muon, summary.nGoodPrecLayers,
                       xAOD::numberOfGoodPrecisionLayers);
  retrieveSummaryValue(muon, summary.innerSmallHits,
                       xAOD::MuonSummaryType::innerSmallHits);
  retrieveSummaryValue(muon, summary.innerLargeHits,
                       xAOD::MuonSummaryType::innerLargeHits);
  retrieveSummaryValue(muon, summary.innerSmallHoles,
                       xAOD::MuonSummaryType::innerSmallHoles);
  retrieveSummaryValue(muon, summary.innerLargeHoles,
                       xAOD::MuonSummaryType::innerLargeHoles);
  retrieveSummaryValue(muon, summary.middleSmallHits,
                       xAOD::MuonSummaryType::middleSmallHits);
  retrieveSummaryValue(muon, summary.middleLargeHits,
                       xAOD::MuonSummaryType::middleLargeHits);
  retrieveSummaryValue(muon, summary.middleSmallHoles,
                       xAOD::MuonSummaryType::middleSmallHoles);
  retrieveSummaryValue(muon, summary.middleLargeHoles,
                       xAOD::MuonSummaryType::middleLargeHoles);
  retrieveSummaryValue(muon, summary.outerSmallHits,
                       xAOD::MuonSummaryType::outerSmallHits);
  retrieveSummaryValue(muon, summary.outerLargeHits,
                       xAOD::MuonSummaryType::outerLargeHits);
  retrieveSummaryValue(muon, summary.outerSmallHoles,
                       xAOD::MuonSummaryType::outerSmallHoles);
  retrieveSummaryValue(muon, summary.outerLargeHoles,
                       xAOD::MuonSummaryType::outerLargeHoles);
  retrieveSummaryValue(muon, summary.extendedSmallHits,
                       xAOD::MuonSummaryType::extendedSmallHits);
  retrieveSummaryValue(muon, summary.extendedLargeHits,
                       xAOD::MuonSummaryType::extendedLargeHits);
  retrieveSummaryValue(muon, summary.extendedSmallHoles,
                       xAOD::MuonSummaryType::extendedSmallHoles);
  retrieveSummaryValue(muon, summary.extendedLargeHoles,
                       xAOD::MuonSummaryType::extendedLargeHoles);
  retrieveSummaryValue(muon, summary.innerClosePrecisionHits,
                       xAOD::MuonSummaryType::innerClosePrecisionHits);
  retrieveSummaryValue(muon, summary.middleClosePrecisionHits,
                       xAOD::MuonSummaryType::middleClosePrecisionHits);
  retrieveSummaryValue(muon, summary.outerClosePrecisionHits,
                       xAOD::MuonSummaryType::outerClosePrecisionHits);
  retrieveSummaryValue(muon, summary.extendedClosePrecisionHits,
                       xAOD::MuonSummaryType::extendedClosePrecisionHits);
  retrieveSummaryValue(muon, summary.innerOutBoundsPrecisionHits,
                       xAOD::MuonSummaryType::innerOutBoundsPrecisionHits);
  retrieveSummaryValue(muon, summary.middleOutBoundsPrecisionHits,
                       xAOD::MuonSummaryType::middleOutBoundsPrecisionHits);
  retrieveSummaryValue(muon, summary.outerOutBoundsPrecisionHits,
                       xAOD::MuonSummaryType::outerOutBoundsPrecisionHits);
  retrieveSummaryValue(muon, summary.extendedOutBoundsPrecisionHits,
                       xAOD::MuonSummaryType::extendedOutBoundsPrecisionHits);
  retrieveSummaryValue(
      muon, summary.combinedTrackOutBoundsPrecisionHits,
      xAOD::MuonSummaryType::combinedTrackOutBoundsPrecisionHits);
  retrieveSummaryValue(muon, summary.isSmallGoodSectors,
                       xAOD::MuonSummaryType::isSmallGoodSectors);
  retrieveSummaryValue(muon, summary.isEndcapGoodLayers,
                       xAOD::MuonSummaryType::isEndcapGoodLayers);
  if (!m_isRun3)
    retrieveSummaryValue(muon, summary.cscUnspoiledEtaHits,
                         xAOD::MuonSummaryType::cscUnspoiledEtaHits);
  if (m_isRun3) {
    retrieveSummaryValue(muon, summary.etaLayer1STGCHits,
                         xAOD::MuonSummaryType::etaLayer1STGCHits);
    retrieveSummaryValue(muon, summary.etaLayer2STGCHits,
                         xAOD::MuonSummaryType::etaLayer2STGCHits);
    retrieveSummaryValue(muon, summary.phiLayer1STGCHits,
                         xAOD::MuonSummaryType::phiLayer1STGCHits);
    retrieveSummaryValue(muon, summary.phiLayer2STGCHits,
                         xAOD::MuonSummaryType::phiLayer2STGCHits);
    retrieveSummaryValue(muon, summary.MMHits, xAOD::MuonSummaryType::MMHits);
  }
  if (muon.author() == xAOD::Muon::MuidSA && std::abs(muon.eta()) > 2.5)
    return;
  if (muon.muonType() == xAOD::Muon::SiliconAssociatedForwardMuon)
    return;

  const xAOD::TrackParticle* track =
      muon.trackParticle(xAOD::Muon::InnerDetectorTrackParticle);
  if (!track)
    track = muon.primaryTrackParticle();
  if (!track)
    return;

  retrieveSummaryValue(*track, summary.numberOfPixelHits,
                       xAOD::SummaryType::numberOfPixelHits);
  retrieveSummaryValue(*track, summary.numberOfPixelDeadSensors,
                       xAOD::SummaryType::numberOfPixelDeadSensors);
  retrieveSummaryValue(*track, summary.numberOfPixelHoles,
                       xAOD::SummaryType::numberOfPixelHoles);
  retrieveSummaryValue(*track, summary.numberOfSCTHits,
                       xAOD::SummaryType::numberOfSCTHits);
  retrieveSummaryValue(*track, summary.numberOfSCTDeadSensors,
                       xAOD::SummaryType::numberOfSCTDeadSensors);
  retrieveSummaryValue(*track, summary.numberOfSCTHoles,
                       xAOD::SummaryType::numberOfSCTHoles);
  retrieveSummaryValue(*track, summary.numberOfTRTHits,
                       xAOD::SummaryType::numberOfTRTHits);
  retrieveSummaryValue(*track, summary.numberOfTRTOutliers,
                       xAOD::SummaryType::numberOfTRTOutliers);
  // retrieveSummaryValue(*track, summary.numberOfTRTHoles,
  //                      xAOD::SummaryType::numberOfTRTHoles); //this is
  //                      currently not available
}

float SoftMuonSelectorAlg::retrieveFloatJetMomentFromVector(
    const xAOD::Jet* jet, const std::string& name, unsigned int pos) {
  const SG::AuxElement::ConstAccessor<std::vector<float>> acc_name(name);
  if (!jet)
    return -999;
  if (!acc_name.isAvailable(*jet))
    return -999;
  std::vector<float> v = acc_name(*jet);
  if (pos >= v.size())
    return -999;
  return v[pos];
}

int SoftMuonSelectorAlg::retrieveIntJetMomentFromVector(const xAOD::Jet* jet,
                                                        const std::string& name,
                                                        unsigned int pos) {
  const SG::AuxElement::ConstAccessor<std::vector<int>> acc_name(name);
  if (!jet)
    return -999;
  if (!acc_name.isAvailable(*jet))
    return -999;
  std::vector<int> v = acc_name(*jet);
  if (pos >= v.size())
    return -999;
  return v[pos];
}
float SoftMuonSelectorAlg::retrieveFloatJetMoment(const xAOD::Jet* jet,
                                                  const std::string& name) {
  SG::AuxElement::ConstAccessor<float> acc_name(name);
  if (!jet)
    return -999;
  if (!acc_name.isAvailable(*jet))
    return -999;
  return acc_name(*jet);
}
int SoftMuonSelectorAlg::retrieveIntJetMoment(const xAOD::Jet* jet,
                                              const std::string& name) {
  SG::AuxElement::ConstAccessor<int> acc_name(name);
  if (!jet)
    return -999;
  if (!acc_name.isAvailable(*jet))
    return -999;
  return acc_name(*jet);
}

// ---------------------------------------------------

std::pair<float, float> SoftMuonSelectorAlg::IdMsPt(
    const xAOD::Muon& mu) const {

  float idPt = -1;
  float mePt = -1;

  static const SG::AuxElement::Accessor<float> mePt_acc("MuonSpectrometerPt");
  static const SG::AuxElement::Accessor<float> idPt_acc("InnerDetectorPt");

  const xAOD::TrackParticle* idtrack =
      mu.trackParticle(xAOD::Muon::InnerDetectorTrackParticle);
  const xAOD::TrackParticle* metrack =
      mu.trackParticle(xAOD::Muon::ExtrapolatedMuonSpectrometerTrackParticle);
  if (!idtrack || !metrack)
    idPt = mePt = -1.;
  if (!mePt_acc.isAvailable(mu) || !idPt_acc.isAvailable(mu)) {
    ATH_MSG_FATAL("The muon with pT "
                  << mu.pt() << " eta: " << mu.eta() << ", phi:" << mu.phi()
                  << " q:" << mu.charge() << ", author:" << mu.author()
                  << " is not decorated with calibrated momenta. Please fix");
    throw std::runtime_error(
        "MiddleSaver() - qOverP significance calculation failed");
  }

  idPt = idPt_acc(mu);
  mePt = mePt_acc(mu);
  return std::make_pair(idPt, mePt);
}

}  // namespace top
