#include "PartonHistory/CalcPartonHistory.h"
#include "PartonHistory/PartonHistoryUtils.h"
#include "VectorHelpers/DecoratorHelpers.h"

namespace top {
using ROOT::Math::PtEtaPhiMVector;

void CalcPartonHistory::FillGenericPartonHistory(
    xAOD::PartonHistory* PartonHistory, const std::string& retrievalstring,
    const std::string& decorationstring, const int idx, PartonDecorator& dec) {
  PtEtaPhiMVector v;
  int pdgId = 0;
  // Decorating with defaults in case the particle doesn't exist
  dec.decorateDefault(decorationstring, PartonHistory);

  if (RetrieveParticleInfo(retrievalstring, v, pdgId, idx)) {
    dec.decorateParticle(decorationstring, v, pdgId, PartonHistory);
  }
}

void CalcPartonHistory::FillGenericPartonHistory(
    xAOD::PartonHistory* PartonHistory, const std::string& retrievalstring_1,
    const std::string& retrievalstring_2, const std::string& decorationstring,
    const int idx, PartonDecorator& dec) {
  PtEtaPhiMVector v;
  int pdgId = 0;
  // Decorating with defaults in case the particle doesn't exist
  dec.decorateDefault(decorationstring, PartonHistory);

  if (RetrieveParticleInfo(retrievalstring_2, v, pdgId, idx) ||
      RetrieveParticleInfo(retrievalstring_2, v, pdgId, idx)) {
    dec.decorateParticle(decorationstring, v, pdgId, PartonHistory);
  }
  if (RetrieveParticleInfo(retrievalstring_1, v, pdgId, idx) ||
      RetrieveParticleInfo(retrievalstring_2, v, pdgId, idx)) {
    dec.decorateParticle(decorationstring, v, pdgId, PartonHistory);
  }
}

void CalcPartonHistory::FillGenericVectorPartonHistory(
    xAOD::PartonHistory* PartonHistory, const std::string& retrievalstring,
    const std::string& decorationstring, PartonDecorator& dec) {
  std::vector<PtEtaPhiMVector> v;
  std::vector<int> pdgId;
  // Decorating with defaults in case the particle doesn't exist
  dec.decorateVectorDefault(decorationstring, PartonHistory);

  if (RetrieveParticleInfo(retrievalstring, v, pdgId)) {
    dec.decorateVectorParticle(decorationstring, v, pdgId, PartonHistory);
  }
}
void CalcPartonHistory::FillGenericVectorPartonHistory(
    xAOD::PartonHistory* PartonHistory, const std::string& retrievalstring_1,
    const std::string& retrievalstring_2, const std::string& decorationstring,
    PartonDecorator& dec) {
  std::vector<PtEtaPhiMVector> v;
  std::vector<int> pdgId;
  // Decorating with defaults in case the particle doesn't exist
  dec.decorateVectorDefault(decorationstring, PartonHistory);

  if (RetrieveParticleInfo(retrievalstring_2, v, pdgId) ||
      RetrieveParticleInfo(retrievalstring_2, v, pdgId)) {
    dec.decorateVectorParticle(decorationstring, v, pdgId, PartonHistory);
  }
  if (RetrieveParticleInfo(retrievalstring_1, v, pdgId) ||
      RetrieveParticleInfo(retrievalstring_2, v, pdgId)) {
    dec.decorateVectorParticle(decorationstring, v, pdgId, PartonHistory);
  }
}
}  // namespace top
