#include "PartonHistory/CalcTtgammaPartonHistory.h"

#include "PartonHistory/PartonHistoryUtils.h"

namespace top {
CalcTtgammaPartonHistory::CalcTtgammaPartonHistory(
    const std::string& name, const std::vector<std::string>& truthCollections)
    : CalcTtbarPartonHistory(name, truthCollections) {
  initializeDecorators();
}

void CalcTtgammaPartonHistory::initializeDecorators() {
  CalcPartonHistory::InitializeTopDecorators();
  CalcPartonHistory::InitializeAntiTopDecorators();
  CalcPartonHistory::InitializeTtbarDecorators();
  CalcPartonHistory::InitializePhotonDecorators();
}

StatusCode CalcTtgammaPartonHistory::runHistorySaver(
    const xAOD::TruthParticleContainer* truthParticles,
    xAOD::PartonHistory* ttgammaPartonHistory) {
  // Tracing truth particles
  CalcPartonHistory::TraceParticles(truthParticles);

  // standard ttbar parton history from parent class
  ANA_CHECK(CalcTtbarPartonHistory::runHistorySaver(truthParticles,
                                                    ttgammaPartonHistory));
  // Fill photon parton history
  CalcPartonHistory::FillGammaPartonHistory(ttgammaPartonHistory, "", m_dec);

  return StatusCode::SUCCESS;
}

}  // namespace top
