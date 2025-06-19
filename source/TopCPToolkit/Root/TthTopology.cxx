#include "TopCPToolkit/TthTopology.h"

#include "AthContainers/ConstDataVector.h"
#include "PartonHistory/PartonHistory.h"
#include "VectorHelpers/LorentzHelper.h"

namespace top {

/**
 * @brief Default constructor for the TthTopology class.
 */
TthTopology::TthTopology() : m_is_up_type(false) {}

/**
 * @brief Validates the parton decay topology of the Tth system.
 *
 * This method retrieves the decay PDG IDs of the W bosons (after FSR) from
 * the top and anti-top quarks. It checks for hadronic decays and counts the
 * number of leptons. If any PDG ID has an invalid value (0), the topology is
 * considered invalid.
 *
 * @return True if the topology is valid; otherwise, False.
 */
bool TthTopology::checkTopology() {
  // Accessors for PDG IDs of W decay products (after FSR)
  static const SG::AuxElement::ConstAccessor<int>
      acc_MC_Wdecay1_afterFSR_from_t_pdgId("MC_Wdecay1_afterFSR_from_t_pdgId");
  static const SG::AuxElement::ConstAccessor<int>
      acc_MC_Wdecay2_afterFSR_from_t_pdgId("MC_Wdecay2_afterFSR_from_t_pdgId");
  static const SG::AuxElement::ConstAccessor<int>
      acc_MC_Wdecay1_afterFSR_from_tbar_pdgId(
          "MC_Wdecay1_afterFSR_from_tbar_pdgId");
  static const SG::AuxElement::ConstAccessor<int>
      acc_MC_Wdecay2_afterFSR_from_tbar_pdgId(
          "MC_Wdecay2_afterFSR_from_tbar_pdgId");
  static const SG::AuxElement::ConstAccessor<int> acc_MC_Hdecay1_afterFSR_pdgId(
      "MC_Hdecay1_afterFSR_pdgId");
  static const SG::AuxElement::ConstAccessor<int> acc_MC_Hdecay2_afterFSR_pdgId(
      "MC_Hdecay2_afterFSR_pdgId");
  static const SG::AuxElement::ConstAccessor<int>
      acc_MC_Hdecay1_decay1_afterFSR_pdgId("MC_Hdecay1_decay1_afterFSR_pdgId");
  static const SG::AuxElement::ConstAccessor<int>
      acc_MC_Hdecay1_decay2_afterFSR_pdgId("MC_Hdecay1_decay2_afterFSR_pdgId");
  static const SG::AuxElement::ConstAccessor<int>
      acc_MC_Hdecay2_decay1_afterFSR_pdgId("MC_Hdecay2_decay1_afterFSR_pdgId");
  static const SG::AuxElement::ConstAccessor<int>
      acc_MC_Hdecay2_decay2_afterFSR_pdgId("MC_Hdecay2_decay2_afterFSR_pdgId");
  setNLeptons(0);
  m_t_isHadronic = false;
  m_tbar_isHadronic = false;

  // Retrieve PDG IDs from the parton history
  m_MC_Wdecay1_afterFSR_from_t_pdgId =
      acc_MC_Wdecay1_afterFSR_from_t_pdgId(*m_history);
  m_MC_Wdecay2_afterFSR_from_t_pdgId =
      acc_MC_Wdecay2_afterFSR_from_t_pdgId(*m_history);
  m_MC_Wdecay1_afterFSR_from_tbar_pdgId =
      acc_MC_Wdecay1_afterFSR_from_tbar_pdgId(*m_history);
  m_MC_Wdecay2_afterFSR_from_tbar_pdgId =
      acc_MC_Wdecay2_afterFSR_from_tbar_pdgId(*m_history);
  m_MC_Hdecay1_afterFSR_pdgId = acc_MC_Hdecay1_afterFSR_pdgId(*m_history);
  m_MC_Hdecay2_afterFSR_pdgId = acc_MC_Hdecay2_afterFSR_pdgId(*m_history);
  m_MC_Hdecay1_decay1_afterFSR_pdgId =
      acc_MC_Hdecay1_decay1_afterFSR_pdgId(*m_history);
  m_MC_Hdecay1_decay2_afterFSR_pdgId =
      acc_MC_Hdecay1_decay2_afterFSR_pdgId(*m_history);
  m_MC_Hdecay2_decay1_afterFSR_pdgId =
      acc_MC_Hdecay2_decay1_afterFSR_pdgId(*m_history);
  m_MC_Hdecay2_decay1_afterFSR_pdgId =
      acc_MC_Hdecay2_decay1_afterFSR_pdgId(*m_history);

  // Check if the W boson decays hadronically (quark PDG IDs are < 6)
  if ((std::abs(m_MC_Wdecay1_afterFSR_from_t_pdgId) < 6) ||
      (std::abs(m_MC_Wdecay2_afterFSR_from_t_pdgId) < 6))
    m_t_isHadronic = true;

  if ((std::abs(m_MC_Wdecay1_afterFSR_from_tbar_pdgId) < 6) ||
      (std::abs(m_MC_Wdecay2_afterFSR_from_tbar_pdgId) < 6))
    m_tbar_isHadronic = true;

  m_Hdecay1_isHadronic = (std::abs(m_MC_Hdecay1_afterFSR_pdgId) < 6);
  m_Hdecay2_isHadronic = (std::abs(m_MC_Hdecay2_afterFSR_pdgId) < 6);

  m_Hdecay1_decay1_isHadronic =
      (std::abs(m_MC_Hdecay1_decay1_afterFSR_pdgId) < 6);
  m_Hdecay1_decay2_isHadronic =
      (std::abs(m_MC_Hdecay1_decay2_afterFSR_pdgId) < 6);
  m_Hdecay2_decay1_isHadronic =
      (std::abs(m_MC_Hdecay2_decay1_afterFSR_pdgId) < 6);
  m_Hdecay2_decay2_isHadronic =
      (std::abs(m_MC_Hdecay2_decay2_afterFSR_pdgId) < 6);

  // Count leptonic decays
  if (!m_t_isHadronic)
    setNLeptons(getNLeptons() + 1);
  if (!m_tbar_isHadronic)
    setNLeptons(getNLeptons() + 1);

  // Check for invalid PDG ID values
  if (m_MC_Wdecay1_afterFSR_from_t_pdgId == 0)
    return false;
  if (m_MC_Wdecay2_afterFSR_from_t_pdgId == 0)
    return false;
  if (m_MC_Wdecay1_afterFSR_from_tbar_pdgId == 0)
    return false;
  if (m_MC_Wdecay2_afterFSR_from_tbar_pdgId == 0)
    return false;

  if (m_MC_Hdecay1_afterFSR_pdgId == 0)
    return false;
  if (m_MC_Hdecay2_afterFSR_pdgId == 0)
    return false;
  if ((std::abs(m_MC_Hdecay1_afterFSR_pdgId) == 23 ||
       std::abs(m_MC_Hdecay1_afterFSR_pdgId) == 24) &&
      (m_MC_Hdecay1_decay1_afterFSR_pdgId == 0 ||
       m_MC_Hdecay1_decay2_afterFSR_pdgId == 0))
    return false;
  if ((std::abs(m_MC_Hdecay2_afterFSR_pdgId) == 23 ||
       std::abs(m_MC_Hdecay2_afterFSR_pdgId) == 24) &&
      (m_MC_Hdecay2_decay1_afterFSR_pdgId == 0 ||
       m_MC_Hdecay2_decay2_afterFSR_pdgId == 0))
    return false;

  return true;  // Topology is valid
}

/**
 * @brief Fills the Tth topology information.
 *
 * This method initializes parton accessors for retrieving parton four-vectors.
 * It checks the topology using `checkTopology()` and processes the decay modes
 * (hadronic or leptonic) for the top and anti-top quarks. Truth jets,
 * electrons, and muons are filled accordingly.
 *
 * @return True if the topology is successfully filled; otherwise, False.
 */
bool TthTopology::fillTopology(std::vector<TruthObject>& truth_jets,
                               std::vector<TruthObject>& truth_electrons,
                               std::vector<TruthObject>& truth_muons) {
  // Initialize parton accessors for relevant partons
  m_partonAcc.initializePtEtaPhiMAccessor("MC_b_afterFSR_from_t");
  m_partonAcc.initializePtEtaPhiMAccessor("MC_bbar_afterFSR_from_tbar");
  m_partonAcc.initializePtEtaPhiMAccessor("MC_Wdecay1_afterFSR_from_t");
  m_partonAcc.initializePtEtaPhiMAccessor("MC_Wdecay2_afterFSR_from_t");
  m_partonAcc.initializePtEtaPhiMAccessor("MC_Wdecay1_afterFSR_from_tbar");
  m_partonAcc.initializePtEtaPhiMAccessor("MC_Wdecay2_afterFSR_from_tbar");
  m_partonAcc.initializePtEtaPhiMAccessor("MC_Hdecay1_afterFSR");
  m_partonAcc.initializePtEtaPhiMAccessor("MC_Hdecay2_afterFSR");
  m_partonAcc.initializePtEtaPhiMAccessor("MC_Hdecay1_decay1_afterFSR");
  m_partonAcc.initializePtEtaPhiMAccessor("MC_Hdecay1_decay2_afterFSR");
  m_partonAcc.initializePtEtaPhiMAccessor("MC_Hdecay2_decay1_afterFSR");
  m_partonAcc.initializePtEtaPhiMAccessor("MC_Hdecay2_decay2_afterFSR");

  // Validate the topology
  if (!checkTopology())
    return false;
  // Process top quark (t)
  m_partonAcc.getPtEtaPhiMVector("MC_b_afterFSR_from_t", m_b, m_history);
  truth_jets.emplace_back(-1, m_b, false, 0);

  if (m_t_isHadronic) {  // Hadronic decay
    m_is_up_type = (std::abs(m_MC_Wdecay1_afterFSR_from_t_pdgId) == 2 ||
                    std::abs(m_MC_Wdecay1_afterFSR_from_t_pdgId) == 4);
    m_up_source = m_is_up_type ? "MC_Wdecay1_afterFSR_from_t"
                               : "MC_Wdecay2_afterFSR_from_t";
    m_down_source = m_is_up_type ? "MC_Wdecay2_afterFSR_from_t"
                                 : "MC_Wdecay1_afterFSR_from_t";
    handleHadronicDecay(m_history, m_partonAcc, m_up_source, m_down_source,
                        truth_jets, truth_electrons, truth_muons);
  } else {  // Leptonic decay
    m_lepton_pdgId = std::abs(m_MC_Wdecay1_afterFSR_from_t_pdgId);
    m_lep_source = (m_lepton_pdgId == 11 || m_lepton_pdgId == 13)
                       ? "MC_Wdecay1_afterFSR_from_t"
                       : "MC_Wdecay2_afterFSR_from_t";
    handleLeptonicDecay(m_history, m_partonAcc, m_lepton_pdgId, m_lep_source,
                        truth_jets, truth_electrons, truth_muons);
  }

  // Process anti-top quark (tbar)
  m_partonAcc.getPtEtaPhiMVector("MC_bbar_afterFSR_from_tbar", m_bbar,
                                 m_history);
  truth_jets.emplace_back(-1, m_bbar, false, 0);

  if (m_tbar_isHadronic) {  // Hadronic decay
    m_is_up_type = (std::abs(m_MC_Wdecay1_afterFSR_from_tbar_pdgId) == 2 ||
                    std::abs(m_MC_Wdecay1_afterFSR_from_tbar_pdgId) == 4);
    m_up_source = m_is_up_type ? "MC_Wdecay1_afterFSR_from_tbar"
                               : "MC_Wdecay2_afterFSR_from_tbar";
    m_down_source = m_is_up_type ? "MC_Wdecay2_afterFSR_from_tbar"
                                 : "MC_Wdecay1_afterFSR_from_tbar";
    handleHadronicDecay(m_history, m_partonAcc, m_up_source, m_down_source,
                        truth_jets, truth_electrons, truth_muons);
  } else {  // Leptonic decay
    m_lepton_pdgId = std::abs(m_MC_Wdecay1_afterFSR_from_tbar_pdgId);
    m_lep_source = (m_lepton_pdgId == 11 || m_lepton_pdgId == 13)
                       ? "MC_Wdecay1_afterFSR_from_tbar"
                       : "MC_Wdecay2_afterFSR_from_tbar";
    handleLeptonicDecay(m_history, m_partonAcc, m_lepton_pdgId, m_lep_source,
                        truth_jets, truth_electrons, truth_muons);
  }

  // Hqq (e.g. Hbb, Hcc)
  if (m_Hdecay1_isHadronic || m_Hdecay2_isHadronic) {
    m_is_up_type = (std::abs(m_MC_Hdecay1_afterFSR_pdgId) == 2 ||
                    std::abs(m_MC_Hdecay1_afterFSR_pdgId) == 4);
    m_up_source = m_is_up_type ? "MC_Hdecay1_afterFSR" : "MC_Hdecay2_afterFSR";
    m_down_source =
        m_is_up_type ? "MC_Hdecay2_afterFSR" : "MC_Hdecay1_afterFSR";
    handleHadronicDecay(m_history, m_partonAcc, m_up_source, m_down_source,
                        truth_jets, truth_electrons, truth_muons);
  }
  // Htautau, Hyy
  // TODO implement a solution for this that allows for matching of taus and
  // photons!
  else if (std::abs(m_MC_Hdecay1_afterFSR_pdgId) == 15 ||
           std::abs(m_MC_Hdecay1_afterFSR_pdgId) == 22) {
    // for Htautau and Hgammagamma we will H1, H11, H12, H2, H21 and H21 as
    // empty vectors to avoid matching
    for (int i = 0; i < 2; i++) {
      truth_jets.emplace_back(-1, PtEtaPhiMVector(0, 0, 0, 0), true, 0);
      truth_electrons.emplace_back(-1, PtEtaPhiMVector(0, 0, 0, 0), true, 0);
      truth_muons.emplace_back(-1, PtEtaPhiMVector(0, 0, 0, 0), true, 0);
    }
  } else {
    // HWW/HZZ
    if (m_Hdecay1_decay1_isHadronic || m_Hdecay1_decay2_isHadronic) {
      m_is_up_type = (std::abs(m_MC_Hdecay1_decay1_afterFSR_pdgId) == 2 ||
                      std::abs(m_MC_Hdecay1_decay1_afterFSR_pdgId) == 4);
      m_up_source = m_is_up_type ? "MC_Hdecay1_decay1_afterFSR"
                                 : "MC_Hdecay1_decay2_afterFSR";
      m_down_source = m_is_up_type ? "MC_Hdecay1_decay2_afterFSR"
                                   : "MC_Hdecay1_decay1_afterFSR";
      handleHadronicDecay(m_history, m_partonAcc, m_up_source, m_down_source,
                          truth_jets, truth_electrons, truth_muons);
    } else {
      m_lepton_pdgId = std::abs(m_MC_Hdecay1_decay1_afterFSR_pdgId);
      m_lep_source = (m_lepton_pdgId == 11 || m_lepton_pdgId == 13)
                         ? "MC_Hdecay1_decay1_afterFSR"
                         : "MC_Hdecay1_decay2_afterFSR";
      handleLeptonicDecay(m_history, m_partonAcc, m_lepton_pdgId, m_lep_source,
                          truth_jets, truth_electrons, truth_muons);
    }
    if (m_Hdecay2_decay1_isHadronic || m_Hdecay2_decay2_isHadronic) {
      m_is_up_type = (std::abs(m_MC_Hdecay2_decay1_afterFSR_pdgId) == 2 ||
                      std::abs(m_MC_Hdecay2_decay1_afterFSR_pdgId) == 4);
      m_up_source = m_is_up_type ? "MC_Hdecay2_decay1_afterFSR"
                                 : "MC_Hdecay2_decay2_afterFSR";
      m_down_source = m_is_up_type ? "MC_Hdecay2_decay2_afterFSR"
                                   : "MC_Hdecay2_decay1_afterFSR";
      handleHadronicDecay(m_history, m_partonAcc, m_up_source, m_down_source,
                          truth_jets, truth_electrons, truth_muons);
    } else {
      m_lepton_pdgId = std::abs(m_MC_Hdecay2_decay1_afterFSR_pdgId);
      m_lep_source = (m_lepton_pdgId == 11 || m_lepton_pdgId == 13)
                         ? "MC_Hdecay2_decay1_afterFSR"
                         : "MC_Hdecay2_decay2_afterFSR";
      handleLeptonicDecay(m_history, m_partonAcc, m_lepton_pdgId, m_lep_source,
                          truth_jets, truth_electrons, truth_muons);
    }
  }
  return true;  // Topology successfully filled
}
}  // namespace top
