#include "TopCPToolkit/BaseTopology.h"

namespace top {
using ROOT::Math::PtEtaPhiMVector;

/**
 * @brief Default constructor for the BaseTopology class.
 */
BaseTopology::BaseTopology() : m_history(nullptr) {}

/**
 * @brief Sets the parton history pointer for the topology.
 *
 * @param Pointer to the xAOD::PartonHistory object, which contains parton-level
 * truth information.
 */
void BaseTopology::setHistory(const xAOD::PartonHistory* history) {
  m_history = history;
}

/**
 * @brief Handles the hadronic decay scenario and updates truth jet collections.
 *
 * This method retrieves two parton vectors (up and down) using the provided
 * sources and adds them as truth jets to the internal collections. It also
 * initializes empty truth electron and muon placeholders.
 *
 * @param history Pointer to the xAOD::PartonHistory object for truth-level
 * access.
 * @param partonAcc Reference to a PartonAccessor object to extract
 * PtEtaPhiMVector data.
 * @param up_source Name of the source for the "up" parton vector.
 * @param down_source Name of the source for the "down" parton vector.
 */
void BaseTopology::handleHadronicDecay(
    const xAOD::PartonHistory* history, PartonAccessor& partonAcc,
    const std::string& up_source, const std::string& down_source,
    std::vector<TruthObject>& truth_jets,
    std::vector<TruthObject>& truth_electrons,
    std::vector<TruthObject>& truth_muons) {
  PtEtaPhiMVector up, down;

  // Extract parton vectors using the accessor
  partonAcc.getPtEtaPhiMVector(up_source, up, history);
  partonAcc.getPtEtaPhiMVector(down_source, down, history);

  // Add the up and down vectors as truth jets
  truth_jets.emplace_back(-1, up, false, 0);
  truth_jets.emplace_back(-1, down, false, 0);

  // Add empty truth electrons and muons as placeholders
  truth_electrons.emplace_back(-1, PtEtaPhiMVector(0, 0, 0, 0), true, 0);
  truth_muons.emplace_back(-1, PtEtaPhiMVector(0, 0, 0, 0), true, 0);
}

/**
 * @brief Handles the leptonic decay scenario and updates truth electron/muon
 * collections.
 *
 * This method checks the lepton PDG ID to determine if the particle is an
 * electron or muon. It retrieves the corresponding parton vector and updates
 * the internal truth electron or muon collection accordingly. Empty jets are
 * added as placeholders.
 *
 * @param history Pointer to the xAOD::PartonHistory object for truth-level
 * access.
 * @param partonAcc Reference to a PartonAccessor object to extract
 * PtEtaPhiMVector data.
 * @param lepton_pdgId PDG ID of the lepton (11 for electron, 13 for muon).
 * @param lep_source Name of the source for the lepton parton vector.
 */
void BaseTopology::handleLeptonicDecay(
    const xAOD::PartonHistory* history, PartonAccessor& partonAcc,
    int lepton_pdgId, const std::string& lep_source,
    std::vector<TruthObject>& truth_jets,
    std::vector<TruthObject>& truth_electrons,
    std::vector<TruthObject>& truth_muons) {
  PtEtaPhiMVector down;

  // Retrieve the parton vector for the lepton
  partonAcc.getPtEtaPhiMVector(lep_source, down, history);
  if (lepton_pdgId == 11) {  // Electron
    truth_electrons.emplace_back(-1, down, false, 0);
    truth_muons.emplace_back(-1, PtEtaPhiMVector(0, 0, 0, 0), true, 0);
  } else if (lepton_pdgId == 13) {  // Muon
    truth_electrons.emplace_back(-1, PtEtaPhiMVector(0, 0, 0, 0), true, 0);
    truth_muons.emplace_back(-1, down, false, 0);
  } else if (lepton_pdgId == 15) {  // Tau - no matching is performed
    truth_electrons.emplace_back(-1, PtEtaPhiMVector(0, 0, 0, 0), true, 0);
    truth_muons.emplace_back(-1, PtEtaPhiMVector(0, 0, 0, 0), true, 0);
  }

  // Add empty truth jets as placeholders
  truth_jets.emplace_back(-1, PtEtaPhiMVector(0, 0, 0, 0), true, 0);
  truth_jets.emplace_back(-1, PtEtaPhiMVector(0, 0, 0, 0), true, 0);
}
}  // namespace top
