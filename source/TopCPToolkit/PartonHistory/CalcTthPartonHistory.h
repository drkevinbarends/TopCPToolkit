#ifndef PARTONS_CALCTTHPARTONHISTORY_H
#define PARTONS_CALCTTHPARTONHISTORY_H

// Framework include(s):
#include "PartonHistory/CalcPartonHistory.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "PartonHistory/PartonHistory.h"

namespace top {
  class CalcTthPartonHistory: public CalcPartonHistory {
  public:
    explicit CalcTthPartonHistory(const std::string &name,
				  const std::vector<std::string>& truthCollections = {"TruthTop","TruthBosonsWithDecayParticles"});
    virtual ~CalcTthPartonHistory() {}

    //Storing parton history for ttH resonance analysis
    CalcTthPartonHistory(const CalcTthPartonHistory& rhs) = delete;
    CalcTthPartonHistory(CalcTthPartonHistory&& rhs) = delete;
    CalcTthPartonHistory& operator = (const CalcTthPartonHistory& rhs) = delete;

  protected:
    virtual StatusCode runHistorySaver(const xAOD::TruthParticleContainer* truthParticles,
                                       xAOD::PartonHistory* tthPartonHistory) override;

    virtual void initializeDecorators() override;
  };
}

#endif
