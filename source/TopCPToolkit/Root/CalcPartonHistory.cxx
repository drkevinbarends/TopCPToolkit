#include "PartonHistory/PartonHistory.h"
#include "PartonHistory/PartonHistoryUtils.h"
#include "PartonHistory/CalcPartonHistory.h"
#include "VectorHelpers/LorentzHelper.h"
#include "xAODTruth/TruthVertex.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "AthContainers/ConstDataVector.h"

#ifdef XAOD_STANDALONE
// access to tds in non-athena release
#define TDS() evtStore()->tds()
#else
// access to tds in athena release
#define TDS() evtStore()
#endif

namespace top {
  using ROOT::Math::PtEtaPhiMVector;

  CalcPartonHistory::CalcPartonHistory(const std::string& name,
				       const std::vector<std::string>& truthCollection) :
    asg::AsgTool(name),
    m_truthCollections(truthCollection)
  {
    declareProperty("outputSGKey", m_PartonsSGKey = "PartonHistoryTtbar",
                    "Store Gate output name for the PartonHistory object");
  }

  StatusCode CalcPartonHistory::buildContainerFromMultipleCollections(const std::vector<std::string> &collections, const std::string& out_contName)
  {
    ConstDataVector<DataVector<xAOD::TruthParticle_v1> > *out_cont = new ConstDataVector<DataVector<xAOD::TruthParticle_v1> > (SG::VIEW_ELEMENTS);

    for(const std::string& collection : collections)
    {
      const xAOD::TruthParticleContainer* cont=nullptr;
      ANA_CHECK(evtStore()->retrieve(cont,collection));
      for(const xAOD::TruthParticle* p : *cont) out_cont->push_back(p);
    }

    //we give control of the container to the store, because in this way we are able to retrieve it as a const data vector, see https://twiki.cern.ch/twiki/bin/view/AtlasComputing/DataVector#ConstDataVector
    StatusCode save = TDS()->record(out_cont,out_contName);
    if (!save) return StatusCode::FAILURE;

    return StatusCode::SUCCESS;
  }

  StatusCode CalcPartonHistory::linkBosonCollections()
  {
    return decorateCollectionWithLinksToAnotherCollection("TruthBoson","TruthBosonsWithDecayParticles","AT_linkToTruthBosonsWithDecayParticles");
  }

  StatusCode CalcPartonHistory::decorateCollectionWithLinksToAnotherCollection(const std::string &collectionToDecorate, const std::string &collectionToLink, const std::string &nameOfDecoration)
  {
    const xAOD::TruthParticleContainer* cont1(nullptr);
    const xAOD::TruthParticleContainer* cont2(nullptr);
    ANA_CHECK(evtStore()->retrieve(cont1,collectionToDecorate));
    ANA_CHECK(evtStore()->retrieve(cont2,collectionToLink));

    for(const xAOD::TruthParticle *p : *cont1)
    {
      const xAOD::TruthParticle* link =0;
      for(const xAOD::TruthParticle *p2 : *cont2)
      {
        if(p->pdgId()==p2->pdgId() && p->barcode()==p2->barcode())
        {
          link=p2;
          break;
        }
      }
      p->auxdecor<const xAOD::TruthParticle*>(nameOfDecoration)=link;

    }
    return StatusCode::SUCCESS;
  }

  const xAOD::TruthParticle* CalcPartonHistory::getTruthParticleLinkedFromDecoration(const xAOD::TruthParticle* part, const std::string &decorationName)
  {
    if(!part->isAvailable<const xAOD::TruthParticle*>(decorationName)) return part;

    const xAOD::TruthParticle* link=part->auxdecor<const xAOD::TruthParticle*>(decorationName);
    if(link) return link;

    return part;
  }

  bool CalcPartonHistory::ExistsInMap(const std::string& key) {
    // Checks wether a given key exists in the particle map
    return (!(particleMap.find(key) == particleMap.end()));
  }

  bool CalcPartonHistory::Retrievep4(const std::string& key, PtEtaPhiMVector& p4) {
    // Retrieves the PtEtaPhiMVector associated with a given key from the particleMap.
    // Returns true if the key exists and assigns the PtEtaPhiMVector to p4, otherwise returns false.
    if (ExistsInMap(key)) {
      p4 = GetPtEtaPhiMfromTruth(particleMap[key]);
      return true;
    }
    return false;
  }

  bool CalcPartonHistory::Retrievep4Gamma(PtEtaPhiMVector& p4, int& parentpdgId) {
    // Retrieves the PtEtaPhiMVector associated with a 'GammaRad' key from the particleMap.
    // Also retrieves the pdgId of the parent
    // Returns true if the key exists and assigns the PtEtaPhiMVector to p4, otherwise returns false.
    // Also assigns parent pdgId to parentpdgId
    std::vector<const xAOD::TruthParticle*> gammaRadParticles;
    // Extract particles with "GammaRad" in their key
    for (const auto& entry : particleMap) {
      if (entry.first.find("GammaRad") != std::string::npos) {
	gammaRadParticles.push_back(entry.second);
      }
    }
    // Check if we found any matching particles
    if (gammaRadParticles.empty()) {
        parentpdgId = 0;  // No match found, set parentPdgId to a sentinel value
        return false;
    }
    // We can have multiple photons. Sort the particles by their pt(), in descending order
    std::sort(gammaRadParticles.begin(), gammaRadParticles.end(),
        [](const xAOD::TruthParticle* a, const xAOD::TruthParticle* b) {
            return a->pt() > b->pt();
        }
    );
    // The particle with the highest pt
    const xAOD::TruthParticle* highestPtParticle = gammaRadParticles.at(0);
    p4 = GetPtEtaPhiMfromTruth(highestPtParticle);
    parentpdgId = highestPtParticle->parent()->pdgId();
    return true;
  }
  
  bool CalcPartonHistory::RetrievepdgId(const std::string& key, int& pdgId) {
    // Retrieves the PDG ID associated with a given key from the particleMap.
    // Returns true if the key exists and assigns the PDG ID to pdgId, otherwise returns false.
    if (ExistsInMap(key)) {
      pdgId = particleMap[key]->pdgId();
      return true;
    }
    return false;
  }

  bool CalcPartonHistory::RetrieveParticleInfo(const std::string& prefix, PtEtaPhiMVector& particle, int& pdgId) {
    return Retrievep4(prefix, particle) && RetrievepdgId(prefix, pdgId);
  }

  bool CalcPartonHistory::RetrieveParticleInfo(const std::string& prefix, const std::string& alt_prefix, PtEtaPhiMVector& particle, int& pdgId) {
    if (! (Retrievep4(prefix, particle) && RetrievepdgId(prefix, pdgId))) {
      return Retrievep4(alt_prefix, particle) && RetrievepdgId(alt_prefix, pdgId);
    }
    return true;
  }

  void CalcPartonHistory::EnsureKeyExists(const std::string& key, const std::string& fallbackKey) {
    // Ensures that a given key exists in the particleMap.
    // If the key does not exist and the fallbackKey exists, assigns the value of the fallbackKey to the key.
    // NOTE: This does not imply that the parton history is perfect. It rather is supposed to work on the
    // following cases:
    //
    // Assume there is a parton history like t->W instead of t->t->W. In this case this will ensure that both
    // afterFSR and beforeFSR exist (and have the same value)
    if (particleMap.find(key) == particleMap.end() && particleMap.find(fallbackKey) != particleMap.end()) {
      particleMap[key] = particleMap[fallbackKey];
    }
  }
  
  void CalcPartonHistory::EnsureTtbarKeysExist() {
    // Ensures that all relevant Ttbar key exists in the particleMap.
    // If a key does not exist and the fallbackKey exists, assigns the value of the fallbackKey to the key.
    EnsureKeyExists("MC_t_Wp_afterFSR", "MC_t_Wp");
    EnsureKeyExists("MC_tbar_Wm_afterFSR", "MC_tbar_Wm");
    EnsureKeyExists("MC_t_Wp_beforeFSR", "MC_t_Wp_afterFSR");
    EnsureKeyExists("MC_tbar_Wm_beforeFSR", "MC_tbar_Wm_afterFSR");
    EnsureKeyExists("MC_t_afterFSR", "MC_t");
    EnsureKeyExists("MC_tbar_afterFSR", "MC_tbar");
    EnsureKeyExists("MC_t_beforeFSR", "MC_t_afterFSR");
    EnsureKeyExists("MC_tbar_beforeFSR", "MC_tbar_afterFSR");
    EnsureKeyExists("MC_t_b_afterFSR", "MC_t_b");
    EnsureKeyExists("MC_tbar_bbar_afterFSR", "MC_tbar_bbar");
    EnsureKeyExists("MC_t_b_beforeFSR", "MC_t_b_afterFSR");
    EnsureKeyExists("MC_tbar_bbar_beforeFSR", "MC_tbar_bbar_afterFSR");
  }

  std::string CalcPartonHistory::GetParticleType(const xAOD::TruthParticle* particle) {
    // returns a string representing the particle type based on the pdgId of a truth particle
    // At the moment not all of these states are necessary, however, they are usefull for debugging purposes
    // More specifically they can be used to investigate/print out the content of the particleMap
    static const std::unordered_map<int, std::string> pdgMap = {
      {1, "_q"}, {2, "_q"}, {3, "_q"}, {4, "_q"}, // light quarks
      {-1, "_qbar"}, {-2, "_qbar"}, {-3, "_qbar"}, {-4, "_qbar"}, // anti light quarks
      {6, "_t"}, {-6, "_tbar"}, // top & anti-top
      {5, "_b"}, {-5, "_bbar"}, // b & anti-b
      {25, "_H"}, // Higgs
      {24, "_Wp"}, {-24, "_Wm"}, // W+ & W-
      {23, "_Z"}, // Z
      {22, "_gamma"}, // photons
      {21, "_g"}, // gluons
      {11, "_l"}, {13, "_l"}, {15, "_l"}, // light leptons
      {-11, "_lbar"}, {-13, "_lbar"}, {-15, "_lbar"}, // anti light leptons
      {12, "_nu"}, {14, "_nu"},{16, "_nu"}, // neutrinos
      {-12, "_nubar"}, {-14, "_nubar"}, {-16, "_nubar"}, // anti-neutrinos
      {2212, "_p"}, // proton
      {1103, "_dd"}, {2101, "_ud"}, {2103, "_ud"}, {2203, "_uu"}, // diquark states
      {3101, "_sd"}, {3103, "_sd"}, {3201, "_su"}, {3203, "_su"}, {3303, "_ss"}, // diquark states
      {4101, "_cd"}, {4103, "_cd"}, {4201, "_cu"}, {4203, "_cu"}, {4301, "_cs"}, // diquark states
      {4303, "_cs"}, {4403, "_cc"}, {5101, "_bd"}, {5103, "_bd"}, {5201, "_bu"}, // diquark states
      {5203, "_bu"}, {5301, "_bs"}, {5303, "_bs"}, {5401, "_bc"}, {5403, "_bc"}, // diquark states
      {5503, "_bb"} // diquark states
    };

    int pdgId = particle->pdgId();
    auto it = pdgMap.find(pdgId);
    if (it != pdgMap.end()) {
      return it->second;
    } else {
      return "_" + std::to_string(pdgId);
    }
  }

  bool CalcPartonHistory::hasParentPdgId(const xAOD::TruthParticle* particle, int PdgId) {
    // Checks if the parent of the given particle has a specific PDG ID.
    return particle->parent() && particle->parent()->pdgId() == PdgId;
  }

  bool CalcPartonHistory::hasIdenticalChild(const xAOD::TruthParticle* particle) {
    // Checks if the given particle has at least one child with an identical PDG ID.
    bool check = false;
    for (size_t i = 0; i < particle->nChildren(); i++) {
      if (particle->child(i)->pdgId() == particle->pdgId()) check=true;
    }
    return check;
  }

  bool CalcPartonHistory::hasParentAbsPdgId(const xAOD::TruthParticle* particle, int absPdgId) {
    // Checks if the parent of the given particle has a specific absolute PDG ID.
    return particle->parent() && std::abs(particle->parent()->pdgId()) == absPdgId;
  }

  void CalcPartonHistory::TraceParticle(const xAOD::TruthParticle* particle,
                                      std::vector<const xAOD::TruthParticle*>& currentPath,
                                      std::vector<std::vector<const xAOD::TruthParticle*>>& allPaths) {
    // Recursively traces the path of a particle through its decay chain, recording all possible paths using a DFS approach.

    // Exit if the particle does not exist or if it is a "broken top"
    if (!particle || PartonHistoryUtils::isBrokenTop(particle)) return;

    // Handle after FSR for W bosons (coming from tops), here we need to link collections
    if (PartonHistoryUtils::isAfterFSR(particle) && abs(particle->pdgId()) == 24) {
      particle = getTruthParticleLinkedFromDecoration(particle, "AT_linkToTruthBosonsWithDecayParticles");
    }
    // Add the current particle to the current path we are building
    currentPath.push_back(particle);

    // If the particle has 0 children we have reached the end of the decay chain, record the current path as complete
    if (particle->nChildren() == 0) {
      allPaths.push_back(currentPath);
    }
    else {
      // We have already pushed the beforeFSR particle, now we proceed to the afterFSR one
      const xAOD::TruthParticle* particle_afterFSR = PartonHistoryUtils::findAfterFSR(particle);
      // If we have a W boson we have to link the decorations
      if (abs(particle_afterFSR->pdgId()) == 24) {
	particle_afterFSR = getTruthParticleLinkedFromDecoration(particle_afterFSR, "AT_linkToTruthBosonsWithDecayParticles");
      }
      // We push it to the current path if we haven't pushed it already
      // Check if the afterFSR particle is the same as the beforeFSR particle
      if (particle_afterFSR != particle) {
	TraceParticle(particle_afterFSR, currentPath, allPaths);
      }
      else {
	// Now recursively trace the children
	for (size_t i = 0; i < particle_afterFSR->nChildren(); i++) {
	  if (const xAOD::TruthParticle* child = particle_afterFSR->child(i)) {
	    TraceParticle(child, currentPath, allPaths);
	  }
	}
      }
    }
    // Remove the current particle from the current path before returning
    currentPath.pop_back();
  }

  void CalcPartonHistory::AddToParticleMap(const xAOD::TruthParticle* particle, const std::string& key) {
    // Add a truth particle to the particle map using a defined key
    particleMap[key] = particle;
  }

  bool CalcPartonHistory::handleBeforeFSR(const xAOD::TruthParticle* particle, const std::string& newKey, std::string& key) {
    // Handles the "Before FSR" case for a given particle. These are cases where we have identical, consecutive particles in the path.
    // If the particle has an identical child, we add the particle to the map with the key "newKey_beforeFSR" and updates the key.
    if (hasIdenticalChild(particle)) {
      std::string postfix = "_beforeFSR";
      AddToParticleMap(particle, key + newKey + postfix);
      key += newKey;
      return true;
    }
    return false;
  }

  bool CalcPartonHistory::handleAfterFSR(const xAOD::TruthParticle* particle, const std::string& newKey, const std::string& oldKey, std::string& key) {
    // Handles the "After FSR" case for a given particle.
    // If the particle is a top quark, W boson, Z boson, or Higgs boson with exactly 2 children, or if it has a parent with the same PDG ID,
    // We add the particle to the map with the key "newKey_afterFSR" and update the key.
    std::string postfix = "_afterFSR";
    // if this is a t, W, Z or H it can only be "AfterFSR" if it has 2 children!
    if (abs(particle->pdgId()) == 6 || (particle->pdgId() == 23 || abs(particle->pdgId()) == 24 || particle->pdgId() == 25)) {
      if (! (particle->nChildren() == 2)) return false;
      else {
	if (!(newKey == oldKey)) key += newKey;
	AddToParticleMap(particle, key + postfix);
	return true;
      }
    }
    if (hasParentPdgId(particle, particle->pdgId())) {
      if (!(newKey == oldKey)) key += newKey;
      AddToParticleMap(particle, key + postfix);
      return true;
    }
    return false;
  }

  bool CalcPartonHistory::handleDecay(const xAOD::TruthParticle* particle, std::string& key, int decayID) {
    // Handles the case where a particle originates from a decay.
    // We add the particle to the map with a key based on its decay ID and update the key.
    if (particle->nParents() == 0) return false;
    if ((particle->pdgId() == 22) ||
	((hasParentAbsPdgId(particle, 25) && !hasParentPdgId(particle, particle->pdgId())) ||
	 (hasParentAbsPdgId(particle, 24) && !hasParentPdgId(particle, particle->pdgId())) ||
	 (hasParentAbsPdgId(particle, 23) && !hasParentPdgId(particle, particle->pdgId())))) {
      std::string postfix;
      // Sometimes we have decays like W-> (l,nu,gamma) where the photon is FSR
      if (particle->pdgId() == 22) postfix = "_GammaRad";
      else postfix = "Decay";
      postfix += std::to_string(decayID);
      AddToParticleMap(particle, key + postfix);
      key += postfix + "_";
      return true;
    }
    return false;
  }

  void CalcPartonHistory::handleSameAsParent(const xAOD::TruthParticle* particle, std::string& key) {
    // Handles particles that are the same as their parent.
    // We add the particle to the map with the current key.
    AddToParticleMap(particle, key);
  }

  void CalcPartonHistory::handleDefault(const xAOD::TruthParticle* particle, const std::string& newKey, std::string& key) {
    // Handles default cases for particles.
    // We add the particle to the map with a key based on newKey and update the key.
    AddToParticleMap(particle, key + newKey);
    key += newKey;
  }

  void CalcPartonHistory::FillParticleMap(std::vector<std::vector<const xAOD::TruthParticle*>>& allPaths) {
    // Fills the particle map with particles from all paths.
    // We clear the existing map and processes each path to add particles with appropriate keys.
    particleMap.clear();
    for (const auto& path : allPaths) {
      std::string key = "MC";
      std::string old_key = "";
      std::string new_key = "";
      std::string postfix = "";
      
      for (auto it = path.begin(); it != path.end(); it++) {
	const xAOD::TruthParticle* particle = *it;
	bool isbeforeFSR = (hasIdenticalChild(particle));
	bool isafterFSR = (hasParentPdgId(particle, particle->pdgId()));

	int decayID = particle->pdgId() < 0 ? 2 : 1;
	// if we have a Z or a gamma, we have to define the decayID differently
	if ((particle->pdgId() == 23 || particle->pdgId() == 22) && particle->nParents() != 0) {
	  // We assign 1 for the first child and 2 for the second
	  for (size_t i = 0; i < particle->parent()->nChildren(); ++i) {
	    if (particle->parent()->child(i) == particle) {
	      decayID = i + 1;  // Assuming decayID starts from 1
	      break;
	    }
	  }
	}

	old_key = new_key;
	new_key = GetParticleType(particle);
	if (isbeforeFSR && isafterFSR) {
	  continue; // e.g. t-t-t
	}
	if (handleDecay(particle, key, decayID)) continue;
	if (handleBeforeFSR(particle, new_key, key)) continue;
	if (handleAfterFSR(particle, new_key, old_key, key)) continue;
	if (hasParentPdgId(particle, particle->pdgId())) {
	  handleSameAsParent(particle, key);
	  continue;
	}
	if (new_key != "") handleDefault(particle, new_key, key);
      }
    }
  }

  void CalcPartonHistory::TraceParticles(const xAOD::TruthParticleContainer* truthParticles) {
    // Traces the paths of all particles in the truthParticles container and fills the particle map.
    std::vector<std::vector<const xAOD::TruthParticle*>> allPaths;
    for (const xAOD::TruthParticle* particle : *truthParticles) {
      std::vector<const xAOD::TruthParticle*> currentPath;
      if (PartonHistoryUtils::hasParticleIdenticalParent(particle)) continue;
      TraceParticle(particle, currentPath, allPaths);
    }
    FillParticleMap(allPaths);
  }

  StatusCode CalcPartonHistory::execute() {
    const xAOD::TruthParticleContainer *truthParticles {nullptr};
    ANA_CHECK(linkTruthContainers(truthParticles));

    // Create the partonHistory xAOD object
    //cppcheck-suppress uninitvar
    xAOD::PartonHistory* partonHistory = new xAOD::PartonHistory {};
    //cppcheck-suppress uninitvar
    xAOD::PartonHistoryAux* partonHistoryAux = new xAOD::PartonHistoryAux {};
    partonHistory->setStore(partonHistoryAux);

    ANA_CHECK(runHistorySaver(truthParticles, partonHistory));

    // Save to StoreGate / TStore
    StatusCode save = TDS()->record(partonHistory, m_PartonsSGKey);
    StatusCode saveAux = TDS()->record(partonHistoryAux, m_PartonsSGKey + "Aux.");
    if (!save || !saveAux) return StatusCode::FAILURE;

    return StatusCode::SUCCESS;
  }

  StatusCode CalcPartonHistory::linkTruthContainers(const xAOD::TruthParticleContainer* &tp) {
    //in DAOD_PHYS we don't have the truth particles container
    //the functions ued in this class always start from the top, so it's enough to do the following
    const std::string &truthParticlesSGKey = m_PartonsSGKey + "_TruthParticles";
    if (!evtStore()->contains<xAOD::TruthParticleContainer>(truthParticlesSGKey)) {
      ANA_CHECK(buildContainerFromMultipleCollections(m_truthCollections, truthParticlesSGKey));
      ANA_CHECK(evtStore()->retrieve(tp, truthParticlesSGKey));
      //we need to be able to navigate from the Ws to their decayProducts, see CalcPartonHistory.h for details
      ANA_CHECK(linkBosonCollections());
    }
    else {
      ANA_CHECK(evtStore()->retrieve(tp, truthParticlesSGKey));
    }

    return StatusCode::SUCCESS;
  }
} // namespace top
