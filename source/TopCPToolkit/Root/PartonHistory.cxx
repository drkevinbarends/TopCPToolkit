#include "PartonHistory/PartonHistory.h"
#include "xAODCore/AddDVProxy.h"

namespace xAOD {
  /// Aux Container
  PartonHistoryAuxContainer::PartonHistoryAuxContainer() :
    AuxContainerBase() {
  }

  /// Interface class
  PartonHistory::PartonHistory() :
    SG::AuxElement() {
  }

  PartonHistoryAux::PartonHistoryAux() :
    AuxInfoBase() {
  }
}
//ADD_NS_DV_PROXY(xAOD, PartonHistoryContainer);
