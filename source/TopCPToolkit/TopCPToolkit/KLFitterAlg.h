#ifndef TOPCPTOOLKIT_KLFITTERALG_H_
#define TOPCPTOOLKIT_KLFITTERALG_H_

#include <SystematicsHandles/SysReadHandle.h>
#include <SelectionHelpers/SysReadSelectionHandle.h>
#include <SystematicsHandles/SysWriteHandle.h>
#include <SystematicsHandles/SysReadDecorHandle.h>
#include <AnaAlgorithm/AnaAlgorithm.h>
#include <xAODEgamma/ElectronContainer.h>
#include <xAODMuon/MuonContainer.h>
#include <xAODJet/JetContainer.h>
#include <xAODMissingET/MissingETContainer.h>
#include <xAODEventInfo/EventInfo.h>
#include "FTagAnalysisInterfaces/IBTaggingEfficiencyTool.h"

#include "TopCPToolkit/KLFitterResultContainer.h"
#include "TopCPToolkit/KLFitterResultAuxContainer.h"

#include "KLFitter/Fitter.h"
#include "KLFitter/DetectorAtlas_8TeV.h"
#include "KLFitter/LikelihoodTopLeptonJets.h"
#include "KLFitter/LikelihoodTTHLeptonJets.h"
#include "KLFitter/LikelihoodTopLeptonJets_JetAngles.h"
#include "KLFitter/LikelihoodTopLeptonJets_Angular.h"
#include "KLFitter/LikelihoodTTZTrilepton.h"
#include "KLFitter/LikelihoodTopAllHadronic.h"
#include "KLFitter/BoostedLikelihoodTopLeptonJets.h"

#include "TopCPToolkit/KLFitterEnums.h"

namespace top {

  class KLFitterAlg final : public EL::AnaAlgorithm {

  public:
    KLFitterAlg(const std::string &name, ISvcLocator *pSvcLocator);
    virtual StatusCode initialize() final;
    virtual StatusCode execute() final;
    virtual StatusCode finalize() final;

  private:
    StatusCode execute_syst(const CP::SystematicSet &sys);
    StatusCode add_leptons(const std::vector<const xAOD::Electron*> &selected_electrons,
                           const std::vector<const xAOD::Muon*> &selected_muons,
                           KLFitter::Particles *myParticles);

    StatusCode add_jets(const std::vector<const xAOD::Jet*> &selected_jets,
                        KLFitter::Particles *myParticles);

    StatusCode setJetskLeadingN(const std::vector<const xAOD::Jet*> &jets,
                                KLFitter::Particles *inputParticles, const size_t njets);

    StatusCode retrieveEfficiencies(const xAOD::Jet *jet, float *eff, float *ineff);

    StatusCode setJetskBtagPriority(const std::vector<const xAOD::Jet*> &jets,
                                    KLFitter::Particles* inputParticles,
                                    const size_t maxJets);

    StatusCode evaluatePermutations(const CP::SystematicSet &sys, const std::vector<size_t> &electron_indices,
                                    const std::vector<size_t> &muon_indices, const std::vector<size_t> &jet_indices);

    template<typename T> std::vector<const T*> sortPt(const std::vector<const T*> &particles,
                                                      std::vector<size_t> &indices) {
      std::vector<std::pair<const T*, size_t>> particle_index(particles.size());
      size_t indx {0};
      for (const T* const p : particles) {
        particle_index[indx] = {p, indx};
        ++indx;
      }
      std::sort(particle_index.begin(), particle_index.end(),
                [](std::pair<const T*, size_t>& x, std::pair<const T*, size_t>& y){return x.first->pt() > y.first->pt();});
      std::vector<const T*> sorted_particles(particles.size());
      indx = 0;
      indices.resize(particles.size());
      for (auto &elem : particle_index) {
        sorted_particles[indx] = elem.first;
        indices[indx] = elem.second;
        ++indx;
      }
      return sorted_particles;
    }

    // systematics
    CP::SysListHandle m_systematicsList {this};

    // inputs needed for reconstruction
    CP::SysReadHandle<xAOD::ElectronContainer> m_electronsHandle {
      this, "electrons", "", "the electron container to use"
    };
    CP::SysReadSelectionHandle m_electronSelection {
      this, "electronSelection", "", "the selection on the input electrons"
    };

    CP::SysReadHandle<xAOD::MuonContainer> m_muonsHandle {
      this, "muons", "", "the muon container to use"
    };
    CP::SysReadSelectionHandle m_muonSelection {
      this, "muonSelection", "", "the selection on the input muons"
    };

    CP::SysReadHandle<xAOD::JetContainer> m_jetsHandle {
      this, "jets", "", "the jet container to use"
    };
    CP::SysReadSelectionHandle m_jetSelection {
      this, "jetSelection", "", "the selection on the input jets"
    };

    CP::SysReadHandle<xAOD::MissingETContainer> m_metHandle {
      this, "met", "", "the MET container to use"
    };

    CP::SysReadHandle<xAOD::EventInfo> m_eventInfoHandle {
      this, "eventInfo", "EventInfo", "the EventInfo container to read selection deciosions from"
    };

    // output container
    CP::SysWriteHandle<xAOD::KLFitterResultContainer, xAOD::KLFitterResultAuxContainer> m_outHandle {
      this, "result", "KLFitterResult_%SYS%", "the output KLFitterResultContainer"
    };

    CP::SysReadSelectionHandle m_selection {
      this, "selectionDecorationName", "", "Name of the selection on which this KLFitter instance is allowed to run"
    };

    // configurable properties
    std::string m_leptonType;
    KLFEnums::LeptonType m_leptonTypeEnum;
    std::string m_LHType;
    KLFEnums::Likelihood m_LHTypeEnum;
    std::string m_transferFunctionsPath;
    std::string m_jetSelectionMode;
    KLFEnums::JetSelectionMode m_jetSelectionModeEnum;
    std::string m_bTaggingMethod;
    std::string m_bTagDecoration;
    std::string m_METterm;

    float m_massTop;
    bool m_fixedTopMass;
    bool m_saveAllPermutations;
    bool m_failOnLessThanXJets;

    bool m_useBtagPriority;
    size_t m_njetsRequirement;

    // Tools and functions for btagging
    // bool HasTag(const xAOD::Jet& jet, double& weight) const;

    std::unique_ptr<KLFitter::Fitter> m_myFitter;
    std::unique_ptr<KLFitter::DetectorAtlas_8TeV> m_myDetector;
    KLFEnums::JetSelectionMode m_jetSelectionModeKLFitterEnum;
    KLFitter::LikelihoodBase::BtaggingMethod m_bTaggingMethodEnum;

    KLFitter::LikelihoodTopLeptonJets::LeptonType m_leptonTypeKLFitterEnum;
    KLFitter::LikelihoodTTHLeptonJets::LeptonType m_leptonTypeKLFitterEnum_TTH;
    KLFitter::LikelihoodTopLeptonJets_JetAngles::LeptonType m_leptonTypeKLFitterEnum_JetAngles;
    KLFitter::LikelihoodTopLeptonJets_Angular::LeptonType m_leptonTypeKLFitterEnum_Angular;
    KLFitter::LikelihoodTTZTrilepton::LeptonType m_leptonTypeKLFitterEnum_TTZ;
    KLFitter::BoostedLikelihoodTopLeptonJets::LeptonType m_leptonTypeKLFitterEnum_BoostedLJets;

    std::unique_ptr<KLFitter::LikelihoodTopLeptonJets> m_myLikelihood;
    std::unique_ptr<KLFitter::LikelihoodTTHLeptonJets> m_myLikelihood_TTH;
    std::unique_ptr<KLFitter::LikelihoodTopLeptonJets_JetAngles> m_myLikelihood_JetAngles;
    std::unique_ptr<KLFitter::LikelihoodTopLeptonJets_Angular> m_myLikelihood_Angular;
    std::unique_ptr<KLFitter::LikelihoodTTZTrilepton> m_myLikelihood_TTZ;
    std::unique_ptr<KLFitter::LikelihoodTopAllHadronic> m_myLikelihood_AllHadronic;
    std::unique_ptr<KLFitter::BoostedLikelihoodTopLeptonJets> m_myLikelihood_BoostedLJets;

    ToolHandle<IBTaggingEfficiencyTool> m_btagging_eff_tool;

    std::unique_ptr<SG::AuxElement::ConstAccessor<char>> m_bTagDecoAcc;
  };
} // namespace top

#endif
