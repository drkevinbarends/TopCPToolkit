#ifndef SOFTMUONSELECTORALG_H
#define SOFTMUONSELECTORALG_H

#include <AnaAlgorithm/AnaAlgorithm.h>
#include <SelectionHelpers/SysReadSelectionHandle.h>
#include <SelectionHelpers/SysWriteSelectionHandle.h>
#include <SystematicsHandles/SysListHandle.h>
#include <SystematicsHandles/SysReadHandle.h>
#include <SystematicsHandles/SysWriteDecorHandle.h>
#include <TVector2.h>
#include <xAODEventInfo/EventInfo.h>
#include <xAODJet/JetContainer.h>
#include <xAODMuon/MuonContainer.h>

#include "AthContainers/ConstDataVector.h"
#include "MuonSelectorTools/MuonSelectionTool.h"
#include "TruthUtils/HepMCHelpers.h"
#include "xAODTracking/TrackParticlexAODHelpers.h"  //To get the impact parameters
#include "xAODTruth/TruthParticleContainer.h"
#include "xAODTruth/xAODTruthHelpers.h"

namespace top {

class SoftMuonSelectorAlg final : public EL::AnaAlgorithm {

 public:
  SoftMuonSelectorAlg(const std::string& name, ISvcLocator* pSvcLocator);
  virtual StatusCode initialize() override;
  virtual StatusCode execute() override;

 private:
  ToolHandle<CP::MuonSelectionTool> m_selTool_Tight{
      this, "SoftMuonSelTool_Tight", ""};

  CP::SysListHandle m_systematicsList{this};

  CP::SysReadSelectionHandle m_preselection{
      this, "eventPreselection", "",
      "event preselection to check before running this algorithm"};

  CP::SysReadHandle<xAOD::JetContainer> m_jetsHandle{
      this, "jets", "", "the jet container to use"};
  CP::SysReadSelectionHandle m_jetSelection{this, "jetSelection", "",
                                            "the selection on the input jets"};

  CP::SysReadHandle<xAOD::EventInfo> m_eventInfoHandle{
      this, "eventInfo", "EventInfo", "the input EventInfo container"};

  // Soft Muons
  CP::SysReadHandle<xAOD::MuonContainer> m_softmuonsHandle{
      this, "softmuons", "", "the input soft muon container"};
  CP::SysReadSelectionHandle m_softmuonSelection{
      this, "softmuonSelection", "", "the selection on the input soft muons"};

  // Input information from the yaml file
  float m_softmuonDRJetcut;
  bool m_saveSoftMuonAdditionalInfo;
  bool m_saveSoftMuonNearestJetInfo;
  bool m_softMuonDRJetUseRapidity;
  bool m_saveSoftMuonTruthInfo;
  bool m_isRun3;

  CP::SysReadSelectionHandle m_selection{
      this, "selection", "",
      "Name of the selection on which the SoftMuonSelectors instance is "
      "allowed to run"};

 private:
  virtual std::pair<int, float> calculateMinDRSoftMuonJet(
      const xAOD::Muon* softmuon,
      const ConstDataVector<xAOD::JetContainer>& selected_jets);

  CP::SysWriteDecorHandle<float> m_SoftMuonJetDRminHandle{
      this, "SoftMuonJetDRmin", "SoftMuonJetDRmin_%SYS%",
      "decoration name for the dR of the closest jet to the soft muon"};

  CP::SysWriteSelectionHandle m_SoftMuonPassDRJetcut{
      this, "SoftMuonPassDRJetcut", "SoftMuonPassDRJetcut_%SYS%",
      "decoration name for tagging soft muons that pass the dR condition with "
      "its closest jet in the event"};

  CP::SysWriteSelectionHandle m_pass_SoftMuonPassDRJetcut{
      this, "pass_SoftMuonPassDRJetcut",
      "pass_SoftMuonPassDRJetcut_%SYS%,as_char",
      "decoration name for tagging the events that have at least one soft muon "
      "passing the dR condition with its closest jet in the event"};

  // Functions to add additional variables with additional information about the
  // soft muons, and/or the nearest jet to the soft muon
 private:
  virtual void SaveAdditionalSoftMuonVariables(
      const xAOD::Muon* softmuon,
      /*const xAOD::EventInfo *evtInfo to calculate impact parameters*/
      const CP::SystematicSet& sys);
  virtual void SaveAdditionalInformationFromNearestJet(
      const xAOD::Muon* softmuon,
      const ConstDataVector<xAOD::JetContainer>& selected_jets,
      const CP::SystematicSet& sys);
  virtual StatusCode SaveAdditionalSoftMuonTruthVariables(
      const xAOD::Muon* softmuon, const CP::SystematicSet& sys);

  // Complementary functionalities to calculate and save additional variables
  // related with the soft muon properties or with the properties of the closest
  // jet to the soft muon

  // ------------------------------------------------------------------------------
  // Some function to calculate some of these variables
  virtual std::pair<float, float> IdMsPt(const xAOD::Muon& muon) const;

  // Get nearest jet information
  float retrieveFloatJetMomentFromVector(const xAOD::Jet* jet,
                                         const std::string& name,
                                         unsigned int pos);
  int retrieveIntJetMomentFromVector(const xAOD::Jet* jet,
                                     const std::string& name, unsigned int pos);
  float retrieveFloatJetMoment(const xAOD::Jet* jet, const std::string& name);
  int retrieveIntJetMoment(const xAOD::Jet* jet, const std::string& name);

  // Get all the hits of the muons in the detectors
  struct hitSummary {
    uint8_t nprecisionLayers{0};
    uint8_t nprecisionHoleLayers{0};
    uint8_t nGoodPrecLayers{0};
    uint8_t innerSmallHits{0};
    uint8_t innerLargeHits{0};
    uint8_t innerSmallHoles{0};
    uint8_t innerLargeHoles{0};
    uint8_t middleSmallHits{0};
    uint8_t middleLargeHits{0};
    uint8_t middleSmallHoles{0};
    uint8_t middleLargeHoles{0};
    uint8_t outerSmallHits{0};
    uint8_t outerLargeHits{0};
    uint8_t outerSmallHoles{0};
    uint8_t outerLargeHoles{0};
    uint8_t extendedSmallHits{0};
    uint8_t extendedLargeHits{0};
    uint8_t extendedSmallHoles{0};
    uint8_t extendedLargeHoles{0};
    uint8_t innerClosePrecisionHits{0};
    uint8_t middleClosePrecisionHits{0};
    uint8_t outerClosePrecisionHits{0};
    uint8_t extendedClosePrecisionHits{0};
    uint8_t innerOutBoundsPrecisionHits{0};
    uint8_t middleOutBoundsPrecisionHits{0};
    uint8_t outerOutBoundsPrecisionHits{0};
    uint8_t extendedOutBoundsPrecisionHits{0};
    uint8_t combinedTrackOutBoundsPrecisionHits{0};
    uint8_t isSmallGoodSectors{0};
    uint8_t isEndcapGoodLayers{0};
    uint8_t cscUnspoiledEtaHits{0};
    uint8_t etaLayer1STGCHits{0};
    uint8_t etaLayer2STGCHits{0};
    uint8_t phiLayer1STGCHits{0};
    uint8_t phiLayer2STGCHits{0};
    uint8_t MMHits{0};
    uint8_t numberOfPixelHits{0};
    uint8_t numberOfPixelDeadSensors{0};
    uint8_t numberOfPixelHoles{0};
    uint8_t numberOfSCTHits{0};
    uint8_t numberOfSCTDeadSensors{0};
    uint8_t numberOfSCTHoles{0};
    uint8_t numberOfTRTHits{0};
    uint8_t numberOfTRTOutliers{0};
    uint8_t numberOfTRTHoles{0};
  };
  template <class P, class T, class S>
  inline void retrieveSummaryValue(const P& muon, T& value,
                                   const S type) const {
    if (!muon.summaryValue(value, type)) {
      ATH_MSG_FATAL(__FILE__ << ":" << __LINE__
                             << " Failed to retrieve summary value " << type);
      throw std::runtime_error("MuonSelectorTool summary retrieval failed");
    }
  }
  inline void fillSummary(const xAOD::Muon& muon, hitSummary& summary) const;

  // ------------------------------------------------------------------------------
  // list of variables.

  //---------- Now a series of variables about the soft muon properties
  CP::SysWriteDecorHandle<int> m_softmu_author_Handle{
      this, "author", "author_%SYS%",
      "decoration name for the soft muon author"};
  CP::SysWriteDecorHandle<uint16_t> m_softmu_allAuthors_Handle{
      this, "allAuthors", "allAuthors_%SYS%",
      "decoration name for the soft muon allAuthors"};
  CP::SysWriteDecorHandle<int> m_softmu_resolutionCategory_Handle{
      this, "resolutionCategory", "resolutionCategory_%SYS%",
      "decoration name for the soft muon resolution category"};
  CP::SysWriteDecorHandle<bool> m_softmu_passedIDCuts_Handle{
      this, "passedIDCuts", "passedIDCuts_%SYS%",
      "decoration name for checking if muon passes ID cuts"};
  CP::SysWriteDecorHandle<bool> m_softmu_passedHighPtCuts_Handle{
      this, "passedHighPtCuts", "passedHighPtCuts_%SYS%",
      "decoration name for checking if muon passes HighPt cuts"};
  CP::SysWriteDecorHandle<bool> m_softmu_passedLowPtCuts_Handle{
      this, "passedLowPtCuts", "passedLowPtCuts_%SYS%",
      "decoration name for checking if muon passes LowPt cuts"};
  CP::SysWriteDecorHandle<bool> m_softmu_isBadMuon_Handle{
      this, "isBadMuon", "isBadMuon_%SYS%",
      "decoration name for checking if muon isBad"};
  //---------- Four momenta from only the inner detector and the muon
  // spectrometer. Necessary for calculation of other important variables.
  CP::SysWriteDecorHandle<float> m_softmu_pt_id_Handle{
      this, "pt_id", "pt_id_%SYS%",
      "decoration name for the soft muon pt in the Inner Detector"};
  CP::SysWriteDecorHandle<float> m_softmu_eta_id_Handle{
      this, "eta_id", "eta_id_%SYS%",
      "decoration name for the soft muon eta in the Inner Detector"};
  CP::SysWriteDecorHandle<float> m_softmu_phi_id_Handle{
      this, "phi_id", "phi_id_%SYS%",
      "decoration name for the soft muon phi in the Inner Detector"};
  CP::SysWriteDecorHandle<float> m_softmu_e_id_Handle{
      this, "e_id", "e_id_%SYS%",
      "decoration name for the soft muon energy in the Inner Detector"};

  CP::SysWriteDecorHandle<float> m_softmu_pt_me_Handle{
      this, "pt_me", "pt_me_%SYS%",
      "decoration name for the soft muon pt in the Muon Spectrometer"};
  CP::SysWriteDecorHandle<float> m_softmu_eta_me_Handle{
      this, "eta_me", "eta_me_%SYS%",
      "decoration name for the soft muon eta in the Muon Spectrometer"};
  CP::SysWriteDecorHandle<float> m_softmu_phi_me_Handle{
      this, "phi_me", "phi_me_%SYS%",
      "decoration name for the soft muon phi in the Muon Spectrometer"};
  CP::SysWriteDecorHandle<float> m_softmu_e_me_Handle{
      this, "e_me", "e_me_%SYS%",
      "decoration name for the soft muon energy in the Muon Spectrometer"};

  //---------- Second series of variables about the soft muon properties
  CP::SysWriteDecorHandle<float> m_softmu_qOverP_Handle{
      this, "qOverP", "qOverP_%SYS%",
      "decoration name for the soft muon qOverP"};
  CP::SysWriteDecorHandle<float> m_softmu_qOverP_ID_Handle{
      this, "qOverP_ID", "qOverP_ID_%SYS%",
      "decoration name for the soft muon qOverP for the Inner Detector"};
  CP::SysWriteDecorHandle<float> m_softmu_qOverP_ME_Handle{
      this, "qOverP_ME", "qOverP_ME_%SYS%",
      "decoration name for the soft muon qOverP for the Muon Spectrometer"};
  CP::SysWriteDecorHandle<float> m_softmu_qOverPerr_Handle{
      this, "qOverPerr", "qOverPerr_%SYS%",
      "decoration name for the soft muon qOverP error"};
  CP::SysWriteDecorHandle<float> m_softmu_qOverPerr_ID_Handle{
      this, "qOverPerr_ID", "qOverPerr_ID_%SYS%",
      "decoration name for the soft muon qOverP error for the Inner Detector"};
  CP::SysWriteDecorHandle<float> m_softmu_qOverPerr_ME_Handle{
      this, "qOverPerr_ME", "qOverPerr_ME_%SYS%",
      "decoration name for the soft muon qOverP error for the Muon "
      "Spectrometer"};
  CP::SysWriteDecorHandle<float> m_softmu_d0_Handle{
      this, "d0", "d0_%SYS%", "decoration name for the soft muon d0"};
  CP::SysWriteDecorHandle<float> m_softmu_reducedChi2_Handle{
      this, "reducedChi2", "reducedChi2_%SYS%",
      "decoration name for the soft muon reduced Chi2"};
  CP::SysWriteDecorHandle<float> m_softmu_reducedChi2_ID_Handle{
      this, "reducedChi2_ID", "reducedChi2_ID_%SYS%",
      "decoration name for the soft muon reduced Chi2 for the Inner Detector"};
  CP::SysWriteDecorHandle<float> m_softmu_reducedChi2_ME_Handle{
      this, "reducedChi2_ME", "reducedChi2_ME_%SYS%",
      "decoration name for the soft muon reduced Chi2 for the Muon "
      "Spectrometer"};
  CP::SysWriteDecorHandle<float> m_softmu_reducedSegmentChi2_Handle{
      this, "reducedSegmentChi2", "reducedSegmentChi2_%SYS%",
      "decoration name for the soft muon reduced segment Chi2"};
  CP::SysWriteDecorHandle<float> m_softmu_momentumBalanceSignificance_Handle{
      this, "momentumBalanceSignificance", "momentumBalanceSignificance_%SYS%",
      "decoration name for the soft muon momentum balance significance"};
  CP::SysWriteDecorHandle<float>
      m_softmu_scatteringCurvatureSignificance_Handle{
          this, "scatteringCurvatureSignificance",
          "scatteringCurvatureSignificance_%SYS%",
          "decoration name for the soft muon scattering curvature "
          "significance"};
  CP::SysWriteDecorHandle<float>
      m_softmu_scatteringNeighbourSignificance_Handle{
          this, "scatteringNeighbourSignificance",
          "scatteringNeighbourSignificance_%SYS%",
          "decoration name for the soft muon scattering neighbour "
          "significance"};
  CP::SysWriteDecorHandle<float> m_softmu_qOverPSignificance_Handle{
      this, "qOverPSignificance", "qOverPSignificance_%SYS%",
      "decoration name for the soft muon qOverP significance"};
  CP::SysWriteDecorHandle<float> m_softmu_rhoPrime_Handle{
      this, "rhoPrime", "rhoPrime_%SYS%",
      "decoration name for the soft muon rho prime"};
  CP::SysWriteDecorHandle<float> m_softmu_segmentDeltaEta_Handle{
      this, "segmentDeltaEta", "segmentDeltaEta_%SYS%",
      "decoration name for the soft muon segmentDeltaEta"};
  CP::SysWriteDecorHandle<float> m_softmu_spectrometerFieldIntegral_Handle{
      this, "spectrometerFieldIntegral", "spectrometerFieldIntegral_%SYS%",
      "decoration name for the soft muon spectrometer field integral"};

  //---------- Now a series of variables about the hits and holes of the muon
  // tracks
  CP::SysWriteDecorHandle<int> m_softmu_nprecisionLayers_Handle{
      this, "nprecisionLayers", "nprecisionLayers_%SYS%",
      "decoration name for the soft muon number of precision layers"};
  CP::SysWriteDecorHandle<int> m_softmu_nprecisionHoleLayers_Handle{
      this, "nprecisionHoleLayers", "nprecisionHoleLayers_%SYS%",
      "decoration name for the soft muon number of precision hole layers"};
  CP::SysWriteDecorHandle<int> m_softmu_nGoodPrecLayers_Handle{
      this, "nGoodPrecLayers", "nGoodPrecLayers_%SYS%",
      "decoration name for the soft muon number of good precision layers"};
  CP::SysWriteDecorHandle<int> m_softmu_innerSmallHits_Handle{
      this, "innerSmallHits", "innerSmallHits_%SYS%",
      "decoration name for the soft muon inner small hits"};
  CP::SysWriteDecorHandle<int> m_softmu_innerLargeHits_Handle{
      this, "innerLargeHits", "innerLargeHits_%SYS%",
      "decoration name for the soft muon inner large hits"};
  CP::SysWriteDecorHandle<int> m_softmu_innerSmallHoles_Handle{
      this, "innerSmallHoles", "innerSmallHoles_%SYS%",
      "decoration name for the soft muon inner small holes"};
  CP::SysWriteDecorHandle<int> m_softmu_innerLargeHoles_Handle{
      this, "innerLargeHoles", "innerLargeHoles_%SYS%",
      "decoration name for the soft muon inner large holes"};
  CP::SysWriteDecorHandle<int> m_softmu_middleSmallHits_Handle{
      this, "middleSmallHits", "middleSmallHits_%SYS%",
      "decoration name for the soft muon middle small hits"};
  CP::SysWriteDecorHandle<int> m_softmu_middleLargeHits_Handle{
      this, "middleLargeHits", "middleLargeHits_%SYS%",
      "decoration name for the soft muon middle large hits"};
  CP::SysWriteDecorHandle<int> m_softmu_middleSmallHoles_Handle{
      this, "middleSmallHoles", "middleSmallHoles_%SYS%",
      "decoration name for the soft muon middle small holes"};
  CP::SysWriteDecorHandle<int> m_softmu_middleLargeHoles_Handle{
      this, "middleLargeHoles", "middleLargeHoles_%SYS%",
      "decoration name for the soft muon middle large holes"};
  CP::SysWriteDecorHandle<int> m_softmu_outerSmallHits_Handle{
      this, "outerSmallHits", "outerSmallHits_%SYS%",
      "decoration name for the soft muon outer small hits"};
  CP::SysWriteDecorHandle<int> m_softmu_outerLargeHits_Handle{
      this, "outerLargeHits", "outerLargeHits_%SYS%",
      "decoration name for the soft muon outer large hits"};
  CP::SysWriteDecorHandle<int> m_softmu_outerSmallHoles_Handle{
      this, "outerSmallHoles", "outerSmallHoles_%SYS%",
      "decoration name for the soft muon outer small holes"};
  CP::SysWriteDecorHandle<int> m_softmu_outerLargeHoles_Handle{
      this, "outerLargeHoles", "outerLargeHoles_%SYS%",
      "decoration name for the soft muon outer large holes"};
  CP::SysWriteDecorHandle<int> m_softmu_extendedSmallHits_Handle{
      this, "extendedSmallHits", "extendedSmallHits_%SYS%",
      "decoration name for the soft muon extended small hits"};
  CP::SysWriteDecorHandle<int> m_softmu_extendedLargeHits_Handle{
      this, "extendedLargeHits", "extendedLargeHits_%SYS%",
      "decoration name for the soft muon extended large hits"};
  CP::SysWriteDecorHandle<int> m_softmu_extendedSmallHoles_Handle{
      this, "extendedSmallHoles", "extendedSmallHoles_%SYS%",
      "decoration name for the soft muon extended small holes"};
  CP::SysWriteDecorHandle<int> m_softmu_extendedLargeHoles_Handle{
      this, "extendedLargeHoles", "extendedLargeHoles_%SYS%",
      "decoration name for the soft muon extended large holes"};
  CP::SysWriteDecorHandle<int> m_softmu_innerClosePrecisionHits_Handle{
      this, "innerClosePrecisionHits", "innerClosePrecisionHits_%SYS%",
      "decoration name for the soft muon inner close precision hits"};
  CP::SysWriteDecorHandle<int> m_softmu_middleClosePrecisionHits_Handle{
      this, "middleClosePrecisionHits", "middleClosePrecisionHits_%SYS%",
      "decoration name for the soft muon middle close precision hits"};
  CP::SysWriteDecorHandle<int> m_softmu_outerClosePrecisionHits_Handle{
      this, "outerClosePrecisionHits", "outerClosePrecisionHits_%SYS%",
      "decoration name for the soft muon outer close precision hits"};
  CP::SysWriteDecorHandle<int> m_softmu_extendedClosePrecisionHits_Handle{
      this, "extendedClosePrecisionHits", "extendedClosePrecisionHits_%SYS%",
      "decoration name for the soft muon extended close precision hits"};
  CP::SysWriteDecorHandle<int> m_softmu_innerOutBoundsPrecisionHits_Handle{
      this, "innerOutBoundsPrecisionHits", "innerOutBoundsPrecisionHits_%SYS%",
      "decoration name for the soft muon inner outbounds precision hits"};
  CP::SysWriteDecorHandle<int> m_softmu_middleOutBoundsPrecisionHits_Handle{
      this, "middleOutBoundsPrecisionHits",
      "middleOutBoundsPrecisionHits_%SYS%",
      "decoration name for the soft muon middle outbounds precision hits"};
  CP::SysWriteDecorHandle<int> m_softmu_outerOutBoundsPrecisionHits_Handle{
      this, "outerOutBoundsPrecisionHits", "outerOutBoundsPrecisionHits_%SYS%",
      "decoration name for the soft muon outer outbounds precision hits"};
  CP::SysWriteDecorHandle<int> m_softmu_extendedOutBoundsPrecisionHits_Handle{
      this, "extendedOutBoundsPrecisionHits",
      "extendedOutBoundsPrecisionHits_%SYS%",
      "decoration name for the soft muon extended outbounds precision hits"};
  CP::SysWriteDecorHandle<int>
      m_softmu_combinedTrackOutBoundsPrecisionHits_Handle{
          this, "combinedTrackOutBoundsPrecisionHits",
          "combinedTrackOutBoundsPrecisionHits_%SYS%",
          "decoration name for the soft muon combined track outbounds "
          "precision hits"};
  CP::SysWriteDecorHandle<int> m_softmu_isSmallGoodSectors_Handle{
      this, "isSmallGoodSectors", "isSmallGoodSectors_%SYS%",
      "decoration name for the soft muon is small good sectors"};
  CP::SysWriteDecorHandle<int> m_softmu_isEndcapGoodLayers_Handle{
      this, "isEndcapGoodLayers", "isEndcapGoodLayers_%SYS%",
      "decoration name for the soft muon is endcap good sectors"};
  CP::SysWriteDecorHandle<int> m_softmu_cscUnspoiledEtaHits_Handle{
      this, "cscUnspoiledEtaHits", "cscUnspoiledEtaHits_%SYS%",
      "decoration name for the soft muon CSC unspoiled eta hits"};
  CP::SysWriteDecorHandle<int> m_softmu_etaLayer1STGCHits_Handle{
      this, "etaLayer1STGCHits", "etaLayer1STGCHits_%SYS%",
      "decoration name for the soft muon etaLayer1 sTGC hits"};
  CP::SysWriteDecorHandle<int> m_softmu_etaLayer2STGCHits_Handle{
      this, "etaLayer2STGCHits", "etaLayer2STGCHits_%SYS%",
      "decoration name for the soft muon etaLayer2 sTGC hits"};
  CP::SysWriteDecorHandle<int> m_softmu_phiLayer1STGCHits_Handle{
      this, "phiLayer1STGCHits", "phiLayer1STGCHits_%SYS%",
      "decoration name for the soft muon phiLayer1 sTGC hits"};
  CP::SysWriteDecorHandle<int> m_softmu_phiLayer2STGCHits_Handle{
      this, "phiLayer2STGCHits", "phiLayer2STGCHits_%SYS%",
      "decoration name for the soft muon phiLayer2 sTGC hits"};
  CP::SysWriteDecorHandle<int> m_softmu_MMHits_Handle{
      this, "MMHits", "MMHits_%SYS%",
      "decoration name for the soft muon MM hits"};
  CP::SysWriteDecorHandle<int> m_softmu_numberOfPixelHits_Handle{
      this, "numberOfPixelHits", "numberOfPixelHits_%SYS%",
      "decoration name for the soft muon number of pixel hits"};
  CP::SysWriteDecorHandle<int> m_softmu_numberOfPixelDeadSensors_Handle{
      this, "numberOfPixelDeadSensors", "numberOfPixelDeadSensors_%SYS%",
      "decoration name for the soft muon number of pixel dead sensors"};
  CP::SysWriteDecorHandle<int> m_softmu_numberOfPixelHoles_Handle{
      this, "numberOfPixelHoles", "numberOfPixelHoles_%SYS%",
      "decoration name for the soft muon number of pixel holes"};
  CP::SysWriteDecorHandle<int> m_softmu_numberOfSCTHits_Handle{
      this, "numberOfSCTHits", "numberOfSCTHits_%SYS%",
      "decoration name for the soft muon number of SCT hits"};
  CP::SysWriteDecorHandle<int> m_softmu_numberOfSCTDeadSensors_Handle{
      this, "numberOfSCTDeadSensors", "numberOfSCTDeadSensors_%SYS%",
      "decoration name for the soft muon number of SCT dead sensors"};
  CP::SysWriteDecorHandle<int> m_softmu_numberOfSCTHoles_Handle{
      this, "numberOfSCTHoles", "numberOfSCTHoles_%SYS%",
      "decoration name for the soft muon number of SCT holes"};
  CP::SysWriteDecorHandle<int> m_softmu_numberOfTRTHits_Handle{
      this, "numberOfTRTHits", "numberOfTRTHits_%SYS%",
      "decoration name for the soft muon number of TRT hits"};
  CP::SysWriteDecorHandle<int> m_softmu_numberOfTRTOutliers_Handle{
      this, "numberOfTRTOutliers", "numberOfTRTOutliers_%SYS%",
      "decoration name for the soft muon number of TRT outliers"};
  CP::SysWriteDecorHandle<int> m_softmu_numberOfTRTHoles_Handle{
      this, "numberOfTRTHoles", "numberOfTRTHoles_%SYS%",
      "decoration name for the soft muon number of TRT holes"};

  CP::SysWriteDecorHandle<bool> m_softmu_isBIS78_Handle{
      this, "isBIS78", "isBIS78_%SYS%",
      "does the muon fall in the BIS78 region?"};
  CP::SysWriteDecorHandle<bool> m_softmu_isBEE_Handle{
      this, "isBEE", "isBEE_%SYS%", "does the muon fall in the BEE region?"};
  CP::SysWriteDecorHandle<bool> m_softmu_isBMG_Handle{
      this, "isBMG", "isBMG_%SYS%", "does the muon fall in the BMG region?"};

  //---------- Now a series of variables about the soft muon properties
  CP::SysWriteDecorHandle<float>
      m_softmu_ptcone20_Nonprompt_All_MaxWeightTTVA_pt500_Handle{
          this, "ptcone20_Nonprompt_All_MaxWeightTTVA_pt500",
          "ptcone20_Nonprompt_All_MaxWeightTTVA_pt500_%SYS%",
          "decoration name for the soft muon ptcone20 Nonprompt All "
          "MaxWeightTTVA pt500"};
  CP::SysWriteDecorHandle<float>
      m_softmu_ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt500_Handle{
          this, "ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt500",
          "ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt500_%SYS%",
          "decoration name for the soft muon ptvarcone30 Nonprompt All "
          "MaxWeightTTVA pt500"};
  CP::SysWriteDecorHandle<float>
      m_softmu_ptcone20_Nonprompt_All_MaxWeightTTVA_pt1000_Handle{
          this, "ptcone20_Nonprompt_All_MaxWeightTTVA_pt1000",
          "ptcone20_Nonprompt_All_MaxWeightTTVA_pt1000_%SYS%",
          "decoration name for the soft muon ptcone20 Nonprompt All "
          "MaxWeightTTVA pt1000"};
  CP::SysWriteDecorHandle<float>
      m_softmu_ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt1000_Handle{
          this, "ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt1000",
          "ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt1000_%SYS%",
          "decoration name for the soft muon ptvarcone30 Nonprompt All "
          "MaxWeightTTVA pt1000"};
  CP::SysWriteDecorHandle<float> m_softmu_neflowisol20_Handle{
      this, "neflowisol20", "neflowisol20_%SYS%",
      "decoration name for the soft muon neflowisol20"};
  CP::SysWriteDecorHandle<float> m_softmu_EnergyLoss_Handle{
      this, "EnergyLoss", "EnergyLoss_%SYS%",
      "decoration name for the soft muon energy loss"};
  CP::SysWriteDecorHandle<float> m_softmu_EnergyLossSigma_Handle{
      this, "EnergyLossSigma", "EnergyLossSigma_%SYS%",
      "decoration name for the soft muon energy loss sigma"};
  CP::SysWriteDecorHandle<unsigned char> m_softmu_EnergyLossType_Handle{
      this, "EnergyLossType", "EnergyLossType_%SYS%",
      "decoration name for the soft muon energy loss type"};
  CP::SysWriteDecorHandle<float> m_softmu_ParamEnergyLoss_Handle{
      this, "ParamEnergyLoss", "ParamEnergyLoss_%SYS%",
      "decoration name for the soft muon parameterized energy loss"};
  CP::SysWriteDecorHandle<float> m_softmu_MeasEnergyLoss_Handle{
      this, "MeasEnergyLoss", "MeasEnergyLoss_%SYS%",
      "decoration name for the soft muon measured energy loss"};
  CP::SysWriteDecorHandle<float> m_softmu_ParamEnergyLossSigmaPlus_Handle{
      this, "ParamEnergyLossSigmaPlus", "ParamEnergyLossSigmaPlus_%SYS%",
      "decoration name for the soft muon parameterized energy loss sigma plus"};
  CP::SysWriteDecorHandle<float> m_softmu_ParamEnergyLossSigmaMinus_Handle{
      this, "ParamEnergyLossSigmaMinus", "ParamEnergyLossSigmaMinus_%SYS%",
      "decoration name for the soft muon parameterized energy loss sigma "
      "minus"};
  CP::SysWriteDecorHandle<float> m_softmu_MeasEnergyLossSigma_Handle{
      this, "MeasEnergyLossSigma", "MeasEnergyLossSigma_%SYS%",
      "decoration name for the soft muon measured energy loss sigma"};

  CP::SysWriteDecorHandle<int> m_softmu_nSegments_Handle{
      this, "nSegments", "nSegments_%SYS%", "number of segments of the muon"};
  CP::SysWriteDecorHandle<int> m_softmu_Seg1ChamberId_Handle{
      this, "Seg1ChamberId", "Seg1ChamberId_%SYS%",
      "Chamber Index of first segment of muon"};
  CP::SysWriteDecorHandle<int> m_softmu_Seg2ChamberId_Handle{
      this, "Seg2ChamberId", "Seg2ChamberId_%SYS%",
      "Chamber Index of second segment of muon"};

  //---------- Now a series of variables about the jet that is closer to the
  // soft muon
  CP::SysWriteDecorHandle<int> m_softmu_nearestJet_Index_Handle{
      this, "nearestJet_Index", "nearestJet_Index_%SYS%",
      "decoration name for the soft muon nearest jet index"};
  CP::SysWriteDecorHandle<float> m_softmu_nearestJet_pt_Handle{
      this, "nearestJet_pt", "nearestJet_pt_%SYS%",
      "decoration name for the soft muon nearest jet pt"};
  CP::SysWriteDecorHandle<float> m_softmu_nearestJet_eta_Handle{
      this, "nearestJet_eta", "nearestJet_eta_%SYS%",
      "decoration name for the soft muon nearest jet eta"};
  CP::SysWriteDecorHandle<float> m_softmu_nearestJet_phi_Handle{
      this, "nearestJet_phi", "nearestJet_phi_%SYS%",
      "decoration name for the soft muon nearest jet phi"};
  CP::SysWriteDecorHandle<float> m_softmu_nearestJet_e_Handle{
      this, "nearestJet_e", "nearestJet_e_%SYS%",
      "decoration name for the soft muon nearest jet e"};
  CP::SysWriteDecorHandle<float> m_softmu_nearestJet_dR_Handle{
      this, "nearestJet_dR", "nearestJet_dR_%SYS%",
      "decoration name for the soft muon nearest jet delta R"};
  CP::SysWriteDecorHandle<float> m_softmu_nearestJet_EMFrac_Handle{
      this, "nearestJet_EMFrac", "nearestJet_EMFrac_%SYS%",
      "decoration name for the soft muon nearest jet EM fraction"};
  CP::SysWriteDecorHandle<int> m_softmu_nearestJet_NumTrkPt500_Handle{
      this, "nearestJet_NumTrkPt500", "nearestJet_NumTrkPt500_%SYS%",
      "decoration name for the soft muon nearest jet number of tracks with pt "
      "> 500"};
  CP::SysWriteDecorHandle<float> m_softmu_nearestJet_SumPtTrkPt500_Handle{
      this, "nearestJet_SumPtTrkPt500", "nearestJet_SumPtTrkPt500_%SYS%",
      "decoration name for the soft muon nearest jet sum pt of tracks with pt "
      "> 500"};
  CP::SysWriteDecorHandle<int> m_softmu_nearestJet_NumTrkPt1000_Handle{
      this, "nearestJet_NumTrkPt1000", "nearestJet_NumTrkPt1000_%SYS%",
      "decoration name for the soft muon nearest jet number of tracks with pt "
      "> 1000"};
  CP::SysWriteDecorHandle<float> m_softmu_nearestJet_SumPtTrkPt1000_Handle{
      this, "nearestJet_SumPtTrkPt1000", "nearestJet_SumPtTrkPt1000_%SYS%",
      "decoration name for the soft muon nearest jet sum pt of tracks with pt "
      "> 1000"};
  CP::SysWriteDecorHandle<int> m_softmu_nearestJet_N90Constituents_Handle{
      this, "nearestJet_N90Constituents", "nearestJet_N90Constituents_%SYS%",
      "decoration name for the soft muon nearest jet N90 constituents"};
  CP::SysWriteDecorHandle<float> m_softmu_nearestJet_TrackWidthPt500_Handle{
      this, "nearestJet_TrackWidthPt500", "nearestJet_TrackWidthPt500_%SYS%",
      "decoration name for the soft muon nearest jet track width with pt > "
      "500"};
  CP::SysWriteDecorHandle<float> m_softmu_nearestJet_TrackWidthPt1000_Handle{
      this, "nearestJet_TrackWidthPt1000", "nearestJet_TrackWidthPt1000_%SYS%",
      "decoration name for the soft muon nearest jet track width with pt > "
      "1000"};
  CP::SysWriteDecorHandle<float> m_softmu_nearestJet_Width_Handle{
      this, "nearestJet_Width", "nearestJet_Width_%SYS%",
      "decoration name for the soft muon nearest jet width"};
  CP::SysWriteDecorHandle<float> m_softmu_nearestJet_Charge_Handle{
      this, "nearestJet_Charge", "nearestJet_Charge_%SYS%",
      "decoration name for the soft muon nearest jet charge"};
  CP::SysWriteDecorHandle<float>
      m_softmu_nearestJet_ChargedPFOWidthPt500_Handle{
          this, "nearestJet_ChargedPFOWidthPt500",
          "nearestJet_ChargedPFOWidthPt500_%SYS%",
          "decoration name for the soft muon nearest jet charged PFO width "
          "with pt > 500"};
  CP::SysWriteDecorHandle<float>
      m_softmu_nearestJet_ChargedPFOWidthPt1000_Handle{
          this, "nearestJet_ChargedPFOWidthPt1000",
          "nearestJet_ChargedPFOWidthPt1000_%SYS%",
          "decoration name for the soft muon nearest jet charged PFO width "
          "with pt > 1000"};

  CP::SysWriteDecorHandle<int> m_softmu_truthmuon_barcode_Handle{
      this, "truthmuon_barcode", "truthmuon_barcode_%SYS%",
      "barcode of the truth muon associated with the soft muon, useful for "
      "unfolding"};
  CP::SysWriteDecorHandle<int> m_softmu_truthBAncestor_pdgId_Handle{
      this, "truthBAncestor_pdgId", "truthBAncestor_pdgId_%SYS%",
      "decoration name for the pdgId of the last B-hadron ancestor of the soft "
      "muon in MC"};
  CP::SysWriteDecorHandle<float> m_softmu_truthBAncestor_pt_Handle{
      this, "truthBAncestor_pt", "truthBAncestor_pt_%SYS%",
      "decoration name for the pt of the last B-hadron ancestor of the soft "
      "muon in MC"};
  CP::SysWriteDecorHandle<float> m_softmu_truthBAncestor_eta_Handle{
      this, "truthBAncestor_eta", "truthBAncestor_eta_%SYS%",
      "decoration name for the eta of the last B-hadron ancestor of the soft "
      "muon in MC"};
  CP::SysWriteDecorHandle<float> m_softmu_truthBAncestor_phi_Handle{
      this, "truthBAncestor_phi", "truthBAncestor_phi_%SYS%",
      "decoration name for the phi of the last B-hadron ancestor of the soft "
      "muon in MC"};
  CP::SysWriteDecorHandle<float> m_softmu_truthBAncestor_e_Handle{
      this, "truthBAncestor_e", "truthBAncestor_e_%SYS%",
      "decoration name for the e of the last B-hadron ancestor of the soft "
      "muon in MC"};
  CP::SysWriteDecorHandle<int> m_softmu_truthCAncestor_pdgId_Handle{
      this, "truthCAncestor_pdgId", "truthCAncestor_pdgId_%SYS%",
      "decoration name for the pdgId of the last C-hadron ancestor of the soft "
      "muon in MC"};
  CP::SysWriteDecorHandle<float> m_softmu_truthCAncestor_pt_Handle{
      this, "truthCAncestor_pt", "truthCAncestor_pt_%SYS%",
      "decoration name for the pt of the last C-hadron ancestor of the soft "
      "muon in MC"};
  CP::SysWriteDecorHandle<float> m_softmu_truthCAncestor_eta_Handle{
      this, "truthCAncestor_eta", "truthCAncestor_eta_%SYS%",
      "decoration name for the eta of the last C-hadron ancestor of the soft "
      "muon in MC"};
  CP::SysWriteDecorHandle<float> m_softmu_truthCAncestor_phi_Handle{
      this, "truthCAncestor_phi", "truthCAncestor_phi_%SYS%",
      "decoration name for the phi of the last C-hadron ancestor of the soft "
      "muon in MC"};
  CP::SysWriteDecorHandle<float> m_softmu_truthCAncestor_e_Handle{
      this, "truthCAncestor_e", "truthCAncestor_e_%SYS%",
      "decoration name for the e of the last C-hadron ancestor of the soft "
      "muon in MC"};
  CP::SysWriteDecorHandle<int> m_softmu_truthTauAncestor_pdgId_Handle{
      this, "truthTauAncestor_pdgId", "truthTauAncestor_pdgId_%SYS%",
      "decoration name for the pdgId of the last Tau ancestor of the soft muon "
      "in MC"};
  CP::SysWriteDecorHandle<int> m_softmu_truthFirstBAncestor_pdgId_Handle{
      this, "truthFirstBAncestor_pdgId", "truthFirstBAncestor_pdgId_%SYS%",
      "decoration name for the pdgId of the first B-hadron ancestor of the "
      "soft muon in MC"};
  CP::SysWriteDecorHandle<int> m_softmu_truthFirstCAncestor_pdgId_Handle{
      this, "truthFirstCAncestor_pdgId", "truthFirstCAncestor_pdgId_%SYS%",
      "decoration name for the pdgId of the first C-hadron ancestor of the "
      "soft muon in MC"};
};

}  // namespace top

#endif
