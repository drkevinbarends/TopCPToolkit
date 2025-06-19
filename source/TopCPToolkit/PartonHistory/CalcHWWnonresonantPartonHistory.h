#ifndef PARTONS_CALCHWWNONRESONANTPARTONHISTORY_H
#define PARTONS_CALCHWWNONRESONANTPARTONHISTORY_H

// Framework include(s):
#include "PartonHistory/CalcPartonHistory.h"
#include "PartonHistory/PartonHistory.h"
#include "xAODTruth/TruthParticleContainer.h"

namespace top {
class CalcHWWnonresonantPartonHistory : public CalcPartonHistory {
 public:
  explicit CalcHWWnonresonantPartonHistory(
      const std::string& name,
      const std::vector<std::string>& truthCollections = {
          "TruthBosonsWithDecayParticles", "TruthElectrons", "TruthMuons",
          "TruthTaus", "TruthNeutrinos"});
  virtual ~CalcHWWnonresonantPartonHistory() {}

  // Storing parton history for H->WW* resonance analysis
  CalcHWWnonresonantPartonHistory(const CalcHWWnonresonantPartonHistory& rhs) =
      delete;
  CalcHWWnonresonantPartonHistory(CalcHWWnonresonantPartonHistory&& rhs) =
      delete;
  CalcHWWnonresonantPartonHistory& operator=(
      const CalcHWWnonresonantPartonHistory& rhs) = delete;

 protected:
  virtual StatusCode runHistorySaver(
      const xAOD::TruthParticleContainer* truthParticles,
      xAOD::PartonHistory* hwwnonresonantPartonHistory) override;

  virtual void initializeDecorators() override;
};
}  // namespace top

#endif
