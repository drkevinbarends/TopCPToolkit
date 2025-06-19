#include "TopCPToolkit/PartonMatchAlg.h"

#include "AthContainers/ConstDataVector.h"
#include "PartonHistory/PartonHistory.h"
#include "TopCPToolkit/FourTopTopology.h"
#include "TopCPToolkit/TtbarTopology.h"
#include "TopCPToolkit/TthTopology.h"
#include "TopCPToolkit/TtzTopology.h"
#include "VectorHelpers/LorentzHelper.h"

namespace top {
using ROOT::Math::PtEtaPhiEVector;
using ROOT::Math::PtEtaPhiMVector;
using ROOT::Math::VectorUtil::DeltaR;

PartonMatchAlg::PartonMatchAlg(const std::string &name,
                               ISvcLocator *pSvcLocator)
    : EL::AnaAlgorithm(name, pSvcLocator) {
  declareProperty("criticalJetDR", m_criticalJetDR,
                  "Maximum delta R for matching jets");
  declareProperty("criticalLeptonDR", m_criticalLeptonDR,
                  "Maximum delta R for matching leptons");
  declareProperty("acceptDoubleMatches", m_acceptDoubleMatches,
                  "Boolean to decide whether to accept double-matches");
  declareProperty("partonContainerName", m_partonContainerName,
                  "Name of the parton container");
  declareProperty("partonTopology", m_partonTopology,
                  "Name of the parton topology");
}

StatusCode PartonMatchAlg::initialize() {
  ANA_MSG_INFO("Initializing PartonMatchAlg " << name());
  ANA_MSG_INFO("  - partonContainerName " << m_partonContainerName);
  ANA_MSG_INFO("  - criticalJetDR " << m_criticalJetDR);
  ANA_MSG_INFO("  - criticalLeptonDR " << m_criticalLeptonDR);
  ANA_MSG_INFO("  - acceptDoubleMatches " << m_acceptDoubleMatches);
  if (m_partonTopology == "Ttbar") {
    m_topology = std::make_unique<TtbarTopology>();
  } else if (m_partonTopology == "FourTop") {
    m_topology = std::make_unique<FourTopTopology>();
  } else if (m_partonTopology == "Tth") {
    m_topology = std::make_unique<TthTopology>();
  } else if (m_partonTopology == "Ttz") {
    m_topology = std::make_unique<TtzTopology>();
  } else {
    ANA_MSG_ERROR("Unknown topology: " << m_partonTopology);
    return StatusCode::FAILURE;
  }

  ANA_CHECK(m_jet_truth_candidates_decor.initialize(
      m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));
  ANA_CHECK(m_jet_truth_idx_decor.initialize(
      m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));
  ANA_CHECK(m_electron_truth_candidates_decor.initialize(
      m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));
  ANA_CHECK(m_electron_truth_idx_decor.initialize(
      m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));
  ANA_CHECK(m_muon_truth_candidates_decor.initialize(
      m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));
  ANA_CHECK(m_muon_truth_idx_decor.initialize(
      m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));
  ANA_CHECK(m_event_nLeptons_decor.initialize(
      m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));

  ANA_CHECK(m_jetsHandle.initialize(m_systematicsList));
  ANA_CHECK(m_jetSelection.initialize(m_systematicsList, m_jetsHandle,
                                      SG::AllowEmpty));
  ANA_CHECK(m_electronsHandle.initialize(m_systematicsList));
  ANA_CHECK(m_electronSelection.initialize(m_systematicsList, m_electronsHandle,
                                           SG::AllowEmpty));
  ANA_CHECK(m_muonsHandle.initialize(m_systematicsList));
  ANA_CHECK(m_muonSelection.initialize(m_systematicsList, m_muonsHandle,
                                       SG::AllowEmpty));
  ANA_CHECK(m_eventInfoHandle.initialize(m_systematicsList));
  ANA_CHECK(m_selection.initialize(m_systematicsList, m_eventInfoHandle,
                                   SG::AllowEmpty));

  ANA_CHECK(m_systematicsList.initialize());

  return StatusCode::SUCCESS;
}

StatusCode PartonMatchAlg::execute() {
  for (const auto &sys : m_systematicsList.systematicsVector()) {
    ANA_CHECK(execute_syst(sys));
  }
  return StatusCode::SUCCESS;
}

StatusCode PartonMatchAlg::finalize() {
  ANA_MSG_INFO("Finalizing PartonMatchAlg");

  return StatusCode::SUCCESS;
}

StatusCode PartonMatchAlg::execute_syst(const CP::SystematicSet &sys) {
  const xAOD::EventInfo *evtInfo = nullptr;
  ANA_CHECK(m_eventInfoHandle.retrieve(evtInfo, sys));

  if (m_selection && !m_selection.getBool(*evtInfo, sys)) {
    return StatusCode::SUCCESS;
  }

  const xAOD::JetContainer *jets = nullptr;
  const xAOD::ElectronContainer *electrons = nullptr;
  const xAOD::MuonContainer *muons = nullptr;

  ANA_CHECK(m_jetsHandle.retrieve(jets, sys));
  ANA_CHECK(m_electronsHandle.retrieve(electrons, sys));
  ANA_CHECK(m_muonsHandle.retrieve(muons, sys));

  ConstDataVector<xAOD::JetContainer> selected_jets(SG::VIEW_ELEMENTS);
  ConstDataVector<xAOD::ElectronContainer> selected_electrons(
      SG::VIEW_ELEMENTS);
  ConstDataVector<xAOD::MuonContainer> selected_muons(SG::VIEW_ELEMENTS);

  for (const xAOD::Jet *jet : *jets) {
    if (m_jetSelection.getBool(*jet, sys))
      selected_jets.push_back(jet);
  }
  for (const xAOD::Electron *electron : *electrons) {
    if (m_electronSelection.getBool(*electron, sys))
      selected_electrons.push_back(electron);
  }
  for (const xAOD::Muon *muon : *muons) {
    if (m_muonSelection.getBool(*muon, sys))
      selected_muons.push_back(muon);
  }

  reset();

  for (std::size_t ijet = 0; ijet < selected_jets.size(); ++ijet) {
    PtEtaPhiEVector reco_jet;
    reco_jet.SetCoordinates(
        selected_jets.at(ijet)->pt(), selected_jets.at(ijet)->eta(),
        selected_jets.at(ijet)->phi(), selected_jets.at(ijet)->e());
    m_reco_jets.emplace_back(ijet, reco_jet, false);
  }

  for (std::size_t ielectron = 0; ielectron < selected_electrons.size();
       ++ielectron) {
    PtEtaPhiEVector reco_electron;
    reco_electron.SetCoordinates(selected_electrons.at(ielectron)->pt(),
                                 selected_electrons.at(ielectron)->eta(),
                                 selected_electrons.at(ielectron)->phi(),
                                 selected_electrons.at(ielectron)->e());
    m_reco_electrons.emplace_back(ielectron, reco_electron, false);
  }

  for (std::size_t imuon = 0; imuon < selected_muons.size(); ++imuon) {
    PtEtaPhiEVector reco_muon;
    reco_muon.SetCoordinates(
        selected_muons.at(imuon)->pt(), selected_muons.at(imuon)->eta(),
        selected_muons.at(imuon)->phi(), selected_muons.at(imuon)->e());
    m_reco_muons.emplace_back(imuon, reco_muon, false);
  }

  const xAOD::PartonHistory *hist = nullptr;
  ANA_CHECK(evtStore()->retrieve(hist, m_partonContainerName));
  m_topology->setHistory(hist);

  m_topology->fillTopology(m_truth_jets, m_truth_electrons, m_truth_muons);
  m_nLeptons = m_topology->getNLeptons();
  m_jet_truth_candidates_decor.set(*evtInfo, std::vector<int>(), sys);
  m_jet_truth_idx_decor.set(*evtInfo, std::vector<int>(), sys);
  m_electron_truth_candidates_decor.set(*evtInfo, std::vector<int>(), sys);
  m_electron_truth_idx_decor.set(*evtInfo, std::vector<int>(), sys);
  m_muon_truth_candidates_decor.set(*evtInfo, std::vector<int>(), sys);
  m_muon_truth_idx_decor.set(*evtInfo, std::vector<int>(), sys);
  m_event_nLeptons_decor.set(*evtInfo, -1, sys);

  processCandidatesAndMatch();

  m_event_nLeptons_decor.set(*evtInfo, m_nLeptons, sys);
  m_jet_truth_candidates_decor.set(*evtInfo, m_truth_jet_candidates, sys);
  m_jet_truth_idx_decor.set(*evtInfo, m_truth_jet_indices, sys);
  m_electron_truth_candidates_decor.set(*evtInfo, m_truth_electron_candidates,
                                        sys);
  m_electron_truth_idx_decor.set(*evtInfo, m_truth_electron_indices, sys);
  m_muon_truth_candidates_decor.set(*evtInfo, m_truth_muon_candidates, sys);
  m_muon_truth_idx_decor.set(*evtInfo, m_truth_muon_indices, sys);
  return StatusCode::SUCCESS;
}

void PartonMatchAlg::getTruthCandidates(std::vector<TruthObject> &truth,
                                        std::vector<RecoObject> &reco,
                                        float criticalDR) {
  int nmatches = 0;
  for (std::size_t i = 0; i < truth.size(); ++i) {
    nmatches = 0;
    for (std::size_t j = 0; j < reco.size(); ++j) {
      double dr = DeltaR(truth[i].getP4(), reco[j].getP4());
      if (dr < criticalDR)
        nmatches++;
    }
    truth[i].setMatches(nmatches);
  }
}

void PartonMatchAlg::processTruthObjects(std::vector<TruthObject> &truthJets,
                                         std::vector<int> &candidates,
                                         std::vector<int> &indices) {
  for (std::size_t i = 0; i < truthJets.size(); i++) {
    candidates.push_back(truthJets[i].getMatches());
    if (truthJets[i].getMatches() == 1 ||
        (truthJets[i].getMatches() > 1 && m_acceptDoubleMatches)) {
      indices.push_back(truthJets[i].getIdx());
    } else {
      indices.push_back(-1);
    }
  }
}

void PartonMatchAlg::matchRecursive(std::vector<TruthObject> &truthJets,
                                    std::vector<RecoObject> &recoJets,
                                    float criticalDR) {
  double minDeltaR = std::numeric_limits<double>::max();
  std::size_t bestTruthIndex = 0, bestRecoIndex = 0;

  // Find the closest pair of jets in DeltaR
  bool match = false;
  for (std::size_t i = 0; i < truthJets.size(); ++i) {
    for (std::size_t j = 0; j < recoJets.size(); ++j) {
      double dr = DeltaR(truthJets[i].getP4(), recoJets[j].getP4());
      if (dr < minDeltaR && dr < criticalDR) {
        if (truthJets[i].isMatched() || recoJets[j].isMatched())
          continue;
        minDeltaR = dr;
        bestTruthIndex = i;
        bestRecoIndex = j;
        match = true;
      }
    }
  }
  // Record the match
  if (match) {
    truthJets[bestTruthIndex].setIdx(bestRecoIndex);
    truthJets[bestTruthIndex].setMatched();
    recoJets[bestRecoIndex].setMatched();
    // Recursively match the remaining jets
    matchRecursive(truthJets, recoJets, m_criticalJetDR);
  } else {  // Base case: no jets left to match
    return;
  }
}

void PartonMatchAlg::reset() {
  setNLeptons(0);

  setRecoJets({});
  setTruthJets({});
  setTruthJetIndices({});
  setTruthJetCandidates({});

  setTruthElectrons({});
  setRecoElectrons({});
  setTruthElectronIndices({});
  setTruthElectronCandidates({});

  setTruthMuons({});
  setRecoMuons({});
  setTruthMuonIndices({});
  setTruthMuonCandidates({});
}

void PartonMatchAlg::processCandidatesAndMatch() {
  // Determine number of candidates per jet, electron, and muon
  getTruthCandidates(m_truth_jets, m_reco_jets, m_criticalJetDR);
  getTruthCandidates(m_truth_electrons, m_reco_electrons, m_criticalLeptonDR);
  getTruthCandidates(m_truth_muons, m_reco_muons, m_criticalLeptonDR);

  // Perform the matching
  matchRecursive(m_truth_jets, m_reco_jets, m_criticalJetDR);
  matchRecursive(m_truth_electrons, m_reco_electrons, m_criticalLeptonDR);
  matchRecursive(m_truth_muons, m_reco_muons, m_criticalLeptonDR);

  // Fill truth index vector
  processTruthObjects(m_truth_jets, m_truth_jet_candidates,
                      m_truth_jet_indices);
  processTruthObjects(m_truth_electrons, m_truth_electron_candidates,
                      m_truth_electron_indices);
  processTruthObjects(m_truth_muons, m_truth_muon_candidates,
                      m_truth_muon_indices);
}
}  // namespace top
