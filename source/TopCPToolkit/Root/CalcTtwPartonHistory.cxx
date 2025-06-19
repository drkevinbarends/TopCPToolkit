#include "PartonHistory/CalcTtwPartonHistory.h"

#include "PartonHistory/CalcTtbarPartonHistory.h"
#include "PartonHistory/PartonHistoryUtils.h"

namespace top {
CalcTtwPartonHistory::CalcTtwPartonHistory(
    const std::string& name, const std::vector<std::string>& truthCollections)
    : CalcTtbarPartonHistory(name, truthCollections) {
  initializeDecorators();
}

void CalcTtwPartonHistory::initializeDecorators() {
  CalcPartonHistory::InitializeTopDecorators();
  CalcPartonHistory::InitializeAntiTopDecorators();
  CalcPartonHistory::InitializeTtbarDecorators();
  CalcPartonHistory::InitializeWDecorators();
}

StatusCode CalcTtwPartonHistory::runHistorySaver(
    const xAOD::TruthParticleContainer* truthParticles,
    xAOD::PartonHistory* ttwPartonHistory) {
  // Tracing truth particles
  CalcPartonHistory::TraceParticles(truthParticles);

  // standard ttbar parton history from parent class
  ANA_CHECK(CalcTtbarPartonHistory::runHistorySaver(truthParticles,
                                                    ttwPartonHistory));
  // Fill W parton history
  CalcPartonHistory::FillGenericPartonHistory(ttwPartonHistory, "MC_Wm",
                                              "MC_Wp", "MC_W", 0, m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      ttwPartonHistory, "MC_WmDecay1", "MC_WpDecay1", "MC_Wdecay1", 0, m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      ttwPartonHistory, "MC_WmDecay2", "MC_WpDecay2", "MC_Wdecay2", 0, m_dec);

  return StatusCode::SUCCESS;
}

}  // namespace top
