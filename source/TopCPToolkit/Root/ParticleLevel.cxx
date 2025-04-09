#include "TopParticleLevel/ParticleLevel.h"

namespace top {

bool ParticleLevel::isLeptonFromTau(const xAOD::TruthParticle* truthParticle) {
  if (truthParticle->absPdgId() != 11 && truthParticle->absPdgId() != 12 &&
      truthParticle->absPdgId() != 13 && truthParticle->absPdgId() != 14 &&
      truthParticle->absPdgId() != 15 && truthParticle->absPdgId() != 16) {
    return false;
  }
  return isFrom(truthParticle, {15, -15}, true);
}

bool ParticleLevel::isFrom(const xAOD::TruthParticle* truthParticle,
                           const std::vector<int>& parentPDGIds,
                           bool bOnlyDirect) {
  // If the input does not have aproper production vertex reference or
  // there are no incoming particle to the production vertex, directly
  // return false
  if (!truthParticle->hasProdVtx() || !truthParticle->prodVtx() ||
      truthParticle->prodVtx()->nIncomingParticles() == 0) {
    return false;
  }

  // The current production vertex
  const xAOD::TruthVertex* prodVtx = truthParticle->prodVtx();
  // The previous production vertex (initialised to the current
  // production vertex)
  const xAOD::TruthVertex* prevProdVtx = prodVtx;

  auto numIncoming = prodVtx->nIncomingParticles();
  for (std::size_t motherIndex = 0; motherIndex < numIncoming; ++motherIndex) {
    const xAOD::TruthParticle* mother = nullptr;
    int motherPdgId = truthParticle->pdgId();

    // Ascend through the decay chain until we find the "actual"
    // decay, i.e. a change in the PDG ID. This skips all the
    // non-decay truth table entries which come from "particle
    // evolution" rather than physical decay
    while (truthParticle->pdgId() == motherPdgId) {
      mother = prodVtx->incomingParticle(motherIndex);
      if (mother) {
        motherPdgId = mother->pdgId();
      } else {
        break;
      }

      if (truthParticle->pdgId() != motherPdgId) {
        break;
      }

      // Include protection against cyclic or broken reference
      // chains which can occur in SHERPA samples
      if (!mother->hasProdVtx()) {
        break;
      }
      if (prevProdVtx == mother->prodVtx()) {
        break;
      }

      // Update the previous / current production vertex references
      prevProdVtx = prodVtx;
      prodVtx = mother->prodVtx();

      // safeguard
      if (!prodVtx || prodVtx->nIncomingParticles() == 0) {
        break;
      }
    }

    // Check that the mother pointer is valid. If it is not valid, then
    // the particle could not possibly come from any of the requested
    // PDG Ids, hence return false
    if (!mother) {
      return false;
    }

    // If the mother PDG ID is in the parentPDGIds collection, then return true
    if (std::find(parentPDGIds.begin(), parentPDGIds.end(), motherPdgId) !=
        parentPDGIds.end()) {
      return true;
    }

    // If we allow chained matching (I.e. not only _direct_ decays) and
    // the mother particle does come from the request PDG ID(s), return true
    if (!bOnlyDirect && isFrom(mother, parentPDGIds, bOnlyDirect)) {
      return true;
    }
  }

  // If we did't find an ancestor with the requested pdg id, return false
  return false;
}

}  // namespace top
