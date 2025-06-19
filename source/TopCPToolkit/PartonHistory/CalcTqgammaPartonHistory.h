#ifndef PARTONS_CALCTQGAMMAPARTONHISTORY_H
#define PARTONS_CALCTQGAMMAPARTONHISTORY_H

// Framework include(s):
#include "PartonHistory/CalcPartonHistory.h"
#include "PartonHistory/PartonHistory.h"
#include "xAODTruth/TruthParticleContainer.h"

namespace top {
class CalcTqgammaPartonHistory : public CalcPartonHistory {
 public:
  explicit CalcTqgammaPartonHistory(
      const std::string& name,
      const std::vector<std::string>& truthCollections = {
          "TruthTop", "TruthBosonsWithDecayParticles", "TruthPhotons",
          "TruthBottom"});
  virtual ~CalcTqgammaPartonHistory() {}

  // Storing parton history for tqgamma/tllq resonance analysis
  CalcTqgammaPartonHistory(const CalcTqgammaPartonHistory& rhs) = delete;
  CalcTqgammaPartonHistory(CalcTqgammaPartonHistory&& rhs) = delete;
  CalcTqgammaPartonHistory& operator=(const CalcTqgammaPartonHistory& rhs) =
      delete;

 protected:
  virtual StatusCode runHistorySaver(
      const xAOD::TruthParticleContainer* truthParticles,
      xAOD::PartonHistory* tqgammaPartonHistory) override;

  virtual void initializeDecorators() override;
};
}  // namespace top

#endif
