#include "PartonHistory/CalcTthPartonHistory.h"
#include "PartonHistory/PartonHistoryUtils.h"

namespace top {
  CalcTthPartonHistory::CalcTthPartonHistory(const std::string& name,
					     const std::vector<std::string>& truthCollections) :
    CalcPartonHistory(name, truthCollections)
    {
      initializeDecorators();
    }

  void CalcTthPartonHistory::initializeDecorators() {
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

    m_H_dec.initializeFloatDecorator({"MC_H_m", "MC_H_pt", "MC_H_eta", "MC_H_phi"});
    m_H_dec.initializeFloatDecorator({"MC_Hdecay1_m", "MC_Hdecay1_pt", "MC_Hdecay1_eta", "MC_Hdecay1_phi"});
    m_H_dec.initializeIntDecorator("MC_Hdecay1_pdgId");
    m_H_dec.initializeFloatDecorator({"MC_Hdecay2_m", "MC_Hdecay2_pt", "MC_Hdecay2_eta", "MC_Hdecay2_phi"});
    m_H_dec.initializeIntDecorator("MC_Hdecay2_pdgId");
    m_H_dec.initializeFloatDecorator({"MC_Hdecay1_decay1_m", "MC_Hdecay1_decay1_pt", "MC_Hdecay1_decay1_eta", "MC_Hdecay1_decay1_phi"});
    m_H_dec.initializeIntDecorator("MC_Hdecay1_decay1_pdgId");
    m_H_dec.initializeFloatDecorator({"MC_Hdecay1_decay2_m", "MC_Hdecay1_decay2_pt", "MC_Hdecay1_decay2_eta", "MC_Hdecay1_decay2_phi"});
    m_H_dec.initializeIntDecorator("MC_Hdecay1_decay2_pdgId");
    m_H_dec.initializeFloatDecorator({"MC_Hdecay2_decay1_m", "MC_Hdecay2_decay1_pt", "MC_Hdecay2_decay1_eta", "MC_Hdecay2_decay1_phi"});
    m_H_dec.initializeIntDecorator("MC_Hdecay2_decay1_pdgId");
    m_H_dec.initializeFloatDecorator({"MC_Hdecay2_decay2_m", "MC_Hdecay2_decay2_pt", "MC_Hdecay2_decay2_eta", "MC_Hdecay2_decay2_phi"});
    m_H_dec.initializeIntDecorator("MC_Hdecay2_decay2_pdgId");
  }

  StatusCode CalcTthPartonHistory::runHistorySaver(const xAOD::TruthParticleContainer* truthParticles,
                                                 xAOD::PartonHistory* tthPartonHistory) {
    // Tracing truth particles
    CalcPartonHistory::TraceParticles(truthParticles);

    // Ensuring the necessary keys exist
    CalcPartonHistory::EnsureTtbarKeysExist();

    // Fill top parton history
    CalcPartonHistory::FillTopPartonHistory(tthPartonHistory, 0, m_t_dec); // t mode
    // Fill anti top parton history
    CalcPartonHistory::FillTopPartonHistory(tthPartonHistory, 1, m_tbar_dec); // ttbar mode
    // Fill ttbar parton history
    CalcPartonHistory::FillTtbarPartonHistory(tthPartonHistory, m_ttbar_dec);
    // Fill Higgs parton history
    CalcPartonHistory::FillHiggsPartonHistory(tthPartonHistory, m_H_dec);

    return StatusCode::SUCCESS;
  }

} // namespace top
