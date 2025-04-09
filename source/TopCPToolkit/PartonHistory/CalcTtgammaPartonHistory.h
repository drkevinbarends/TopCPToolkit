#ifndef PARTONS_CALCTTGAMMAPARTONHISTORY_H
#define PARTONS_CALCTTGAMMAPARTONHISTORY_H

// Framework include(s):
#include "PartonHistory/CalcPartonHistory.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "PartonHistory/PartonHistory.h"

namespace top {
  class CalcTtgammaPartonHistory: public CalcPartonHistory {
  public:
    explicit CalcTtgammaPartonHistory(const std::string &name,
				  const std::vector<std::string>& truthCollections = {"TruthTop", "TruthBosonsWithDecayParticles", "TruthPhotons"});
    virtual ~CalcTtgammaPartonHistory() {}

    //Storing parton history for ttgamma analysis
    CalcTtgammaPartonHistory(const CalcTtgammaPartonHistory& rhs) = delete;
    CalcTtgammaPartonHistory(CalcTtgammaPartonHistory&& rhs) = delete;
    CalcTtgammaPartonHistory& operator = (const CalcTtgammaPartonHistory& rhs) = delete;

  protected:
    virtual StatusCode runHistorySaver(const xAOD::TruthParticleContainer* truthParticles,
                                       xAOD::PartonHistory* ttgammaPartonHistory) override;

    virtual void initializeDecorators() override;
  };
}

#endif
