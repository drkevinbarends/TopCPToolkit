#ifndef PARTONS_CALCTTHPARTONHISTORY_H
#define PARTONS_CALCTTHPARTONHISTORY_H

// Framework include(s):
#include "PartonHistory/CalcPartonHistory.h"
#include "PartonHistory/CalcTtbarPartonHistory.h"
#include "PartonHistory/PartonHistory.h"
#include "xAODTruth/TruthParticleContainer.h"

namespace top {
class CalcTthPartonHistory : public CalcTtbarPartonHistory {
 public:
  explicit CalcTthPartonHistory(
      const std::string& name,
      const std::vector<std::string>& truthCollections = {
          "TruthTop", "TruthBosonsWithDecayParticles", "TruthBottom"});
  virtual ~CalcTthPartonHistory() {}

  // Storing parton history for ttH resonance analysis
  CalcTthPartonHistory(const CalcTthPartonHistory& rhs) = delete;
  CalcTthPartonHistory(CalcTthPartonHistory&& rhs) = delete;
  CalcTthPartonHistory& operator=(const CalcTthPartonHistory& rhs) = delete;

 protected:
  virtual StatusCode runHistorySaver(
      const xAOD::TruthParticleContainer* truthParticles,
      xAOD::PartonHistory* tthPartonHistory) override;

  virtual void initializeDecorators() override;
};
}  // namespace top

#endif
