#include "PartonHistory/CalcTqgammaPartonHistory.h"

#include "PartonHistory/PartonHistoryUtils.h"

namespace top {
CalcTqgammaPartonHistory::CalcTqgammaPartonHistory(
    const std::string& name, const std::vector<std::string>& truthCollections)
    : CalcPartonHistory(name, truthCollections) {
  initializeDecorators();
}

void CalcTqgammaPartonHistory::initializeDecorators() {
  CalcPartonHistory::InitializeTopDecorators();
  CalcPartonHistory::InitializeBottomDecorators();
  CalcPartonHistory::InitializePhotonDecorators();
}

StatusCode CalcTqgammaPartonHistory::runHistorySaver(
    const xAOD::TruthParticleContainer* truthParticles,
    xAOD::PartonHistory* tqgammaPartonHistory) {
  // Tracing truth particles
  CalcPartonHistory::TraceParticles(truthParticles);

  // Fill top parton history (flavour agnostic)
  CalcPartonHistory::FillGenericPartonHistory(
      tqgammaPartonHistory, "MC_t_beforeFSR", "MC_tbar_beforeFSR",
      "MC_t_beforeFSR", 0, m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      tqgammaPartonHistory, "MC_t_b_beforeFSR", "MC_tbar_bbar_beforeFSR",
      "MC_b_beforeFSR_from_t", 0, m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      tqgammaPartonHistory, "MC_t_Wp_beforeFSR", "MC_t_Wm_beforeFSR",
      "MC_W_beforeFSR_from_t", 0, m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      tqgammaPartonHistory, "MC_t_WpDecay1_beforeFSR",
      "MC_tbar_WmDecay1_beforeFSR", "MC_Wdecay1_beforeFSR_from_t", 0, m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      tqgammaPartonHistory, "MC_t_WpDecay2_beforeFSR",
      "MC_tbar_WmDecay2_beforeFSR", "MC_Wdecay2_beforeFSR_from_t", 0, m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      tqgammaPartonHistory, "MC_t_afterFSR", "MC_tbar_afterFSR",
      "MC_t_afterFSR", 0, m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      tqgammaPartonHistory, "MC_t_b_afterFSR", "MC_tbar_bbar_afterFSR",
      "MC_b_afterFSR_from_t", 0, m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      tqgammaPartonHistory, "MC_t_Wp_afterFSR", "MC_t_Wm_afterFSR",
      "MC_W_afterFSR_from_t", 0, m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      tqgammaPartonHistory, "MC_t_WpDecay1_afterFSR",
      "MC_tbar_WmDecay1_afterFSR", "MC_Wdecay1_afterFSR_from_t", 0, m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      tqgammaPartonHistory, "MC_t_WpDecay2_afterFSR",
      "MC_tbar_WmDecay2_afterFSR", "MC_Wdecay2_afterFSR_from_t", 0, m_dec);
  // Fill b (spectator b) parton history (flavour agnostic)
  CalcPartonHistory::FillGenericPartonHistory(
      tqgammaPartonHistory, "MC_b_beforeFSR", "MC_bbar_beforeFSR",
      "MC_b_beforeFSR", 0, m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      tqgammaPartonHistory, "MC_b_afterFSR", "MC_bbar_afterFSR",
      "MC_b_afterFSR", 0, m_dec);
  // Fill Gamma parton history
  CalcPartonHistory::FillGammaPartonHistory(tqgammaPartonHistory, "", m_dec);

  return StatusCode::SUCCESS;
}

}  // namespace top
