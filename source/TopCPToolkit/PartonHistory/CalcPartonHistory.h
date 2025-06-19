#ifndef PARTONS_CALCPARTONHISTORY_H
#define PARTONS_CALCPARTONHISTORY_H

// system include(s):
#include <memory>
#include <vector>

// Framework include(s):
#include "AsgTools/AsgTool.h"
#include "AthContainers/AuxElement.h"
#include "AthContainers/DataVector.h"
#include "PartonHistory/PartonHistory.h"
#include "PartonHistory/PartonHistoryUtils.h"
#include "VectorHelpers/DecoratorHelpers.h"
#include "xAODTruth/TruthParticleContainer.h"

namespace top {
using ROOT::Math::PtEtaPhiMVector;

class CalcPartonHistory : public asg::AsgTool {

 public:
  explicit CalcPartonHistory(
      const std::string& name,
      const std::vector<std::string>& truthCollections = {"TruthTop"});
  virtual ~CalcPartonHistory() {};

  CalcPartonHistory(const CalcPartonHistory& rhs) = delete;
  CalcPartonHistory(CalcPartonHistory&& rhs) = delete;
  CalcPartonHistory& operator=(const CalcPartonHistory& rhs) = delete;

  std::map<std::string, std::vector<const xAOD::TruthParticle*>> particleMap;

  PartonDecorator m_dec;

  void AddToParticleMap(const xAOD::TruthParticle* particle,
                        const std::string& key);
  bool ExistsInMap(const std::string& key);
  bool ExistsInKey(const std::string& key, const xAOD::TruthParticle* particle);
  bool Retrievep4(const std::string& key, PtEtaPhiMVector& p4);
  bool Retrievep4(const std::string& key, PtEtaPhiMVector& p4, const int& idx);
  bool Retrievep4Gamma(PtEtaPhiMVector& p4, int& parentpdgId);
  bool RetrievepdgId(const std::string& key, std::vector<int>& pdgIds);
  bool RetrievepdgId(const std::string& key, int& pdgId);
  bool RetrievepdgId(const std::string& key, int& pdgId, const int& idx);
  bool RetrieveParticleInfo(const std::string& prefix,
                            std::vector<const xAOD::TruthParticle*>& particles);
  bool RetrieveParticleInfo(const std::string& prefix,
                            PtEtaPhiMVector& particle, int& pdgId);
  bool RetrieveParticleInfo(const std::string& prefix,
                            PtEtaPhiMVector& particle, int& pdgId,
                            const int& idx);
  bool RetrieveParticleInfo(const std::string& prefix,
                            const std::string& alt_prefix,
                            PtEtaPhiMVector& particle, int& pdgId);
  bool RetrieveParticleInfo(const std::string& prefix,
                            std::vector<PtEtaPhiMVector>& particles,
                            std::vector<int>& pdgIds);

  void Initialize4TopDecorators();
  void InitializeTopDecorators();
  void InitializeAntiTopDecorators();
  void InitializeBottomDecorators();
  void InitializeVectorBottomDecorators();
  void InitializeAntiBottomDecorators();
  void InitializeVectorAntiBottomDecorators();
  void InitializeTtbarDecorators();
  void InitializeHiggsDecorators();
  void InitializePhotonDecorators();
  void InitializeZDecorators();
  void InitializeWDecorators();

  std::string GetParticleType(const xAOD::TruthParticle* particle);

  void TraceParticle(
      const xAOD::TruthParticle* particle,
      std::vector<const xAOD::TruthParticle*>& currentPath,
      std::vector<std::vector<const xAOD::TruthParticle*>>& allPaths);
  void TraceParticles(const xAOD::TruthParticleContainer* truthParticles);

  bool handleFSR(const xAOD::TruthParticle* particle, const std::string& newKey,
                 std::string& key);
  bool handleDecay(const xAOD::TruthParticle* particle, std::string& key,
                   int decayID);
  void handleSameAsParent(const xAOD::TruthParticle* particle,
                          std::string& key);
  void handleDefault(const xAOD::TruthParticle* particle,
                     const std::string& newKey, std::string& key);

  void FillParticleMap(
      std::vector<std::vector<const xAOD::TruthParticle*>>& allPaths);
  void FillGenericPartonHistory(xAOD::PartonHistory* PartonHistory,
                                const std::string& retrievalstring,
                                const std::string& decorationstring,
                                const int idx, PartonDecorator& dec);
  void FillGenericPartonHistory(xAOD::PartonHistory* PartonHistory,
                                const std::string& retrievalstring_1,
                                const std::string& retrievalstring_2,
                                const std::string& decorationstring,
                                const int idx, PartonDecorator& dec);
  void FillGenericVectorPartonHistory(xAOD::PartonHistory* PartonHistory,
                                      const std::string& retrievalstring_1,
                                      const std::string& retrievalstring_2,
                                      const std::string& decorationstring,
                                      PartonDecorator& dec);
  void FillGenericVectorPartonHistory(xAOD::PartonHistory* PartonHistory,
                                      const std::string& retrievalstring,
                                      const std::string& decorationstring,
                                      PartonDecorator& dec);
  void FillGammaPartonHistory(xAOD::PartonHistory* PartonHistory,
                              const std::string& parent, PartonDecorator& dec);
  void FillZPartonHistory(xAOD::PartonHistory* PartonHistory,
                          const std::string& parent, PartonDecorator& dec);
  void FillTtbarPartonHistory(xAOD::PartonHistory* PartonHistory,
                              PartonDecorator& dec);
  void FillHiggsPartonHistory(xAOD::PartonHistory* PartonHistory,
                              const std::string& mode, PartonDecorator& dec);

  // Some specific functions for HWW in Sherpa:
  void setHiggs(xAOD::PartonHistory* PartonHistory, const std::string& fsr,
                PartonDecorator& dec);
  bool getW(const std::string& str_lep, const std::string& str_nu,
            PtEtaPhiMVector& p1, int& pdgId1, PtEtaPhiMVector& p2, int& pdgId2);

  virtual StatusCode execute();

 protected:
  const std::vector<std::string> m_truthCollections;
  std::string m_PartonsSGKey;  // name of the output PartonHistory container to
                               // be stored in eventStore

  // this method is used to perform the linking of various TRUTH3 particle
  // containers for Partons to work
  virtual StatusCode linkTruthContainers(
      const xAOD::TruthParticleContainer*& tp);

  // this is the method that runs the actual parton history reconstruction
  virtual StatusCode runHistorySaver(
      const xAOD::TruthParticleContainer* truthParticles,
      xAOD::PartonHistory* ttbarPartonHistory) = 0;

  virtual void initializeDecorators() = 0;

  /** used to build container from multiple collections
   *   in DAOD_PHYS we don't have the TruthParticles collection, so we have to
   * build a TruthParticleContainer (named out_contName) by merging several
   * collections; this is stored in the evtStore this method has to use some
   * tricks, like the helper m_tempParticles ConstDataVector, due to the desing
   * of DataVector, see
   * https://twiki.cern.ch/twiki/bin/view/AtlasComputing/DataVector
   */
  StatusCode buildContainerFromMultipleCollections(
      const std::vector<std::string>& collections,
      const std::string& out_contName);

  /** currently in DAOD_PHYS TruthTop have links to Ws from the TruthBoson
   * collection, which have no link to their decay products; we have therefore
   * to associate the W from the TruthBoson collections to those in the
   * TruthBosonsWithDecayParticles collection. This method will use the helper
   * method decorateCollectionWithLinksToAnotherCollection to decorate bosons in
   * the TruthBoson collection with "AT_linkToTruthBosonsWithDecayParticles",
   * which is a link to the same bosons in the TruthBosonsWithDecayParticles
   * collection
   */
  StatusCode linkBosonCollections();

  /// helper method to handle retriveing the truth particle linked in the
  /// decoration of another particle
  const xAOD::TruthParticle* getTruthParticleLinkedFromDecoration(
      const xAOD::TruthParticle* part, const std::string& decorationName);

 private:
  /**helper method currently used in DAOD_PHYS to link particles from a given
   * collection to the same particles included in another collection; needed
   * because particles may be duplicated in different collections, but their
   * navigation links may only be there in some of them...
   */
  StatusCode decorateCollectionWithLinksToAnotherCollection(
      const std::string& collectionToDecorate,
      const std::string& collectionToLink, const std::string& nameOfDecoration);
};

}  // namespace top

#endif
