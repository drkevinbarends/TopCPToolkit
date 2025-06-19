/*
  Copyright (C) 2002-2025 CERN for the benefit of the ATLAS collaboration
*/

/// @author Marco Vanadia <marco.vanadia@cern.ch>

#include "TopCPToolkit/ParticleLevelSoftMuonAlg.h"

#include "TopCPToolkit/ParticleLevelSoftMuonOrigin.h"

namespace CP {

StatusCode ParticleLevelSoftMuonAlg::initialize() {

  ANA_CHECK(m_jetsKey.initialize());
  ANA_CHECK(m_muonsKey.initialize());
  // decorators
  m_dec_jetAssociated = std::make_unique<SG::AuxElement::Decorator<char>>(
      m_jetAssociated.value());

  if (!m_jetSelection.empty())
    ANA_CHECK(m_jetSelection.initialize());
  if (!m_muonSelection.empty())
    ANA_CHECK(m_muonSelection.initialize());

  ANA_CHECK(m_pass_PLSoftMuonSelection_key.initialize());
  ANA_CHECK(m_dec_softmu_truthBAncestor_pdgId.initialize());
  ANA_CHECK(m_dec_softmu_truthBAncestor_pt.initialize());
  ANA_CHECK(m_dec_softmu_truthBAncestor_eta.initialize());
  ANA_CHECK(m_dec_softmu_truthBAncestor_phi.initialize());
  ANA_CHECK(m_dec_softmu_truthBAncestor_e.initialize());
  ANA_CHECK(m_dec_softmu_truthCAncestor_pdgId.initialize());
  ANA_CHECK(m_dec_softmu_truthCAncestor_pt.initialize());
  ANA_CHECK(m_dec_softmu_truthCAncestor_eta.initialize());
  ANA_CHECK(m_dec_softmu_truthCAncestor_phi.initialize());
  ANA_CHECK(m_dec_softmu_truthCAncestor_e.initialize());
  ANA_CHECK(m_dec_softmu_truthTauAncestor_pdgId.initialize());
  ANA_CHECK(m_dec_softmu_truthFirstBAncestor_pdgId.initialize());
  ANA_CHECK(m_dec_softmu_truthFirstCAncestor_pdgId.initialize());

  return StatusCode::SUCCESS;
}

float ParticleLevelSoftMuonAlg::dressedDeltaR(const xAOD::Jet* p1,
                                              PtEtaPhiEVector& p2,
                                              bool useRapidity) {
  if (useRapidity)
    return xAOD::P4Helpers::deltaR(p1->rapidity(), p1->phi(), p2.Rapidity(),
                                   p2.Phi());
  else
    return xAOD::P4Helpers::deltaR(p1->eta(), p1->phi(), p2.Eta(), p2.Phi());
}

StatusCode ParticleLevelSoftMuonAlg::execute() {

  const EventContext& ctx = Gaudi::Hive::currentContext();

  SG::ReadHandle<xAOD::JetContainer> jets(m_jetsKey, ctx);
  SG::ReadHandle<xAOD::TruthParticleContainer> muons(m_muonsKey, ctx);

  // accessors
  static const SG::AuxElement::ConstAccessor<float> acc_pt_dressed(
      "pt_dressed");
  static const SG::AuxElement::ConstAccessor<float> acc_eta_dressed(
      "eta_dressed");
  static const SG::AuxElement::ConstAccessor<float> acc_phi_dressed(
      "phi_dressed");
  static const SG::AuxElement::ConstAccessor<float> acc_e_dressed("e_dressed");
  static const SG::AuxElement::ConstAccessor<unsigned int> acc_truth_origin(
      "classifierParticleOrigin");

  ANA_CHECK(evtStore()->retrieve(evtInfo, "EventInfo"));
  SG::WriteDecorHandle<xAOD::EventInfo, char> dec_PLSoftMuonSelection(
      m_pass_PLSoftMuonSelection_key);
  dec_PLSoftMuonSelection(*evtInfo) = 0;

  SG::WriteDecorHandle<xAOD::TruthParticleContainer, int>
      dec_softmu_truthBAncestor_pdgId(m_dec_softmu_truthBAncestor_pdgId, ctx);
  SG::WriteDecorHandle<xAOD::TruthParticleContainer, float>
      dec_softmu_truthBAncestor_pt(m_dec_softmu_truthBAncestor_pt, ctx);
  SG::WriteDecorHandle<xAOD::TruthParticleContainer, float>
      dec_softmu_truthBAncestor_eta(m_dec_softmu_truthBAncestor_eta, ctx);
  SG::WriteDecorHandle<xAOD::TruthParticleContainer, float>
      dec_softmu_truthBAncestor_phi(m_dec_softmu_truthBAncestor_phi, ctx);
  SG::WriteDecorHandle<xAOD::TruthParticleContainer, float>
      dec_softmu_truthBAncestor_e(m_dec_softmu_truthBAncestor_e, ctx);
  SG::WriteDecorHandle<xAOD::TruthParticleContainer, int>
      dec_softmu_truthCAncestor_pdgId(m_dec_softmu_truthCAncestor_pdgId, ctx);
  SG::WriteDecorHandle<xAOD::TruthParticleContainer, float>
      dec_softmu_truthCAncestor_pt(m_dec_softmu_truthCAncestor_pt, ctx);
  SG::WriteDecorHandle<xAOD::TruthParticleContainer, float>
      dec_softmu_truthCAncestor_eta(m_dec_softmu_truthCAncestor_eta, ctx);
  SG::WriteDecorHandle<xAOD::TruthParticleContainer, float>
      dec_softmu_truthCAncestor_phi(m_dec_softmu_truthCAncestor_phi, ctx);
  SG::WriteDecorHandle<xAOD::TruthParticleContainer, float>
      dec_softmu_truthCAncestor_e(m_dec_softmu_truthCAncestor_e, ctx);
  SG::WriteDecorHandle<xAOD::TruthParticleContainer, int>
      dec_softmu_truthTauAncestor_pdgId(m_dec_softmu_truthTauAncestor_pdgId,
                                        ctx);
  SG::WriteDecorHandle<xAOD::TruthParticleContainer, int>
      dec_softmu_truthFirstBAncestor_pdgId(
          m_dec_softmu_truthFirstBAncestor_pdgId, ctx);
  SG::WriteDecorHandle<xAOD::TruthParticleContainer, int>
      dec_softmu_truthFirstCAncestor_pdgId(
          m_dec_softmu_truthFirstCAncestor_pdgId, ctx);

  for (const auto* muon : *muons) {

    (*m_dec_jetAssociated)(*muon) =
        false;  // first we create the default for all muons

    dec_softmu_truthBAncestor_pdgId(*muon) = 0;
    dec_softmu_truthBAncestor_pt(*muon) = 0;
    dec_softmu_truthBAncestor_eta(*muon) = 0;
    dec_softmu_truthBAncestor_phi(*muon) = 0;
    dec_softmu_truthBAncestor_e(*muon) = 0;
    dec_softmu_truthCAncestor_pdgId(*muon) = 0;
    dec_softmu_truthCAncestor_pt(*muon) = 0;
    dec_softmu_truthCAncestor_eta(*muon) = 0;
    dec_softmu_truthCAncestor_phi(*muon) = 0;
    dec_softmu_truthCAncestor_e(*muon) = 0;
    dec_softmu_truthTauAncestor_pdgId(*muon) = 0;
    dec_softmu_truthFirstBAncestor_pdgId(*muon) = 0;
    dec_softmu_truthFirstCAncestor_pdgId(*muon) = 0;

    if (m_muonSelection && !m_muonSelection.getBool(*muon))
      continue;

    PtEtaPhiEVector muon_tlv;
    if (m_useDressedProperties)
      muon_tlv = PtEtaPhiEVector(acc_pt_dressed(*muon), acc_eta_dressed(*muon),
                                 acc_phi_dressed(*muon), acc_e_dressed(*muon));
    else
      muon_tlv =
          PtEtaPhiEVector(muon->pt(), muon->eta(), muon->phi(), muon->e());

    bool isWithinJet = false;

    for (const auto* jet : *jets) {
      if (m_jetSelection && !m_jetSelection.getBool(*jet))
        continue;

      if (dressedDeltaR(jet, muon_tlv, m_useRapidity) < m_matchingDRValue) {
        isWithinJet = true;
        break;
      }
    }

    (*m_dec_jetAssociated)(*muon) = isWithinJet;
    if (isWithinJet) {
      dec_PLSoftMuonSelection(*evtInfo) = 1;

      const xAOD::TruthParticle* lastBAncestor = nullptr;
      const xAOD::TruthParticle* lastCAncestor = nullptr;
      const xAOD::TruthParticle* lastTauAncestor = nullptr;
      const xAOD::TruthParticle* firstBAncestor = nullptr;
      const xAOD::TruthParticle* firstCAncestor = nullptr;
      const xAOD::TruthParticleContainer* contHF = nullptr;
      ANA_CHECK(evtStore()->retrieve(
          contHF, "TruthHFWithDecayParticles"));  // we need this container,
                                                  // available in DAOD_PHYS, to
                                                  // be able to navigate back HF
                                                  // ancestors of the muon
      if (!contHF) {
        ANA_MSG_ERROR(
            "Cannot retrieve TruthHFWithDecayParticles, you cannot save "
            "additional soft muon variables without that");
        return StatusCode::FAILURE;
      }
      top::getSoftMuonAncestorInformation(muon, lastBAncestor, lastCAncestor,
                                          lastTauAncestor, firstBAncestor,
                                          firstCAncestor, contHF);

      dec_softmu_truthBAncestor_pdgId(*muon) =
          lastBAncestor ? lastBAncestor->pdgId() : 0;
      dec_softmu_truthBAncestor_pt(*muon) =
          lastBAncestor ? lastBAncestor->pt() : 0;
      dec_softmu_truthBAncestor_eta(*muon) =
          lastBAncestor ? lastBAncestor->eta() : 0;
      dec_softmu_truthBAncestor_phi(*muon) =
          lastBAncestor ? lastBAncestor->phi() : 0;
      dec_softmu_truthBAncestor_e(*muon) =
          lastBAncestor ? lastBAncestor->e() : 0;
      dec_softmu_truthCAncestor_pdgId(*muon) =
          lastCAncestor ? lastCAncestor->pdgId() : 0;
      dec_softmu_truthCAncestor_pt(*muon) =
          lastCAncestor ? lastCAncestor->pt() : 0;
      dec_softmu_truthCAncestor_eta(*muon) =
          lastCAncestor ? lastCAncestor->eta() : 0;
      dec_softmu_truthCAncestor_phi(*muon) =
          lastCAncestor ? lastCAncestor->phi() : 0;
      dec_softmu_truthCAncestor_e(*muon) =
          lastCAncestor ? lastCAncestor->e() : 0;
      dec_softmu_truthTauAncestor_pdgId(*muon) =
          lastTauAncestor ? lastTauAncestor->pdgId() : 0;
      dec_softmu_truthFirstBAncestor_pdgId(*muon) =
          firstBAncestor ? firstBAncestor->pdgId() : 0;
      dec_softmu_truthFirstCAncestor_pdgId(*muon) =
          firstCAncestor ? firstCAncestor->pdgId() : 0;
    }
  }

  return StatusCode::SUCCESS;
}

}  // namespace CP
