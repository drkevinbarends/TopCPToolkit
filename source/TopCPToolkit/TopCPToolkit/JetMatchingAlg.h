#ifndef TOP_JET_MATCHING_ALGORITHM_H
#define TOP_JET_MATCHING_ALGORITHM_H

// Algorithm includes
#include <AnaAlgorithm/AnaAlgorithm.h>
#include <SystematicsHandles/SysReadHandle.h>
#include <SelectionHelpers/SysReadSelectionHandle.h>
#include <SystematicsHandles/SysWriteDecorHandle.h>

// Framework includes
#include <xAODEgamma/ElectronContainer.h>
#include <xAODMuon/MuonContainer.h>
#include <xAODJet/JetContainer.h>
#include <xAODTruth/TruthParticleContainer.h>
#include <xAODMissingET/MissingETContainer.h>
#include <xAODEventInfo/EventInfo.h>

namespace top {
  using ROOT::Math::PtEtaPhiMVector;
  using ROOT::Math::PtEtaPhiEVector;
  
  class JetMatchingAlg final : public EL::AnaAlgorithm {

  public:
    JetMatchingAlg(const std::string &name, ISvcLocator *pSvcLocator);
    virtual StatusCode initialize() override;
    virtual StatusCode execute() override;

  private:
    //systematics
    CP::SysListHandle m_systematicsList {this};
    CP::SysReadHandle<xAOD::EventInfo> m_eventInfoHandle {
      this, "eventInfo", "EventInfo", "the EventInfo container to read selection decisions from"
    };
    CP::SysReadHandle<xAOD::JetContainer> m_jetsHandle {
      this, "jets", "", "the jet container to use"
    };
    CP::SysReadHandle<xAOD::JetContainer> m_truthJetsHandle {
      this, "truthJets", "", "the truthjet container to use"
    };
    CP::SysReadHandle<xAOD::TruthParticleContainer> m_truthElectronsHandle {
      this, "truthElectrons", "", "the electron container to use"
    };
    CP::SysReadHandle<xAOD::TruthParticleContainer> m_truthMuonsHandle {
      this, "truthMuons", "", "the muon container to use"
    };
    CP::SysReadSelectionHandle m_jetSelection {
      this, "jetSelection", "", "the selection on the input jets"
    };
    CP::SysReadSelectionHandle m_selection {
      this, "eventSelection", "", "Name of the selection on which this JetMatching instance is allowed to run"
    };

    //output decorations
    CP::SysWriteDecorHandle<int> m_truth_jet_paired_index {
      this, "truth_jet_paired_index", "truth_jet_paired_index_%SYS%", "Index of the matched true jet"
    };
    CP::SysWriteDecorHandle<float> m_reco_to_reco_jet_closest_dR {
      this, "reco_to_reco_jet_closest_dR", "reco_to_reco_jet_closest_dR_%SYS%", "dR of the closest reco jet"
    };
    CP::SysWriteDecorHandle<float> m_truth_to_truth_jet_closest_dR {
      this, "truth_to_truth_jet_closest_dR", "truth_to_truth_jet_closest_dR_%SYS%", "dR between the matched truth jet and its closest truth jet"
    };
    CP::SysWriteDecorHandle<bool> m_has_truth_lepton {
      this, "has_truth_lepton", "has_truth_lepton_%SYS%", "reco jet has truth lepton at dR < 0.4"
    };

    CP::SysWriteDecorHandle<float> m_overlapping_truth_lepton_pt {
      this, "overlapping_truth_lepton_pt", "overlapping_truth_lepton_pt_%SYS%", "pt of the overlapping truth lepton"
    };


    float m_criticalDR = 0.3;
    float m_criticalDR_leptons = 0.4;
    
    float get_minDR_reco(PtEtaPhiEVector jet1, unsigned int ijet1, ConstDataVector<xAOD::JetContainer> selected_jets);
    float get_minDR_truth(const xAOD::JetContainer &truth_jets, int truth_jet_index);
    int get_matched_truth(PtEtaPhiEVector reco_jet, const xAOD::JetContainer &truth_jets);
    bool find_close_lepton(PtEtaPhiEVector& reco_jet, const xAOD::TruthParticleContainer &truth_electrons, const xAOD::TruthParticleContainer &truth_muons, double& overlapping_truth_lepton_pt);
  };

} // namespace top

#endif
