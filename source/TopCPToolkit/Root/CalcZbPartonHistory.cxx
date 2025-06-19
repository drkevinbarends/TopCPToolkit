#include "PartonHistory/CalcZbPartonHistory.h"

#include "PartonHistory/PartonHistoryUtils.h"

namespace top {
CalcZbPartonHistory::CalcZbPartonHistory(
    const std::string& name, const std::vector<std::string>& truthCollections)
    : CalcPartonHistory(name, truthCollections) {
  initializeDecorators();
}

void CalcZbPartonHistory::initializeDecorators() {
  CalcPartonHistory::InitializeBottomDecorators();
  CalcPartonHistory::InitializeAntiBottomDecorators();
  CalcPartonHistory::InitializeZDecorators();
}

StatusCode CalcZbPartonHistory::runHistorySaver(
    const xAOD::TruthParticleContainer* truthParticles,
    xAOD::PartonHistory* ZbPartonHistory) {
  // Tracing truth particles
  CalcPartonHistory::TraceParticles(truthParticles);

  // Fill b (spectator b) parton history (flavour agnostic)
  CalcPartonHistory::FillGenericPartonHistory(ZbPartonHistory, "MC_b_beforeFSR",
                                              "MC_bbar_beforeFSR",
                                              "MC_b_beforeFSR", 2, m_dec);
  CalcPartonHistory::FillGenericPartonHistory(ZbPartonHistory, "MC_b_afterFSR",
                                              "MC_bbar_afterFSR",
                                              "MC_b_afterFSR", 1, m_dec);

  // Fill Z parton history
  CalcPartonHistory::FillZPartonHistory(ZbPartonHistory, "", m_dec);

  return StatusCode::SUCCESS;
}

}  // namespace top
