#include "PartonHistory/CalcTqgammaPartonHistory.h"
#include "PartonHistory/PartonHistoryUtils.h"

namespace top {
  CalcTqgammaPartonHistory::CalcTqgammaPartonHistory(const std::string& name,
					     const std::vector<std::string>& truthCollections) :
    CalcPartonHistory(name, truthCollections)
  {
    initializeDecorators();
  }
  
  void CalcTqgammaPartonHistory::initializeDecorators() {
    m_t_dec.initializeFloatDecorator({"MC_t_beforeFSR_m", "MC_t_beforeFSR_pt", "MC_t_beforeFSR_eta", "MC_t_beforeFSR_phi"});
    m_t_dec.initializeFloatDecorator({"MC_t_afterFSR_m", "MC_t_afterFSR_pt", "MC_t_afterFSR_eta", "MC_t_afterFSR_phi"});
    m_t_dec.initializeIntDecorator("MC_t_pdgId");
    m_t_dec.initializeFloatDecorator({"MC_b_from_t_m", "MC_b_from_t_pt", "MC_b_from_t_eta", "MC_b_from_t_phi"});
    m_t_dec.initializeIntDecorator("MC_b_from_t_pdgId");
    m_t_dec.initializeFloatDecorator({"MC_W_from_t_m", "MC_W_from_t_pt", "MC_W_from_t_eta", "MC_W_from_t_phi"});
    m_t_dec.initializeIntDecorator("MC_W_from_t_pdgId");
    m_t_dec.initializeFloatDecorator({"MC_Wdecay1_from_t_m", "MC_Wdecay1_from_t_pt", "MC_Wdecay1_from_t_eta", "MC_Wdecay1_from_t_phi"});
    m_t_dec.initializeIntDecorator("MC_Wdecay1_from_t_pdgId");
    m_t_dec.initializeFloatDecorator({"MC_Wdecay2_from_t_m", "MC_Wdecay2_from_t_pt", "MC_Wdecay2_from_t_eta", "MC_Wdecay2_from_t_phi"});
    m_t_dec.initializeIntDecorator("MC_Wdecay2_from_t_pdgId");

    m_b_dec.initializeFloatDecorator({"MC_b_beforeFSR_m", "MC_b_beforeFSR_pt", "MC_b_beforeFSR_eta", "MC_b_beforeFSR_phi"});
    m_b_dec.initializeFloatDecorator({"MC_b_afterFSR_m", "MC_b_afterFSR_pt", "MC_b_afterFSR_eta", "MC_b_afterFSR_phi"});
    m_b_dec.initializeIntDecorator("MC_b_pdgId");
    
    m_gamma_dec.initializeFloatDecorator({"MC_gamma_m", "MC_gamma_pt", "MC_gamma_eta", "MC_gamma_phi"});
    m_gamma_dec.initializeIntDecorator("MC_gamma_origin");
  }

  StatusCode CalcTqgammaPartonHistory::runHistorySaver(const xAOD::TruthParticleContainer* truthParticles,
						       xAOD::PartonHistory* tqgammaPartonHistory) {
    // Tracing truth particles
    CalcPartonHistory::TraceParticles(truthParticles);

    // Ensuring the necessary keys exist
    CalcPartonHistory::EnsureTtbarKeysExist(); // this will fail on ttbar but that is ok.
    
    // Fill top parton history (flavour agnostic)
    CalcPartonHistory::FillTopPartonHistory(tqgammaPartonHistory, 2, m_t_dec);
    // Fill b (spectator b) parton history (flavour agnostic)
    CalcPartonHistory::FillBottomPartonHistory(tqgammaPartonHistory, "", 2, m_b_dec);
    // Fill Gamma parton history
    CalcPartonHistory::FillGammaPartonHistory(tqgammaPartonHistory, "", m_gamma_dec);

    return StatusCode::SUCCESS;
  }

} // namespace top
