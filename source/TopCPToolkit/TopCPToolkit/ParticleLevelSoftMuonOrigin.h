#ifndef TRUTH__PARTICLELEVEL_SOFTMUONORIGIN_H
#define TRUTH__PARTICLELEVEL_SOFTMUONORIGIN_H

#include "xAODTruth/TruthParticleContainer.h"

namespace top {
static void getSoftMuonAncestorInformation(
    const xAOD::TruthParticle* truthmu,
    const xAOD::TruthParticle*& lastBAncestor,
    const xAOD::TruthParticle*& lastCAncestor,
    const xAOD::TruthParticle*& lastTauAncestor,
    const xAOD::TruthParticle*& firstBAncestor,
    const xAOD::TruthParticle*& firstCAncestor,
    const xAOD::TruthParticleContainer* contHF) {
  lastBAncestor = nullptr;
  lastCAncestor = nullptr;
  lastTauAncestor = nullptr;
  firstBAncestor = nullptr;
  firstCAncestor = nullptr;

  // we now need to find the same truth muon in the "TruthHFWithDecayParticles"
  // container to be able to navigate it; there must be a better way of doing
  // this but I didn't find it.
  const xAOD::TruthParticle* correspondingHFMuon = nullptr;
  for (const xAOD::TruthParticle* tp : *contHF) {
    if (tp->barcode() == truthmu->barcode()) {
      correspondingHFMuon = tp;
      break;
    }
  }
  if (!correspondingHFMuon || correspondingHFMuon->nParents() < 1)
    return;  // in this case we cannot navigate back

  const xAOD::TruthParticle* muonParent = correspondingHFMuon->parent(0);
  if (!muonParent)
    return;  // the parent has an empty pointer

  // let's make sure the muon parent is not a muon (can happen in some
  // generators in case of FSR)
  unsigned int n_iter = 0;
  const xAOD::TruthParticle* parent = muonParent;
  while (parent && parent->isMuon()) {
    n_iter++;
    parent = parent->parent(0);
    if (n_iter > 100) {
      // ANA_MSG_WARNING("SoftMuonSelectorAlg::SaveAdditionalSoftMuonTruthVariables
      // reched max number of iterations in ancestors navigation, probably there
      // is a closed loop in ancestors history");
      break;
    }
  }
  if (parent && !parent->isMuon())
    muonParent = parent;

  // first we check if the muon parent is a Tau
  if (muonParent->isTau()) {
    lastTauAncestor = muonParent;

    const xAOD::TruthParticle* parent = lastTauAncestor->parent(0);
    unsigned int n_iter = 0;
    while (parent && parent->isTau()) {
      n_iter++;
      parent = parent->parent(0);
      if (n_iter > 100) {
        // ANA_MSG_WARNING("SoftMuonSelectorAlg::SaveAdditionalSoftMuonTruthVariables
        // reached max number of iterations in ancestors navigation, probably
        // there is a closed loop in ancestors history");
        break;
      }
    }
    if (parent && !parent->isTau())
      muonParent = parent;  // in this way if we have a tau->mu we can continue
                            // in the chain of ancestors to find if we have a
                            // B->tau->mu or a B->C->tau->mu decay
  }
  // then we find a C-hadron parent
  if (muonParent->isCharmHadron()) {
    lastCAncestor = muonParent;
    // now we have to find if this is coming from a B-hadron
    const xAOD::TruthParticle* parent = lastCAncestor;
    const xAOD::TruthParticle* previousParent = nullptr;
    unsigned int n_iter = 0;
    while (parent && parent->isCharmHadron()) {
      n_iter++;
      previousParent = parent;
      parent = parent->parent(0);
      if (n_iter > 100) {
        // ANA_MSG_WARNING("SoftMuonSelectorAlg::SaveAdditionalSoftMuonTruthVariables
        // reached max number of iterations in ancestors navigation, probably
        // there is a closed loop in ancestors history");
        break;
      }
    }
    firstCAncestor = previousParent;
    if (parent && !parent->isCharmHadron())
      muonParent =
          parent;  // in this way if we have a C->mu we can continue in the
                   // chain of ancestors to find if we have a B->C->mu decay
  }
  // finally let's see if we have a B-hadron parent
  if (muonParent->isBottomHadron()) {
    lastBAncestor = muonParent;
    const xAOD::TruthParticle* parent = lastBAncestor;
    const xAOD::TruthParticle* previousParent = nullptr;
    unsigned int n_iter = 0;
    while (parent && parent->isBottomHadron()) {
      n_iter++;
      previousParent = parent;
      parent = parent->parent(0);
      if (n_iter > 100) {
        // ANA_MSG_WARNING("SoftMuonSelectorAlg::SaveAdditionalSoftMuonTruthVariables
        // reached max number of iterations in ancestors navigation, probably
        // there is a closed loop in ancestors history");
        break;
      }
    }
    firstBAncestor = previousParent;
  }

  return;
}
}  // namespace top

#endif
