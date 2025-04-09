#include "TopCPToolkit/TutorialAlg.h"

namespace top {

  TutorialAlg::TutorialAlg(const std::string &name,
                           ISvcLocator *pSvcLocator)
    : EL::AnaAlgorithm(name, pSvcLocator) {}

  StatusCode TutorialAlg::initialize() {
    ANA_MSG_INFO("Initialising the algorithm for the TopCPToolkit tutorial");

    ANA_CHECK(m_systematicsList.initialize());

    return StatusCode::SUCCESS;
  }

  StatusCode TutorialAlg::execute() {

    // for (const auto &sys : m_systematicsList.systematicsVector()) {
    // }

    return StatusCode::SUCCESS;
  }

} //namespace top