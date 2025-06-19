#include "PartonHistory/CalcHWWPartonHistory.h"

#include "PartonHistory/PartonHistoryUtils.h"

namespace top {
CalcHWWPartonHistory::CalcHWWPartonHistory(
    const std::string& name, const std::vector<std::string>& truthCollections)
    : CalcPartonHistory(name, truthCollections) {
  initializeDecorators();
}

void CalcHWWPartonHistory::initializeDecorators() {
  CalcPartonHistory::InitializeHiggsDecorators();
}

StatusCode CalcHWWPartonHistory::runHistorySaver(
    const xAOD::TruthParticleContainer* truthParticles,
    xAOD::PartonHistory* hwwPartonHistory) {
  CalcPartonHistory::TraceParticles(truthParticles);
  CalcPartonHistory::FillHiggsPartonHistory(hwwPartonHistory, "resonant",
                                            m_dec);

  return StatusCode::SUCCESS;
}

}  // namespace top
