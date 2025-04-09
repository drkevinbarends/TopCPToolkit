#ifndef PARTONS_CALCHZZPARTONHISTORY_H
#define PARTONS_CALCHZZPARTONHISTORY_H

// Framework include(s):
#include "PartonHistory/CalcPartonHistory.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "PartonHistory/PartonHistory.h"

namespace top {
  class CalcHZZPartonHistory: public CalcPartonHistory {
  public:
    explicit CalcHZZPartonHistory(const std::string &name,
				  const std::vector<std::string>& truthCollections = {"TruthBosonsWithDecayParticles"});
    virtual ~CalcHZZPartonHistory() {}

    //Storing parton history for H->ZZ* resonance analysis
    CalcHZZPartonHistory(const CalcHZZPartonHistory& rhs) = delete;
    CalcHZZPartonHistory(CalcHZZPartonHistory&& rhs) = delete;
    CalcHZZPartonHistory& operator = (const CalcHZZPartonHistory& rhs) = delete;

  protected:
    virtual StatusCode runHistorySaver(const xAOD::TruthParticleContainer* truthParticles,
                                       xAOD::PartonHistory* hzzPartonHistory) override;

    virtual void initializeDecorators() override;
  };
}

#endif
