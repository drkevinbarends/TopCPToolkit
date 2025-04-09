#ifndef PARTONS_CALCHWWPARTONHISTORY_H
#define PARTONS_CALCHWWPARTONHISTORY_H

// Framework include(s):
#include "PartonHistory/CalcPartonHistory.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "PartonHistory/PartonHistory.h"

namespace top {
  class CalcHWWPartonHistory: public CalcPartonHistory {
  public:
    explicit CalcHWWPartonHistory(const std::string &name,
				  const std::vector<std::string>& truthCollections = {"TruthBosonsWithDecayParticles"});
    virtual ~CalcHWWPartonHistory() {}

    //Storing parton history for H->WW* resonance analysis
    CalcHWWPartonHistory(const CalcHWWPartonHistory& rhs) = delete;
    CalcHWWPartonHistory(CalcHWWPartonHistory&& rhs) = delete;
    CalcHWWPartonHistory& operator = (const CalcHWWPartonHistory& rhs) = delete;

  protected:
    virtual StatusCode runHistorySaver(const xAOD::TruthParticleContainer* truthParticles,
                                       xAOD::PartonHistory* hwwPartonHistory) override;

    virtual void initializeDecorators() override;
  };
}

#endif
