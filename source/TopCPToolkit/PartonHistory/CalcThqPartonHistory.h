#ifndef PARTONS_CALCTHQPARTONHISTORY_H
#define PARTONS_CALCTHQPARTONHISTORY_H

// Framework include(s):
#include "PartonHistory/CalcPartonHistory.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "PartonHistory/PartonHistory.h"

namespace top {
  class CalcThqPartonHistory: public CalcPartonHistory {
  public:
    explicit CalcThqPartonHistory(const std::string &name,
				  const std::vector<std::string>& truthCollections = {"TruthTop","TruthBosonsWithDecayParticles", "TruthBottom"});
    virtual ~CalcThqPartonHistory() {}

    //Storing parton history for thq/tllq resonance analysis
    CalcThqPartonHistory(const CalcThqPartonHistory& rhs) = delete;
    CalcThqPartonHistory(CalcThqPartonHistory&& rhs) = delete;
    CalcThqPartonHistory& operator = (const CalcThqPartonHistory& rhs) = delete;

  protected:
    virtual StatusCode runHistorySaver(const xAOD::TruthParticleContainer* truthParticles,
                                       xAOD::PartonHistory* thqPartonHistory) override;

    virtual void initializeDecorators() override;
  };
}

#endif
