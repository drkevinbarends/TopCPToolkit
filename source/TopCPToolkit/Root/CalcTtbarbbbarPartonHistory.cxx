#include "PartonHistory/CalcTtbarbbbarPartonHistory.h"

#include "PartonHistory/PartonHistoryUtils.h"

namespace top {
CalcTtbarbbbarPartonHistory::CalcTtbarbbbarPartonHistory(
    const std::string& name, const std::vector<std::string>& truthCollections)
    : CalcTtbarPartonHistory(name, truthCollections) {
  initializeDecorators();
}

void CalcTtbarbbbarPartonHistory::initializeDecorators() {
  CalcPartonHistory::InitializeTopDecorators();
  CalcPartonHistory::InitializeAntiTopDecorators();
  CalcPartonHistory::InitializeVectorBottomDecorators();
  CalcPartonHistory::InitializeVectorAntiBottomDecorators();
  CalcPartonHistory::InitializeTtbarDecorators();
}

StatusCode CalcTtbarbbbarPartonHistory::runHistorySaver(
    const xAOD::TruthParticleContainer* truthParticles,
    xAOD::PartonHistory* ttbarbbbarPartonHistory) {
  // Tracing truth particles
  CalcPartonHistory::TraceParticles(truthParticles);

  ANA_CHECK(CalcTtbarPartonHistory::runHistorySaver(truthParticles,
                                                    ttbarbbbarPartonHistory));

  // Fill extra b's
  CalcPartonHistory::FillGenericVectorPartonHistory(
      ttbarbbbarPartonHistory, "MC_b_beforeFSR", "MC_b_beforeFSR", m_dec);
  CalcPartonHistory::FillGenericVectorPartonHistory(
      ttbarbbbarPartonHistory, "MC_bbar_beforeFSR", "MC_bbar_beforeFSR", m_dec);
  CalcPartonHistory::FillGenericVectorPartonHistory(
      ttbarbbbarPartonHistory, "MC_b_afterFSR", "MC_b_afterFSR", m_dec);
  CalcPartonHistory::FillGenericVectorPartonHistory(
      ttbarbbbarPartonHistory, "MC_bbar_afterFSR", "MC_bbar_afterFSR", m_dec);
  return StatusCode::SUCCESS;
}

}  // namespace top
