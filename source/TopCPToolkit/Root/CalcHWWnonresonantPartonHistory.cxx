#include "PartonHistory/CalcHWWnonresonantPartonHistory.h"

#include "PartonHistory/PartonHistoryUtils.h"
#include "xAODTruth/TruthVertex.h"

namespace top {
CalcHWWnonresonantPartonHistory::CalcHWWnonresonantPartonHistory(
    const std::string& name, const std::vector<std::string>& truthCollections)
    : CalcPartonHistory(name, truthCollections) {
  initializeDecorators();
}

void CalcHWWnonresonantPartonHistory::initializeDecorators() {
  CalcPartonHistory::InitializeHiggsDecorators();
}

StatusCode CalcHWWnonresonantPartonHistory::runHistorySaver(
    const xAOD::TruthParticleContainer* truthParticles,
    xAOD::PartonHistory* hwwnonresonantPartonHistory) {
  CalcPartonHistory::TraceParticles(truthParticles);

  CalcPartonHistory::FillHiggsPartonHistory(hwwnonresonantPartonHistory,
                                            "non_resonant_WW", m_dec);

  return StatusCode::SUCCESS;
}

}  // namespace top
