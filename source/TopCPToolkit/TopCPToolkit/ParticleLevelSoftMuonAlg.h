/*
  Copyright (C) 2002-2025 CERN for the benefit of the ATLAS collaboration
*/

/// @author Marco Vanadia <marco.vanadia@cern.ch>

#ifndef TRUTH__PARTICLELEVEL_SOFTMUON__ALG_H
#define TRUTH__PARTICLELEVEL_SOFTMUON__ALG_H

// Algorithm includes
#include <AnaAlgorithm/AnaAlgorithm.h>
#include <AsgDataHandles/ReadHandle.h>
#include <AsgDataHandles/ReadHandleKey.h>
#include <AsgDataHandles/WriteDecorHandle.h>
#include <AsgDataHandles/WriteDecorHandleKey.h>
#include <AsgTools/PropertyWrapper.h>
#include <SelectionHelpers/SelectionReadHandle.h>

// Framework includes
#include <FourMomUtils/xAODP4Helpers.h>
#include <MCTruthClassifier/IMCTruthClassifier.h>
#include <VectorHelpers/LorentzHelper.h>
#include <xAODEventInfo/EventInfo.h>
#include <xAODTruth/TruthParticleContainer.h>

namespace CP {
using ROOT::Math::PtEtaPhiEVector;
class ParticleLevelSoftMuonAlg : public EL::AnaAlgorithm {
 public:
  using EL::AnaAlgorithm::AnaAlgorithm;
  virtual StatusCode initialize() final;
  virtual StatusCode execute() final;

 private:
  SG::ReadHandleKey<xAOD::JetContainer> m_jetsKey{
      this, "jets", "", "the name of the input truth jet container"};
  SG::ReadHandleKey<xAOD::TruthParticleContainer> m_muonsKey{
      this, "muons", "", "the name of the input truth muons container"};
  Gaudi::Property<std::string> m_jetAssociated{
      this, "jetAssociated", "jetAssociated",
      "decoration for truth soft muons in jets"};
  Gaudi::Property<bool> m_useRapidity{
      this, "useRapidityForDeltaR", true,
      "whether to use rapidity instead of pseudo-rapidity for the calculation "
      "of DeltaR"};
  Gaudi::Property<bool> m_useDressedProperties{
      this, "useDressedProperties", true,
      "whether to use dressed muon kinematics rather than simple "
      "P4 kinematics"};
  Gaudi::Property<float> m_matchingDRValue{this, "matchingDRValue", 0.4,
                                           "max DeltaR between muon and jet"};
  CP::SelectionReadHandle m_jetSelection{
      this, "jetSelection", "", "the selection on the input truth jets"};
  CP::SelectionReadHandle m_muonSelection{
      this, "muonSelection", "", "the selection on the input truth muons"};

  SG::WriteDecorHandleKey<xAOD::EventInfo> m_pass_PLSoftMuonSelection_key{
      this, "passPLSoftMuonSelectionKey", "EventInfo.passPLSoftMuonSelection",
      "Does the event pass the soft muon selection at particle level?"};
  SG::WriteDecorHandleKey<xAOD::TruthParticleContainer>
      m_dec_softmu_truthBAncestor_pdgId{this, "dec_truthBAncestor_pdgId",
                                        "TruthMuons.truthBAncestor_pdgId",
                                        "truthB ancestor pdgId decoration"};
  SG::WriteDecorHandleKey<xAOD::TruthParticleContainer>
      m_dec_softmu_truthBAncestor_pt{this, "dec_truthBAncestor_pt",
                                     "TruthMuons.truthBAncestor_pt",
                                     "truthB ancestor pt decoration"};
  SG::WriteDecorHandleKey<xAOD::TruthParticleContainer>
      m_dec_softmu_truthBAncestor_eta{this, "dec_truthBAncestor_eta",
                                      "TruthMuons.truthBAncestor_eta",
                                      "truthB ancestor eta decoration"};
  SG::WriteDecorHandleKey<xAOD::TruthParticleContainer>
      m_dec_softmu_truthBAncestor_phi{this, "dec_truthBAncestor_phi",
                                      "TruthMuons.truthBAncestor_phi",
                                      "truthB ancestor phi decoration"};
  SG::WriteDecorHandleKey<xAOD::TruthParticleContainer>
      m_dec_softmu_truthBAncestor_e{this, "dec_truthBAncestor_e",
                                    "TruthMuons.truthBAncestor_e",
                                    "truthB ancestor e decoration"};
  SG::WriteDecorHandleKey<xAOD::TruthParticleContainer>
      m_dec_softmu_truthCAncestor_pdgId{this, "dec_truthCAncestor_pdgId",
                                        "TruthMuons.truthCAncestor_pdgId",
                                        "truthC ancestor pdgId decoration"};
  SG::WriteDecorHandleKey<xAOD::TruthParticleContainer>
      m_dec_softmu_truthCAncestor_pt{this, "dec_truthCAncestor_pt",
                                     "TruthMuons.truthCAncestor_pt",
                                     "truthC ancestor pt decoration"};
  SG::WriteDecorHandleKey<xAOD::TruthParticleContainer>
      m_dec_softmu_truthCAncestor_eta{this, "dec_truthCAncestor_eta",
                                      "TruthMuons.truthCAncestor_eta",
                                      "truthC ancestor eta decoration"};
  SG::WriteDecorHandleKey<xAOD::TruthParticleContainer>
      m_dec_softmu_truthCAncestor_phi{this, "dec_truthCAncestor_phi",
                                      "TruthMuons.truthCAncestor_phi",
                                      "truthC ancestor phi decoration"};
  SG::WriteDecorHandleKey<xAOD::TruthParticleContainer>
      m_dec_softmu_truthCAncestor_e{this, "dec_truthCAncestor_e",
                                    "TruthMuons.truthCAncestor_e",
                                    "truthC ancestor e decoration"};
  SG::WriteDecorHandleKey<xAOD::TruthParticleContainer>
      m_dec_softmu_truthTauAncestor_pdgId{this, "dec_truthTauAncestor_pdgId",
                                          "TruthMuons.truthTauAncestor_pdgId",
                                          "truthTau ancestor pdgId decoration"};
  SG::WriteDecorHandleKey<xAOD::TruthParticleContainer>
      m_dec_softmu_truthFirstBAncestor_pdgId{
          this, "dec_truthFirstBAncestor_pdgId",
          "TruthMuons.truthFirstBAncestor_pdgId",
          "truthFirstB ancestor pdgId decoration"};
  SG::WriteDecorHandleKey<xAOD::TruthParticleContainer>
      m_dec_softmu_truthFirstCAncestor_pdgId{
          this, "dec_truthFirstCAncestor_pdgId",
          "TruthMuons.truthFirstCAncestor_pdgId",
          "truthFirstC ancestor pdgId decoration"};

  std::unique_ptr<const SG::AuxElement::Decorator<char>> m_dec_jetAssociated{};

  float dressedDeltaR(const xAOD::Jet* p1, PtEtaPhiEVector& p2,
                      bool useRapidity);

  const xAOD::EventInfo* evtInfo = 0;
};

}  // namespace CP

#endif
