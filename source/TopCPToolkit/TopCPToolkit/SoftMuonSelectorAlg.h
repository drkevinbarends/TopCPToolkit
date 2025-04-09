#ifndef SOFTMUONSELECTORALG_H
#define SOFTMUONSELECTORALG_H

#include <AnaAlgorithm/AnaAlgorithm.h>
#include <SystematicsHandles/SysListHandle.h>
#include <SystematicsHandles/SysReadHandle.h>
#include <SystematicsHandles/SysWriteDecorHandle.h>
#include <SelectionHelpers/SysReadSelectionHandle.h>
#include <SelectionHelpers/SysWriteSelectionHandle.h>

#include <xAODMuon/MuonContainer.h>
#include <xAODJet/JetContainer.h>
#include <xAODEventInfo/EventInfo.h>

#include "AthContainers/ConstDataVector.h"
#include "xAODTracking/TrackParticlexAODHelpers.h" //To get the impact parameters
#include <TVector2.h>

namespace top {

  class SoftMuonSelectorAlg final : public EL::AnaAlgorithm {

    public:
      SoftMuonSelectorAlg(const std::string &name, ISvcLocator *pSvcLocator);
      virtual StatusCode initialize() override;
      virtual StatusCode execute() override;

    private:
      CP::SysListHandle m_systematicsList {this};

      CP::SysReadSelectionHandle m_preselection {
        this, "eventPreselection", "", "event preselection to check before running this algorithm"
      };

      CP::SysReadHandle<xAOD::JetContainer> m_jetsHandle {
        this, "jets", "", "the jet container to use"
      };
      CP::SysReadSelectionHandle m_jetSelection {
        this, "jetSelection", "", "the selection on the input jets"
      };

      CP::SysReadHandle<xAOD::EventInfo> m_eventInfoHandle {
        this, "eventInfo", "EventInfo", "the input EventInfo container"
      };

      //Soft Muons
      CP::SysReadHandle<xAOD::MuonContainer> m_softmuonsHandle {
        this, "softmuons", "", "the input soft muon container"
      };
      CP::SysReadSelectionHandle m_softmuonSelection {
        this, "softmuonSelection", "", "the selection on the input soft muons"
      };

      // Input information from the yaml file
      float m_softmuonDRJetcut;
      bool  m_saveSoftMuonAdditionalInfo;
      bool  m_saveSoftMuonNearestJetInfo;
      bool  m_softMuonDRJetUseRapidity;

      CP::SysReadSelectionHandle m_selection {
        this, "selection", "", "Name of the selection on which the SoftMuonSelectors instance is allowed to run"
      };

    private:
      virtual std::pair<int, float> calculateMinDRSoftMuonJet(const xAOD::Muon* softmuon, const ConstDataVector<xAOD::JetContainer>& selected_jets);

      CP::SysWriteDecorHandle<float> m_SoftMuonJetDRminHandle {
        this, "SoftMuonJetDRmin", "SoftMuonJetDRmin_%SYS%", "decoration name for the dR of the closest jet to the soft muon"
      };

      CP::SysWriteDecorHandle<bool> m_SoftMuonPassDRJetcut {
        this, "SoftMuonPassDRJetcut", "SoftMuonPassDRJetcut_%SYS%", "decoration name for tagging soft muons that pass the dR condition with its closest jet in the event"
      };

      CP::SysWriteSelectionHandle m_pass_SoftMuonPassDRJetcut {
	 this, "pass_SoftMuonPassDRJetcut", "pass_SoftMuonPassDRJetcut_%SYS%,as_char", "decoration name for tagging the events that have at least one soft muon passing the dR condition with its closest jet in the event"
      };

    //Functions to add additional variables with additional information about the soft muons, and/or the nearest jet to the soft muon
    private:
      virtual void SaveAdditionalSoftMuonVariables(const xAOD::Muon* softmuon, const xAOD::EventInfo *evtInfo /*to calculate impact parameters*/, const CP::SystematicSet& sys);
      virtual void SaveAdditionalInformationFromNearestJet(const xAOD::Muon* softmuon, const ConstDataVector<xAOD::JetContainer>& selected_jets, const CP::SystematicSet& sys);

      // Complementary functionalities to calculate and save additional variables related with the soft muon properties
      // or with the properties of the closest jet to the soft muon

      // ------------------------------------------------------------------------------
      // Some function to calculate some of these variables
      virtual std::pair<float, float> IdMsPt(const xAOD::Muon& muon) const;
      virtual float qOverPsignificance(const xAOD::Muon& muon) const;
      virtual float rhoPrime(const xAOD::Muon& muon) const;

      // Get nearest jet information
      float retrieveFloatJetMomentFromVector(const xAOD::Jet* jet, const std::string& name, unsigned int pos);
      int retrieveIntJetMomentFromVector(const xAOD::Jet* jet, const std::string& name, unsigned int pos);
      float retrieveFloatJetMoment(const xAOD::Jet* jet, const std::string& name);
      int retrieveIntJetMoment(const xAOD::Jet* jet, const std::string& name);

      // Get all the hits of the muons in the detectors
      struct hitSummary {
        uint8_t nprecisionLayers{0};
        uint8_t nprecisionHoleLayers{0};
        uint8_t nGoodPrecLayers{0};
        uint8_t innerSmallHits{0};
        uint8_t innerLargeHits{0};
        uint8_t middleSmallHits{0};
        uint8_t middleLargeHits{0};
        uint8_t outerSmallHits{0};
        uint8_t outerLargeHits{0};
        uint8_t extendedSmallHits{0};
        uint8_t extendedLargeHits{0};
        uint8_t extendedSmallHoles{0};
        uint8_t isSmallGoodSectors{0};
        uint8_t cscUnspoiledEtaHits{0};
        uint8_t numberOfPixelHits{0};
        uint8_t numberOfPixelDeadSensors{0};
        uint8_t numberOfPixelHoles{0};
        uint8_t numberOfSCTHits{0};
        uint8_t numberOfSCTDeadSensors{0};
        uint8_t numberOfSCTHoles{0};
        uint8_t numberOfTRTHits{0};
        uint8_t numberOfTRTOutliers{0};
      };
      template <class P, class T, class S> inline void retrieveSummaryValue(const P& muon, T& value, const S type) const {
            if (!muon.summaryValue(value, type)) {
                ATH_MSG_FATAL(__FILE__ << ":" << __LINE__ << " Failed to retrieve summary value " << type);
                throw std::runtime_error("MuonSelectorTool summary retrieval failed");
            }
        }
      inline void fillSummary(const xAOD::Muon& muon, hitSummary& summary) const;



      // ------------------------------------------------------------------------------
      //list of variables.

      //---------- Now a series of variables about the soft muon properties
      CP::SysWriteDecorHandle<int> m_softmu_pass_tight_Handle {
        this, "softmu_pass_tight", "softmu_pass_tight_%SYS%", "decoration name for the soft muon pass tight"
      };
      CP::SysWriteDecorHandle<int> m_softmu_resolutionCategory_Handle {
        this, "softmu_resolutionCategory", "softmu_resolutionCategory_%SYS%", "decoration name for the soft muon resolution category"
      };

      //---------- Four momenta from only the inner detector and the muon spectrometer. Necessary for calculation of other important variables.
      CP::SysWriteDecorHandle<float> m_softmu_pt_id_Handle {
        this, "softmu_pt_id", "softmu_pt_id_%SYS%", "decoration name for the soft muon pt in the Inner Detector"
      };
      CP::SysWriteDecorHandle<float> m_softmu_eta_id_Handle {
        this, "softmu_eta_id", "softmu_eta_id_%SYS%", "decoration name for the soft muon eta in the Inner Detector"
      };
      CP::SysWriteDecorHandle<float> m_softmu_phi_id_Handle {
        this, "softmu_phi_id", "softmu_phi_id_%SYS%", "decoration name for the soft muon phi in the Inner Detector"
      };
      CP::SysWriteDecorHandle<float> m_softmu_e_id_Handle {
        this, "softmu_e_id", "softmu_e_id_%SYS%", "decoration name for the soft muon energy in the Inner Detector"
      };

      CP::SysWriteDecorHandle<float> m_softmu_pt_me_Handle {
        this, "softmu_pt_me", "softmu_pt_me_%SYS%", "decoration name for the soft muon pt in the Muon Spectrometer"
      };
      CP::SysWriteDecorHandle<float> m_softmu_eta_me_Handle {
        this, "softmu_eta_me", "softmu_eta_me_%SYS%", "decoration name for the soft muon eta in the Muon Spectrometer"
      };
      CP::SysWriteDecorHandle<float> m_softmu_phi_me_Handle {
        this, "softmu_phi_me", "softmu_phi_me_%SYS%", "decoration name for the soft muon phi in the Muon Spectrometer"
      };
      CP::SysWriteDecorHandle<float> m_softmu_e_me_Handle {
        this, "softmu_e_me", "softmu_e_me_%SYS%", "decoration name for the soft muon energy in the Muon Spectrometer"
      };

      //---------- Second series of variables about the soft muon properties
      CP::SysWriteDecorHandle<float> m_softmu_qOverP_Handle {
        this, "softmu_qOverP", "softmu_qOverP_%SYS%", "decoration name for the soft muon qOverP"
      };
      CP::SysWriteDecorHandle<float> m_softmu_qOverP_ID_Handle {
        this, "softmu_qOverP_ID", "softmu_qOverP_ID_%SYS%", "decoration name for the soft muon qOverP for the Inner Detector"
      };
      CP::SysWriteDecorHandle<float> m_softmu_qOverP_ME_Handle {
        this, "softmu_qOverP_ME", "softmu_qOverP_ME_%SYS%", "decoration name for the soft muon qOverP for the Muon Spectrometer"
      };
      CP::SysWriteDecorHandle<float> m_softmu_qOverPerr_Handle {
        this, "softmu_qOverPerr", "softmu_qOverPerr_%SYS%", "decoration name for the soft muon qOverP error"
      };
      CP::SysWriteDecorHandle<float> m_softmu_qOverPerr_ID_Handle {
        this, "softmu_qOverPerr_ID", "softmu_qOverPerr_ID_%SYS%", "decoration name for the soft muon qOverP error for the Inner Detector"
      };
      CP::SysWriteDecorHandle<float> m_softmu_qOverPerr_ME_Handle {
        this, "softmu_qOverPerr_ME", "softmu_qOverPerr_ME_%SYS%", "decoration name for the soft muon qOverP error for the Muon Spectrometer"
      };
      CP::SysWriteDecorHandle<float> m_softmu_z0_sintheta_Handle {
        this, "softmu_z0_sintheta", "softmu_z0_sintheta_%SYS%", "decoration name for the soft muon z0 sin(theta)"
      };
      CP::SysWriteDecorHandle<float> m_softmu_d0_Handle {
        this, "softmu_d0", "softmu_d0_%SYS%", "decoration name for the soft muon d0"
      };
      CP::SysWriteDecorHandle<float> m_softmu_d0sig_Handle {
        this, "softmu_d0sig", "softmu_d0sig_%SYS%", "decoration name for the soft muon d0 significance"
      };
      CP::SysWriteDecorHandle<float> m_softmu_reducedChi2_Handle {
        this, "softmu_reducedChi2", "softmu_reducedChi2_%SYS%", "decoration name for the soft muon reduced Chi2"
      };
      CP::SysWriteDecorHandle<float> m_softmu_reducedChi2_ID_Handle {
        this, "softmu_reducedChi2_ID", "softmu_reducedChi2_ID_%SYS%", "decoration name for the soft muon reduced Chi2 for the Inner Detector"
      };
      CP::SysWriteDecorHandle<float> m_softmu_reducedChi2_ME_Handle {
        this, "softmu_reducedChi2_ME", "softmu_reducedChi2_ME_%SYS%", "decoration name for the soft muon reduced Chi2 for the Muon Spectrometer"
      };
      CP::SysWriteDecorHandle<float> m_softmu_reducedSegmentChi2_Handle {
        this, "softmu_reducedSegmentChi2", "softmu_reducedSegmentChi2_%SYS%", "decoration name for the soft muon reduced segment Chi2"
      };
      CP::SysWriteDecorHandle<float> m_softmu_momentumBalanceSignificance_Handle {
        this, "softmu_momentumBalanceSignificance", "softmu_momentumBalanceSignificance_%SYS%", "decoration name for the soft muon momentum balance significance"
      };
      CP::SysWriteDecorHandle<float> m_softmu_scatteringCurvatureSignificance_Handle {
        this, "softmu_scatteringCurvatureSignificance", "softmu_scatteringCurvatureSignificance_%SYS%", "decoration name for the soft muon scattering curvature significance"
      };
      CP::SysWriteDecorHandle<float> m_softmu_scatteringNeighbourSignificance_Handle {
        this, "softmu_scatteringNeighbourSignificance", "softmu_scatteringNeighbourSignificance_%SYS%", "decoration name for the soft muon scattering neighbour significance"
      };
      CP::SysWriteDecorHandle<float> m_softmu_qOverPSignificance_Handle {
        this, "softmu_qOverPSignificance", "softmu_qOverPSignificance_%SYS%", "decoration name for the soft muon qOverP significance"
      };
      CP::SysWriteDecorHandle<float> m_softmu_rhoPrime_Handle {
        this, "softmu_rhoPrime", "softmu_rhoPrime_%SYS%", "decoration name for the soft muon rho prime"
      };
      CP::SysWriteDecorHandle<float> m_softmu_spectrometerFieldIntegral_Handle {
        this, "softmu_spectrometerFieldIntegral", "softmu_spectrometerFieldIntegral_%SYS%", "decoration name for the soft muon spectrometer field integral"
      };

      //---------- Now a series of variables about the hits and holes of the muon tracks
      CP::SysWriteDecorHandle<int> m_softmu_nprecisionLayers_Handle {
        this, "softmu_nprecisionLayers", "softmu_nprecisionLayers_%SYS%", "decoration name for the soft muon number of precision layers"
      };
      CP::SysWriteDecorHandle<int> m_softmu_nprecisionHoleLayers_Handle {
        this, "softmu_nprecisionHoleLayers", "softmu_nprecisionHoleLayers_%SYS%", "decoration name for the soft muon number of precision hole layers"
      };
      CP::SysWriteDecorHandle<int> m_softmu_nGoodPrecLayers_Handle {
        this, "softmu_nGoodPrecLayers", "softmu_nGoodPrecLayers_%SYS%", "decoration name for the soft muon number of good precision layers"
      };
      CP::SysWriteDecorHandle<int> m_softmu_innerSmallHits_Handle {
        this, "softmu_innerSmallHits", "softmu_innerSmallHits_%SYS%", "decoration name for the soft muon inner small hits"
      };
      CP::SysWriteDecorHandle<int> m_softmu_innerLargeHits_Handle {
        this, "softmu_innerLargeHits", "softmu_innerLargeHits_%SYS%", "decoration name for the soft muon inner large hits"
      };
      CP::SysWriteDecorHandle<int> m_softmu_middleSmallHits_Handle {
        this, "softmu_middleSmallHits", "softmu_middleSmallHits_%SYS%", "decoration name for the soft muon middle small hits"
      };
      CP::SysWriteDecorHandle<int> m_softmu_middleLargeHits_Handle {
        this, "softmu_middleLargeHits", "softmu_middleLargeHits_%SYS%", "decoration name for the soft muon middle large hits"
      };
      CP::SysWriteDecorHandle<int> m_softmu_outerSmallHits_Handle {
        this, "softmu_outerSmallHits", "softmu_outerSmallHits_%SYS%", "decoration name for the soft muon outer small hits"
      };
      CP::SysWriteDecorHandle<int> m_softmu_outerLargeHits_Handle {
        this, "softmu_outerLargeHits", "softmu_outerLargeHits_%SYS%", "decoration name for the soft muon outer large hits"
      };
      CP::SysWriteDecorHandle<int> m_softmu_extendedSmallHits_Handle {
        this, "softmu_extendedSmallHits", "softmu_extendedSmallHits_%SYS%", "decoration name for the soft muon extended small hits"
      };
      CP::SysWriteDecorHandle<int> m_softmu_extendedLargeHits_Handle {
        this, "softmu_extendedLargeHits", "softmu_extendedLargeHits_%SYS%", "decoration name for the soft muon extended large hits"
      };
      CP::SysWriteDecorHandle<int> m_softmu_extendedSmallHoles_Handle {
        this, "softmu_extendedSmallHoles", "softmu_extendedSmallHoles_%SYS%", "decoration name for the soft muon extended small holes"
      };
      CP::SysWriteDecorHandle<int> m_softmu_isSmallGoodSectors_Handle {
        this, "softmu_isSmallGoodSectors", "softmu_isSmallGoodSectors_%SYS%", "decoration name for the soft muon is small good sectors"
      };
      CP::SysWriteDecorHandle<int> m_softmu_cscUnspoiledEtaHits_Handle {
        this, "softmu_cscUnspoiledEtaHits", "softmu_cscUnspoiledEtaHits_%SYS%", "decoration name for the soft muon CSC unspoiled eta hits"
      };
      CP::SysWriteDecorHandle<int> m_softmu_numberOfPixelHits_Handle {
        this, "softmu_numberOfPixelHits", "softmu_numberOfPixelHits_%SYS%", "decoration name for the soft muon number of pixel hits"
      };
      CP::SysWriteDecorHandle<int> m_softmu_numberOfPixelDeadSensors_Handle {
        this, "softmu_numberOfPixelDeadSensors", "softmu_numberOfPixelDeadSensors_%SYS%", "decoration name for the soft muon number of pixel dead sensors"
      };
      CP::SysWriteDecorHandle<int> m_softmu_numberOfPixelHoles_Handle {
        this, "softmu_numberOfPixelHoles", "softmu_numberOfPixelHoles_%SYS%", "decoration name for the soft muon number of pixel holes"
      };
      CP::SysWriteDecorHandle<int> m_softmu_numberOfSCTHits_Handle {
        this, "softmu_numberOfSCTHits", "softmu_numberOfSCTHits_%SYS%", "decoration name for the soft muon number of SCT hits"
      };
      CP::SysWriteDecorHandle<int> m_softmu_numberOfSCTDeadSensors_Handle {
        this, "softmu_numberOfSCTDeadSensors", "softmu_numberOfSCTDeadSensors_%SYS%", "decoration name for the soft muon number of SCT dead sensors"
      };
      CP::SysWriteDecorHandle<int> m_softmu_numberOfSCTHoles_Handle {
        this, "softmu_numberOfSCTHoles", "softmu_numberOfSCTHoles_%SYS%", "decoration name for the soft muon number of SCT holes"
      };
      CP::SysWriteDecorHandle<int> m_softmu_numberOfTRTHits_Handle {
        this, "softmu_numberOfTRTHits", "softmu_numberOfTRTHits_%SYS%", "decoration name for the soft muon number of TRT hits"
      };
      CP::SysWriteDecorHandle<int> m_softmu_numberOfTRTOutliers_Handle {
        this, "softmu_numberOfTRTOutliers", "softmu_numberOfTRTOutliers_%SYS%", "decoration name for the soft muon number of TRT outliers"
      };

      //---------- Now a series of variables about the soft muon properties
      CP::SysWriteDecorHandle<float> m_softmu_ptcone20_Nonprompt_All_MaxWeightTTVA_pt500_Handle {
        this, "softmu_ptcone20_Nonprompt_All_MaxWeightTTVA_pt500", "softmu_ptcone20_Nonprompt_All_MaxWeightTTVA_pt500_%SYS%", "decoration name for the soft muon ptcone20 Nonprompt All MaxWeightTTVA pt500"
      };
      CP::SysWriteDecorHandle<float> m_softmu_ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt500_Handle {
        this, "softmu_ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt500", "softmu_ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt500_%SYS%", "decoration name for the soft muon ptvarcone30 Nonprompt All MaxWeightTTVA pt500"
      };
      CP::SysWriteDecorHandle<float> m_softmu_ptcone20_Nonprompt_All_MaxWeightTTVA_pt1000_Handle {
        this, "softmu_ptcone20_Nonprompt_All_MaxWeightTTVA_pt1000", "softmu_ptcone20_Nonprompt_All_MaxWeightTTVA_pt1000_%SYS%", "decoration name for the soft muon ptcone20 Nonprompt All MaxWeightTTVA pt1000"
      };
      CP::SysWriteDecorHandle<float> m_softmu_ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt1000_Handle {
        this, "softmu_ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt1000", "softmu_ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt1000_%SYS%", "decoration name for the soft muon ptvarcone30 Nonprompt All MaxWeightTTVA pt1000"
      };
      CP::SysWriteDecorHandle<float> m_softmu_neflowisol20_Handle {
        this, "softmu_neflowisol20", "softmu_neflowisol20_%SYS%", "decoration name for the soft muon neflowisol20"
      };
      CP::SysWriteDecorHandle<float> m_softmu_EnergyLoss_Handle {
        this, "softmu_EnergyLoss", "softmu_EnergyLoss_%SYS%", "decoration name for the soft muon energy loss"
      };
      CP::SysWriteDecorHandle<float> m_softmu_EnergyLossSigma_Handle {
        this, "softmu_EnergyLossSigma", "softmu_EnergyLossSigma_%SYS%", "decoration name for the soft muon energy loss sigma"
      };
      CP::SysWriteDecorHandle<unsigned char> m_softmu_EnergyLossType_Handle {
        this, "softmu_EnergyLossType", "softmu_EnergyLossType_%SYS%", "decoration name for the soft muon energy loss type"
      };
      CP::SysWriteDecorHandle<float> m_softmu_ParamEnergyLoss_Handle {
        this, "softmu_ParamEnergyLoss", "softmu_ParamEnergyLoss_%SYS%", "decoration name for the soft muon parameterized energy loss"
      };
      CP::SysWriteDecorHandle<float> m_softmu_MeasEnergyLoss_Handle {
        this, "softmu_MeasEnergyLoss", "softmu_MeasEnergyLoss_%SYS%", "decoration name for the soft muon measured energy loss"
      };
      CP::SysWriteDecorHandle<float> m_softmu_ParamEnergyLossSigmaPlus_Handle {
        this, "softmu_ParamEnergyLossSigmaPlus", "softmu_ParamEnergyLossSigmaPlus_%SYS%", "decoration name for the soft muon parameterized energy loss sigma plus"
      };
      CP::SysWriteDecorHandle<float> m_softmu_ParamEnergyLossSigmaMinus_Handle {
        this, "softmu_ParamEnergyLossSigmaMinus", "softmu_ParamEnergyLossSigmaMinus_%SYS%", "decoration name for the soft muon parameterized energy loss sigma minus"
      };
      CP::SysWriteDecorHandle<float> m_softmu_MeasEnergyLossSigma_Handle {
        this, "softmu_MeasEnergyLossSigma", "softmu_MeasEnergyLossSigma_%SYS%", "decoration name for the soft muon measured energy loss sigma"
      };

      //---------- Now a series of variables about the soft muon properties
      CP::SysWriteDecorHandle<float> m_IdMsPt_Handle {
        this, "IdMsPt", "IdMsPt_%SYS%", "decoration name for the ID-MS Pt"
      };
      CP::SysWriteDecorHandle<float> m_qOverPsignificance_Handle {
        this, "qOverPsignificance", "qOverPsignificance_%SYS%", "decoration name for the qOverP significance"
      };
      CP::SysWriteDecorHandle<float> m_rhoPrime_Handle {
        this, "rhoPrime", "rhoPrime_%SYS%", "decoration name for the rho prime"
      };

      //---------- Now a series of variables about the jet that is closer to the soft muon
      CP::SysWriteDecorHandle<int> m_softmu_nearestJet_Index_Handle {
        this, "softmu_nearestJet_Index", "softmu_nearestJet_Index_%SYS%", "decoration name for the soft muon nearest jet index"
      };
      CP::SysWriteDecorHandle<float> m_softmu_nearestJet_pt_Handle {
        this, "softmu_nearestJet_pt", "softmu_nearestJet_pt_%SYS%", "decoration name for the soft muon nearest jet pt"
      };
      CP::SysWriteDecorHandle<float> m_softmu_nearestJet_eta_Handle {
        this, "softmu_nearestJet_eta", "softmu_nearestJet_eta_%SYS%", "decoration name for the soft muon nearest jet eta"
      };
      CP::SysWriteDecorHandle<float> m_softmu_nearestJet_phi_Handle {
        this, "softmu_nearestJet_phi", "softmu_nearestJet_phi_%SYS%", "decoration name for the soft muon nearest jet phi"
      };
      CP::SysWriteDecorHandle<float> m_softmu_nearestJet_e_Handle {
        this, "softmu_nearestJet_e", "softmu_nearestJet_e_%SYS%", "decoration name for the soft muon nearest jet e"
      };
      CP::SysWriteDecorHandle<float> m_softmu_nearestJet_dR_Handle {
        this, "softmu_nearestJet_dR", "softmu_nearestJet_dR_%SYS%", "decoration name for the soft muon nearest jet delta R"
      };
      CP::SysWriteDecorHandle<float> m_softmu_nearestJet_EMFrac_Handle {
        this, "softmu_nearestJet_EMFrac", "softmu_nearestJet_EMFrac_%SYS%", "decoration name for the soft muon nearest jet EM fraction"
      };
      CP::SysWriteDecorHandle<int> m_softmu_nearestJet_NumTrkPt500_Handle {
        this, "softmu_nearestJet_NumTrkPt500", "softmu_nearestJet_NumTrkPt500_%SYS%", "decoration name for the soft muon nearest jet number of tracks with pt > 500"
      };
      CP::SysWriteDecorHandle<float> m_softmu_nearestJet_SumPtTrkPt500_Handle {
        this, "softmu_nearestJet_SumPtTrkPt500", "softmu_nearestJet_SumPtTrkPt500_%SYS%", "decoration name for the soft muon nearest jet sum pt of tracks with pt > 500"
      };
      CP::SysWriteDecorHandle<int> m_softmu_nearestJet_NumTrkPt1000_Handle {
        this, "softmu_nearestJet_NumTrkPt1000", "softmu_nearestJet_NumTrkPt1000_%SYS%", "decoration name for the soft muon nearest jet number of tracks with pt > 1000"
      };
      CP::SysWriteDecorHandle<float> m_softmu_nearestJet_SumPtTrkPt1000_Handle {
        this, "softmu_nearestJet_SumPtTrkPt1000", "softmu_nearestJet_SumPtTrkPt1000_%SYS%", "decoration name for the soft muon nearest jet sum pt of tracks with pt > 1000"
      };
      CP::SysWriteDecorHandle<int> m_softmu_nearestJet_N90Constituents_Handle {
        this, "softmu_nearestJet_N90Constituents", "softmu_nearestJet_N90Constituents_%SYS%", "decoration name for the soft muon nearest jet N90 constituents"
      };
      CP::SysWriteDecorHandle<float> m_softmu_nearestJet_TrackWidthPt500_Handle {
        this, "softmu_nearestJet_TrackWidthPt500", "softmu_nearestJet_TrackWidthPt500_%SYS%", "decoration name for the soft muon nearest jet track width with pt > 500"
      };
      CP::SysWriteDecorHandle<float> m_softmu_nearestJet_TrackWidthPt1000_Handle {
        this, "softmu_nearestJet_TrackWidthPt1000", "softmu_nearestJet_TrackWidthPt1000_%SYS%", "decoration name for the soft muon nearest jet track width with pt > 1000"
      };
      CP::SysWriteDecorHandle<float> m_softmu_nearestJet_Width_Handle {
        this, "softmu_nearestJet_Width", "softmu_nearestJet_Width_%SYS%", "decoration name for the soft muon nearest jet width"
      };
      CP::SysWriteDecorHandle<float> m_softmu_nearestJet_Charge_Handle {
        this, "softmu_nearestJet_Charge", "softmu_nearestJet_Charge_%SYS%", "decoration name for the soft muon nearest jet charge"
      };
      CP::SysWriteDecorHandle<float> m_softmu_nearestJet_ChargedPFOWidthPt500_Handle {
        this, "softmu_nearestJet_ChargedPFOWidthPt500", "softmu_nearestJet_ChargedPFOWidthPt500_%SYS%", "decoration name for the soft muon nearest jet charged PFO width with pt > 500"
      };
      CP::SysWriteDecorHandle<float> m_softmu_nearestJet_ChargedPFOWidthPt1000_Handle {
        this, "softmu_nearestJet_ChargedPFOWidthPt1000", "softmu_nearestJet_ChargedPFOWidthPt1000_%SYS%", "decoration name for the soft muon nearest jet charged PFO width with pt > 1000"
      };

  };

} // namsepace top

#endif
