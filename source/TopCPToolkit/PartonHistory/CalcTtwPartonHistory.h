#ifndef PARTONS_CALCTTWPARTONHISTORY_H
#define PARTONS_CALCTTWPARTONHISTORY_H

// Framework include(s):
#include "PartonHistory/CalcPartonHistory.h"
#include "PartonHistory/CalcTtbarPartonHistory.h"
#include "PartonHistory/PartonHistory.h"
#include "xAODTruth/TruthParticleContainer.h"

namespace top {
class CalcTtwPartonHistory : public CalcTtbarPartonHistory {
 public:
  explicit CalcTtwPartonHistory(
      const std::string& name,
      const std::vector<std::string>& truthCollections = {
          "TruthTop", "TruthBosonsWithDecayParticles"});
  virtual ~CalcTtwPartonHistory() {}

  // Storing parton history for ttw/ttll resonance analysis
  CalcTtwPartonHistory(const CalcTtwPartonHistory& rhs) = delete;
  CalcTtwPartonHistory(CalcTtwPartonHistory&& rhs) = delete;
  CalcTtwPartonHistory& operator=(const CalcTtwPartonHistory& rhs) = delete;

 protected:
  virtual StatusCode runHistorySaver(
      const xAOD::TruthParticleContainer* truthParticles,
      xAOD::PartonHistory* ttwPartonHistory) override;

  virtual void initializeDecorators() override;
};
}  // namespace top

#endif
