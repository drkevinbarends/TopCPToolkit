#include "PartonHistory/CalcTtgammaPartonHistory.h"
#include "PartonHistory/PartonHistoryUtils.h"

namespace top {
  CalcTtgammaPartonHistory::CalcTtgammaPartonHistory(const std::string& name,
					     const std::vector<std::string>& truthCollections) :
    CalcPartonHistory(name, truthCollections)
    {
      initializeDecorators();
    }

  void CalcTtgammaPartonHistory::initializeDecorators() {
    m_t_dec.initializeFloatDecorator({"MC_t_beforeFSR_m", "MC_t_beforeFSR_pt", "MC_t_beforeFSR_eta", "MC_t_beforeFSR_phi"});
    m_t_dec.initializeFloatDecorator({"MC_t_afterFSR_m", "MC_t_afterFSR_pt", "MC_t_afterFSR_eta", "MC_t_afterFSR_phi"});
    m_t_dec.initializeIntDecorator("MC_t_pdgId");
    m_t_dec.initializeFloatDecorator({"MC_b_from_t_m", "MC_b_from_t_pt", "MC_b_from_t_eta", "MC_b_from_t_phi"});
    m_t_dec.initializeFloatDecorator({"MC_W_from_t_m", "MC_W_from_t_pt", "MC_W_from_t_eta", "MC_W_from_t_phi"});
    m_t_dec.initializeFloatDecorator({"MC_Wdecay1_from_t_m", "MC_Wdecay1_from_t_pt", "MC_Wdecay1_from_t_eta", "MC_Wdecay1_from_t_phi"});
    m_t_dec.initializeIntDecorator("MC_Wdecay1_from_t_pdgId");
    m_t_dec.initializeFloatDecorator({"MC_Wdecay2_from_t_m", "MC_Wdecay2_from_t_pt", "MC_Wdecay2_from_t_eta", "MC_Wdecay2_from_t_phi"});
    m_t_dec.initializeIntDecorator("MC_Wdecay2_from_t_pdgId");

    m_tbar_dec.initializeFloatDecorator({"MC_tbar_beforeFSR_m", "MC_tbar_beforeFSR_pt", "MC_tbar_beforeFSR_eta", "MC_tbar_beforeFSR_phi"});
    m_tbar_dec.initializeFloatDecorator({"MC_tbar_afterFSR_m", "MC_tbar_afterFSR_pt", "MC_tbar_afterFSR_eta", "MC_tbar_afterFSR_phi"});
    m_tbar_dec.initializeIntDecorator("MC_tbar_pdgId");
    m_tbar_dec.initializeFloatDecorator({"MC_b_from_tbar_m", "MC_b_from_tbar_pt", "MC_b_from_tbar_eta", "MC_b_from_tbar_phi"});
    m_tbar_dec.initializeFloatDecorator({"MC_W_from_tbar_m", "MC_W_from_tbar_pt", "MC_W_from_tbar_eta", "MC_W_from_tbar_phi"});
    m_tbar_dec.initializeFloatDecorator({"MC_Wdecay1_from_tbar_m", "MC_Wdecay1_from_tbar_pt", "MC_Wdecay1_from_tbar_eta", "MC_Wdecay1_from_tbar_phi"});
    m_tbar_dec.initializeIntDecorator("MC_Wdecay1_from_tbar_pdgId");
    m_tbar_dec.initializeFloatDecorator({"MC_Wdecay2_from_tbar_m", "MC_Wdecay2_from_tbar_pt", "MC_Wdecay2_from_tbar_eta", "MC_Wdecay2_from_tbar_phi"});
    m_tbar_dec.initializeIntDecorator("MC_Wdecay2_from_tbar_pdgId");

    m_ttbar_dec.initializeFloatDecorator({"MC_ttbar_beforeFSR_m", "MC_ttbar_beforeFSR_pt", "MC_ttbar_beforeFSR_eta", "MC_ttbar_beforeFSR_phi"});
    m_ttbar_dec.initializeFloatDecorator({"MC_ttbar_afterFSR_m", "MC_ttbar_afterFSR_pt", "MC_ttbar_afterFSR_eta", "MC_ttbar_afterFSR_phi"});
    m_ttbar_dec.initializeFloatDecorator({"MC_ttbar_fromDecay_beforeFSR_m", "MC_ttbar_fromDecay_beforeFSR_pt", "MC_ttbar_fromDecay_beforeFSR_eta", "MC_ttbar_fromDecay_beforeFSR_phi"});
    m_ttbar_dec.initializeFloatDecorator({"MC_ttbar_fromDecay_afterFSR_m", "MC_ttbar_fromDecay_afterFSR_pt", "MC_ttbar_fromDecay_afterFSR_eta", "MC_ttbar_fromDecay_afterFSR_phi"});

    m_gamma_dec.initializeFloatDecorator({"MC_gamma_m", "MC_gamma_pt", "MC_gamma_eta", "MC_gamma_phi"});
    m_gamma_dec.initializeIntDecorator("MC_gamma_origin");
  }

  StatusCode CalcTtgammaPartonHistory::runHistorySaver(const xAOD::TruthParticleContainer* truthParticles,
                                                 xAOD::PartonHistory* ttgammaPartonHistory) {
    // Tracing truth particles
    CalcPartonHistory::TraceParticles(truthParticles);

    // Ensuring the necessary keys exist
    CalcPartonHistory::EnsureTtbarKeysExist();

    // Fill top parton history
    CalcPartonHistory::FillTopPartonHistory(ttgammaPartonHistory, 0, m_t_dec); // t mode
    // Fill anti top parton history
    CalcPartonHistory::FillTopPartonHistory(ttgammaPartonHistory, 1, m_tbar_dec); // tbar mode
    // Fill ttbar parton history
    CalcPartonHistory::FillTtbarPartonHistory(ttgammaPartonHistory, m_ttbar_dec);
    // Fill photon parton history
    CalcPartonHistory::FillGammaPartonHistory(ttgammaPartonHistory, "", m_gamma_dec);

    return StatusCode::SUCCESS;
  }

} // namespace top
