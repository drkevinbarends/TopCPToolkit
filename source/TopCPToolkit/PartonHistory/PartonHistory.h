#ifndef PARTONS_PARTONHISTORY_H
#define PARTONS_PARTONHISTORY_H

// EDM include(s):
#include "AthContainers/DataVector.h"
#include "AthContainers/AuxElement.h"
#include "xAODCore/AuxInfoBase.h"
#include "xAODCore/AuxContainerBase.h"
#include "xAODCore/CLASS_DEF.h"
#include <vector>

namespace xAOD {
  /// Aux Container
  class PartonHistoryAuxContainer: public AuxContainerBase {
  public:
    /// Default constructor
    PartonHistoryAuxContainer();
  }; // end Aux container

  /// Interface class
  class PartonHistory: public SG::AuxElement {
  public:
    /// Default constructor
    PartonHistory();

#ifdef XAOD_STANDALONE
    /// Default desturctor
    virtual ~PartonHistory() {} // avoid virtual methods until NtupleMaker no longer using reinterpret_cast (see https://gitlab.cern.ch/atlas/athena/-/merge_requests/71369)
#endif

  };

  class PartonHistoryAux: public AuxInfoBase {
  public:
    PartonHistoryAux();

#ifdef XAOD_STANDALONE
    virtual ~PartonHistoryAux() {}
#endif
  };

  typedef DataVector < xAOD::PartonHistory > PartonHistoryContainer;
}

#include "xAODCore/BaseInfo.h"
SG_BASE( xAOD::PartonHistory, SG::AuxElement );
SG_BASE( xAOD::PartonHistoryAux, xAOD::AuxInfoBase );
SG_BASE( xAOD::PartonHistoryAuxContainer, xAOD::AuxContainerBase );

// Dictonaries
CLASS_DEF(xAOD::PartonHistory, 135846343, 1)
CLASS_DEF(xAOD::PartonHistoryAux, 151403368, 1)
CLASS_DEF(xAOD::PartonHistoryContainer, 1219079565, 1)
CLASS_DEF(xAOD::PartonHistoryAuxContainer, 1244378748, 1)

#endif
