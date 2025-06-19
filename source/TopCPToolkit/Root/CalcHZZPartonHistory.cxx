#include "PartonHistory/CalcHZZPartonHistory.h"

#include "PartonHistory/PartonHistoryUtils.h"

namespace top {
CalcHZZPartonHistory::CalcHZZPartonHistory(
    const std::string& name, const std::vector<std::string>& truthCollections)
    : CalcPartonHistory(name, truthCollections) {
  initializeDecorators();
}

void CalcHZZPartonHistory::initializeDecorators() {
  CalcPartonHistory::InitializeHiggsDecorators();
}

StatusCode CalcHZZPartonHistory::runHistorySaver(
    const xAOD::TruthParticleContainer* truthParticles,
    xAOD::PartonHistory* hzzPartonHistory) {
  CalcPartonHistory::TraceParticles(truthParticles);
  CalcPartonHistory::FillHiggsPartonHistory(hzzPartonHistory, "resonant",
                                            m_dec);

  return StatusCode::SUCCESS;
}

}  // namespace top
