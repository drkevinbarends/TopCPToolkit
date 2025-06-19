#include "TopCPToolkit/ZTopology.h"

#include "AthContainers/ConstDataVector.h"
#include "PartonHistory/PartonHistory.h"
#include "VectorHelpers/LorentzHelper.h"

namespace top {
/**
 * @brief Default constructor for the ZTopology class.
 */
ZTopology::ZTopology() : m_is_up_type(false) {}

/**
 * @brief Validates the parton decay topology of the Z system.
 *
 * This method retrieves the decay PDG IDs of the W bosons (after FSR) from
 * the top and anti-top quarks. It checks for hadronic decays and counts the
 * number of leptons. If any PDG ID has an invalid value (0), the topology is
 * considered invalid.
 *
 * @return True if the topology is valid; otherwise, False.
 */
bool ZTopology::checkTopology() {
  // Accessors for PDG IDs of W decay products (after FSR)
  static const SG::AuxElement::ConstAccessor<int> acc_MC_Zdecay1_afterFSR_pdgId(
      "MC_Zdecay1_afterFSR_pdgId");
  static const SG::AuxElement::ConstAccessor<int> acc_MC_Zdecay2_afterFSR_pdgId(
      "MC_Zdecay2_afterFSR_pdgId");
  setNLeptons(0);
  m_Z_isHadronic = false;

  // Retrieve PDG IDs from the parton history
  m_MC_Zdecay1_afterFSR_pdgId = acc_MC_Zdecay1_afterFSR_pdgId(*m_history);
  m_MC_Zdecay2_afterFSR_pdgId = acc_MC_Zdecay2_afterFSR_pdgId(*m_history);

  // Check if the Z boson decays hadronically (quark PDG IDs are < 6)
  if ((std::abs(m_MC_Zdecay1_afterFSR_pdgId) < 6) ||
      (std::abs(m_MC_Zdecay2_afterFSR_pdgId) < 6))
    m_Z_isHadronic = true;

  // Count leptonic decays
  if (!m_Z_isHadronic)
    setNLeptons(getNLeptons() + 2);

  // Check for invalid PDG ID values
  if (m_MC_Zdecay1_afterFSR_pdgId == 0)
    return false;
  if (m_MC_Zdecay2_afterFSR_pdgId == 0)
    return false;
  return true;  // Topology is valid
}

/**
 * @brief Fills the Z topology information.
 *
 * This method initializes parton accessors for retrieving parton four-vectors.
 * It checks the topology using `checkTopology()` and processes the decay modes
 * (hadronic or leptonic) for the top and anti-top quarks. Truth jets,
 * electrons, and muons are filled accordingly.
 *
 * @return True if the topology is successfully filled; otherwise, False.
 */
bool ZTopology::fillTopology(std::vector<TruthObject>& truth_jets,
                             std::vector<TruthObject>& truth_electrons,
                             std::vector<TruthObject>& truth_muons) {
  // Initialize parton accessors for relevant partons
  m_partonAcc.initializePtEtaPhiMAccessor("MC_Zdecay1_afterFSR");
  m_partonAcc.initializePtEtaPhiMAccessor("MC_Zdecay2_afterFSR");

  // Validate the topology
  if (!checkTopology())
    return false;

  if (m_Z_isHadronic) {  // Hadronic decay
    m_is_up_type = (std::abs(m_MC_Zdecay1_afterFSR_pdgId) == 2 ||
                    std::abs(m_MC_Zdecay1_afterFSR_pdgId) == 4);
    m_up_source = m_is_up_type ? "MC_Zdecay1_afterFSR" : "MC_Zdecay2_afterFSR";
    m_down_source =
        m_is_up_type ? "MC_Zdecay2_afterFSR" : "MC_Zdecay1_afterFSR";
    handleHadronicDecay(m_history, m_partonAcc, m_up_source, m_down_source,
                        truth_jets, truth_electrons, truth_muons);
  } else {  // Leptonic decay
    m_lepton_pdgId = std::abs(m_MC_Zdecay1_afterFSR_pdgId);
    m_lep_source = (m_lepton_pdgId == 11 || m_lepton_pdgId == 13)
                       ? "MC_Zdecay1_afterFSR"
                       : "MC_Zdecay2_afterFSR";
    handleLeptonicDecay(m_history, m_partonAcc, m_lepton_pdgId, m_lep_source,
                        truth_jets, truth_electrons, truth_muons);
  }
  return true;  // Topology successfully filled
}
}  // namespace top
