#ifndef PARTONS_CALCZPARTONHISTORY_H
#define PARTONS_CALCZPARTONHISTORY_H

// Framework include(s):
#include "PartonHistory/CalcPartonHistory.h"
#include "PartonHistory/PartonHistory.h"
#include "xAODTruth/TruthParticleContainer.h"

namespace top {
class CalcZbPartonHistory : public CalcPartonHistory {
 public:
  explicit CalcZbPartonHistory(
      const std::string& name,
      const std::vector<std::string>& truthCollections = {
          "TruthBottom", "TruthBosonsWithDecayParticles", "TruthMuons",
          "TruthElectrons"});
  virtual ~CalcZbPartonHistory() {}

  // Storing parton history for Zb process
  CalcZbPartonHistory(const CalcZbPartonHistory& rhs) = delete;
  CalcZbPartonHistory(CalcZbPartonHistory&& rhs) = delete;
  CalcZbPartonHistory& operator=(const CalcZbPartonHistory& rhs) = delete;

 protected:
  virtual StatusCode runHistorySaver(
      const xAOD::TruthParticleContainer* truthParticles,
      xAOD::PartonHistory* ZbPartonHistory) override;

  virtual void initializeDecorators() override;
};
}  // namespace top

#endif
