#include "TopCPToolkit/FourTopTopology.h"

#include "AthContainers/ConstDataVector.h"
#include "PartonHistory/PartonHistory.h"
#include "VectorHelpers/LorentzHelper.h"

namespace top {

/**
 * @brief Default constructor for the FourTopTopology class.
 */
FourTopTopology::FourTopTopology() : m_is_up_type(false) {}

/**
 * @brief Checks the parton-level decay topology of a four-top-quark system.
 *
 * This method retrieves the PDG IDs of the W boson decay products (after FSR)
 * from four top quarks (t1, t2, tbar1, tbar2). It identifies whether each top
 * or anti-top quark decays hadronically or leptonically. The number of leptonic
 * decays is tracked, and the method ensures no invalid PDG ID values are
 * present.
 *
 * @return True if the topology is valid, otherwise False.
 */
bool FourTopTopology::checkTopology() {
  // Static accessors for W boson decay products (after FSR) for all four tops
  static const SG::AuxElement::ConstAccessor<int>
      acc_MC_Wdecay1_afterFSR_from_t1_pdgId(
          "MC_Wdecay1_afterFSR_from_t1_pdgId");
  static const SG::AuxElement::ConstAccessor<int>
      acc_MC_Wdecay2_afterFSR_from_t1_pdgId(
          "MC_Wdecay2_afterFSR_from_t1_pdgId");
  static const SG::AuxElement::ConstAccessor<int>
      acc_MC_Wdecay1_afterFSR_from_tbar1_pdgId(
          "MC_Wdecay1_afterFSR_from_tbar1_pdgId");
  static const SG::AuxElement::ConstAccessor<int>
      acc_MC_Wdecay2_afterFSR_from_tbar1_pdgId(
          "MC_Wdecay2_afterFSR_from_tbar1_pdgId");
  static const SG::AuxElement::ConstAccessor<int>
      acc_MC_Wdecay1_afterFSR_from_t2_pdgId(
          "MC_Wdecay1_afterFSR_from_t2_pdgId");
  static const SG::AuxElement::ConstAccessor<int>
      acc_MC_Wdecay2_afterFSR_from_t2_pdgId(
          "MC_Wdecay2_afterFSR_from_t2_pdgId");
  static const SG::AuxElement::ConstAccessor<int>
      acc_MC_Wdecay1_afterFSR_from_tbar2_pdgId(
          "MC_Wdecay1_afterFSR_from_tbar2_pdgId");
  static const SG::AuxElement::ConstAccessor<int>
      acc_MC_Wdecay2_afterFSR_from_tbar2_pdgId(
          "MC_Wdecay2_afterFSR_from_tbar2_pdgId");
  m_t1_isHadronic = false;
  m_t2_isHadronic = false;
  m_tbar1_isHadronic = false;
  m_tbar2_isHadronic = false;
  setNLeptons(0);

  // Retrieve PDG IDs for all four tops and anti-tops
  m_MC_Wdecay1_afterFSR_from_t1_pdgId =
      acc_MC_Wdecay1_afterFSR_from_t1_pdgId(*m_history);
  m_MC_Wdecay2_afterFSR_from_t1_pdgId =
      acc_MC_Wdecay2_afterFSR_from_t1_pdgId(*m_history);
  m_MC_Wdecay1_afterFSR_from_tbar1_pdgId =
      acc_MC_Wdecay1_afterFSR_from_tbar1_pdgId(*m_history);
  m_MC_Wdecay2_afterFSR_from_tbar1_pdgId =
      acc_MC_Wdecay2_afterFSR_from_tbar1_pdgId(*m_history);
  m_MC_Wdecay1_afterFSR_from_t2_pdgId =
      acc_MC_Wdecay1_afterFSR_from_t2_pdgId(*m_history);
  m_MC_Wdecay2_afterFSR_from_t2_pdgId =
      acc_MC_Wdecay2_afterFSR_from_t2_pdgId(*m_history);
  m_MC_Wdecay1_afterFSR_from_tbar2_pdgId =
      acc_MC_Wdecay1_afterFSR_from_tbar2_pdgId(*m_history);
  m_MC_Wdecay2_afterFSR_from_tbar2_pdgId =
      acc_MC_Wdecay2_afterFSR_from_tbar2_pdgId(*m_history);

  // Determine decay type (hadronic or leptonic) for each top quark
  if ((std::abs(m_MC_Wdecay1_afterFSR_from_t1_pdgId) < 6) ||
      (std::abs(m_MC_Wdecay2_afterFSR_from_t1_pdgId) < 6))
    m_t1_isHadronic = true;
  else
    setNLeptons(getNLeptons() + 1);

  if ((std::abs(m_MC_Wdecay1_afterFSR_from_tbar1_pdgId) < 6) ||
      (std::abs(m_MC_Wdecay2_afterFSR_from_tbar1_pdgId) < 6))
    m_tbar1_isHadronic = true;
  else
    setNLeptons(getNLeptons() + 1);

  if ((std::abs(m_MC_Wdecay1_afterFSR_from_t2_pdgId) < 6) ||
      (std::abs(m_MC_Wdecay2_afterFSR_from_t2_pdgId) < 6))
    m_t2_isHadronic = true;
  else
    setNLeptons(getNLeptons() + 1);

  if ((std::abs(m_MC_Wdecay1_afterFSR_from_tbar2_pdgId) < 6) ||
      (std::abs(m_MC_Wdecay2_afterFSR_from_tbar2_pdgId) < 6))
    m_tbar2_isHadronic = true;
  else
    setNLeptons(getNLeptons() + 1);

  // Validate PDG ID values (check for invalid '0' entries)
  if (m_MC_Wdecay1_afterFSR_from_t1_pdgId == 0 ||
      m_MC_Wdecay2_afterFSR_from_t1_pdgId == 0)
    return false;
  if (m_MC_Wdecay1_afterFSR_from_tbar1_pdgId == 0 ||
      m_MC_Wdecay2_afterFSR_from_tbar1_pdgId == 0)
    return false;
  if (m_MC_Wdecay1_afterFSR_from_t2_pdgId == 0 ||
      m_MC_Wdecay2_afterFSR_from_t2_pdgId == 0)
    return false;
  if (m_MC_Wdecay1_afterFSR_from_tbar2_pdgId == 0 ||
      m_MC_Wdecay2_afterFSR_from_tbar2_pdgId == 0)
    return false;

  return true;  // Topology is valid
}

/**
 * @brief Fills the four-top-quark decay topology.
 *
 * This method initializes parton accessors for all relevant partons involved in
 * the decay. It processes each top/anti-top quark to determine if it decayed
 * hadronically or leptonically and updates the truth jet and lepton
 * collections.
 *
 * @return True if the topology is successfully filled; otherwise, False.
 */
bool FourTopTopology::fillTopology(std::vector<TruthObject>& truth_jets,
                                   std::vector<TruthObject>& truth_electrons,
                                   std::vector<TruthObject>& truth_muons) {
  // Initialize accessors for all parton sources
  m_partonAcc.initializePtEtaPhiMAccessor("MC_b_afterFSR_from_t1");
  m_partonAcc.initializePtEtaPhiMAccessor("MC_bbar_afterFSR_from_tbar1");
  m_partonAcc.initializePtEtaPhiMAccessor("MC_Wdecay1_afterFSR_from_t1");
  m_partonAcc.initializePtEtaPhiMAccessor("MC_Wdecay2_afterFSR_from_t1");
  m_partonAcc.initializePtEtaPhiMAccessor("MC_Wdecay1_afterFSR_from_tbar1");
  m_partonAcc.initializePtEtaPhiMAccessor("MC_Wdecay2_afterFSR_from_tbar1");
  m_partonAcc.initializePtEtaPhiMAccessor("MC_b_afterFSR_from_t2");
  m_partonAcc.initializePtEtaPhiMAccessor("MC_bbar_afterFSR_from_tbar2");
  m_partonAcc.initializePtEtaPhiMAccessor("MC_Wdecay1_afterFSR_from_t2");
  m_partonAcc.initializePtEtaPhiMAccessor("MC_Wdecay2_afterFSR_from_t2");
  m_partonAcc.initializePtEtaPhiMAccessor("MC_Wdecay1_afterFSR_from_tbar2");
  m_partonAcc.initializePtEtaPhiMAccessor("MC_Wdecay2_afterFSR_from_tbar2");

  // Validate topology
  if (!checkTopology())
    return false;
  m_partonAcc.getPtEtaPhiMVector("MC_b_afterFSR_from_t1", m_b1, m_history);
  truth_jets.emplace_back(-1, m_b1, false, 0);
  if (m_t1_isHadronic) {
    m_is_up_type = (std::abs(m_MC_Wdecay1_afterFSR_from_t1_pdgId) == 2 ||
                    std::abs(m_MC_Wdecay1_afterFSR_from_t1_pdgId) == 4);
    m_up_source = m_is_up_type ? "MC_Wdecay1_afterFSR_from_t1"
                               : "MC_Wdecay2_afterFSR_from_t1";
    m_down_source = m_is_up_type ? "MC_Wdecay2_afterFSR_from_t1"
                                 : "MC_Wdecay1_afterFSR_from_t1";
    handleHadronicDecay(m_history, m_partonAcc, m_up_source, m_down_source,
                        truth_jets, truth_electrons, truth_muons);
  } else {
    m_lepton_pdgId = std::abs(m_MC_Wdecay1_afterFSR_from_t1_pdgId);
    m_lep_source = (m_lepton_pdgId == 11 || m_lepton_pdgId == 13)
                       ? "MC_Wdecay1_afterFSR_from_t1"
                       : "MC_Wdecay2_afterFSR_from_t1";
    handleLeptonicDecay(m_history, m_partonAcc, m_lepton_pdgId, m_lep_source,
                        truth_jets, truth_electrons, truth_muons);
  }
  m_partonAcc.getPtEtaPhiMVector("MC_bbar_afterFSR_from_tbar1", m_bbar1,
                                 m_history);
  truth_jets.emplace_back(-1, m_bbar1, false, 0);
  if (m_tbar1_isHadronic) {
    m_is_up_type = (std::abs(m_MC_Wdecay1_afterFSR_from_tbar1_pdgId) == 2 ||
                    std::abs(m_MC_Wdecay1_afterFSR_from_tbar1_pdgId) == 4);
    m_up_source = m_is_up_type ? "MC_Wdecay1_afterFSR_from_tbar1"
                               : "MC_Wdecay2_afterFSR_from_tbar1";
    m_down_source = m_is_up_type ? "MC_Wdecay2_afterFSR_from_tbar1"
                                 : "MC_Wdecay1_afterFSR_from_tbar1";
    handleHadronicDecay(m_history, m_partonAcc, m_up_source, m_down_source,
                        truth_jets, truth_electrons, truth_muons);
  } else {
    m_lepton_pdgId = std::abs(m_MC_Wdecay1_afterFSR_from_tbar1_pdgId);
    m_lep_source = (m_lepton_pdgId == 11 || m_lepton_pdgId == 13)
                       ? "MC_Wdecay1_afterFSR_from_tbar1"
                       : "MC_Wdecay2_afterFSR_from_tbar1";
    handleLeptonicDecay(m_history, m_partonAcc, m_lepton_pdgId, m_lep_source,
                        truth_jets, truth_electrons, truth_muons);
  }
  m_partonAcc.getPtEtaPhiMVector("MC_b_afterFSR_from_t2", m_b2, m_history);
  truth_jets.emplace_back(-1, m_b2, false, 0);
  if (m_t2_isHadronic) {
    m_is_up_type = (std::abs(m_MC_Wdecay1_afterFSR_from_t2_pdgId) == 2 ||
                    std::abs(m_MC_Wdecay1_afterFSR_from_t2_pdgId) == 4);
    m_up_source = m_is_up_type ? "MC_Wdecay1_afterFSR_from_t2"
                               : "MC_Wdecay2_afterFSR_from_t2";
    m_down_source = m_is_up_type ? "MC_Wdecay2_afterFSR_from_t2"
                                 : "MC_Wdecay1_afterFSR_from_t2";
    handleHadronicDecay(m_history, m_partonAcc, m_up_source, m_down_source,
                        truth_jets, truth_electrons, truth_muons);
  } else {
    m_lepton_pdgId = std::abs(m_MC_Wdecay1_afterFSR_from_t2_pdgId);
    m_lep_source = (m_lepton_pdgId == 11 || m_lepton_pdgId == 13)
                       ? "MC_Wdecay1_afterFSR_from_t2"
                       : "MC_Wdecay2_afterFSR_from_t2";
    handleHadronicDecay(m_history, m_partonAcc, m_up_source, m_down_source,
                        truth_jets, truth_electrons, truth_muons);
  }
  m_partonAcc.getPtEtaPhiMVector("MC_bbar_afterFSR_from_tbar2", m_bbar2,
                                 m_history);
  truth_jets.emplace_back(-1, m_bbar2, false, 0);
  if (m_tbar2_isHadronic) {
    m_is_up_type = (std::abs(m_MC_Wdecay1_afterFSR_from_tbar2_pdgId) == 2 ||
                    std::abs(m_MC_Wdecay1_afterFSR_from_tbar2_pdgId) == 4);
    m_up_source = m_is_up_type ? "MC_Wdecay1_afterFSR_from_tbar2"
                               : "MC_Wdecay2_afterFSR_from_tbar2";
    m_down_source = m_is_up_type ? "MC_Wdecay2_afterFSR_from_tbar2"
                                 : "MC_Wdecay1_afterFSR_from_tbar2";
    handleHadronicDecay(m_history, m_partonAcc, m_up_source, m_down_source,
                        truth_jets, truth_electrons, truth_muons);
  } else {
    m_lepton_pdgId = std::abs(m_MC_Wdecay1_afterFSR_from_tbar2_pdgId);
    m_lep_source = (m_lepton_pdgId == 11 || m_lepton_pdgId == 13)
                       ? "MC_Wdecay1_afterFSR_from_tbar2"
                       : "MC_Wdecay2_afterFSR_from_tbar2";
    handleLeptonicDecay(m_history, m_partonAcc, m_lepton_pdgId, m_lep_source,
                        truth_jets, truth_electrons, truth_muons);
  }
  return true;
}
}  // namespace top
