#include "PartonHistory/CalcPartonHistory.h"
#include "PartonHistory/PartonHistoryUtils.h"
#include "VectorHelpers/DecoratorHelpers.h"

namespace top {
  using ROOT::Math::PtEtaPhiMVector;

  void CalcPartonHistory::FillZPartonHistory(xAOD::PartonHistory* PartonHistory, const std::string& parent, PartonDecorator& dec) {
    std::string parentstring = parent.empty() ? "" : "_from_" + parent;
    std::string basePrefix = parent.empty() ? "" : parent + "_";
    std::string prefix;
    PtEtaPhiMVector Z, Zdecay1, Zdecay2;
    int ZpdgId, Zdecay1pdgId, Zdecay2pdgId;

    dec.decorateDefault("MC_Z" + parentstring, PartonHistory);
    dec.decorateDefault("MC_Zdecay1" + parentstring, PartonHistory);
    dec.decorateDefault("MC_Zdecay2" + parentstring, PartonHistory);
    dec.decorateCustom("MC_Z_isOnshell", 0, PartonHistory);

    bool isonshell = false;
    static const SG::AuxElement::Decorator<int> dec_MC_Z_IsOnShell("MC_Z_IsOnShell");
    prefix = basePrefix + "Z";
    if ( RetrieveParticleInfo(prefix + "_afterFSR", Z, ZpdgId) ) {
      dec.decorateParticle("MC_Z" + parentstring, Z, ZpdgId, PartonHistory);
      isonshell = true;
      if ( RetrieveParticleInfo(prefix + "Decay1", Zdecay1, Zdecay1pdgId) ) {
	dec.decorateParticle("MC_Zdecay1" + parentstring, Zdecay1, Zdecay1pdgId, PartonHistory);
      }
      if ( RetrieveParticleInfo(prefix + "Decay2", Zdecay2, Zdecay2pdgId) ) {
	dec.decorateParticle("MC_Zdecay2" + parentstring, Zdecay2, Zdecay2pdgId, PartonHistory);
      }
    }
    else {
      bool has_Zdecay1 = (RetrieveParticleInfo("MC_l", Zdecay1, Zdecay1pdgId) ||
			  RetrieveParticleInfo("MC_q", Zdecay1, Zdecay1pdgId) ||
			  RetrieveParticleInfo("MC_nu", Zdecay1, Zdecay1pdgId));
      bool has_Zdecay2 = (RetrieveParticleInfo("MC_lbar", Zdecay2, Zdecay2pdgId) ||
			  RetrieveParticleInfo("MC_qbar", Zdecay2, Zdecay2pdgId) ||
			  RetrieveParticleInfo("MC_nubar", Zdecay2, Zdecay2pdgId));
      if (has_Zdecay1 && has_Zdecay2) {
	// If we find these two particles, we construct a Z from them
	Z = Zdecay1 + Zdecay2;
	dec.decorateParticle("MC_Z" + parentstring, Z, ZpdgId, PartonHistory);
	dec.decorateParticle("MC_Zdecay1" + parentstring, Zdecay1, Zdecay1pdgId, PartonHistory);
	dec.decorateParticle("MC_Zdecay2" + parentstring, Zdecay2, Zdecay2pdgId, PartonHistory);
      }
    }
    int onshell = (int)isonshell;
    dec.decorateCustom("MC_Z_isOnshell", onshell, PartonHistory);
  }
}
