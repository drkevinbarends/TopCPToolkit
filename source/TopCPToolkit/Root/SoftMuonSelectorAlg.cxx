#include "TopCPToolkit/SoftMuonSelectorAlg.h"

namespace top {

  SoftMuonSelectorAlg::SoftMuonSelectorAlg(const std::string &name,
                           ISvcLocator *pSvcLocator)
    : EL::AnaAlgorithm(name, pSvcLocator)
    , m_softmuonDRJetcut(0.4)
    {
    declareProperty("SoftMuonDRJet", m_softmuonDRJetcut = 0.4, "Soft Muon maximum dR wrt nearest selected jet. Can be set to 999. to keep all soft muons. Default 0.4");
    declareProperty("SaveSoftMuonAdditionalInfo", m_saveSoftMuonAdditionalInfo = false, "Whether to store additional information associated with the soft muon.");
    declareProperty("SaveSoftMuonNearestJetInfo", m_saveSoftMuonNearestJetInfo = false, "Whether to store additional information associated with the nearest jet associated to the soft muon.");
    declareProperty("SoftMuonDRJetUseRapidity", m_softMuonDRJetUseRapidity = false, "Whether to use the rapidity instead of the pseudo-rapidity when calculating the Delta R between the soft muon and the closest jet.");
    }

  StatusCode SoftMuonSelectorAlg::initialize() {
    ANA_MSG_INFO("Initialising the SoftMuonSelector algorithm for TopCPToolkit");

    ANA_CHECK(m_jetsHandle.initialize(m_systematicsList));
    ANA_CHECK(m_softmuonsHandle.initialize(m_systematicsList));
    ANA_CHECK(m_eventInfoHandle.initialize(m_systematicsList));

//    ANA_CHECK(m_selection.initialize(m_systematicsList, m_eventInfoHandle));

    ANA_CHECK(m_softmuonSelection.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_jetSelection.initialize(m_systematicsList, m_jetsHandle, SG::AllowEmpty));

    // Initialize output variables
    ANA_CHECK(m_SoftMuonJetDRminHandle.initialize(m_systematicsList, m_softmuonsHandle));
    ANA_CHECK(m_SoftMuonPassDRJetcut.initialize(m_systematicsList, m_softmuonsHandle));

    ANA_CHECK(m_pass_SoftMuonPassDRJetcut.initialize(m_systematicsList, m_eventInfoHandle));

    // ------------------------------------------------
    // Additional information of the soft muon
    ANA_CHECK(m_softmu_pass_tight_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_resolutionCategory_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));

    ANA_CHECK(m_softmu_pt_id_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_eta_id_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_phi_id_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_e_id_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_pt_me_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_eta_me_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_phi_me_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_e_me_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));

    ANA_CHECK(m_softmu_qOverP_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_qOverP_ID_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_qOverP_ME_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_qOverPerr_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_qOverPerr_ID_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_qOverPerr_ME_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_z0_sintheta_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_d0_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_d0sig_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_reducedChi2_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_reducedChi2_ID_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_reducedChi2_ME_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_reducedSegmentChi2_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_momentumBalanceSignificance_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_scatteringCurvatureSignificance_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_scatteringNeighbourSignificance_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_qOverPSignificance_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_rhoPrime_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_spectrometerFieldIntegral_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));

    ANA_CHECK(m_softmu_nprecisionLayers_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_nprecisionHoleLayers_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_nGoodPrecLayers_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_innerSmallHits_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_innerLargeHits_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_middleSmallHits_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_middleLargeHits_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_outerSmallHits_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_outerLargeHits_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_extendedSmallHits_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_extendedLargeHits_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_extendedSmallHoles_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_isSmallGoodSectors_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_cscUnspoiledEtaHits_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_numberOfPixelHits_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_numberOfPixelDeadSensors_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_numberOfPixelHoles_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_numberOfSCTHits_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_numberOfSCTDeadSensors_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_numberOfSCTHoles_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_numberOfTRTHits_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_numberOfTRTOutliers_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));

    ANA_CHECK(m_softmu_ptcone20_Nonprompt_All_MaxWeightTTVA_pt500_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt500_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_ptcone20_Nonprompt_All_MaxWeightTTVA_pt1000_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt1000_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_neflowisol20_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_EnergyLoss_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_EnergyLossSigma_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_EnergyLossType_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_ParamEnergyLoss_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_MeasEnergyLoss_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_ParamEnergyLossSigmaPlus_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_ParamEnergyLossSigmaMinus_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_MeasEnergyLossSigma_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));

    ANA_CHECK(m_IdMsPt_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_qOverPsignificance_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_rhoPrime_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));

    ANA_CHECK(m_softmu_nearestJet_Index_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_nearestJet_pt_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_nearestJet_eta_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_nearestJet_phi_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_nearestJet_e_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_nearestJet_dR_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_nearestJet_EMFrac_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_nearestJet_NumTrkPt500_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_nearestJet_SumPtTrkPt500_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_nearestJet_NumTrkPt1000_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_nearestJet_SumPtTrkPt1000_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_nearestJet_N90Constituents_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_nearestJet_TrackWidthPt500_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_nearestJet_TrackWidthPt1000_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_nearestJet_Width_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_nearestJet_Charge_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_nearestJet_ChargedPFOWidthPt500_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_softmu_nearestJet_ChargedPFOWidthPt1000_Handle.initialize(m_systematicsList, m_softmuonsHandle, SG::AllowEmpty));

    // ------------------------------------------------

    // Initialize the systematic list
    ANA_CHECK(m_systematicsList.initialize());

    return StatusCode::SUCCESS;
  }

  StatusCode SoftMuonSelectorAlg::execute() {

    for (const auto &sys : m_systematicsList.systematicsVector()) {

      const xAOD::EventInfo *evtInfo = nullptr;
      const xAOD::JetContainer *jets = nullptr;
      const xAOD::MuonContainer *softmuons = nullptr;

      ANA_CHECK(m_eventInfoHandle.retrieve(evtInfo, sys));

      ANA_CHECK(m_jetsHandle.retrieve(jets, sys));
      ANA_CHECK(m_softmuonsHandle.retrieve(softmuons, sys));

      // Fill the output variables with a default value:
      for (const xAOD::Muon *softmuon : *softmuons) {
        m_SoftMuonJetDRminHandle.set(*softmuon, -99, sys);
        m_SoftMuonPassDRJetcut.set(*softmuon, false, sys);
      }
      m_pass_SoftMuonPassDRJetcut.setBool(*evtInfo, 0, sys);

      // --------------------------------------------------------

      for (const xAOD::Muon *softmuon : *softmuons) {

        if(m_saveSoftMuonAdditionalInfo){
          m_softmu_pass_tight_Handle.set(*softmuon, -99, sys);
          m_softmu_resolutionCategory_Handle.set(*softmuon, -99, sys);

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
          m_softmu_z0_sintheta_Handle.set(*softmuon, -99, sys);
          m_softmu_d0_Handle.set(*softmuon, -99, sys);
          m_softmu_d0sig_Handle.set(*softmuon, -99, sys);
          m_softmu_reducedChi2_Handle.set(*softmuon, -99, sys);
          m_softmu_reducedChi2_ID_Handle.set(*softmuon, -99, sys);
          m_softmu_reducedChi2_ME_Handle.set(*softmuon, -99, sys);
          m_softmu_reducedSegmentChi2_Handle.set(*softmuon, -99, sys);
          m_softmu_momentumBalanceSignificance_Handle.set(*softmuon, -99, sys);
          m_softmu_scatteringCurvatureSignificance_Handle.set(*softmuon, -99, sys);
          m_softmu_scatteringNeighbourSignificance_Handle.set(*softmuon, -99, sys);
          m_softmu_qOverPSignificance_Handle.set(*softmuon, -99, sys);
          m_softmu_rhoPrime_Handle.set(*softmuon, -99, sys);
          m_softmu_spectrometerFieldIntegral_Handle.set(*softmuon, -99, sys);

          m_softmu_nprecisionLayers_Handle.set(*softmuon, -99, sys);
          m_softmu_nprecisionHoleLayers_Handle.set(*softmuon, -99, sys);
          m_softmu_nGoodPrecLayers_Handle.set(*softmuon, -99, sys);
          m_softmu_innerSmallHits_Handle.set(*softmuon, -99, sys);
          m_softmu_innerLargeHits_Handle.set(*softmuon, -99, sys);
          m_softmu_middleSmallHits_Handle.set(*softmuon, -99, sys);
          m_softmu_middleLargeHits_Handle.set(*softmuon, -99, sys);
          m_softmu_outerSmallHits_Handle.set(*softmuon, -99, sys);
          m_softmu_outerLargeHits_Handle.set(*softmuon, -99, sys);
          m_softmu_extendedSmallHits_Handle.set(*softmuon, -99, sys);
          m_softmu_extendedLargeHits_Handle.set(*softmuon, -99, sys);
          m_softmu_extendedSmallHoles_Handle.set(*softmuon, -99, sys);
          m_softmu_isSmallGoodSectors_Handle.set(*softmuon, -99, sys);
          m_softmu_cscUnspoiledEtaHits_Handle.set(*softmuon, -99, sys);
          m_softmu_numberOfPixelHits_Handle.set(*softmuon, -99, sys);
          m_softmu_numberOfPixelDeadSensors_Handle.set(*softmuon, -99, sys);
          m_softmu_numberOfPixelHoles_Handle.set(*softmuon, -99, sys);
          m_softmu_numberOfSCTHits_Handle.set(*softmuon, -99, sys);
          m_softmu_numberOfSCTDeadSensors_Handle.set(*softmuon, -99, sys);
          m_softmu_numberOfSCTHoles_Handle.set(*softmuon, -99, sys);
          m_softmu_numberOfTRTHits_Handle.set(*softmuon, -99, sys);
          m_softmu_numberOfTRTOutliers_Handle.set(*softmuon, -99, sys);

          m_softmu_ptcone20_Nonprompt_All_MaxWeightTTVA_pt500_Handle.set(*softmuon, -99, sys);
          m_softmu_ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt500_Handle.set(*softmuon, -99, sys);
          m_softmu_ptcone20_Nonprompt_All_MaxWeightTTVA_pt1000_Handle.set(*softmuon, -99, sys);
          m_softmu_ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt1000_Handle.set(*softmuon, -99, sys);
          m_softmu_neflowisol20_Handle.set(*softmuon, -99, sys);
          m_softmu_EnergyLoss_Handle.set(*softmuon, -99, sys);
          m_softmu_EnergyLossSigma_Handle.set(*softmuon, -99, sys);
          m_softmu_EnergyLossType_Handle.set(*softmuon, -99, sys);
          m_softmu_ParamEnergyLoss_Handle.set(*softmuon, -99, sys);
          m_softmu_MeasEnergyLoss_Handle.set(*softmuon, -99, sys);
          m_softmu_ParamEnergyLossSigmaPlus_Handle.set(*softmuon, -99, sys);
          m_softmu_ParamEnergyLossSigmaMinus_Handle.set(*softmuon, -99, sys);
          m_softmu_MeasEnergyLossSigma_Handle.set(*softmuon, -99, sys);

          m_IdMsPt_Handle.set(*softmuon, -99, sys);
          m_qOverPsignificance_Handle.set(*softmuon, -99, sys);
          m_rhoPrime_Handle.set(*softmuon, -99, sys);
        }

        if(m_saveSoftMuonNearestJetInfo){

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
          m_softmu_nearestJet_ChargedPFOWidthPt500_Handle.set(*softmuon, -99, sys);
          m_softmu_nearestJet_ChargedPFOWidthPt1000_Handle.set(*softmuon, -99, sys);
        }
      }

      // --------------------------------------------------------

      // first figure out if this event even passes the selection in which we are to run this KLFitter instance

//      if (!m_selection.getBool(*evtInfo, sys))
//      return StatusCode::SUCCESS;


      // perform selection of objects
      ConstDataVector<xAOD::MuonContainer> selected_softmuons(SG::VIEW_ELEMENTS);
      ConstDataVector<xAOD::JetContainer> selected_jets(SG::VIEW_ELEMENTS);

      for (const xAOD::Muon *softmu : *softmuons) {
        if (m_softmuonSelection.getBool(*softmu, sys))
          selected_softmuons.push_back(softmu);
      }

      for (const xAOD::Jet *jet : *jets) {
        if (m_jetSelection.getBool(*jet, sys))
          selected_jets.push_back(jet);
      }


      if ( selected_softmuons.size() > 0) {

        // Calculate SoftMuonJetDRmin for each soft muon
        bool PassSoftMuonDRJetCut = false;

        for (const xAOD::Muon* softmuon : selected_softmuons){

          auto [nearestJetIndex, dRMin] = SoftMuonSelectorAlg::calculateMinDRSoftMuonJet(softmuon, selected_jets);

	  if(dRMin<m_softmuonDRJetcut){

            m_softmu_nearestJet_Index_Handle.set(*softmuon, nearestJetIndex, sys);
	    m_SoftMuonJetDRminHandle.set(*softmuon, dRMin, sys);

            m_SoftMuonPassDRJetcut.set(*softmuon, true, sys);

            PassSoftMuonDRJetCut = true;
          }
        }

        if(PassSoftMuonDRJetCut){ m_pass_SoftMuonPassDRJetcut.setBool(*evtInfo, 1, sys);}
        else{ m_pass_SoftMuonPassDRJetcut.setBool(*evtInfo, 0, sys);}


        //If the event has any soft muons, let's fill additional variables
        if(PassSoftMuonDRJetCut){
          // If the option is set, save additional variables related to the soft muon

          for (const xAOD::Muon* softmuon : selected_softmuons){

            //Does this muon pass the DRmin cut?
            if(softmuon->auxdataConst<bool>(m_SoftMuonPassDRJetcut.getName(sys))){

              if(m_saveSoftMuonAdditionalInfo){
                SoftMuonSelectorAlg::SaveAdditionalSoftMuonVariables(softmuon, evtInfo, sys);
              }

              if(m_saveSoftMuonNearestJetInfo){
                SoftMuonSelectorAlg::SaveAdditionalInformationFromNearestJet(softmuon, selected_jets, sys);
              }
            }
          }
        }
      }
    }

    return StatusCode::SUCCESS;
  }


  std::pair<int, float>  SoftMuonSelectorAlg::calculateMinDRSoftMuonJet(const xAOD::Muon* softmuon, const ConstDataVector<xAOD::JetContainer>& selected_jets){

    int nearestJetIndex = -1;
    float dRMin = 100.0;

    // Loop over jets, calculate dR and record smallest value
    int ijet=0;
    for (const xAOD::Jet *jet : selected_jets) {

      float dr = softmuon->p4().DeltaR(jet->p4(), m_softMuonDRJetUseRapidity);

      if(dr < dRMin){
        dRMin = dr;
        nearestJetIndex=ijet;
      }
      ijet++;
    }

//    ANA_MSG_INFO("  DR min " << dRMin);

    return std::make_pair(nearestJetIndex, dRMin);
  }



// Safe addigional information closes jet

//Save additional soft muon related variables.
  void SoftMuonSelectorAlg::SaveAdditionalSoftMuonVariables(const xAOD::Muon* softmuon, const xAOD::EventInfo *evtInfo, const CP::SystematicSet& sys){

    auto [idPt, mePt] = SoftMuonSelectorAlg::IdMsPt(*softmuon);

    m_softmu_pt_id_Handle.set(*softmuon, idPt, sys);
    m_softmu_pt_me_Handle.set(*softmuon, mePt, sys);

/*
    m_outtree_softmu_pass_tight.push_back(m_mseltool_tight->accept(*sm) ? 1 : 0);
    m_outtree_softmu_resolutionCategory.push_back(m_mseltool_tight->getResolutionCategory(*sm));

*/

    const xAOD::TrackParticle*       idtrack = softmuon->trackParticle( xAOD::Muon::InnerDetectorTrackParticle );
    const xAOD::TrackParticle*       metrack = softmuon->trackParticle( xAOD::Muon::ExtrapolatedMuonSpectrometerTrackParticle );
    const xAOD::TrackParticle*       cbtrack = softmuon->trackParticle( xAOD::Muon::CombinedTrackParticle );

    if (idtrack) {
      m_softmu_eta_id_Handle.set(*softmuon, idtrack->eta(), sys);
      m_softmu_phi_id_Handle.set(*softmuon, idtrack->phi(), sys);
      m_softmu_e_id_Handle.set(*softmuon, idtrack->e(), sys);
      m_softmu_qOverP_ID_Handle.set(*softmuon, idtrack->charge()*sin(idtrack->theta()) / idPt, sys);
    }
    else {
      m_softmu_eta_id_Handle.set(*softmuon, -999., sys);
      m_softmu_phi_id_Handle.set(*softmuon, -999., sys);
      m_softmu_e_id_Handle.set(*softmuon, -999., sys);
      m_softmu_qOverP_ID_Handle.set(*softmuon, -999., sys);
    }
    if (metrack) {
      m_softmu_eta_me_Handle.set(*softmuon, metrack->eta(), sys);
      m_softmu_phi_me_Handle.set(*softmuon, metrack->phi(), sys);
      m_softmu_e_me_Handle.set(*softmuon, metrack->e(), sys);
      m_softmu_qOverP_ME_Handle.set(*softmuon, metrack->charge()*sin(metrack->theta()) / mePt, sys);
    }
    else {
      m_softmu_eta_me_Handle.set(*softmuon, -999., sys);
      m_softmu_phi_me_Handle.set(*softmuon, -999., sys);
      m_softmu_e_me_Handle.set(*softmuon, -999., sys);
      m_softmu_qOverP_ME_Handle.set(*softmuon, -999., sys);
    }
    if (cbtrack) {
      m_softmu_qOverP_Handle.set(*softmuon, cbtrack->charge()*sin(cbtrack->theta()) / softmuon->pt(), sys);
    }
    else {
      m_softmu_qOverP_Handle.set(*softmuon, -999., sys);
    }


    float qOverPerr=-999;
    float qOverPerr_ID=-999;
    float qOverPerr_ME=-999;
    std::vector<float> CovMatrixVec;
    if(cbtrack && cbtrack->definingParametersCovMatrixVec().size()==15) qOverPerr=sqrt( cbtrack->definingParametersCovMatrix()(4,4) );
    if(idtrack && idtrack->definingParametersCovMatrixVec().size()==15) qOverPerr_ID=sqrt( idtrack->definingParametersCovMatrix()(4,4) );
    if(metrack && metrack->definingParametersCovMatrixVec().size()==15) qOverPerr_ME=sqrt( metrack->definingParametersCovMatrix()(4,4) );
    m_softmu_qOverPerr_Handle.set(*softmuon, qOverPerr, sys);
    m_softmu_qOverPerr_ID_Handle.set(*softmuon, qOverPerr_ID, sys);
    m_softmu_qOverPerr_ME_Handle.set(*softmuon, qOverPerr_ME, sys);

    // Impact patameter variables. (they may be saved by default in some versions, if so, comment these lines)

//----------------
/*
SG::ReadHandleKey<xAOD::VertexContainer> m_primaryVerticesKey {this, "primaryVertices", "PrimaryVertices", "the name of the PrimaryVertex container to retrieve"};
 ANA_CHECK (m_primaryVerticesKey.initialize());
   SG::ReadHandle<xAOD::VertexContainer> vertices(m_primaryVerticesKey);
  const xAOD::Vertex *primaryVertex {nullptr};


     for (const xAOD::Vertex *vertex : *vertices)
     {
       if (vertex->vertexType() == xAOD::VxType::PriVtx)
      {
        // The default "PrimaryVertex" container is ordered in
         // sum-pt, and the tracking group recommends to pick the one
        // with the maximum sum-pt, so this will do the right thing.
         // If the user needs a different primary vertex, they need to
        // provide a reordered primary vertex container and point
         // this algorithm to it.  Currently there is no central
         // algorithm to do that, so users will have to write their
        // own (15 Aug 18).
       if (primaryVertex == nullptr)
        {
          primaryVertex = vertex;
           break;
       }
       }
     }


   const double vertex_z = primaryVertex ? primaryVertex->z() : 0;





ATH_CHECK( evtStore()->retrieve( evtInfo, "EventInfo" ) );
  const xAOD::Vertex* pv = this->GetPrimVtx();



evtInfo->GetPrimVtx();


const xAOD::Vertex        *priVtx

  if(priVtx) {
    const double deltaZ0  = track.z0() + track.vz() - priVtx->z();
    Z0Sin                 = deltaZ0*std::sin(track.theta());
  }

*/
//----------------


//    float vtx_z = evtInfo->auxdata<float>("AnalysisTop_PRIVTX_z_position");
//    float delta_z0 = softmuon->primaryTrackParticle()->z0() + softmuon->primaryTrackParticle()->vz() - vtx_z;
//    float delta_z0_sintheta= delta_z0 * std::sin(softmuon->primaryTrackParticle()->theta());

//    m_softmu_z0_sintheta_Handle.set(*softmuon, delta_z0_sintheta, sys);

    float d0=softmuon->primaryTrackParticle()->d0();
    float d0sig=xAOD::TrackingHelpers::d0significance( softmuon->primaryTrackParticle(), evtInfo->beamPosSigmaX(), evtInfo->beamPosSigmaY(), evtInfo->beamPosSigmaXY() );
    m_softmu_d0_Handle.set(*softmuon, d0, sys);
    m_softmu_d0sig_Handle.set(*softmuon, d0sig, sys);


    m_softmu_reducedChi2_Handle.set(*softmuon, ( cbtrack && cbtrack->isAvailable<float>("numberDoF") && cbtrack->numberDoF()>0 ? cbtrack->chiSquared() / cbtrack->numberDoF() : -999), sys);
    m_softmu_reducedChi2_ID_Handle.set(*softmuon, ( idtrack && idtrack->isAvailable<float>("numberDoF") && idtrack->numberDoF()>0 ? idtrack->chiSquared() / idtrack->numberDoF() : -999), sys);
    m_softmu_reducedChi2_ME_Handle.set(*softmuon, ( metrack && metrack->isAvailable<float>("numberDoF") && metrack->numberDoF()>0 ? metrack->chiSquared() / metrack->numberDoF() : -999), sys);
    float reducedSegmentChi2=-999, reducedSegmentChi2Val=-999; if ( softmuon->parameter(reducedSegmentChi2Val, xAOD::Muon::segmentChi2OverDoF) ) reducedSegmentChi2 = reducedSegmentChi2Val;
    m_softmu_reducedSegmentChi2_Handle.set(*softmuon, reducedSegmentChi2, sys);

   float momentumBalanceSignificance = -999; softmuon->parameter(momentumBalanceSignificance ,xAOD::Muon::momentumBalanceSignificance);
    m_softmu_momentumBalanceSignificance_Handle.set(*softmuon, momentumBalanceSignificance, sys);
    float scatteringCurvatureSignificance = -999; softmuon->parameter(scatteringCurvatureSignificance ,xAOD::Muon::scatteringCurvatureSignificance);
    m_softmu_scatteringCurvatureSignificance_Handle.set(*softmuon, scatteringCurvatureSignificance, sys);
    float scatteringNeighbourSignificance = -999; softmuon->parameter(scatteringNeighbourSignificance ,xAOD::Muon::scatteringNeighbourSignificance);
    m_softmu_scatteringNeighbourSignificance_Handle.set(*softmuon, scatteringNeighbourSignificance, sys);

    /*
    float qOverPSignificance=-999;
    if( idtrack && metrack ) {                                                                                                float DeltaQoverPError = sqrt( pow(qOverPerr_ID,2) + pow(qOverPerr_ME,2) );
      if( DeltaQoverPError>0 ) {                                                                                                  qOverPSignificance = fabs( m_outtree_softmu_qOverP_ID.back() - m_outtree_softmu_qOverP_ME.back() ) / DeltaQoverPError;
        }
    }
    * */
    float qOverPSignificance=qOverPsignificance(*softmuon);
    m_softmu_qOverPSignificance_Handle.set(*softmuon, qOverPSignificance, sys);

    float rhoP=rhoPrime(*softmuon);
    m_softmu_rhoPrime_Handle.set(*softmuon, rhoP, sys);

    float value = -999;
    m_softmu_spectrometerFieldIntegral_Handle.set(*softmuon, (softmuon->parameter(value,xAOD::Muon::spectrometerFieldIntegral) ? value : -999), sys);

      hitSummary summary{};
      SoftMuonSelectorAlg::fillSummary(*softmuon, summary);
      m_softmu_nprecisionLayers_Handle.set(*softmuon, summary.nprecisionLayers, sys);
      m_softmu_nprecisionHoleLayers_Handle.set(*softmuon, summary.nprecisionHoleLayers, sys);
      m_softmu_nGoodPrecLayers_Handle.set(*softmuon, summary.nGoodPrecLayers, sys);
      m_softmu_innerSmallHits_Handle.set(*softmuon, summary.innerSmallHits, sys);
      m_softmu_innerLargeHits_Handle.set(*softmuon, summary.innerLargeHits, sys);
      m_softmu_middleSmallHits_Handle.set(*softmuon, summary.middleSmallHits, sys);
      m_softmu_middleLargeHits_Handle.set(*softmuon, summary.middleLargeHits, sys);
      m_softmu_outerSmallHits_Handle.set(*softmuon, summary.outerSmallHits, sys);
      m_softmu_outerLargeHits_Handle.set(*softmuon, summary.outerLargeHits, sys);
      m_softmu_extendedSmallHits_Handle.set(*softmuon, summary.extendedSmallHits, sys);
      m_softmu_extendedLargeHits_Handle.set(*softmuon, summary.extendedLargeHits, sys);
      m_softmu_extendedSmallHoles_Handle.set(*softmuon, summary.extendedSmallHoles, sys);
      m_softmu_isSmallGoodSectors_Handle.set(*softmuon, summary.isSmallGoodSectors, sys);
//      m_softmu_cscUnspoiledEtaHits_Handle.set(*softmuon, summary.cscUnspoiledEtaHits, sys);
      m_softmu_numberOfPixelHits_Handle.set(*softmuon, summary.numberOfPixelHits, sys);
      m_softmu_numberOfPixelDeadSensors_Handle.set(*softmuon, summary.numberOfPixelDeadSensors, sys);
      m_softmu_numberOfPixelHoles_Handle.set(*softmuon, summary.numberOfPixelHoles, sys);
      m_softmu_numberOfSCTHits_Handle.set(*softmuon, summary.numberOfSCTHits, sys);
      m_softmu_numberOfSCTDeadSensors_Handle.set(*softmuon, summary.numberOfSCTDeadSensors, sys);
      m_softmu_numberOfSCTHoles_Handle.set(*softmuon, summary.numberOfSCTHoles, sys);
      m_softmu_numberOfTRTHits_Handle.set(*softmuon, summary.numberOfTRTHits, sys);
      m_softmu_numberOfTRTOutliers_Handle.set(*softmuon, summary.numberOfTRTOutliers, sys);

      static const SG::AuxElement::ConstAccessor< float > ptcone20_Nonprompt_All_MaxWeightTTVA_pt500_acc("ptcone20_Nonprompt_All_MaxWeightTTVA_pt500");
      float mu_ptcone20_Nonprompt_All_MaxWeightTTVA_pt500 = ptcone20_Nonprompt_All_MaxWeightTTVA_pt500_acc(*softmuon);
      static const SG::AuxElement::ConstAccessor< float > ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt500_acc("ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt500");
      float mu_ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt500 = ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt500_acc(*softmuon);
      static const SG::AuxElement::ConstAccessor< float > ptcone20_Nonprompt_All_MaxWeightTTVA_pt1000_acc("ptcone20_Nonprompt_All_MaxWeightTTVA_pt1000");
      float mu_ptcone20_Nonprompt_All_MaxWeightTTVA_pt1000 = ptcone20_Nonprompt_All_MaxWeightTTVA_pt1000_acc(*softmuon);
      static const SG::AuxElement::ConstAccessor< float > ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt1000_acc("ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt1000");
      float mu_ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt1000 = ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt1000_acc(*softmuon);
      static const SG::AuxElement::ConstAccessor< float > neflowisol20_acc("neflowisol20");
      float mu_neflowisol20 = neflowisol20_acc(*softmuon);

      m_softmu_ptcone20_Nonprompt_All_MaxWeightTTVA_pt500_Handle.set(*softmuon, mu_ptcone20_Nonprompt_All_MaxWeightTTVA_pt500, sys);
      m_softmu_ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt500_Handle.set(*softmuon, mu_ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt500, sys);
      m_softmu_ptcone20_Nonprompt_All_MaxWeightTTVA_pt1000_Handle.set(*softmuon, mu_ptcone20_Nonprompt_All_MaxWeightTTVA_pt1000, sys);
      m_softmu_ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt1000_Handle.set(*softmuon, mu_ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt1000, sys);
      m_softmu_neflowisol20_Handle.set(*softmuon, mu_neflowisol20, sys);

      m_softmu_EnergyLoss_Handle.set(*softmuon, ( softmuon->isAvailable<float>("EnergyLoss") ? softmuon->auxdata<float>("EnergyLoss") : -999), sys);
      m_softmu_EnergyLossSigma_Handle.set(*softmuon, ( softmuon->isAvailable<float>("EnergyLossSigma") ? softmuon->auxdata<float>("EnergyLossSigma") : -999), sys);
      m_softmu_EnergyLossType_Handle.set(*softmuon, ( softmuon->isAvailable<unsigned char>("energyLossType") ? softmuon->auxdata<unsigned char>("energyLossType") : -999), sys);
      m_softmu_ParamEnergyLoss_Handle.set(*softmuon, ( softmuon->isAvailable<float>("ParamEnergyLoss") ? softmuon->auxdata<float>("ParamEnergyLoss") : -999), sys);
      m_softmu_MeasEnergyLoss_Handle.set(*softmuon, ( softmuon->isAvailable<float>("MeasEnergyLoss") ? softmuon->auxdata<float>("MeasEnergyLoss") : -999), sys);
      m_softmu_ParamEnergyLossSigmaPlus_Handle.set(*softmuon, ( softmuon->isAvailable<float>("ParamEnergyLossSigmaPlus") ? softmuon->auxdata<float>("ParamEnergyLossSigmaPlus") : -999), sys);
      m_softmu_ParamEnergyLossSigmaPlus_Handle.set(*softmuon, ( softmuon->isAvailable<float>("ParamEnergyLossSigmaMinus") ? softmuon->auxdata<float>("ParamEnergyLossSigmaMinus") : -999), sys);
      m_softmu_MeasEnergyLossSigma_Handle.set(*softmuon, ( softmuon->isAvailable<float>("MeasEnergyLossSigma") ? softmuon->auxdata<float>("MeasEnergyLossSigma") : -999), sys);

    return;
  }


  void SoftMuonSelectorAlg::SaveAdditionalInformationFromNearestJet(const xAOD::Muon* softmuon, const ConstDataVector<xAOD::JetContainer>& selected_jets, const CP::SystematicSet& sys){

    // Get the index of the nearest jet
    int nearestJetIndex = -1;

    const std::string& decorName = m_softmu_nearestJet_Index_Handle.getName(sys);

    if (softmuon->isAvailable<int>(decorName)) {
      nearestJetIndex = softmuon->auxdataConst<int>(decorName);
    }

    int ijet =0;
    for (const xAOD::Jet *jet : selected_jets) {

      if (ijet == nearestJetIndex) {

        m_softmu_nearestJet_pt_Handle.set(*softmuon, (nearestJetIndex>=0 ? jet->pt() : -999), sys);
        m_softmu_nearestJet_eta_Handle.set(*softmuon, (nearestJetIndex>=0 ? jet->eta() : -999), sys);
        m_softmu_nearestJet_phi_Handle.set(*softmuon, (nearestJetIndex>=0 ? jet->phi() : -999), sys);
        m_softmu_nearestJet_e_Handle.set(*softmuon, (nearestJetIndex>=0 ? jet->e() : -999), sys);
        m_softmu_nearestJet_EMFrac_Handle.set(*softmuon, (nearestJetIndex>=0 ? retrieveFloatJetMoment(jet, "EMFrac") : -999), sys);
        m_softmu_nearestJet_NumTrkPt500_Handle.set(*softmuon, (nearestJetIndex>=0 ? retrieveIntJetMomentFromVector(jet, "NumTrkPt500",0) : -999), sys);
        m_softmu_nearestJet_SumPtTrkPt500_Handle.set(*softmuon, (nearestJetIndex>=0 ? retrieveFloatJetMomentFromVector(jet, "SumPtTrkPt500",0) : -999), sys);
        m_softmu_nearestJet_NumTrkPt1000_Handle.set(*softmuon, (nearestJetIndex>=0 ? retrieveIntJetMomentFromVector(jet, "NumTrkPt1000",0) : -999), sys);
        m_softmu_nearestJet_SumPtTrkPt1000_Handle.set(*softmuon, (nearestJetIndex>=0 ? retrieveFloatJetMomentFromVector(jet, "SumPtTrkPt1000",0) : -999), sys);
        m_softmu_nearestJet_N90Constituents_Handle.set(*softmuon, (nearestJetIndex>=0 ? retrieveFloatJetMoment(jet, "N90Constituents") : -999), sys);
        m_softmu_nearestJet_TrackWidthPt500_Handle.set(*softmuon, (nearestJetIndex>=0 ? retrieveFloatJetMomentFromVector(jet, "TrackWidthPt500",0) : -999), sys);
        m_softmu_nearestJet_TrackWidthPt1000_Handle.set(*softmuon, (nearestJetIndex>=0 ? retrieveFloatJetMomentFromVector(jet, "TrackWidthPt1000",0) : -999), sys);
        m_softmu_nearestJet_Width_Handle.set(*softmuon, (nearestJetIndex>=0 ? retrieveFloatJetMoment(jet, "Width") : -999), sys);
        m_softmu_nearestJet_Charge_Handle.set(*softmuon, (nearestJetIndex>=0 ? retrieveFloatJetMoment(jet, "Charge") : -999), sys);
        m_softmu_nearestJet_ChargedPFOWidthPt1000_Handle.set(*softmuon, (nearestJetIndex>=0 ? retrieveFloatJetMomentFromVector(jet, "ChargedPFOWidthPt1000",0) : -999), sys);
        m_softmu_nearestJet_ChargedPFOWidthPt500_Handle.set(*softmuon, (nearestJetIndex>=0 ? retrieveFloatJetMomentFromVector(jet, "ChargedPFOWidthPt500",0) : -999), sys);

        break;  // Exit the loop once the nearest jet is found
      }
      ijet++;

    }


    return;
  }

   // -----------------------------------------------------------------------------------------------------
  //Auxiliary functions to hel in the calculation of some of the additional variables
  void SoftMuonSelectorAlg::fillSummary(const xAOD::Muon& muon, hitSummary& summary) const {
        retrieveSummaryValue(muon, summary.nprecisionLayers, xAOD::SummaryType::numberOfPrecisionLayers);
        retrieveSummaryValue(muon, summary.nprecisionHoleLayers, xAOD::SummaryType::numberOfPrecisionHoleLayers);
        retrieveSummaryValue(muon, summary.nGoodPrecLayers, xAOD::numberOfGoodPrecisionLayers);
        retrieveSummaryValue(muon, summary.innerSmallHits, xAOD::MuonSummaryType::innerSmallHits);
        retrieveSummaryValue(muon, summary.innerLargeHits, xAOD::MuonSummaryType::innerLargeHits);
        retrieveSummaryValue(muon, summary.middleSmallHits, xAOD::MuonSummaryType::middleSmallHits);
        retrieveSummaryValue(muon, summary.middleLargeHits, xAOD::MuonSummaryType::middleLargeHits);
        retrieveSummaryValue(muon, summary.outerSmallHits, xAOD::MuonSummaryType::outerSmallHits);
        retrieveSummaryValue(muon, summary.outerLargeHits, xAOD::MuonSummaryType::outerLargeHits);
        retrieveSummaryValue(muon, summary.extendedSmallHits, xAOD::MuonSummaryType::extendedSmallHits);
        retrieveSummaryValue(muon, summary.extendedLargeHits, xAOD::MuonSummaryType::extendedLargeHits);
        retrieveSummaryValue(muon, summary.extendedSmallHoles, xAOD::MuonSummaryType::extendedSmallHoles);
        retrieveSummaryValue(muon, summary.isSmallGoodSectors, xAOD::MuonSummaryType::isSmallGoodSectors);
        /*if(!m_isRun3) */
//        retrieveSummaryValue(muon, summary.cscUnspoiledEtaHits, xAOD::MuonSummaryType::cscUnspoiledEtaHits);
        if(muon.author() == xAOD::Muon::MuidSA && std::abs(muon.eta()) > 2.5) return;
        if(muon.muonType() == xAOD::Muon::SiliconAssociatedForwardMuon) return;

        const xAOD::TrackParticle* track = muon.trackParticle( xAOD::Muon::InnerDetectorTrackParticle );
        if(!track) track=muon.primaryTrackParticle();
        if(!track) return;

        retrieveSummaryValue(*track, summary.numberOfPixelHits, xAOD::SummaryType::numberOfPixelHits);
        retrieveSummaryValue(*track, summary.numberOfPixelDeadSensors, xAOD::SummaryType::numberOfPixelDeadSensors);
        retrieveSummaryValue(*track, summary.numberOfPixelHoles, xAOD::SummaryType::numberOfPixelHoles);
        retrieveSummaryValue(*track, summary.numberOfSCTHits, xAOD::SummaryType::numberOfSCTHits);
        retrieveSummaryValue(*track, summary.numberOfSCTDeadSensors, xAOD::SummaryType::numberOfSCTDeadSensors);
        retrieveSummaryValue(*track, summary.numberOfSCTHoles, xAOD::SummaryType::numberOfSCTHoles);
        retrieveSummaryValue(*track, summary.numberOfTRTHits, xAOD::SummaryType::numberOfTRTHits);
        retrieveSummaryValue(*track, summary.numberOfTRTOutliers, xAOD::SummaryType::numberOfTRTOutliers);
  }

  float SoftMuonSelectorAlg::retrieveFloatJetMomentFromVector(const xAOD::Jet* jet, const std::string& name, unsigned int pos)
  {
    if(!jet) return -999;
    if(!jet->isAvailable<std::vector<float> >(name)) return -999;
    std::vector<float> v =  jet->auxdata<std::vector<float> >(name);
    if(pos>=v.size()) return -999;
    return v[pos];
  }

  int SoftMuonSelectorAlg::retrieveIntJetMomentFromVector(const xAOD::Jet* jet, const std::string& name, unsigned int pos)
  {
    if(!jet) return -999;
    if(!jet->isAvailable<std::vector<int> >(name)) return -999;
    std::vector<int> v =  jet->auxdata<std::vector<int> >(name);
    if(pos>=v.size()) return -999;
    return v[pos];
  }
  float SoftMuonSelectorAlg::retrieveFloatJetMoment(const xAOD::Jet* jet, const std::string& name)
  {
    if(!jet) return -999;
    if(!jet->isAvailable<float>(name)) return -999;
    return jet->auxdata<float>(name);
  }
  int SoftMuonSelectorAlg::retrieveIntJetMoment(const xAOD::Jet* jet, const std::string& name)
  {
    if(!jet) return -999;
    if(!jet->isAvailable<int>(name)) return -999;
    return jet->auxdata<int>(name);
  }

  // ---------------------------------------------------

  std::pair<float, float> SoftMuonSelectorAlg::IdMsPt(const xAOD::Muon& mu) const {

    float idPt = -1;
    float mePt = -1;

    static const SG::AuxElement::Accessor<float> mePt_acc("MuonSpectrometerPt");
    static const SG::AuxElement::Accessor<float> idPt_acc("InnerDetectorPt");

    const xAOD::TrackParticle* idtrack = mu.trackParticle(xAOD::Muon::InnerDetectorTrackParticle);
    const xAOD::TrackParticle* metrack = mu.trackParticle(xAOD::Muon::ExtrapolatedMuonSpectrometerTrackParticle);
    if (!idtrack || !metrack) idPt = mePt = -1.;
    if (!mePt_acc.isAvailable(mu) || !idPt_acc.isAvailable(mu)) {
      ATH_MSG_FATAL("The muon with pT " << mu.pt() << " eta: " << mu.eta() << ", phi:" << mu.phi()
                                        << " q:" << mu.charge() << ", author:" << mu.author()
                                        << " is not decorated with calibrated momenta. Please fix");
      throw std::runtime_error("MiddleSaver() - qOverP significance calculation failed");
    }

    idPt = idPt_acc(mu);
    mePt = mePt_acc(mu);
    return std::make_pair(idPt, mePt);
  }

  float SoftMuonSelectorAlg::qOverPsignificance(const xAOD::Muon& muon) const {
    const xAOD::TrackParticle* idtrack = muon.trackParticle(xAOD::Muon::InnerDetectorTrackParticle);
    const xAOD::TrackParticle* metrack = muon.trackParticle(xAOD::Muon::ExtrapolatedMuonSpectrometerTrackParticle);
    if (!idtrack || !metrack) {
      ATH_MSG_VERBOSE("No ID / MS track. Return dummy large value of 1 mio");
      return 1.e6;
    }
    auto [idPt, mePt] = SoftMuonSelectorAlg::IdMsPt(muon);

    const float meP = mePt / std::sin(metrack->theta());
    const float idP = idPt / std::sin(idtrack->theta());

    float qOverPsigma = std::sqrt(idtrack->definingParametersCovMatrix()(4, 4) + metrack->definingParametersCovMatrix()(4, 4));
    return std::abs((metrack->charge() / meP) - (idtrack->charge() / idP)) / qOverPsigma;
  }

  float SoftMuonSelectorAlg::rhoPrime(const xAOD::Muon& muon) const {
    auto [idPt, mePt] = SoftMuonSelectorAlg::IdMsPt(muon);
    return std::abs(idPt - mePt) / muon.pt();
  }

} //namespace top
