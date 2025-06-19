#ifndef PARTONS_CALCTTBARBBBARPARTONHISTORY_H
#define PARTONS_CALCTTBARBBBARPARTONHISTORY_H

// Framework include(s):
#include "PartonHistory/CalcPartonHistory.h"
#include "PartonHistory/CalcTtbarPartonHistory.h"
#include "PartonHistory/PartonHistory.h"
#include "VectorHelpers/DecoratorHelpers.h"
#include "xAODTruth/TruthParticleContainer.h"

namespace top {
class CalcTtbarbbbarPartonHistory : public CalcTtbarPartonHistory {
 public:
  explicit CalcTtbarbbbarPartonHistory(
      const std::string& name,
      const std::vector<std::string>& truthCollections = {"TruthTop",
                                                          "TruthBottom"});
  virtual ~CalcTtbarbbbarPartonHistory() {}

  // Storing parton history for ttbar+bbbar analysis
  CalcTtbarbbbarPartonHistory(const CalcTtbarbbbarPartonHistory& rhs) = delete;
  CalcTtbarbbbarPartonHistory(CalcTtbarbbbarPartonHistory&& rhs) = delete;
  CalcTtbarbbbarPartonHistory& operator=(
      const CalcTtbarbbbarPartonHistory& rhs) = delete;

 protected:
  virtual StatusCode runHistorySaver(
      const xAOD::TruthParticleContainer* truthParticles,
      xAOD::PartonHistory* ttbarbbbarPartonHistory) override;

  virtual void initializeDecorators() override;
};
}  // namespace top

#endif
