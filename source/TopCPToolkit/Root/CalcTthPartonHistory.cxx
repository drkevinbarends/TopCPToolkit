#include "PartonHistory/CalcTthPartonHistory.h"

#include "PartonHistory/CalcTtbarPartonHistory.h"
#include "PartonHistory/PartonHistoryUtils.h"

namespace top {
CalcTthPartonHistory::CalcTthPartonHistory(
    const std::string& name, const std::vector<std::string>& truthCollections)
    : CalcTtbarPartonHistory(name, truthCollections) {
  initializeDecorators();
}

void CalcTthPartonHistory::initializeDecorators() {
  CalcPartonHistory::InitializeTopDecorators();
  CalcPartonHistory::InitializeAntiTopDecorators();
  CalcPartonHistory::InitializeTtbarDecorators();
  CalcPartonHistory::InitializeHiggsDecorators();
}

StatusCode CalcTthPartonHistory::runHistorySaver(
    const xAOD::TruthParticleContainer* truthParticles,
    xAOD::PartonHistory* tthPartonHistory) {
  // Tracing truth particles
  CalcPartonHistory::TraceParticles(truthParticles);

  // standard ttbar parton history from parent class
  ANA_CHECK(CalcTtbarPartonHistory::runHistorySaver(truthParticles,
                                                    tthPartonHistory));
  // Fill Higgs parton history
  CalcPartonHistory::FillHiggsPartonHistory(tthPartonHistory, "resonant",
                                            m_dec);

  return StatusCode::SUCCESS;
}

}  // namespace top
