#include "PartonHistory/CalcPartonHistory.h"

namespace top {
void CalcPartonHistory::Initialize4TopDecorators() {
  for (int idx = 1; idx <= 2; idx++) {
    m_dec.initializePtEtaPhiMDecorator("MC_t" + std::to_string(idx) +
                                       "_beforeFSR");
    m_dec.initializePtEtaPhiMDecorator("MC_b_beforeFSR_from_t" +
                                       std::to_string(idx));
    m_dec.initializePtEtaPhiMDecorator("MC_W_beforeFSR_from_t" +
                                       std::to_string(idx));
    m_dec.initializePtEtaPhiMDecorator("MC_Wdecay1_beforeFSR_from_t" +
                                       std::to_string(idx));
    m_dec.initializePtEtaPhiMDecorator("MC_Wdecay2_beforeFSR_from_t" +
                                       std::to_string(idx));

    m_dec.initializePtEtaPhiMDecorator("MC_t" + std::to_string(idx) +
                                       "_afterFSR");
    m_dec.initializePtEtaPhiMDecorator("MC_b_afterFSR_from_t" +
                                       std::to_string(idx));
    m_dec.initializePtEtaPhiMDecorator("MC_W_afterFSR_from_t" +
                                       std::to_string(idx));
    m_dec.initializePtEtaPhiMDecorator("MC_Wdecay1_afterFSR_from_t" +
                                       std::to_string(idx));
    m_dec.initializePtEtaPhiMDecorator("MC_Wdecay2_afterFSR_from_t" +
                                       std::to_string(idx));

    m_dec.initializePtEtaPhiMDecorator("MC_tbar" + std::to_string(idx) +
                                       "_beforeFSR");
    m_dec.initializePtEtaPhiMDecorator("MC_bbar_beforeFSR_from_tbar" +
                                       std::to_string(idx));
    m_dec.initializePtEtaPhiMDecorator("MC_W_beforeFSR_from_tbar" +
                                       std::to_string(idx));
    m_dec.initializePtEtaPhiMDecorator("MC_Wdecay1_beforeFSR_from_tbar" +
                                       std::to_string(idx));
    m_dec.initializePtEtaPhiMDecorator("MC_Wdecay2_beforeFSR_from_tbar" +
                                       std::to_string(idx));

    m_dec.initializePtEtaPhiMDecorator("MC_tbar" + std::to_string(idx) +
                                       "_afterFSR");
    m_dec.initializePtEtaPhiMDecorator("MC_bbar_afterFSR_from_tbar" +
                                       std::to_string(idx));
    m_dec.initializePtEtaPhiMDecorator("MC_W_afterFSR_from_tbar" +
                                       std::to_string(idx));
    m_dec.initializePtEtaPhiMDecorator("MC_Wdecay1_afterFSR_from_tbar" +
                                       std::to_string(idx));
    m_dec.initializePtEtaPhiMDecorator("MC_Wdecay2_afterFSR_from_tbar" +
                                       std::to_string(idx));

    m_dec.initializeIntDecorator("MC_t" + std::to_string(idx) +
                                 "_beforeFSR_pdgId");
    m_dec.initializeIntDecorator("MC_b_beforeFSR_from_t" + std::to_string(idx) +
                                 "_pdgId");
    m_dec.initializeIntDecorator("MC_W_beforeFSR_from_t" + std::to_string(idx) +
                                 "_pdgId");
    m_dec.initializeIntDecorator("MC_Wdecay1_beforeFSR_from_t" +
                                 std::to_string(idx) + "_pdgId");
    m_dec.initializeIntDecorator("MC_Wdecay2_beforeFSR_from_t" +
                                 std::to_string(idx) + "_pdgId");

    m_dec.initializeIntDecorator("MC_t" + std::to_string(idx) +
                                 "_afterFSR_pdgId");
    m_dec.initializeIntDecorator("MC_b_afterFSR_from_t" + std::to_string(idx) +
                                 "_pdgId");
    m_dec.initializeIntDecorator("MC_W_afterFSR_from_t" + std::to_string(idx) +
                                 "_pdgId");
    m_dec.initializeIntDecorator("MC_Wdecay1_afterFSR_from_t" +
                                 std::to_string(idx) + "_pdgId");
    m_dec.initializeIntDecorator("MC_Wdecay2_afterFSR_from_t" +
                                 std::to_string(idx) + "_pdgId");

    m_dec.initializeIntDecorator("MC_tbar" + std::to_string(idx) +
                                 "_beforeFSR_pdgId");
    m_dec.initializeIntDecorator("MC_bbar_beforeFSR_from_tbar" +
                                 std::to_string(idx) + "_pdgId");
    m_dec.initializeIntDecorator("MC_W_beforeFSR_from_tbar" +
                                 std::to_string(idx) + "_pdgId");
    m_dec.initializeIntDecorator("MC_Wdecay1_beforeFSR_from_tbar" +
                                 std::to_string(idx) + "_pdgId");
    m_dec.initializeIntDecorator("MC_Wdecay2_beforeFSR_from_tbar" +
                                 std::to_string(idx) + "_pdgId");

    m_dec.initializeIntDecorator("MC_tbar" + std::to_string(idx) +
                                 "_afterFSR_pdgId");
    m_dec.initializeIntDecorator("MC_bbar_afterFSR_from_tbar" +
                                 std::to_string(idx) + "_pdgId");
    m_dec.initializeIntDecorator("MC_W_afterFSR_from_tbar" +
                                 std::to_string(idx) + "_pdgId");
    m_dec.initializeIntDecorator("MC_Wdecay1_afterFSR_from_tbar" +
                                 std::to_string(idx) + "_pdgId");
    m_dec.initializeIntDecorator("MC_Wdecay2_afterFSR_from_tbar" +
                                 std::to_string(idx) + "_pdgId");
  }
}

void CalcPartonHistory::InitializeTopDecorators() {
  m_dec.initializePtEtaPhiMDecorator("MC_t_beforeFSR");
  m_dec.initializePtEtaPhiMDecorator("MC_b_beforeFSR_from_t");
  m_dec.initializePtEtaPhiMDecorator("MC_W_beforeFSR_from_t");
  m_dec.initializePtEtaPhiMDecorator("MC_Wdecay1_beforeFSR_from_t");
  m_dec.initializePtEtaPhiMDecorator("MC_Wdecay2_beforeFSR_from_t");

  m_dec.initializePtEtaPhiMDecorator("MC_t_afterFSR");
  m_dec.initializePtEtaPhiMDecorator("MC_b_afterFSR_from_t");
  m_dec.initializePtEtaPhiMDecorator("MC_W_afterFSR_from_t");
  m_dec.initializePtEtaPhiMDecorator("MC_Wdecay1_afterFSR_from_t");
  m_dec.initializePtEtaPhiMDecorator("MC_Wdecay2_afterFSR_from_t");

  m_dec.initializeIntDecorator("MC_t_beforeFSR_pdgId");
  m_dec.initializeIntDecorator("MC_b_beforeFSR_from_t_pdgId");
  m_dec.initializeIntDecorator("MC_W_beforeFSR_from_t_pdgId");
  m_dec.initializeIntDecorator("MC_Wdecay1_beforeFSR_from_t_pdgId");
  m_dec.initializeIntDecorator("MC_Wdecay2_beforeFSR_from_t_pdgId");

  m_dec.initializeIntDecorator("MC_t_afterFSR_pdgId");
  m_dec.initializeIntDecorator("MC_b_afterFSR_from_t_pdgId");
  m_dec.initializeIntDecorator("MC_W_afterFSR_from_t_pdgId");
  m_dec.initializeIntDecorator("MC_Wdecay1_afterFSR_from_t_pdgId");
  m_dec.initializeIntDecorator("MC_Wdecay2_afterFSR_from_t_pdgId");
}

void CalcPartonHistory::InitializeAntiTopDecorators() {
  m_dec.initializePtEtaPhiMDecorator("MC_tbar_beforeFSR");
  m_dec.initializePtEtaPhiMDecorator("MC_bbar_beforeFSR_from_tbar");
  m_dec.initializePtEtaPhiMDecorator("MC_W_beforeFSR_from_tbar");
  m_dec.initializePtEtaPhiMDecorator("MC_Wdecay1_beforeFSR_from_tbar");
  m_dec.initializePtEtaPhiMDecorator("MC_Wdecay2_beforeFSR_from_tbar");

  m_dec.initializePtEtaPhiMDecorator("MC_tbar_afterFSR");
  m_dec.initializePtEtaPhiMDecorator("MC_bbar_afterFSR_from_tbar");
  m_dec.initializePtEtaPhiMDecorator("MC_W_afterFSR_from_tbar");
  m_dec.initializePtEtaPhiMDecorator("MC_Wdecay1_afterFSR_from_tbar");
  m_dec.initializePtEtaPhiMDecorator("MC_Wdecay2_afterFSR_from_tbar");

  m_dec.initializeIntDecorator("MC_tbar_beforeFSR_pdgId");
  m_dec.initializeIntDecorator("MC_bbar_beforeFSR_from_tbar_pdgId");
  m_dec.initializeIntDecorator("MC_W_beforeFSR_from_tbar_pdgId");
  m_dec.initializeIntDecorator("MC_Wdecay1_beforeFSR_from_tbar_pdgId");
  m_dec.initializeIntDecorator("MC_Wdecay2_beforeFSR_from_tbar_pdgId");

  m_dec.initializeIntDecorator("MC_tbar_afterFSR_pdgId");
  m_dec.initializeIntDecorator("MC_bbar_afterFSR_from_tbar_pdgId");
  m_dec.initializeIntDecorator("MC_W_afterFSR_from_tbar_pdgId");
  m_dec.initializeIntDecorator("MC_Wdecay1_afterFSR_from_tbar_pdgId");
  m_dec.initializeIntDecorator("MC_Wdecay2_afterFSR_from_tbar_pdgId");
}

void CalcPartonHistory::InitializeBottomDecorators() {
  m_dec.initializePtEtaPhiMDecorator("MC_b_beforeFSR");
  m_dec.initializePtEtaPhiMDecorator("MC_b_afterFSR");

  m_dec.initializeIntDecorator("MC_b_beforeFSR_pdgId");
  m_dec.initializeIntDecorator("MC_b_afterFSR_pdgId");
}

void CalcPartonHistory::InitializeVectorBottomDecorators() {
  m_dec.initializeVectorPtEtaPhiMDecorator("MC_b_beforeFSR");
  m_dec.initializeVectorPtEtaPhiMDecorator("MC_b_afterFSR");

  m_dec.initializeVectorIntDecorator("MC_b_beforeFSR_pdgId");
  m_dec.initializeVectorIntDecorator("MC_b_afterFSR_pdgId");
}

void CalcPartonHistory::InitializeAntiBottomDecorators() {
  m_dec.initializePtEtaPhiMDecorator("MC_bbar_beforeFSR");
  m_dec.initializePtEtaPhiMDecorator("MC_bbar_afterFSR");

  m_dec.initializeIntDecorator("MC_bbar_beforeFSR_pdgId");
  m_dec.initializeIntDecorator("MC_bbar_afterFSR_pdgId");
}

void CalcPartonHistory::InitializeVectorAntiBottomDecorators() {
  m_dec.initializeVectorPtEtaPhiMDecorator("MC_bbar_beforeFSR");
  m_dec.initializeVectorPtEtaPhiMDecorator("MC_bbar_afterFSR");

  m_dec.initializeVectorIntDecorator("MC_bbar_beforeFSR_pdgId");
  m_dec.initializeVectorIntDecorator("MC_bbar_afterFSR_pdgId");
}

void CalcPartonHistory::InitializeTtbarDecorators() {
  m_dec.initializeFloatDecorator(
      {"MC_ttbar_beforeFSR_m", "MC_ttbar_beforeFSR_pt",
       "MC_ttbar_beforeFSR_eta", "MC_ttbar_beforeFSR_phi"});
  m_dec.initializeFloatDecorator(
      {"MC_ttbar_fromDecay_beforeFSR_m", "MC_ttbar_fromDecay_beforeFSR_pt",
       "MC_ttbar_fromDecay_beforeFSR_eta", "MC_ttbar_fromDecay_beforeFSR_phi"});

  m_dec.initializeFloatDecorator({"MC_ttbar_afterFSR_m", "MC_ttbar_afterFSR_pt",
                                  "MC_ttbar_afterFSR_eta",
                                  "MC_ttbar_afterFSR_phi"});
  m_dec.initializeFloatDecorator(
      {"MC_ttbar_fromDecay_afterFSR_m", "MC_ttbar_fromDecay_afterFSR_pt",
       "MC_ttbar_fromDecay_afterFSR_eta", "MC_ttbar_fromDecay_afterFSR_phi"});
}

void CalcPartonHistory::InitializePhotonDecorators() {
  m_dec.initializePtEtaPhiMDecorator("MC_gamma");
  m_dec.initializeIntDecorator("MC_gamma_origin");
  m_dec.initializeIntDecorator("MC_gamma_pdgId");
}

void CalcPartonHistory::InitializeHiggsDecorators() {
  m_dec.initializePtEtaPhiMDecorator("MC_H_beforeFSR");
  m_dec.initializePtEtaPhiMDecorator("MC_Hdecay1_beforeFSR");
  m_dec.initializePtEtaPhiMDecorator("MC_Hdecay2_beforeFSR");

  m_dec.initializePtEtaPhiMDecorator("MC_Hdecay1_decay1_beforeFSR");
  m_dec.initializePtEtaPhiMDecorator("MC_Hdecay2_decay1_beforeFSR");
  m_dec.initializePtEtaPhiMDecorator("MC_Hdecay1_decay2_beforeFSR");
  m_dec.initializePtEtaPhiMDecorator("MC_Hdecay2_decay2_beforeFSR");

  m_dec.initializePtEtaPhiMDecorator("MC_H_afterFSR");
  m_dec.initializePtEtaPhiMDecorator("MC_Hdecay1_afterFSR");
  m_dec.initializePtEtaPhiMDecorator("MC_Hdecay2_afterFSR");

  m_dec.initializePtEtaPhiMDecorator("MC_Hdecay1_decay1_afterFSR");
  m_dec.initializePtEtaPhiMDecorator("MC_Hdecay2_decay1_afterFSR");
  m_dec.initializePtEtaPhiMDecorator("MC_Hdecay1_decay2_afterFSR");
  m_dec.initializePtEtaPhiMDecorator("MC_Hdecay2_decay2_afterFSR");

  m_dec.initializeIntDecorator("MC_H_beforeFSR_pdgId");
  m_dec.initializeIntDecorator("MC_Hdecay1_beforeFSR_pdgId");
  m_dec.initializeIntDecorator("MC_Hdecay2_beforeFSR_pdgId");

  m_dec.initializeIntDecorator("MC_Hdecay1_decay1_beforeFSR_pdgId");
  m_dec.initializeIntDecorator("MC_Hdecay2_decay1_beforeFSR_pdgId");
  m_dec.initializeIntDecorator("MC_Hdecay1_decay2_beforeFSR_pdgId");
  m_dec.initializeIntDecorator("MC_Hdecay2_decay2_beforeFSR_pdgId");

  m_dec.initializeIntDecorator("MC_H_afterFSR_pdgId");
  m_dec.initializeIntDecorator("MC_Hdecay1_afterFSR_pdgId");
  m_dec.initializeIntDecorator("MC_Hdecay2_afterFSR_pdgId");

  m_dec.initializeIntDecorator("MC_Hdecay1_decay1_afterFSR_pdgId");
  m_dec.initializeIntDecorator("MC_Hdecay2_decay1_afterFSR_pdgId");
  m_dec.initializeIntDecorator("MC_Hdecay1_decay2_afterFSR_pdgId");
  m_dec.initializeIntDecorator("MC_Hdecay2_decay2_afterFSR_pdgId");
}

void CalcPartonHistory::InitializeZDecorators() {
  m_dec.initializePtEtaPhiMDecorator("MC_Z_beforeFSR");
  m_dec.initializePtEtaPhiMDecorator("MC_Zdecay1_beforeFSR");
  m_dec.initializePtEtaPhiMDecorator("MC_Zdecay2_beforeFSR");

  m_dec.initializePtEtaPhiMDecorator("MC_Z_afterFSR");
  m_dec.initializePtEtaPhiMDecorator("MC_Zdecay1_afterFSR");
  m_dec.initializePtEtaPhiMDecorator("MC_Zdecay2_afterFSR");

  m_dec.initializeIntDecorator("MC_Z_beforeFSR_pdgId");
  m_dec.initializeIntDecorator("MC_Zdecay1_beforeFSR_pdgId");
  m_dec.initializeIntDecorator("MC_Zdecay2_beforeFSR_pdgId");

  m_dec.initializeIntDecorator("MC_Z_afterFSR_pdgId");
  m_dec.initializeIntDecorator("MC_Zdecay1_afterFSR_pdgId");
  m_dec.initializeIntDecorator("MC_Zdecay2_afterFSR_pdgId");

  m_dec.initializeIntDecorator("MC_Z_IsOnShell");
}

void CalcPartonHistory::InitializeWDecorators() {
  m_dec.initializePtEtaPhiMDecorator("MC_W_beforeFSR");
  m_dec.initializePtEtaPhiMDecorator("MC_Wdecay1_beforeFSR");
  m_dec.initializePtEtaPhiMDecorator("MC_Wdecay2_beforeFSR");

  m_dec.initializePtEtaPhiMDecorator("MC_W_afterFSR");
  m_dec.initializePtEtaPhiMDecorator("MC_Wdecay1_afterFSR");
  m_dec.initializePtEtaPhiMDecorator("MC_Wdecay2_afterFSR");

  m_dec.initializeIntDecorator("MC_W_beforeFSR_pdgId");
  m_dec.initializeIntDecorator("MC_Wdecay1_beforeFSR_pdgId");
  m_dec.initializeIntDecorator("MC_Wdecay2_beforeFSR_pdgId");

  m_dec.initializeIntDecorator("MC_W_afterFSR_pdgId");
  m_dec.initializeIntDecorator("MC_Wdecay1_afterFSR_pdgId");
  m_dec.initializeIntDecorator("MC_Wdecay2_afterFSR_pdgId");

  m_dec.initializeIntDecorator("MC_W_IsOnShell");
}
}  // namespace top
