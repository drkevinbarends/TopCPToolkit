#include "PartonHistory/CalcThqPartonHistory.h"
#include "PartonHistory/PartonHistoryUtils.h"

namespace top {
  CalcThqPartonHistory::CalcThqPartonHistory(const std::string& name,
					     const std::vector<std::string>& truthCollections) :
    CalcPartonHistory(name, truthCollections)
  {
    initializeDecorators();
  }

   void CalcThqPartonHistory::initializeDecorators() {
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

  StatusCode CalcThqPartonHistory::runHistorySaver(const xAOD::TruthParticleContainer* truthParticles,
                                                 xAOD::PartonHistory* thqPartonHistory) {
    // Tracing truth particles
    CalcPartonHistory::TraceParticles(truthParticles);

    // Ensuring the necessary keys exist
    CalcPartonHistory::EnsureTtbarKeysExist(); // this will fail on ttbar but that is ok.

    // Fill top parton history
    CalcPartonHistory::FillTopPartonHistory(thqPartonHistory, 2, m_t_dec); // flavour agnostic mode
    // Fill b parton history
    CalcPartonHistory::FillBottomPartonHistory(thqPartonHistory, "", 2, m_b_dec); // flavour agnostic mode
    // Fill H parton history
    CalcPartonHistory::FillHiggsPartonHistory(thqPartonHistory, m_H_dec);

    return StatusCode::SUCCESS;
  }

} // namespace top
