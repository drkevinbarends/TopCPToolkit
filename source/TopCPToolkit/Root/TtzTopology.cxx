#include "TopCPToolkit/TtzTopology.h"

#include "AthContainers/ConstDataVector.h"
#include "PartonHistory/PartonHistory.h"
#include "VectorHelpers/LorentzHelper.h"

namespace top {

/**
 * @brief Default constructor for the TtzTopology class.
 */
TtzTopology::TtzTopology() {}

/**
 * @brief Validates the parton decay topology of the Ttz system.
 *
 * This method retrieves the decay PDG IDs of the W bosons (after FSR) from
 * the top and anti-top quarks. It checks for hadronic decays and counts the
 * number of leptons. If any PDG ID has an invalid value (0), the topology is
 * considered invalid.
 *
 * @return True if the topology is valid; otherwise, False.
 */
bool TtzTopology::checkTopology() {
  bool ttbar_topology_check = TtbarTopology::checkTopology();
  bool Z_topology_check = ZTopology::checkTopology();
  return ttbar_topology_check && Z_topology_check;  // Topology is valid
}

/**
 * @brief Fills the Ttz topology information.
 *
 * This method initializes parton accessors for retrieving parton four-vectors.
 * It checks the topology using `checkTopology()` and processes the decay modes
 * (hadronic or leptonic) for the top and anti-top quarks. Truth jets,
 * electrons, and muons are filled accordingly.
 *
 * @return True if the topology is successfully filled; otherwise, False.
 */
bool TtzTopology::fillTopology(std::vector<TruthObject>& truth_jets,
                               std::vector<TruthObject>& truth_electrons,
                               std::vector<TruthObject>& truth_muons) {
  bool ttbar_filled =
      TtbarTopology::fillTopology(truth_jets, truth_electrons, truth_muons);
  bool Z_filled =
      ZTopology::fillTopology(truth_jets, truth_electrons, truth_muons);
  return ttbar_filled && Z_filled;  // Topology successfully filled
}
}  // namespace top
