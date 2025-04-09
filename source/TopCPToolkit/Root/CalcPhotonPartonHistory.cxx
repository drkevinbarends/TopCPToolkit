#include "PartonHistory/CalcPartonHistory.h"
#include "PartonHistory/PartonHistoryUtils.h"
#include "VectorHelpers/DecoratorHelpers.h"

namespace top {
  using ROOT::Math::PtEtaPhiMVector;
  
  void CalcPartonHistory::FillGammaPartonHistory(xAOD::PartonHistory* PartonHistory,
						 const std::string& parent,
						 PartonDecorator& dec) {
    std::string parentstring = parent.empty() ? "" : "_from_" + parent;
    std::string basePrefix = "MC_" + (parent.empty() ? "" : parent + "_");
    PtEtaPhiMVector gamma;
    int gamma_origin = -1;

    dec.decorateDefault("MC_gamma" + parentstring, PartonHistory);
    dec.decorateCustom("MC_gamma_origin", 0, PartonHistory);
    
    if (Retrievep4Gamma(gamma, gamma_origin)) {
      dec.decorateParticle("MC_gamma" + parentstring, gamma, PartonHistory);
    }
    dec.decorateCustom("MC_gamma_origin", gamma_origin, PartonHistory);
  }
}
