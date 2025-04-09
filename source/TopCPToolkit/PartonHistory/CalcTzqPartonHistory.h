#ifndef PARTONS_CALCTZQPARTONHISTORY_H
#define PARTONS_CALCTZQPARTONHISTORY_H

// Framework include(s):
#include "PartonHistory/CalcPartonHistory.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "PartonHistory/PartonHistory.h"

namespace top {
  class CalcTzqPartonHistory: public CalcPartonHistory {
  public:
    explicit CalcTzqPartonHistory(const std::string &name,
				  const std::vector<std::string>& truthCollections = {"TruthTop","TruthBosonsWithDecayParticles", "TruthBottom"});
    virtual ~CalcTzqPartonHistory() {}

    //Storing parton history for tZq/tllq resonance analysis
    CalcTzqPartonHistory(const CalcTzqPartonHistory& rhs) = delete;
    CalcTzqPartonHistory(CalcTzqPartonHistory&& rhs) = delete;
    CalcTzqPartonHistory& operator = (const CalcTzqPartonHistory& rhs) = delete;

  protected:
    virtual StatusCode runHistorySaver(const xAOD::TruthParticleContainer* truthParticles,
                                       xAOD::PartonHistory* tzqPartonHistory) override;

    virtual void initializeDecorators() override;
  };
}

#endif
