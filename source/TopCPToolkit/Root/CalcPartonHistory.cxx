#include "PartonHistory/CalcPartonHistory.h"

#include "AthContainers/ConstDataVector.h"
#include "PartonHistory/PartonHistory.h"
#include "PartonHistory/PartonHistoryUtils.h"
#include "VectorHelpers/LorentzHelper.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "xAODTruth/TruthVertex.h"

#ifdef XAOD_STANDALONE
// access to tds in non-athena release
#define TDS() evtStore()->tds()
#else
// access to tds in athena release
#define TDS() evtStore()
#endif

namespace top {
using ROOT::Math::PtEtaPhiMVector;

CalcPartonHistory::CalcPartonHistory(
    const std::string& name, const std::vector<std::string>& truthCollection)
    : asg::AsgTool(name), m_truthCollections(truthCollection) {
  declareProperty("outputSGKey", m_PartonsSGKey = "PartonHistoryTtbar",
                  "Store Gate output name for the PartonHistory object");
}

bool CalcPartonHistory::ExistsInMap(const std::string& key) {
  // Checks whether a given key exists in the particle map.
  // Returns true if the key is present, otherwise false.
  return particleMap.find(key) != particleMap.end();
}

bool CalcPartonHistory::ExistsInKey(const std::string& key,
                                    const xAOD::TruthParticle* particle) {
  // Checks whether a given particle exists within the vector corresponding to
  // the key in the particleMap. Returns true if the particle is found in the
  // vector associated with the key, otherwise false.
  auto it = particleMap.find(key);
  if (it == particleMap.end())
    return false;  // Handle case where key does not exist
  return std::find(it->second.begin(), it->second.end(), particle) !=
         it->second.end();
}

bool CalcPartonHistory::Retrievep4(const std::string& key,
                                   PtEtaPhiMVector& p4) {
  // Retrieves the PtEtaPhiMVector associated with a given key.
  // Overload that assumes an index of 0 (the first particle in the vector).
  return Retrievep4(key, p4, 0);
}

bool CalcPartonHistory::Retrievep4(const std::string& key, PtEtaPhiMVector& p4,
                                   const int& idx) {
  // Retrieves the PtEtaPhiMVector for a specific particle by its index from the
  // vector associated with the key. If the key exists in the particleMap and
  // the index is within range, the PtEtaPhiMVector is assigned to p4. Returns
  // true if the operation is successful, otherwise returns false.
  auto it = particleMap.find(key);
  if (it != particleMap.end() && idx >= 0 &&
      static_cast<std::make_unsigned_t<decltype(it->second.size())>>(idx) <
          it->second.size()) {
    p4 = GetPtEtaPhiMfromTruth(it->second.at(idx));
    return true;
  }
  return false;
}

bool CalcPartonHistory::Retrievep4Gamma(PtEtaPhiMVector& p4, int& parentpdgId) {
  // Retrieves the PtEtaPhiMVector associated with a 'GammaRad' key from the
  // particleMap. Also retrieves the pdgId of the parent Returns true if the key
  // exists and assigns the PtEtaPhiMVector to p4, otherwise returns false. Also
  // assigns parent pdgId to parentpdgId
  std::vector<const xAOD::TruthParticle*> gammaRadParticles;
  // Extract particles with "GammaRad" in their key
  for (const auto& entry : particleMap) {
    if (entry.first.find("GammaRad") != std::string::npos) {
      for (const auto& particle : entry.second) {
        gammaRadParticles.push_back(particle);
      }
    }
  }
  // Check if we found any matching particles
  if (gammaRadParticles.empty()) {
    parentpdgId = 0;  // No match found, set parentPdgId to a sentinel value
    return false;
  }
  // We can have multiple photons. Sort the particles by their pt(), in
  // descending order
  std::sort(gammaRadParticles.begin(), gammaRadParticles.end(),
            [](const xAOD::TruthParticle* a, const xAOD::TruthParticle* b) {
              return a->pt() > b->pt();
            });
  // The particle with the highest pt
  const xAOD::TruthParticle* highestPtParticle = gammaRadParticles.at(0);
  p4 = GetPtEtaPhiMfromTruth(highestPtParticle);
  if (highestPtParticle->nParents() > 0)
    parentpdgId = highestPtParticle->parent(0)->pdgId();
  else
    parentpdgId = 0;
  return true;
}

bool CalcPartonHistory::RetrievepdgId(const std::string& key, int& pdgId) {
  // Retrieves the PDG ID associated with a given key from the particleMap.
  // Returns true if the key exists and assigns the PDG ID to pdgId, otherwise
  // returns false.
  return RetrievepdgId(key, pdgId, 0);
}

bool CalcPartonHistory::RetrievepdgId(const std::string& key,
                                      std::vector<int>& pdgIds) {
  // Retrieves the PDG ID vector associated with a given key from the
  // particleMap. Returns true if the key exists, otherwise returns false.
  if (ExistsInMap(key)) {
    for (auto p : particleMap[key]) {
      pdgIds.push_back(p->pdgId());
    }
    return true;
  }
  return false;
}

bool CalcPartonHistory::RetrievepdgId(const std::string& key, int& pdgId,
                                      const int& idx) {
  // Retrieves the PDG ID associated with a given key from the particleMap.
  // Returns true if the key exists and assigns the PDG ID to pdgId, otherwise
  // returns false.
  if (ExistsInMap(key)) {
    pdgId = particleMap[key].at(idx)->pdgId();
    return true;
  }
  return false;
}

bool CalcPartonHistory::RetrieveParticleInfo(
    const std::string& prefix,
    std::vector<const xAOD::TruthParticle*>& particles) {
  if (ExistsInMap(prefix)) {
    for (const auto particle : particleMap[prefix]) {
      particles.push_back(particle);
    }
    return true;
  }
  return false;
}

bool CalcPartonHistory::RetrieveParticleInfo(
    const std::string& prefix, std::vector<PtEtaPhiMVector>& particles,
    std::vector<int>& pdgIds) {
  PtEtaPhiMVector particle;
  int pdgId;
  bool check = false;
  for (size_t i = 0; i < particleMap[prefix].size(); i++) {
    check = RetrieveParticleInfo(prefix, particle, pdgId, i);
    particles.push_back(particle);
    pdgIds.push_back(pdgId);
  }
  return check;
}

bool CalcPartonHistory::RetrieveParticleInfo(const std::string& prefix,
                                             PtEtaPhiMVector& particle,
                                             int& pdgId) {
  return RetrieveParticleInfo(prefix, particle, pdgId, 0);
}

bool CalcPartonHistory::RetrieveParticleInfo(const std::string& prefix,
                                             PtEtaPhiMVector& particle,
                                             int& pdgId, const int& idx) {
  return Retrievep4(prefix, particle, idx) && RetrievepdgId(prefix, pdgId, idx);
}

bool CalcPartonHistory::RetrieveParticleInfo(const std::string& prefix,
                                             const std::string& alt_prefix,
                                             PtEtaPhiMVector& particle,
                                             int& pdgId) {
  if (!(Retrievep4(prefix, particle) && RetrievepdgId(prefix, pdgId))) {
    return Retrievep4(alt_prefix, particle) && RetrievepdgId(alt_prefix, pdgId);
  }
  return true;
}

std::string CalcPartonHistory::GetParticleType(
    const xAOD::TruthParticle* particle) {
  // returns a string representing the particle type based on the pdgId of a
  // truth particle At the moment not all of these states are necessary,
  // however, they are usefull for debugging purposes More specifically they can
  // be used to investigate/print out the content of the particleMap
  static const std::unordered_map<int, std::string> pdgMap = {
      {1, "_q"},       {2, "_q"},       {3, "_q"},
      {4, "_q"},  // light quarks
      {-1, "_qbar"},   {-2, "_qbar"},   {-3, "_qbar"},
      {-4, "_qbar"},                                      // anti light quarks
      {6, "_t"},       {-6, "_tbar"},                     // top & anti-top
      {5, "_b"},       {-5, "_bbar"},                     // b & anti-b
      {25, "_H"},                                         // Higgs
      {24, "_Wp"},     {-24, "_Wm"},                      // W+ & W-
      {23, "_Z"},                                         // Z
      {22, "_gamma"},                                     // photons
      {21, "_g"},                                         // gluons
      {11, "_l"},      {13, "_l"},      {15, "_l"},       // light leptons
      {-11, "_lbar"},  {-13, "_lbar"},  {-15, "_lbar"},   // anti light leptons
      {12, "_nu"},     {14, "_nu"},     {16, "_nu"},      // neutrinos
      {-12, "_nubar"}, {-14, "_nubar"}, {-16, "_nubar"},  // anti-neutrinos
      {2212, "_p"},                                       // proton
      {1103, "_dd"},   {2101, "_ud"},   {2103, "_ud"},
      {2203, "_uu"},  // diquark states
      {3101, "_sd"},   {3103, "_sd"},   {3201, "_su"},
      {3203, "_su"},   {3303, "_ss"},  // diquark states
      {4101, "_cd"},   {4103, "_cd"},   {4201, "_cu"},
      {4203, "_cu"},   {4301, "_cs"},  // diquark states
      {4303, "_cs"},   {4403, "_cc"},   {5101, "_bd"},
      {5103, "_bd"},   {5201, "_bu"},  // diquark states
      {5203, "_bu"},   {5301, "_bs"},   {5303, "_bs"},
      {5401, "_bc"},   {5403, "_bc"},  // diquark states
      {5503, "_bb"}                    // diquark states
  };

  int pdgId = particle->pdgId();
  auto it = pdgMap.find(pdgId);
  if (it != pdgMap.end()) {
    return it->second;
  } else {
    return "_" + std::to_string(pdgId);
  }
}

void CalcPartonHistory::TraceParticle(
    const xAOD::TruthParticle* particle,
    std::vector<const xAOD::TruthParticle*>& currentPath,
    std::vector<std::vector<const xAOD::TruthParticle*>>& allPaths) {
  // Recursively traces the path of a particle through its decay chain,
  // recording all possible paths using a DFS approach.

  // Exit if the particle does not exist or if it is a "broken top"
  if (!particle || PartonHistoryUtils::isBrokenTop(particle))
    return;

  // Handle after FSR for W bosons (coming from tops), here we need to link
  // collections
  if (PartonHistoryUtils::isAfterFSR(particle) &&
      abs(particle->pdgId()) == 24) {
    particle = getTruthParticleLinkedFromDecoration(
        particle, "AT_linkToTruthBosonsWithDecayParticles");
  }

  // Add the current particle to the current path we are building
  currentPath.push_back(particle);

  // If the particle has 0 children we have reached the end of the decay chain,
  // record the current path as complete
  if (particle->nChildren() == 0) {
    allPaths.push_back(currentPath);
  } else {
    // We have already pushed the beforeFSR particle, now we proceed to the
    // afterFSR one
    const xAOD::TruthParticle* particle_afterFSR =
        PartonHistoryUtils::findAfterFSR(particle);
    // If we have a W boson we have to link the decorations
    if (abs(particle_afterFSR->pdgId()) == 24) {
      particle_afterFSR = getTruthParticleLinkedFromDecoration(
          particle_afterFSR, "AT_linkToTruthBosonsWithDecayParticles");
    }
    // We push it to the current path if we haven't pushed it already
    // Check if the afterFSR particle is the same as the beforeFSR particle
    if (particle_afterFSR != particle) {
      TraceParticle(particle_afterFSR, currentPath, allPaths);
    } else {
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

void CalcPartonHistory::AddToParticleMap(const xAOD::TruthParticle* particle,
                                         const std::string& key) {
  // Add a truth particle to the particle map using a defined key
  if (ExistsInKey(key, particle)) {
  } else {
    particleMap[key].push_back(particle);
  }
}

bool CalcPartonHistory::handleFSR(const xAOD::TruthParticle* particle,
                                  const std::string& newKey, std::string& key) {
  // Handles the "Before FSR" case for a given particle. These are cases where
  // we have identical, consecutive particles in the path. m If the particle has
  // an identical child, we add the particle to the map with the key
  // "newKey_beforeFSR" and updates the key.
  std::string beforeFSR = "_beforeFSR";
  std::string afterFSR = "_afterFSR";
  if (!PartonHistoryUtils::hasParentPdgId(particle))
    key += newKey;
  if (particle->nParents() == 0) {
    // Particles without parents are always beforeFSR
    AddToParticleMap(particle, key + beforeFSR);
    // If it doesn't have an identical child it is also afterFSR
    if (!PartonHistoryUtils::hasIdenticalChild(particle))
      AddToParticleMap(particle, key + afterFSR);
    return true;
  } else {
    // If it has the same pdgId as the parent it is afterFSR
    if (PartonHistoryUtils::hasParentPdgId(particle)) {
      AddToParticleMap(particle, key + afterFSR);
    }
    // else it must be beforeFSR
    else
      AddToParticleMap(particle, key + beforeFSR);
    // If it has not the parent pdgId and no identical child it is also afterFSR
    if (!PartonHistoryUtils::hasParentPdgId(particle) &&
        !PartonHistoryUtils::hasIdenticalChild(particle)) {
      AddToParticleMap(particle, key + afterFSR);
    }
    return true;
  }
  return false;
}

bool CalcPartonHistory::handleDecay(const xAOD::TruthParticle* particle,
                                    std::string& key, int decayID) {
  // Handles the case where a particle originates from a decay.
  // We add the particle to the map with a key based on its decay ID and update
  // the key.
  if ((particle->pdgId() == 22) ||
      ((PartonHistoryUtils::hasParentAbsPdgId(particle, 25) &&
        !PartonHistoryUtils::hasParentPdgId(particle)) ||
       (PartonHistoryUtils::hasParentAbsPdgId(particle, 24) &&
        !PartonHistoryUtils::hasParentPdgId(particle)) ||
       (PartonHistoryUtils::hasParentAbsPdgId(particle, 23) &&
        !PartonHistoryUtils::hasParentPdgId(particle)))) {
    std::string postfix;
    // Sometimes we have decays like W-> (l,nu,gamma) where the photon is FSR
    if (particle->pdgId() == 22)
      postfix = "_GammaRad";
    else
      postfix = "Decay";
    postfix += std::to_string(decayID);
    const xAOD::TruthParticle* particle_afterFSR =
        PartonHistoryUtils::findAfterFSR(particle);
    AddToParticleMap(particle, key + postfix + "_beforeFSR");
    AddToParticleMap(particle_afterFSR, key + postfix + "_afterFSR");
    key += postfix + "_";
    return true;
  }
  return false;
}

void CalcPartonHistory::handleSameAsParent(const xAOD::TruthParticle* particle,
                                           std::string& key) {
  // Handles particles that are the same as their parent.
  // We add the particle to the map with the current key.
  AddToParticleMap(particle, key);
}

void CalcPartonHistory::handleDefault(const xAOD::TruthParticle* particle,
                                      const std::string& newKey,
                                      std::string& key) {
  // Handles default cases for particles.
  // We add the particle to the map with a key based on newKey and update the
  // key.
  AddToParticleMap(particle, key + newKey);
  key += newKey;
}

void CalcPartonHistory::FillParticleMap(
    std::vector<std::vector<const xAOD::TruthParticle*>>& allPaths) {
  // Fills the particle map with particles from all paths.
  // We clear the existing map and processes each path to add particles with
  // appropriate keys.
  particleMap.clear();
  for (const auto& path : allPaths) {
    std::string key = "MC";
    std::string new_key = "";
    std::string postfix = "";

    for (auto it = path.begin(); it != path.end(); it++) {
      const xAOD::TruthParticle* particle = *it;
      bool isbeforeFSR = (PartonHistoryUtils::hasIdenticalChild(particle));
      bool isafterFSR = (PartonHistoryUtils::hasParentPdgId(particle));

      int decayID = particle->pdgId() < 0 ? 2 : 1;
      // if we have a Z or a gamma, we have to define the decayID differently
      if (particle->nParents() != 0) {
        // We assign 1 for the first child and 2 for the second
        for (size_t i = 0; i < particle->parent(0)->nChildren(); ++i) {
          if (particle->parent(0)->child(i) == particle) {
            decayID = i + 1;  // Assuming decayID starts from 1
            break;
          }
        }
      }

      new_key = GetParticleType(particle);
      if (isbeforeFSR && isafterFSR) {
        continue;  // e.g. t-t-t
      }
      if (handleDecay(particle, key, decayID))
        continue;
      if (handleFSR(particle, new_key, key))
        continue;
      if (PartonHistoryUtils::hasParentPdgId(particle)) {
        handleSameAsParent(particle, key);
        continue;
      }
      if (new_key != "")
        handleDefault(particle, new_key, key);
    }
  }
}

void CalcPartonHistory::TraceParticles(
    const xAOD::TruthParticleContainer* truthParticles) {
  // Traces the paths of all particles in the truthParticles container and fills
  // the particle map.
  std::vector<std::vector<const xAOD::TruthParticle*>> allPaths;
  for (const xAOD::TruthParticle* particle : *truthParticles) {
    std::vector<const xAOD::TruthParticle*> currentPath;
    if (PartonHistoryUtils::hasParticleIdenticalParent(particle))
      continue;
    TraceParticle(particle, currentPath, allPaths);
  }
  FillParticleMap(allPaths);
}

StatusCode CalcPartonHistory::execute() {
  const xAOD::TruthParticleContainer* truthParticles{nullptr};
  ANA_CHECK(linkTruthContainers(truthParticles));

  // Create the partonHistory xAOD object
  // cppcheck-suppress uninitvar
  xAOD::PartonHistory* partonHistory = new xAOD::PartonHistory{};
  // cppcheck-suppress uninitvar
  xAOD::PartonHistoryAux* partonHistoryAux = new xAOD::PartonHistoryAux{};
  partonHistory->setStore(partonHistoryAux);

  ANA_CHECK(runHistorySaver(truthParticles, partonHistory));

  // Save to StoreGate / TStore
  StatusCode save = TDS()->record(partonHistory, m_PartonsSGKey);
  StatusCode saveAux = TDS()->record(partonHistoryAux, m_PartonsSGKey + "Aux.");
  if (!save || !saveAux)
    return StatusCode::FAILURE;

  return StatusCode::SUCCESS;
}

StatusCode CalcPartonHistory::buildContainerFromMultipleCollections(
    const std::vector<std::string>& collections,
    const std::string& out_contName) {
  ConstDataVector<DataVector<xAOD::TruthParticle_v1>>* out_cont =
      new ConstDataVector<DataVector<xAOD::TruthParticle_v1>>(
          SG::VIEW_ELEMENTS);
  std::vector<const xAOD::TruthParticle*> p_candidates;
  std::vector<const xAOD::TruthParticle*> p_parents;

  for (const std::string& collection : collections) {
    const xAOD::TruthParticleContainer* cont = nullptr;
    ANA_CHECK(evtStore()->retrieve(cont, collection));
    p_candidates.insert(p_candidates.end(), cont->begin(), cont->end());
  }

  for (const xAOD::TruthParticle* potential_parent : p_candidates) {
    if (std::none_of(p_candidates.begin(), p_candidates.end(),
                     [&](const xAOD::TruthParticle* other_candidate) {
                       return other_candidate != potential_parent &&
                              PartonHistoryUtils::isChildOf(other_candidate,
                                                            potential_parent);
                     })) {
      p_parents.push_back(potential_parent);
    }
  }

  out_cont->insert(out_cont->end(), p_parents.begin(), p_parents.end());

  // we give control of the container to the store, because in this way we are
  // able to retrieve it as a const data vector, see
  // https://twiki.cern.ch/twiki/bin/view/AtlasComputing/DataVector#ConstDataVector
  StatusCode save = TDS()->record(out_cont, out_contName);
  if (!save)
    return StatusCode::FAILURE;

  return StatusCode::SUCCESS;
}

StatusCode CalcPartonHistory::linkBosonCollections() {
  return decorateCollectionWithLinksToAnotherCollection(
      "TruthBoson", "TruthBosonsWithDecayParticles",
      "AT_linkToTruthBosonsWithDecayParticles");
}

StatusCode CalcPartonHistory::decorateCollectionWithLinksToAnotherCollection(
    const std::string& collectionToDecorate,
    const std::string& collectionToLink, const std::string& nameOfDecoration) {
  const SG::AuxElement::Decorator<const xAOD::TruthParticle*>
      dec_nameOfDecoration(nameOfDecoration);

  const xAOD::TruthParticleContainer* cont1(nullptr);
  const xAOD::TruthParticleContainer* cont2(nullptr);
  ANA_CHECK(evtStore()->retrieve(cont1, collectionToDecorate));
  ANA_CHECK(evtStore()->retrieve(cont2, collectionToLink));

  for (const xAOD::TruthParticle* p : *cont1) {
    const xAOD::TruthParticle* link = 0;
    for (const xAOD::TruthParticle* p2 : *cont2) {
      if (p->pdgId() == p2->pdgId() && p->barcode() == p2->barcode()) {
        link = p2;
        break;
      }
    }
    dec_nameOfDecoration(*p) = link;
  }
  return StatusCode::SUCCESS;
}

const xAOD::TruthParticle*
CalcPartonHistory::getTruthParticleLinkedFromDecoration(
    const xAOD::TruthParticle* part, const std::string& decorationName) {
  const SG::AuxElement::ConstAccessor<const xAOD::TruthParticle*>
      acc_decorationName(decorationName);
  if (!acc_decorationName.isAvailable(*part))
    return part;

  const xAOD::TruthParticle* link = acc_decorationName(*part);
  if (link)
    return link;

  return part;
}

StatusCode CalcPartonHistory::linkTruthContainers(
    const xAOD::TruthParticleContainer*& tp) {
  // in DAOD_PHYS we don't have the truth particles container
  // the functions ued in this class always start from the top, so it's enough
  // to do the following
  const std::string& truthParticlesSGKey = m_PartonsSGKey + "_TruthParticles";
  if (!evtStore()->contains<xAOD::TruthParticleContainer>(
          truthParticlesSGKey)) {
    ANA_CHECK(buildContainerFromMultipleCollections(m_truthCollections,
                                                    truthParticlesSGKey));
    ANA_CHECK(evtStore()->retrieve(tp, truthParticlesSGKey));
    // we need to be able to navigate from the Ws to their decayProducts, see
    // CalcPartonHistory.h for details
    ANA_CHECK(linkBosonCollections());
  } else {
    ANA_CHECK(evtStore()->retrieve(tp, truthParticlesSGKey));
  }

  return StatusCode::SUCCESS;
}
}  // namespace top
