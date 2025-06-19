#include "PartonHistory/CalcPartonHistory.h"
#include "PartonHistory/PartonHistoryUtils.h"
#include "VectorHelpers/DecoratorHelpers.h"

namespace top {
using ROOT::Math::PtEtaPhiMVector;

void CalcPartonHistory::FillTtbarPartonHistory(
    xAOD::PartonHistory* PartonHistory, PartonDecorator& dec) {
  // assuming t and tbar partonhistory ran already
  PtEtaPhiMVector ttbar;

  PtEtaPhiMVector t_beforeFSR;
  PtEtaPhiMVector tbar_beforeFSR;
  PtEtaPhiMVector t_afterFSR;
  PtEtaPhiMVector tbar_afterFSR;

  PtEtaPhiMVector WpDecay1, WpDecay2;
  PtEtaPhiMVector WmDecay1, WmDecay2;

  PtEtaPhiMVector b, bbar;

  // Decorating with defaults in case the particle doesn't exist
  dec.decorateDefaultNoPdgId("MC_ttbar_beforeFSR", PartonHistory);
  dec.decorateDefaultNoPdgId("MC_ttbar_afterFSR", PartonHistory);
  dec.decorateDefaultNoPdgId("MC_ttbar_fromDecay_beforeFSR", PartonHistory);
  dec.decorateDefaultNoPdgId("MC_ttbar_fromDecay_afterFSR", PartonHistory);

  if (Retrievep4("MC_t_beforeFSR", t_beforeFSR) &&
      Retrievep4("MC_tbar_beforeFSR", tbar_beforeFSR)) {
    ttbar = t_beforeFSR + tbar_beforeFSR;
    dec.decorateParticle("MC_ttbar_beforeFSR", ttbar, PartonHistory);
  }

  if (Retrievep4("MC_t_afterFSR", t_afterFSR) &&
      Retrievep4("MC_tbar_afterFSR", tbar_afterFSR)) {
    ttbar = t_afterFSR + tbar_afterFSR;
    dec.decorateParticle("MC_ttbar_afterFSR", ttbar, PartonHistory);
  }

  if (Retrievep4("MC_t_WpDecay1_beforeFSR", WpDecay1) &&
      Retrievep4("MC_t_WpDecay2_beforeFSR", WpDecay2) &&
      Retrievep4("MC_tbar_WmDecay1_beforeFSR", WmDecay1) &&
      Retrievep4("MC_tbar_WmDecay2_beforeFSR", WmDecay2) &&
      Retrievep4("MC_t_b_beforeFSR", b) &&
      Retrievep4("MC_tbar_bbar_beforeFSR", bbar)) {
    ttbar = WpDecay1 + WpDecay2 + WmDecay1 + WmDecay2 + b + bbar;
    // Since this is ttbar we don't set a pdgId
    dec.decorateParticle("MC_ttbar_fromDecay_beforeFSR", ttbar, PartonHistory);
  }

  if (Retrievep4("MC_t_WpDecay1_afterFSR", WpDecay1) &&
      Retrievep4("MC_t_WpDecay2_afterFSR", WpDecay2) &&
      Retrievep4("MC_tbar_WmDecay1_afterFSR", WmDecay1) &&
      Retrievep4("MC_tbar_WmDecay2_afterFSR", WmDecay2) &&
      Retrievep4("MC_t_b_afterFSR", b) &&
      Retrievep4("MC_tbar_bbar_afterFSR", bbar)) {
    ttbar = WpDecay1 + WpDecay2 + WmDecay1 + WmDecay2 + b + bbar;
    // Since this is ttbar we don't set a pdgId
    dec.decorateParticle("MC_ttbar_fromDecay_afterFSR", ttbar, PartonHistory);
  }
}
}  // namespace top
