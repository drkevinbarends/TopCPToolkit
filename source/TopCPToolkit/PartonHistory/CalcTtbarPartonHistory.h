#ifndef PARTONS_CALCTTBARPARTONHISTORY_H
#define PARTONS_CALCTTBARPARTONHISTORY_H

// Framework include(s):
#include "PartonHistory/CalcPartonHistory.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "PartonHistory/PartonHistory.h"
#include "VectorHelpers/DecoratorHelpers.h"

namespace top {
  class CalcTtbarPartonHistory: public CalcPartonHistory {
  public:
    explicit CalcTtbarPartonHistory(const std::string &name,
                                    const std::vector<std::string>& truthCollections = {"TruthTop"});
    virtual ~CalcTtbarPartonHistory() {}

    //Storing parton history for ttbar resonance analysis
    CalcTtbarPartonHistory(const CalcTtbarPartonHistory& rhs) = delete;
    CalcTtbarPartonHistory(CalcTtbarPartonHistory&& rhs) = delete;
    CalcTtbarPartonHistory& operator = (const CalcTtbarPartonHistory& rhs) = delete;

  protected:
    virtual StatusCode runHistorySaver(const xAOD::TruthParticleContainer* truthParticles,
                                       xAOD::PartonHistory* ttbarPartonHistory) override;

    virtual void initializeDecorators() override;
  };
}

#endif
