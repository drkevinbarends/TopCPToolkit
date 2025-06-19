#ifndef PARTONS_CALCTTZPARTONHISTORY_H
#define PARTONS_CALCTTZPARTONHISTORY_H

// Framework include(s):
#include "PartonHistory/CalcPartonHistory.h"
#include "PartonHistory/CalcTtbarPartonHistory.h"
#include "PartonHistory/PartonHistory.h"
#include "xAODTruth/TruthParticleContainer.h"

namespace top {
class CalcTtzPartonHistory : public CalcTtbarPartonHistory {
 public:
  explicit CalcTtzPartonHistory(
      const std::string& name,
      const std::vector<std::string>& truthCollections = {
          "TruthTop", "TruthBosonsWithDecayParticles", "TruthElectrons",
          "TruthMuons", "TruthTaus"});
  virtual ~CalcTtzPartonHistory() {}

  // Storing parton history for ttZ/ttll resonance analysis
  CalcTtzPartonHistory(const CalcTtzPartonHistory& rhs) = delete;
  CalcTtzPartonHistory(CalcTtzPartonHistory&& rhs) = delete;
  CalcTtzPartonHistory& operator=(const CalcTtzPartonHistory& rhs) = delete;

 protected:
  virtual StatusCode runHistorySaver(
      const xAOD::TruthParticleContainer* truthParticles,
      xAOD::PartonHistory* ttzPartonHistory) override;

  virtual void initializeDecorators() override;
};
}  // namespace top

#endif
