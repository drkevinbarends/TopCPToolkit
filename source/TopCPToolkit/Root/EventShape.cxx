#include "TopCPToolkit/EventShape.h"

#include <algorithm>

namespace top {

EventShape::EventShape(const std::string& name, ISvcLocator* pSvcLocator)
    : EL::AnaAlgorithm(name, pSvcLocator) {}

StatusCode EventShape::initialize() {
  ANA_MSG_INFO("Initializing EventShape " << name());

  ANA_CHECK(m_eventInfoHandle.initialize(m_systematicsList));
  ANA_CHECK(m_eventShapeHandle.initialize(m_systematicsList));
  ANA_CHECK(m_density_decor.initialize(m_systematicsList, m_eventInfoHandle,
                                       SG::AllowEmpty));

  ANA_CHECK(m_systematicsList.initialize());
  return StatusCode::SUCCESS;
}

StatusCode EventShape::execute() {
  for (const auto& sys : m_systematicsList.systematicsVector()) {
    // retrieve objects
    const xAOD::EventInfo* evtInfo = nullptr;
    ANA_CHECK(m_eventInfoHandle.retrieve(evtInfo, sys));

    const xAOD::EventShape* evtShape = nullptr;
    ANA_CHECK(m_eventShapeHandle.retrieve(evtShape, sys));
    double density = 0.;

    // default-decorate EventInfo
    m_density_decor.set(*evtInfo, -9999., sys);

    evtShape->getDensity(xAOD::EventShape::Density, density);

    m_density_decor.set(*evtInfo, density, sys);
  }
  return StatusCode::SUCCESS;
}

StatusCode EventShape::finalize() {
  ANA_MSG_INFO("Finalizing EventShape");

  return StatusCode::SUCCESS;
}

}  // namespace top
