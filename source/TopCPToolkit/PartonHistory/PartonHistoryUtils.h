#ifndef PARTONS_TOPPARTONUTILS_H
#define PARTONS_TOPPARTONUTILS_H

#include "xAODTruth/TruthParticleContainer.h"

#include <string>

namespace top {
  using ROOT::Math::PtEtaPhiMVector;
  
  class PartonHistory;

  namespace PartonHistoryUtils {
    ///Return particle after FSR (before the decay vertex)
    const xAOD::TruthParticle* findAfterFSR(const xAOD::TruthParticle* particle);

    ///Looking for tops without children -> must be broken
    bool isBrokenTop(const xAOD::TruthParticle* particle);

    ///Determine whether particle is afterFSR
    bool isAfterFSR(const xAOD::TruthParticle* particle);

    ///Return true when particle is a top before FSR
    bool hasParticleIdenticalParent(const xAOD::TruthParticle* particle);
  
  } // namespace PartonHistoryUtils
} // namespace top

#endif
