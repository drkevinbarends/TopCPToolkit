#ifndef TOP_PARTICLELEVELALG_H
#define TOP_PARTICLELEVELALG_H

#include <AnaAlgorithm/AnaAlgorithm.h>

#include <vector>

// EDM include(s):
#include "AthContainers/AuxElement.h"
#include "AthContainers/DataVector.h"
#include "xAODCore/AuxContainerBase.h"
#include "xAODCore/CLASS_DEF.h"

// TopParticleLevel
#include "MCTruthClassifier/MCTruthClassifierDefs.h"
#include "TopParticleLevel/ParticleLevel.h"
#include "xAODJet/Jet.h"
#include "xAODJet/JetAuxContainer.h"
#include "xAODJet/JetContainer.h"
#include "xAODMissingET/MissingET.h"
#include "xAODMissingET/MissingETAuxContainer.h"
#include "xAODMissingET/MissingETContainer.h"
#include "xAODTruth/TruthParticle.h"
#include "xAODTruth/TruthParticleAuxContainer.h"
#include "xAODTruth/TruthParticleContainer.h"

namespace top {

  class ParticleLevelAlg final : public EL::AnaAlgorithm {

  public:
    ParticleLevelAlg(const std::string &name, ISvcLocator *pSvcLocator);
    virtual StatusCode initialize() override;
    virtual StatusCode execute() override;
    virtual StatusCode finalize() override;

  private:
    std::string outputSGKey;
    StatusCode save;
    StatusCode saveAux;

    bool m_useTruthElectrons, m_useTruthMuons, m_useTruthPhotons, m_useTruthTaus,
      m_useTruthJets, m_useTruthLargeRJets, m_useTruthNeutrinos, m_useTruthMET,
      m_doOverlapRemoval;
    float m_el_ptMin, m_el_etaMax;
    bool m_el_notFromHadron, m_el_tauIsHadron;
    float m_mu_ptMin, m_mu_etaMax;
    bool m_mu_notFromHadron, m_mu_tauIsHadron;
    float m_ph_ptMin, m_ph_etaMax;
    std::string m_ph_origin, m_ph_isolation;
    float m_tau_ptMin, m_tau_etaMax;
    float m_jet_ptMin, m_jet_etaMax;
    float m_ljet_ptMin, m_ljet_etaMax;
    std::string m_ljet_collection;
    float m_nu_ptMin, m_nu_etaMax;

    const xAOD::TruthParticleContainer *inputElectrons=0, *inputMuons=0,
      *inputPhotons=0, *inputTaus=0, *inputNeutrinos=0;
    xAOD::TruthParticleContainer *outputElectrons=0, *outputMuons=0, *outputPhotons=0,
      *outputTaus=0, *outputNeutrinos=0;
    xAOD::TruthParticleAuxContainer *outputElectronsAux=0, *outputMuonsAux=0,
      *outputPhotonsAux=0, *outputTausAux=0, *outputNeutrinosAux=0;

    const xAOD::JetContainer *inputJets=0, *inputLargeRJets=0;
    xAOD::JetContainer *outputJets=0, *outputLargeRJets=0;
    xAOD::JetAuxContainer *outputJetsAux=0, *outputLargeRJetsAux=0;

    const xAOD::MissingETContainer *inputMissingET=0;
    xAOD::MissingETContainer *outputMissingET=0;
    xAOD::MissingETAuxContainer *outputMissingETAux=0;

    std::vector<MCTruthPartClassifier::ParticleOrigin> photon_origins{};
    std::string photon_isolationVar;
    float photon_isolationCut;

    const xAOD::EventInfo *evtInfo=0;
  };

}  // namespace top

#endif
