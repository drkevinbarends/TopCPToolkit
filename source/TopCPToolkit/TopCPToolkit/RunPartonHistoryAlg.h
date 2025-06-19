#ifndef PARTONS_RUNPARTONHISTORYALG_H
#define PARTONS_RUNPARTONHISTORYALG_H

#include <AnaAlgorithm/AnaAlgorithm.h>

// EDM include(s):
#include <vector>

#include "AthContainers/AuxElement.h"
#include "AthContainers/DataVector.h"
#include "xAODCore/AuxContainerBase.h"
#include "xAODCore/CLASS_DEF.h"

// TopParton histories
#include "PartonHistory/CalcFourTopPartonHistory.h"
#include "PartonHistory/CalcPartonHistory.h"
#include "PartonHistory/CalcThqPartonHistory.h"
#include "PartonHistory/CalcTqgammaPartonHistory.h"
#include "PartonHistory/CalcTtbarPartonHistory.h"
#include "PartonHistory/CalcTtbarbbbarPartonHistory.h"
#include "PartonHistory/CalcTtgammaPartonHistory.h"
#include "PartonHistory/CalcTthPartonHistory.h"
#include "PartonHistory/CalcTtwPartonHistory.h"
#include "PartonHistory/CalcTtzPartonHistory.h"
#include "PartonHistory/CalcTzqPartonHistory.h"

// Zb parton history
#include "PartonHistory/CalcZbPartonHistory.h"

// HiggsParton histories
#include "PartonHistory/CalcHWWPartonHistory.h"
#include "PartonHistory/CalcHWWnonresonantPartonHistory.h"
#include "PartonHistory/CalcHZZPartonHistory.h"

namespace top {

class RunPartonHistoryAlg final : public EL::AnaAlgorithm {

 public:
  RunPartonHistoryAlg(const std::string &name, ISvcLocator *pSvcLocator);
  virtual StatusCode initialize() override;
  virtual StatusCode execute() override;
  virtual StatusCode finalize() override;

 private:
  std::string m_PartonScheme;
  std::unique_ptr<CalcPartonHistory> m_PartonHistory;
};

}  // namespace top

#endif
