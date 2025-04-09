#include "PartonHistory/CalcTzqPartonHistory.h"
#include "PartonHistory/PartonHistoryUtils.h"

namespace top {
  CalcTzqPartonHistory::CalcTzqPartonHistory(const std::string& name,
					     const std::vector<std::string>& truthCollections) :
    CalcPartonHistory(name, truthCollections)
  {
    initializeDecorators();
  }

  void CalcTzqPartonHistory::initializeDecorators() {
    m_t_dec.initializeFloatDecorator({"MC_t_beforeFSR_m", "MC_t_beforeFSR_pt", "MC_t_beforeFSR_eta", "MC_t_beforeFSR_phi"});
    m_t_dec.initializeIntDecorator("MC_t_pdgId");
    m_t_dec.initializeFloatDecorator({"MC_t_afterFSR_m", "MC_t_afterFSR_pt", "MC_t_afterFSR_eta", "MC_t_afterFSR_phi"});
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

    m_Z_dec.initializeIntDecorator("MC_Z_pdgId");
    m_Z_dec.initializeFloatDecorator({"MC_Zdecay1_m", "MC_Zdecay1_pt", "MC_Zdecay1_eta", "MC_Zdecay1_phi"});
    m_Z_dec.initializeIntDecorator("MC_Zdecay1_pdgId");
    m_Z_dec.initializeFloatDecorator({"MC_Zdecay2_m", "MC_Zdecay2_pt", "MC_Zdecay2_eta", "MC_Zdecay2_phi"});
    m_Z_dec.initializeIntDecorator("MC_Zdecay2_pdgId");
    m_Z_dec.initializeIntDecorator("MC_Z_isOnshell");
  }

  StatusCode CalcTzqPartonHistory::runHistorySaver(const xAOD::TruthParticleContainer* truthParticles,
                                                 xAOD::PartonHistory* tzqPartonHistory) {
    // Tracing truth particles
    CalcPartonHistory::TraceParticles(truthParticles);

    // Ensuring the necessary keys exist
    CalcPartonHistory::EnsureTtbarKeysExist(); // this will fail on ttbar but that is ok.
    CalcPartonHistory::EnsureKeyExists("MC_Z_afterFSR", "MC_Z");
    CalcPartonHistory::EnsureKeyExists("MC_Z_beforeFSR", "MC_Z_afterFSR");

    // Fill top parton history (flavour agnostic)
    CalcPartonHistory::FillTopPartonHistory(tzqPartonHistory, 2, m_t_dec);
    // Fill b (spectator b) parton history (flavour agnostic)
    CalcPartonHistory::FillBottomPartonHistory(tzqPartonHistory, "", 2, m_b_dec);
    // Fill Z parton history
    CalcPartonHistory::FillZPartonHistory(tzqPartonHistory, "", m_Z_dec);

    return StatusCode::SUCCESS;
  }

} // namespace top
