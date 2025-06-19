#include "PartonHistory/CalcTtzPartonHistory.h"

#include "PartonHistory/CalcTtbarPartonHistory.h"
#include "PartonHistory/PartonHistoryUtils.h"

namespace top {
CalcTtzPartonHistory::CalcTtzPartonHistory(
    const std::string& name, const std::vector<std::string>& truthCollections)
    : CalcTtbarPartonHistory(name, truthCollections) {
  initializeDecorators();
}

void CalcTtzPartonHistory::initializeDecorators() {
  CalcPartonHistory::InitializeTopDecorators();
  CalcPartonHistory::InitializeAntiTopDecorators();
  CalcPartonHistory::InitializeTtbarDecorators();
  CalcPartonHistory::InitializeZDecorators();
}

StatusCode CalcTtzPartonHistory::runHistorySaver(
    const xAOD::TruthParticleContainer* truthParticles,
    xAOD::PartonHistory* ttZPartonHistory) {
  // Tracing truth particles
  CalcPartonHistory::TraceParticles(truthParticles);

  // standard ttbar parton history from parent class
  ANA_CHECK(CalcTtbarPartonHistory::runHistorySaver(truthParticles,
                                                    ttZPartonHistory));
  // Fill Z parton history
  CalcPartonHistory::FillZPartonHistory(ttZPartonHistory, "", m_dec);

  return StatusCode::SUCCESS;
}

}  // namespace top
