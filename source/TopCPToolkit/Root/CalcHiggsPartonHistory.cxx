#include "PartonHistory/CalcPartonHistory.h"
#include "PartonHistory/PartonHistoryUtils.h"
#include "VectorHelpers/DecoratorHelpers.h"

namespace top {
  using ROOT::Math::PtEtaPhiMVector;

  void CalcPartonHistory::FillHiggsPartonHistory(xAOD::PartonHistory* PartonHistory, PartonDecorator& dec) {
    PtEtaPhiMVector v;
    int pdgId = 0;
    dec.decorateDefault("MC_H", PartonHistory);
    dec.decorateDefault("MC_Hdecay1", PartonHistory);
    dec.decorateDefault("MC_Hdecay2", PartonHistory);
    dec.decorateDefault("MC_Hdecay1_decay1", PartonHistory);
    dec.decorateDefault("MC_Hdecay1_decay2", PartonHistory);
    dec.decorateDefault("MC_Hdecay2_decay1", PartonHistory);
    dec.decorateDefault("MC_Hdecay2_decay2", PartonHistory);

    if ( RetrieveParticleInfo("MC_H_beforeFSR", v, pdgId)) {
      dec.decorateParticle("MC_H", v, pdgId, PartonHistory);
    }
    if ( RetrieveParticleInfo("MC_HDecay1", v, pdgId)) {
      dec.decorateParticle("MC_Hdecay1", v, pdgId, PartonHistory);
    }
    if ( RetrieveParticleInfo("MC_HDecay2", v, pdgId)) {
      dec.decorateParticle("MC_Hdecay2", v, pdgId, PartonHistory);
    }
    if ( RetrieveParticleInfo("MC_HDecay1_Decay1", v, pdgId)) {
      dec.decorateParticle("MC_Hdecay1_decay1", v, pdgId, PartonHistory);
    }
    if ( RetrieveParticleInfo("MC_HDecay1_Decay2", v, pdgId)) {
      dec.decorateParticle("MC_Hdecay1_decay2", v, pdgId, PartonHistory);
    }
    if ( RetrieveParticleInfo("MC_HDecay2_Decay1", v, pdgId)) {
      dec.decorateParticle("MC_Hdecay2_decay1", v, pdgId, PartonHistory);
    }
    if ( RetrieveParticleInfo("MC_HDecay2_Decay2", v, pdgId)) {
      dec.decorateParticle("MC_Hdecay2_decay2", v, pdgId, PartonHistory);
    }
  }
}
