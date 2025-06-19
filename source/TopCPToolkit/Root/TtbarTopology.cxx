#include "TopCPToolkit/TtbarTopology.h"

#include "AthContainers/ConstDataVector.h"
#include "PartonHistory/PartonHistory.h"
#include "VectorHelpers/LorentzHelper.h"

namespace top {

/**
 * @brief Default constructor for the TtbarTopology class.
 */
TtbarTopology::TtbarTopology() : m_is_up_type(false) {}

/**
 * @brief Validates the parton decay topology of the Ttbar system.
 *
 * This method retrieves the decay PDG IDs of the W bosons (after FSR) from
 * the top and anti-top quarks. It checks for hadronic decays and counts the
 * number of leptons. If any PDG ID has an invalid value (0), the topology is
 * considered invalid.
 *
 * @return True if the topology is valid; otherwise, False.
 */
bool TtbarTopology::checkTopology() {
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

  // Check if the W boson decays hadronically (quark PDG IDs are < 6)
  if ((std::abs(m_MC_Wdecay1_afterFSR_from_t_pdgId) < 6) ||
      (std::abs(m_MC_Wdecay2_afterFSR_from_t_pdgId) < 6))
    m_t_isHadronic = true;

  if ((std::abs(m_MC_Wdecay1_afterFSR_from_tbar_pdgId) < 6) ||
      (std::abs(m_MC_Wdecay2_afterFSR_from_tbar_pdgId) < 6))
    m_tbar_isHadronic = true;

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

  return true;  // Topology is valid
}

/**
 * @brief Fills the Ttbar topology information.
 *
 * This method initializes parton accessors for retrieving parton four-vectors.
 * It checks the topology using `checkTopology()` and processes the decay modes
 * (hadronic or leptonic) for the top and anti-top quarks. Truth jets,
 * electrons, and muons are filled accordingly.
 *
 * @return True if the topology is successfully filled; otherwise, False.
 */
bool TtbarTopology::fillTopology(std::vector<TruthObject>& truth_jets,
                                 std::vector<TruthObject>& truth_electrons,
                                 std::vector<TruthObject>& truth_muons) {
  // Initialize parton accessors for relevant partons
  m_partonAcc.initializePtEtaPhiMAccessor("MC_b_afterFSR_from_t");
  m_partonAcc.initializePtEtaPhiMAccessor("MC_bbar_afterFSR_from_tbar");
  m_partonAcc.initializePtEtaPhiMAccessor("MC_Wdecay1_afterFSR_from_t");
  m_partonAcc.initializePtEtaPhiMAccessor("MC_Wdecay2_afterFSR_from_t");
  m_partonAcc.initializePtEtaPhiMAccessor("MC_Wdecay1_afterFSR_from_tbar");
  m_partonAcc.initializePtEtaPhiMAccessor("MC_Wdecay2_afterFSR_from_tbar");

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
  return true;  // Topology successfully filled
}
}  // namespace top
