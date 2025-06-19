#include "PartonHistory/CalcFourTopPartonHistory.h"

#include "PartonHistory/PartonHistoryUtils.h"

namespace top {
CalcFourTopPartonHistory::CalcFourTopPartonHistory(
    const std::string& name, const std::vector<std::string>& truthCollections)
    : CalcPartonHistory(name, truthCollections) {
  initializeDecorators();
}

void CalcFourTopPartonHistory::initializeDecorators() {
  CalcPartonHistory::Initialize4TopDecorators();
}

StatusCode CalcFourTopPartonHistory::runHistorySaver(
    const xAOD::TruthParticleContainer* truthParticles,
    xAOD::PartonHistory* fourtopPartonHistory) {
  // Tracing truth particles
  CalcPartonHistory::TraceParticles(truthParticles);

  // Fill top 1 parton history
  CalcPartonHistory::FillGenericPartonHistory(
      fourtopPartonHistory, "MC_t_beforeFSR", "MC_t1_beforeFSR", 0, m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      fourtopPartonHistory, "MC_t_b_beforeFSR", "MC_b_beforeFSR_from_t1", 0,
      m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      fourtopPartonHistory, "MC_t_Wp_beforeFSR", "MC_W_beforeFSR_from_t1", 0,
      m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      fourtopPartonHistory, "MC_t_WpDecay1_beforeFSR",
      "MC_Wdecay1_beforeFSR_from_t1", 0, m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      fourtopPartonHistory, "MC_t_WpDecay2_beforeFSR",
      "MC_Wdecay2_beforeFSR_from_t1", 0, m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      fourtopPartonHistory, "MC_t_afterFSR", "MC_t1_afterFSR", 0, m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      fourtopPartonHistory, "MC_t_b_afterFSR", "MC_b_afterFSR_from_t1", 0,
      m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      fourtopPartonHistory, "MC_t_Wp_afterFSR", "MC_W_afterFSR_from_t1", 0,
      m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      fourtopPartonHistory, "MC_t_WpDecay1_afterFSR",
      "MC_Wdecay1_afterFSR_from_t1", 0, m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      fourtopPartonHistory, "MC_t_WpDecay2_afterFSR",
      "MC_Wdecay2_afterFSR_from_t1", 0, m_dec);
  // Fill top 2 parton history
  CalcPartonHistory::FillGenericPartonHistory(
      fourtopPartonHistory, "MC_t_beforeFSR", "MC_t2_beforeFSR", 1, m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      fourtopPartonHistory, "MC_t_b_beforeFSR", "MC_b_beforeFSR_from_t2", 1,
      m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      fourtopPartonHistory, "MC_t_Wp_beforeFSR", "MC_W_beforeFSR_from_t2", 1,
      m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      fourtopPartonHistory, "MC_t_WpDecay1_beforeFSR",
      "MC_Wdecay1_beforeFSR_from_t2", 1, m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      fourtopPartonHistory, "MC_t_WpDecay2_beforeFSR",
      "MC_Wdecay2_beforeFSR_from_t2", 1, m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      fourtopPartonHistory, "MC_t_afterFSR", "MC_t2_afterFSR", 1, m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      fourtopPartonHistory, "MC_t_b_afterFSR", "MC_b_afterFSR_from_t2", 1,
      m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      fourtopPartonHistory, "MC_t_Wp_afterFSR", "MC_W_afterFSR_from_t2", 1,
      m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      fourtopPartonHistory, "MC_t_WpDecay1_afterFSR",
      "MC_Wdecay1_afterFSR_from_t2", 1, m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      fourtopPartonHistory, "MC_t_WpDecay2_afterFSR",
      "MC_Wdecay2_afterFSR_from_t2", 1, m_dec);
  // Fill anti top 1 parton history
  CalcPartonHistory::FillGenericPartonHistory(fourtopPartonHistory,
                                              "MC_tbar_beforeFSR",
                                              "MC_tbar1_beforeFSR", 0, m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      fourtopPartonHistory, "MC_tbar_bbar_beforeFSR",
      "MC_bbar_beforeFSR_from_tbar1", 0, m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      fourtopPartonHistory, "MC_tbar_Wm_beforeFSR", "MC_W_beforeFSR_from_tbar1",
      0, m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      fourtopPartonHistory, "MC_tbar_WmDecay1_beforeFSR",
      "MC_Wdecay1_beforeFSR_from_tbar1", 0, m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      fourtopPartonHistory, "MC_tbar_WmDecay2_beforeFSR",
      "MC_Wdecay2_beforeFSR_from_tbar1", 0, m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      fourtopPartonHistory, "MC_tbar_afterFSR", "MC_tbar1_afterFSR", 0, m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      fourtopPartonHistory, "MC_tbar_bbar_afterFSR",
      "MC_bbar_afterFSR_from_tbar1", 0, m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      fourtopPartonHistory, "MC_tbar_Wm_afterFSR", "MC_W_afterFSR_from_tbar1",
      0, m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      fourtopPartonHistory, "MC_tbar_WmDecay1_afterFSR",
      "MC_Wdecay1_afterFSR_from_tbar1", 0, m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      fourtopPartonHistory, "MC_tbar_WmDecay2_afterFSR",
      "MC_Wdecay2_afterFSR_from_tbar1", 0, m_dec);
  // Fill anti top 2 parton history
  CalcPartonHistory::FillGenericPartonHistory(fourtopPartonHistory,
                                              "MC_tbar_beforeFSR",
                                              "MC_tbar2_beforeFSR", 1, m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      fourtopPartonHistory, "MC_tbar_bbar_beforeFSR",
      "MC_bbar_beforeFSR_from_tbar2", 1, m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      fourtopPartonHistory, "MC_tbar_Wm_beforeFSR", "MC_W_beforeFSR_from_tbar2",
      1, m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      fourtopPartonHistory, "MC_tbar_WmDecay1_beforeFSR",
      "MC_Wdecay1_beforeFSR_from_tbar2", 1, m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      fourtopPartonHistory, "MC_tbar_WmDecay2_beforeFSR",
      "MC_Wdecay2_beforeFSR_from_tbar2", 1, m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      fourtopPartonHistory, "MC_tbar_afterFSR", "MC_tbar2_afterFSR", 1, m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      fourtopPartonHistory, "MC_tbar_bbar_afterFSR",
      "MC_bbar_afterFSR_from_tbar2", 1, m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      fourtopPartonHistory, "MC_tbar_Wm_afterFSR", "MC_W_afterFSR_from_tbar2",
      1, m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      fourtopPartonHistory, "MC_tbar_WmDecay1_afterFSR",
      "MC_Wdecay1_afterFSR_from_tbar2", 1, m_dec);
  CalcPartonHistory::FillGenericPartonHistory(
      fourtopPartonHistory, "MC_tbar_WmDecay2_afterFSR",
      "MC_Wdecay2_afterFSR_from_tbar2", 1, m_dec);

  return StatusCode::SUCCESS;
}

}  // namespace top
