#ifndef PARTON_TO_JETS_MATCH_ALG_H
#define PARTON_TO_JETS_MATCH_ALG_H

// Algorithm includes
#include <AnaAlgorithm/AnaAlgorithm.h>
#include <SelectionHelpers/SysReadSelectionHandle.h>
#include <SystematicsHandles/SysReadDecorHandle.h>
#include <SystematicsHandles/SysReadHandle.h>
#include <SystematicsHandles/SysWriteDecorHandle.h>
#include <SystematicsHandles/SysWriteHandle.h>
#include <xAODEgamma/ElectronContainer.h>
#include <xAODEventInfo/EventInfo.h>
#include <xAODJet/JetContainer.h>
#include <xAODMuon/MuonContainer.h>

#include "PartonHistory/PartonHistory.h"
#include "TopCPToolkit/BaseTopology.h"
#include "TopCPToolkit/MatchingObjects.h"
#include "VectorHelpers/AccessorHelpers.h"

namespace top {
using ROOT::Math::PtEtaPhiEVector;
using ROOT::Math::PtEtaPhiMVector;

class PartonMatchAlg final : public EL::AnaAlgorithm {

 public:
  PartonMatchAlg(const std::string& name, ISvcLocator* pSvcLocator);
  virtual StatusCode initialize() override final;
  virtual StatusCode execute() override final;
  virtual StatusCode finalize() override final;

  int getNLeptons() const { return m_nLeptons; }
  void setNLeptons(int nLeptons) { m_nLeptons = nLeptons; }

  const std::vector<TruthObject>& getTruthJets() const { return m_truth_jets; }
  void setTruthJets(const std::vector<TruthObject>& jets) {
    m_truth_jets = jets;
  }
  void addTruthJet(TruthObject&& jet) {
    m_truth_jets.emplace_back(std::move(jet));
  }

  const std::vector<RecoObject>& getRecoJets() const { return m_reco_jets; }
  void setRecoJets(const std::vector<RecoObject>& jets) { m_reco_jets = jets; }
  void addRecoJet(RecoObject&& jet) {
    m_reco_jets.emplace_back(std::move(jet));
  }

  const std::vector<int>& getTruthJetIndices() const {
    return m_truth_jet_indices;
  }
  void setTruthJetIndices(const std::vector<int>& indices) {
    m_truth_jet_indices = indices;
  }
  void addTruthJetIndex(int index) { m_truth_jet_indices.emplace_back(index); }

  const std::vector<int>& getTruthJetCandidates() const {
    return m_truth_jet_candidates;
  }
  void setTruthJetCandidates(const std::vector<int>& candidates) {
    m_truth_jet_candidates = candidates;
  }
  void addTruthJetCandidate(int candidate) {
    m_truth_jet_candidates.emplace_back(candidate);
  }

  const std::vector<TruthObject>& getTruthElectrons() const {
    return m_truth_electrons;
  }
  void setTruthElectrons(const std::vector<TruthObject>& electrons) {
    m_truth_electrons = electrons;
  }
  void addTruthElectron(TruthObject&& electron) {
    m_truth_electrons.emplace_back(std::move(electron));
  }

  const std::vector<RecoObject>& getRecoElectrons() const {
    return m_reco_electrons;
  }
  void setRecoElectrons(const std::vector<RecoObject>& electrons) {
    m_reco_electrons = electrons;
  }
  void addRecoElectron(RecoObject&& electron) {
    m_reco_electrons.emplace_back(std::move(electron));
  }

  const std::vector<int>& getTruthElectronIndices() const {
    return m_truth_electron_indices;
  }
  void setTruthElectronIndices(const std::vector<int>& indices) {
    m_truth_electron_indices = indices;
  }
  void addTruthElectronIndex(int index) {
    m_truth_electron_indices.emplace_back(index);
  }

  const std::vector<int>& getTruthElectronCandidates() const {
    return m_truth_electron_candidates;
  }
  void setTruthElectronCandidates(const std::vector<int>& candidates) {
    m_truth_electron_candidates = candidates;
  }
  void addTruthElectronCandidate(int candidate) {
    m_truth_electron_candidates.emplace_back(candidate);
  }

  const std::vector<TruthObject>& getTruthMuons() const {
    return m_truth_muons;
  }
  void setTruthMuons(const std::vector<TruthObject>& muons) {
    m_truth_muons = muons;
  }
  void addTruthMuon(TruthObject&& muon) {
    m_truth_muons.emplace_back(std::move(muon));
  }

  const std::vector<RecoObject>& getRecoMuons() const { return m_reco_muons; }
  void setRecoMuons(const std::vector<RecoObject>& muons) {
    m_reco_muons = muons;
  }
  void addRecoMuon(RecoObject&& muon) {
    m_reco_muons.emplace_back(std::move(muon));
  }

  const std::vector<int>& getTruthMuonIndices() const {
    return m_truth_muon_indices;
  }
  void setTruthMuonIndices(const std::vector<int>& indices) {
    m_truth_muon_indices = indices;
  }
  void addTruthMuonIndex(int index) {
    m_truth_muon_indices.emplace_back(index);
  }

  const std::vector<int>& getTruthMuonCandidates() const {
    return m_truth_muon_candidates;
  }
  void setTruthMuonCandidates(const std::vector<int>& candidates) {
    m_truth_muon_candidates = candidates;
  }
  void addTruthMuonCandidate(int candidate) {
    m_truth_muon_candidates.emplace_back(candidate);
  }

  void reset();
  // Template function for emplace_back
  template <typename T>
  void addToVector(std::vector<T>& vec, T&& value);

  void processCandidatesAndMatch();

  PartonAccessor m_partonAcc;

 private:
  StatusCode execute_syst(const CP::SystematicSet& sys);
  void matchRecursive(std::vector<TruthObject>& truthJets,
                      std::vector<RecoObject>& RecoJets, float criticalDR);
  void getTruthCandidates(std::vector<TruthObject>& truthJets,
                          std::vector<RecoObject>& recoJets, float criticalDR);
  void processTruthObjects(std::vector<TruthObject>& truthJets,
                           std::vector<int>& candidates,
                           std::vector<int>& indices);

  // systematics
  CP::SysListHandle m_systematicsList{this};

  CP::SysReadHandle<xAOD::JetContainer> m_jetsHandle{
      this, "jets", "", "the jet container to use"};
  CP::SysReadSelectionHandle m_jetSelection{this, "jetSelection", "",
                                            "the selection on the input jets"};
  CP::SysReadHandle<xAOD::ElectronContainer> m_electronsHandle{
      this, "electrons", "", "the electron container to use"};
  CP::SysReadSelectionHandle m_electronSelection{
      this, "electronSelection", "", "the selection on the input electrons"};
  CP::SysReadHandle<xAOD::MuonContainer> m_muonsHandle{
      this, "muons", "", "the muon container to use"};
  CP::SysReadSelectionHandle m_muonSelection{
      this, "muonSelection", "", "the selection on the input muons"};
  CP::SysReadHandle<xAOD::EventInfo> m_eventInfoHandle{
      this, "eventInfo", "EventInfo",
      "the EventInfo container to read selection decisions from"};

  CP::SysReadSelectionHandle m_selection{
      this, "eventSelection", "",
      "Name of the selection on which this SPA-Net instance is allowed to run"};

  CP::SysWriteDecorHandle<std::vector<int>> m_jet_truth_candidates_decor{
      this, "event_jet_truth_candidates", "event_jet_truth_candidates_%SYS%",
      "Number of matching candidates of the jet"};
  CP::SysWriteDecorHandle<std::vector<int>> m_jet_truth_idx_decor{
      this, "event_jet_truth_idx", "event_jet_truth_index_%SYS%",
      "Index of the jet matched to the parton truth"};
  CP::SysWriteDecorHandle<std::vector<int>> m_electron_truth_candidates_decor{
      this, "event_electron_truth_candidates",
      "event_electron_truth_candidates_%SYS%",
      "Number of matching candidates of the electron"};
  CP::SysWriteDecorHandle<std::vector<int>> m_electron_truth_idx_decor{
      this, "event_electron_truth_idx", "event_electron_truth_index_%SYS%",
      "Index of the electron matched to the parton truth"};
  CP::SysWriteDecorHandle<std::vector<int>> m_muon_truth_candidates_decor{
      this, "event_muon_truth_candidates", "event_muon_truth_candidates_%SYS%",
      "Number of matching candidates of the muon"};
  CP::SysWriteDecorHandle<std::vector<int>> m_muon_truth_idx_decor{
      this, "event_muon_truth_idx", "event_muon_truth_index_%SYS%",
      "Index of the muon matched to the parton truth"};
  CP::SysWriteDecorHandle<int> m_event_nLeptons_decor{
      this, "event_nLeptons", "event_nLeptons_%SYS%",
      "Number of leptons in the event"};

  std::vector<TruthObject> m_truth_jets;
  std::vector<TruthObject> m_truth_electrons;
  std::vector<TruthObject> m_truth_muons;
  std::vector<RecoObject> m_reco_jets;
  std::vector<RecoObject> m_reco_electrons;
  std::vector<RecoObject> m_reco_muons;
  std::vector<int> m_truth_jet_indices;
  std::vector<int> m_truth_jet_candidates;
  std::vector<int> m_truth_electron_indices;
  std::vector<int> m_truth_electron_candidates;
  std::vector<int> m_truth_muon_indices;
  std::vector<int> m_truth_muon_candidates;
  int m_nLeptons = 0;

  float m_criticalJetDR = 0.3;
  float m_criticalLeptonDR = 0.1;
  bool m_acceptDoubleMatches = false;
  std::string m_partonContainerName;
  std::string m_partonTopology;
  std::unique_ptr<BaseTopology> m_topology;
};
}  // namespace top

#endif
