#include "PartonHistory/CalcPartonHistory.h"
#include "PartonHistory/PartonHistoryUtils.h"
#include "VectorHelpers/DecoratorHelpers.h"

namespace top {
using ROOT::Math::PtEtaPhiMVector;

void CalcPartonHistory::FillZPartonHistory(xAOD::PartonHistory* PartonHistory,
                                           const std::string& parent,
                                           PartonDecorator& dec) {
  std::string parentstring = parent.empty() ? "" : "_from_" + parent;
  std::string prefix = "MC_" + (parent.empty() ? "Z" : parent + "_Z");
  PtEtaPhiMVector Z, Zdecay1, Zdecay2;
  int ZpdgId, Zdecay1pdgId, Zdecay2pdgId;

  dec.decorateDefault("MC_Z_beforeFSR" + parentstring, PartonHistory);
  dec.decorateDefault("MC_Z_afterFSR" + parentstring, PartonHistory);
  dec.decorateDefault("MC_Zdecay1_beforeFSR" + parentstring, PartonHistory);
  dec.decorateDefault("MC_Zdecay1_afterFSR" + parentstring, PartonHistory);
  dec.decorateDefault("MC_Zdecay2_beforeFSR" + parentstring, PartonHistory);
  dec.decorateDefault("MC_Zdecay2_afterFSR" + parentstring, PartonHistory);
  dec.decorateCustom("MC_Z_IsOnShell", 0, PartonHistory);

  bool isonshell = false;
  if (RetrieveParticleInfo(prefix + "_beforeFSR", Z, ZpdgId)) {
    dec.decorateParticle("MC_Z_beforeFSR" + parentstring, Z, ZpdgId,
                         PartonHistory);
    isonshell = true;
    if (RetrieveParticleInfo(prefix + "Decay1_beforeFSR", Zdecay1,
                             Zdecay1pdgId)) {
      dec.decorateParticle("MC_Zdecay1_beforeFSR" + parentstring, Zdecay1,
                           Zdecay1pdgId, PartonHistory);
    }
    if (RetrieveParticleInfo(prefix + "Decay2_beforeFSR", Zdecay2,
                             Zdecay2pdgId)) {
      dec.decorateParticle("MC_Zdecay2_beforeFSR" + parentstring, Zdecay2,
                           Zdecay2pdgId, PartonHistory);
    }
  }

  if (RetrieveParticleInfo(prefix + "_afterFSR", Z, ZpdgId)) {
    dec.decorateParticle("MC_Z_afterFSR" + parentstring, Z, ZpdgId,
                         PartonHistory);
    isonshell = true;
    if (RetrieveParticleInfo(prefix + "Decay1_afterFSR", Zdecay1,
                             Zdecay1pdgId)) {
      dec.decorateParticle("MC_Zdecay1_afterFSR" + parentstring, Zdecay1,
                           Zdecay1pdgId, PartonHistory);
    }
    if (RetrieveParticleInfo(prefix + "Decay2_afterFSR", Zdecay2,
                             Zdecay2pdgId)) {
      dec.decorateParticle("MC_Zdecay2_afterFSR" + parentstring, Zdecay2,
                           Zdecay2pdgId, PartonHistory);
    }
  } else {
    bool has_Zdecay1_beforeFSR =
        (RetrieveParticleInfo("MC_l_beforeFSR", Zdecay1, Zdecay1pdgId));
    bool has_Zdecay2_beforeFSR =
        (RetrieveParticleInfo("MC_lbar_beforeFSR", Zdecay2, Zdecay2pdgId));
    if (has_Zdecay1_beforeFSR && has_Zdecay2_beforeFSR) {
      // If we find these two particles, we construct a Z from them
      Z = Zdecay1 + Zdecay2;
      // Note that we don't differ here between before and after FSR
      dec.decorateParticle("MC_Z_beforeFSR" + parentstring, Z, ZpdgId,
                           PartonHistory);
      dec.decorateParticle("MC_Z_afterFSR" + parentstring, Z, ZpdgId,
                           PartonHistory);
    }
    bool has_Zdecay1_afterFSR =
        (RetrieveParticleInfo("MC_l_afterFSR", Zdecay1, Zdecay1pdgId));
    bool has_Zdecay2_afterFSR =
        (RetrieveParticleInfo("MC_lbar_afterFSR", Zdecay2, Zdecay2pdgId));
    if (has_Zdecay1_afterFSR) {
      dec.decorateParticle("MC_Zdecay1_afterFSR" + parentstring, Zdecay1,
                           Zdecay1pdgId, PartonHistory);
    }
    if (has_Zdecay2_afterFSR) {
      dec.decorateParticle("MC_Zdecay2_afterFSR" + parentstring, Zdecay2,
                           Zdecay2pdgId, PartonHistory);
    }
  }
  int onshell = (int)isonshell;
  dec.decorateCustom("MC_Z_IsOnShell", onshell, PartonHistory);
}
}  // namespace top
