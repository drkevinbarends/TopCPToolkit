#include "PartonHistory/CalcPartonHistory.h"
#include "VectorHelpers/DecoratorHelpers.h"
#include "VectorHelpers/LorentzHelper.h"
#include "xAODTruth/TruthParticleContainer.h"

namespace top {
using ROOT::Math::PtEtaPhiMVector;

void CalcPartonHistory::setHiggs(xAOD::PartonHistory* PartonHistory,
                                 const std::string& fsr, PartonDecorator& dec) {
  PtEtaPhiMVector H;
  PtEtaPhiMVector Wm, Wm_decay1, Wm_decay2;
  PtEtaPhiMVector Wp, Wp_decay1, Wp_decay2;
  int Wm_decay1_pdgId, Wm_decay2_pdgId;
  int Wp_decay1_pdgId, Wp_decay2_pdgId;

  // W+
  bool has_Wp = getW("MC_lbar_" + fsr, "MC_nu_" + fsr, Wp_decay1,
                     Wp_decay1_pdgId, Wp_decay2, Wp_decay2_pdgId);
  if (has_Wp) {
    Wp = Wp_decay1 + Wp_decay2;
    dec.decorateParticle("MC_Hdecay1_" + fsr, Wp, -24, PartonHistory);
    dec.decorateParticle("MC_Hdecay1_decay1_" + fsr, Wp_decay1, Wp_decay1_pdgId,
                         PartonHistory);
    dec.decorateParticle("MC_Hdecay1_decay2_" + fsr, Wp_decay2, Wp_decay2_pdgId,
                         PartonHistory);
  }
  // W-
  bool has_Wm = getW("MC_l_" + fsr, "MC_nubar_" + fsr, Wm_decay1,
                     Wm_decay1_pdgId, Wm_decay2, Wm_decay2_pdgId);
  if (has_Wm) {
    Wm = Wm_decay1 + Wm_decay2;
    dec.decorateParticle("MC_Hdecay2_" + fsr, Wm, 24, PartonHistory);
    dec.decorateParticle("MC_Hdecay2_decay1_" + fsr, Wm_decay1, Wm_decay1_pdgId,
                         PartonHistory);
    dec.decorateParticle("MC_Hdecay2_decay2_" + fsr, Wm_decay2, Wm_decay2_pdgId,
                         PartonHistory);
  }
  if (has_Wm && has_Wp) {
    H = Wp + Wm;
    dec.decorateParticle("MC_H_" + fsr, H, 25, PartonHistory);
  }
}

bool CalcPartonHistory::getW(const std::string& str_lep,
                             const std::string& str_nu, PtEtaPhiMVector& p1,
                             int& pdgId1, PtEtaPhiMVector& p2, int& pdgId2) {
  std::vector<const xAOD::TruthParticle*> W_offshell_decay1_candidates;
  std::vector<const xAOD::TruthParticle*> W_offshell_decay2_candidates;
  bool has_candidates =
      (RetrieveParticleInfo(str_lep, W_offshell_decay1_candidates) &&
       RetrieveParticleInfo(str_nu, W_offshell_decay2_candidates));
  if (has_candidates) {
    for (const auto* particle_1 : W_offshell_decay1_candidates) {
      for (const auto* particle_2 : W_offshell_decay2_candidates) {
        // Condition 1: Opposite charge (pdgId() * pdgId() < 0)
        if ((particle_1->pdgId() * particle_2->pdgId()) > 0) {
          continue;
        }

        // Condition 2: pdgIDs should differ by 1
        if (std::abs(particle_1->absPdgId() - particle_2->absPdgId()) != 1) {
          continue;
        }
        // Condition 3: barcodes should be smaller than 10100. Other particles
        // are from the showering
        if (particle_1->barcode() > 10100 && particle_2->barcode() > 10100) {
          continue;
        }

        p1 = GetPtEtaPhiMfromTruth(particle_1);
        pdgId1 = particle_1->pdgId();
        p2 = GetPtEtaPhiMfromTruth(particle_2);
        pdgId2 = particle_2->pdgId();
        return true;
      }
    }
  }
  return false;
}

void CalcPartonHistory::FillHiggsPartonHistory(
    xAOD::PartonHistory* PartonHistory, const std::string& mode,
    PartonDecorator& dec) {
  PtEtaPhiMVector v;
  int pdgId = 0;
  dec.decorateDefault("MC_H_beforeFSR", PartonHistory);
  dec.decorateDefault("MC_Hdecay1_beforeFSR", PartonHistory);
  dec.decorateDefault("MC_Hdecay2_beforeFSR", PartonHistory);
  dec.decorateDefault("MC_Hdecay1_decay1_beforeFSR", PartonHistory);
  dec.decorateDefault("MC_Hdecay1_decay2_beforeFSR", PartonHistory);
  dec.decorateDefault("MC_Hdecay2_decay1_beforeFSR", PartonHistory);
  dec.decorateDefault("MC_Hdecay2_decay2_beforeFSR", PartonHistory);

  dec.decorateDefault("MC_H_afterFSR", PartonHistory);
  dec.decorateDefault("MC_Hdecay1_afterFSR", PartonHistory);
  dec.decorateDefault("MC_Hdecay2_afterFSR", PartonHistory);
  dec.decorateDefault("MC_Hdecay1_decay1_afterFSR", PartonHistory);
  dec.decorateDefault("MC_Hdecay1_decay2_afterFSR", PartonHistory);
  dec.decorateDefault("MC_Hdecay2_decay1_afterFSR", PartonHistory);
  dec.decorateDefault("MC_Hdecay2_decay2_afterFSR", PartonHistory);

  if (mode == "resonant") {
    if (RetrieveParticleInfo("MC_H_beforeFSR", v, pdgId)) {
      dec.decorateParticle("MC_H_beforeFSR", v, pdgId, PartonHistory);
    }
    if (RetrieveParticleInfo("MC_HDecay1_beforeFSR", v, pdgId)) {
      dec.decorateParticle("MC_Hdecay1_beforeFSR", v, pdgId, PartonHistory);
    }
    if (RetrieveParticleInfo("MC_HDecay2_beforeFSR", v, pdgId)) {
      dec.decorateParticle("MC_Hdecay2_beforeFSR", v, pdgId, PartonHistory);
    }
    if (RetrieveParticleInfo("MC_HDecay1_Decay1_beforeFSR", v, pdgId)) {
      dec.decorateParticle("MC_Hdecay1_decay1_beforeFSR", v, pdgId,
                           PartonHistory);
    }
    if (RetrieveParticleInfo("MC_HDecay1_Decay2_beforeFSR", v, pdgId)) {
      dec.decorateParticle("MC_Hdecay1_decay2_beforeFSR", v, pdgId,
                           PartonHistory);
    }
    if (RetrieveParticleInfo("MC_HDecay2_Decay1_beforeFSR", v, pdgId)) {
      dec.decorateParticle("MC_Hdecay2_decay1_beforeFSR", v, pdgId,
                           PartonHistory);
    }
    if (RetrieveParticleInfo("MC_HDecay2_Decay2_beforeFSR", v, pdgId)) {
      dec.decorateParticle("MC_Hdecay2_decay2_beforeFSR", v, pdgId,
                           PartonHistory);
    }
    if (RetrieveParticleInfo("MC_H_afterFSR", v, pdgId)) {
      dec.decorateParticle("MC_H_afterFSR", v, pdgId, PartonHistory);
    }
    if (RetrieveParticleInfo("MC_HDecay1_afterFSR", v, pdgId)) {
      dec.decorateParticle("MC_Hdecay1_afterFSR", v, pdgId, PartonHistory);
    }
    if (RetrieveParticleInfo("MC_HDecay2_afterFSR", v, pdgId)) {
      dec.decorateParticle("MC_Hdecay2_afterFSR", v, pdgId, PartonHistory);
    }
    if (RetrieveParticleInfo("MC_HDecay1_Decay1_afterFSR", v, pdgId)) {
      dec.decorateParticle("MC_Hdecay1_decay1_afterFSR", v, pdgId,
                           PartonHistory);
    }
    if (RetrieveParticleInfo("MC_HDecay1_Decay2_afterFSR", v, pdgId)) {
      dec.decorateParticle("MC_Hdecay1_decay2_afterFSR", v, pdgId,
                           PartonHistory);
    }
    if (RetrieveParticleInfo("MC_HDecay2_Decay1_afterFSR", v, pdgId)) {
      dec.decorateParticle("MC_Hdecay2_decay1_afterFSR", v, pdgId,
                           PartonHistory);
    }
    if (RetrieveParticleInfo("MC_HDecay2_Decay2_afterFSR", v, pdgId)) {
      dec.decorateParticle("MC_Hdecay2_decay2_afterFSR", v, pdgId,
                           PartonHistory);
    }
  } else if (mode == "non_resonant_WW") {
    setHiggs(PartonHistory, "beforeFSR", dec);
    setHiggs(PartonHistory, "afterFSR", dec);
  }
}
}  // namespace top
