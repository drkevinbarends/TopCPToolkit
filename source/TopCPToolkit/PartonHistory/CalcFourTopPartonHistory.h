#ifndef PARTONS_CALCFOURTOPPARTONHISTORY_H
#define PARTONS_CALCFOURTOPPARTONHISTORY_H

// Framework include(s):
#include "PartonHistory/CalcPartonHistory.h"
#include "PartonHistory/PartonHistory.h"
#include "VectorHelpers/DecoratorHelpers.h"
#include "xAODTruth/TruthParticleContainer.h"

namespace top {
class CalcFourTopPartonHistory : public CalcPartonHistory {
 public:
  explicit CalcFourTopPartonHistory(
      const std::string& name,
      const std::vector<std::string>& truthCollections = {"TruthTop"});
  virtual ~CalcFourTopPartonHistory() {}

  // Storing parton history for 4t resonance analysis
  CalcFourTopPartonHistory(const CalcFourTopPartonHistory& rhs) = delete;
  CalcFourTopPartonHistory(CalcFourTopPartonHistory&& rhs) = delete;
  CalcFourTopPartonHistory& operator=(const CalcFourTopPartonHistory& rhs) =
      delete;

 protected:
  virtual StatusCode runHistorySaver(
      const xAOD::TruthParticleContainer* truthParticles,
      xAOD::PartonHistory* ttbarPartonHistory) override;

  virtual void initializeDecorators() override;
};
}  // namespace top

#endif
