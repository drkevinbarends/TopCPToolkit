#include "PartonHistory/PartonHistory.h"
#include "PartonHistory/PartonHistoryUtils.h"
#include <array>

namespace top {
  using ROOT::Math::PtEtaPhiMVector;
  
  namespace PartonHistoryUtils {

    bool isBrokenTop(const xAOD::TruthParticle* particle) {
      return (particle->pdgId() == 6 && particle->nChildren() == 0);
    }

    bool isAfterFSR(const xAOD::TruthParticle* particle) {
      bool isAfter = true;
      for (size_t j = 0; j < particle->nChildren(); j++) {
	if (particle->child(j)->pdgId() == particle->pdgId()) isAfter = false;
      }
      return isAfter;
    }

    const xAOD::TruthParticle* findAfterFSR(const xAOD::TruthParticle* particle) {
      bool isAfter(false);
      const int particle_ID = particle->pdgId();
      int forLoop = 0;

      while (!isAfter) {
	forLoop = 0;
	for (size_t j = 0; j < particle->nChildren(); j++) {
	  const xAOD::TruthParticle* tmp_children = particle->child(j);
	  if (tmp_children && tmp_children->pdgId() == particle_ID) {
	    particle = particle->child(j);
	    forLoop++;
	    break;
	  }//if
	}//for
	if (forLoop == 0) isAfter = true;
      }//while
      return particle;
    }

    bool hasParticleIdenticalParent(const xAOD::TruthParticle* particle) {
      bool skipit(false);

      for (size_t i = 0; i < particle->nParents(); i++) {
	const xAOD::TruthParticle* parent = particle->parent(i);
	if (parent && parent->pdgId() == particle->pdgId()) {
	  skipit = true;
	  break;
	}//if
      }//for
      return skipit;
    }

    void decorateWithMPtPhi(xAOD::PartonHistory* pHistory, const std::string & prefix, const PtEtaPhiMVector & vec ){
      constexpr size_t nItems=3;
      const std::array<std::string, nItems> suffix{"_m", "_pt", "_phi"};
      const std::array<double, nItems> values{vec.M(), vec.Pt(), vec.Phi()};
      for(size_t i(0);i<nItems;++i){
      	pHistory->auxdecor< float >(prefix+suffix[i]) = values[i];
      }
      return;
    }

    void fillEtaBranch(xAOD::PartonHistory* partonHistory, std::string branchName,
		       TLorentzVector& tlv) {
      partonHistory->auxdecor< float >(branchName) = tlv.Eta();
      return;
    }
  } // namespace PartonHistoryUtils
} // namespace top
