#include "PartonHistory/CalcPartonHistory.h"
#include "PartonHistory/PartonHistoryUtils.h"
#include "VectorHelpers/DecoratorHelpers.h"

namespace top {
  using ROOT::Math::PtEtaPhiMVector;

  void CalcPartonHistory::FillTopPartonHistory(xAOD::PartonHistory* PartonHistory, const int& mode, PartonDecorator& dec) {
    std::string prefix, alt_prefix;
    PtEtaPhiMVector v;
    int pdgId = 0;

    // Filling the parton history for a top quark.

    alt_prefix = "MC_tbar";
    if (mode == 0 || mode == 2) { // 0 == t, 2 == flavour agnostic
      prefix = "MC_t";
      dec.decorateDefault("MC_t_beforeFSR", PartonHistory);
      dec.decorateDefault("MC_t_afterFSR", PartonHistory);
      // Fill the W+ parton history and assign the "t" as parent
      FillWPartonHistory(PartonHistory, "t", mode, dec);
      // Fill the b parton history and assign the "t" as parent
      FillBottomPartonHistory(PartonHistory, "t", mode, dec);
    }
    if (mode == 1 ) { // 1 == W-
      prefix = "MC_tbar";
      dec.decorateDefault("MC_tbar_beforeFSR", PartonHistory);
      dec.decorateDefault("MC_tbar_afterFSR", PartonHistory);
      // Fill the W- parton history and assign the "tbar" as parent
      FillWPartonHistory(PartonHistory, "tbar", mode, dec);
      // Fill the bbar parton history and assign the "tbar" as parent
      FillBottomPartonHistory(PartonHistory, "tbar", mode, dec);
    }

    // Note, the order of evaluation matters here. The prefix depends on the mode.
    // The flavour agnostic part is only triggered if mode == 2 AND the first retrieve function returns false
    if ( RetrieveParticleInfo(prefix + "_beforeFSR", v, pdgId) ||
	 (mode == 2 && RetrieveParticleInfo(prefix + "_beforeFSR", alt_prefix + "_beforeFSR", v, pdgId))) {
      dec.decorateParticle(prefix + "_beforeFSR", v, pdgId, PartonHistory);
    }
    if ( RetrieveParticleInfo(prefix + "_afterFSR", v, pdgId) ||
	 (mode == 2 && RetrieveParticleInfo(prefix + "_afterFSR", alt_prefix + "_afterFSR", v, pdgId))) {
      dec.decorateParticle(prefix + "_afterFSR", v, pdgId, PartonHistory);
    }
  }

  void CalcPartonHistory::FillTtbarPartonHistory(xAOD::PartonHistory* PartonHistory, PartonDecorator& dec) {
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

    if ( Retrievep4("MC_t_beforeFSR", t_beforeFSR) && Retrievep4("MC_tbar_beforeFSR", tbar_beforeFSR)) {
      ttbar = t_beforeFSR + tbar_beforeFSR;
      dec.decorateParticle("MC_ttbar_beforeFSR", ttbar, PartonHistory);
    }

    if ( Retrievep4("MC_t_afterFSR", t_afterFSR) && Retrievep4("MC_tbar_afterFSR", tbar_afterFSR)) {
      ttbar = t_afterFSR + tbar_afterFSR;
      dec.decorateParticle("MC_ttbar_afterFSR", ttbar, PartonHistory);
    }

    if (Retrievep4("MC_t_WpDecay1", WpDecay1)
	&& Retrievep4("MC_t_WpDecay2", WpDecay2)
	&& Retrievep4("MC_t_WmDecay1", WmDecay1)
	&& Retrievep4("MC_t_WmDecay2", WmDecay2)
	&& Retrievep4("MC_t_b_beforeFSR", b)
	&& Retrievep4("MC_tbar_bbar_beforeFSR", bbar)) {
      ttbar = WpDecay1 + WpDecay2 + WmDecay1 + WmDecay2 + b + bbar;
      // Since this is ttbar we don't set a pdgId
      dec.decorateParticle("MC_ttbar_fromDecay_beforeFSR", ttbar, PartonHistory);
    }

    if (Retrievep4("MC_t_WpDecay1", WpDecay1)
	&& Retrievep4("MC_t_WpDecay2", WpDecay2)
	&& Retrievep4("MC_t_WmDecay1", WmDecay1)
	&& Retrievep4("MC_t_WmDecay2", WmDecay2)
	&& Retrievep4("MC_t_b_afterFSR", b)
	&& Retrievep4("MC_tbar_bbar_afterFSR", bbar)) {
      ttbar = WpDecay1 + WpDecay2 + WmDecay1 + WmDecay2 + b + bbar;
      // Since this is ttbar we don't set a pdgId
      dec.decorateParticle("MC_ttbar_fromDecay_afterFSR", ttbar, PartonHistory);
    }
  }
} // namespace top
