#include "PartonHistory/CalcPartonHistory.h"
#include "PartonHistory/PartonHistoryUtils.h"
#include "VectorHelpers/DecoratorHelpers.h"

namespace top {
  using ROOT::Math::PtEtaPhiMVector;

  void CalcPartonHistory::FillBottomPartonHistory(xAOD::PartonHistory* PartonHistory,
						  const std::string& parent,
						  const int& mode,
						  PartonDecorator& dec) {
    std::string parentstring = parent.empty() ? "" : "_from_" + parent;
    std::string basePrefix = "MC_" + (parent.empty() ? "" : parent + "_");
    std::string prefix, alt_prefix, suffix;
    PtEtaPhiMVector v;
    int pdgId = 0;

    alt_prefix = basePrefix + "bar_bbar";
    if (mode == 0 || mode == 2) { // 0 == b, 2 == flavour agnostic
      // Decorating with defaults in case the particle doesn't exist
      suffix = "b";
      prefix = basePrefix + suffix;
      dec.decorateDefault("MC_" + suffix + parentstring, PartonHistory);
      // Currently the parent can only be a top so there we just need to add "bar"
    }
    if ( mode == 1 ) {
      suffix = "bbar";
      prefix = basePrefix + suffix;
      dec.decorateDefault("MC_" + suffix + parentstring, PartonHistory);
    }

    // Note, the order of evaluation matters here. The prefix depends on the mode.
    // The flavour agnostic part is only triggered if mode == 2 AND the first retrieve function returns false
    if ( RetrieveParticleInfo(prefix + "_beforeFSR", v, pdgId) ||
	 (mode == 2 && RetrieveParticleInfo(prefix + parentstring, alt_prefix + parentstring + "bar", v, pdgId))) {
      dec.decorateParticle("MC_" + suffix  + parentstring, v, pdgId, PartonHistory);
    }
  }
}
