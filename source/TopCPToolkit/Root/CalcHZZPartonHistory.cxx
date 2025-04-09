#include "PartonHistory/CalcHZZPartonHistory.h"
#include "PartonHistory/PartonHistoryUtils.h"

namespace top {
  CalcHZZPartonHistory::CalcHZZPartonHistory(const std::string& name,
					     const std::vector<std::string>& truthCollections) :
    CalcPartonHistory(name, truthCollections)
  {
    initializeDecorators();
  }

  void CalcHZZPartonHistory::initializeDecorators() {
    m_H_dec.initializeFloatDecorator({"MC_H_m", "MC_H_pt", "MC_H_eta", "MC_H_phi"});
    m_H_dec.initializeIntDecorator("MC_H_pdgId");
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

  StatusCode CalcHZZPartonHistory::runHistorySaver(const xAOD::TruthParticleContainer* truthParticles,
						   xAOD::PartonHistory* hzzPartonHistory) {
    CalcPartonHistory::TraceParticles(truthParticles);    
    CalcPartonHistory::FillHiggsPartonHistory(hzzPartonHistory, m_H_dec);
    
    return StatusCode::SUCCESS;
  }

} // namespace top
