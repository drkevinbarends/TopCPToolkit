#include <Math/Vector4D.h>
#include <math.h>

#include <vector>

#include "AthContainers/ConstDataVector.h"

// HyPER includes
#include "TopCPToolkit/RunHyPERAlg.h"
#include "TopHyPER/HyPERModel.h"
#include "TopHyPER/HyPERTtbarAllHadronicModel.h"
#include "TopHyPER/HyPERTtbarAllHadronicParser.h"
#include "TopHyPER/HyPERTtbarLJetsModel.h"
#include "TopHyPER/HyPERTtbarLJetsParser.h"
#include "TopHyPER/HyPERUtils.h"

namespace top {
MSG::Level RunHyPERAlg::s_msgLevelHyPER = MSG::INFO;
std::size_t getIndexFromLabel(const std::vector<std::string> &vec,
                              const std::string &label) {
  auto it = std::find(vec.begin(), vec.end(), label);
  if (it == vec.end()) {
    throw std::runtime_error("HyPERAlg:: Could not find the label: " + label);
  } else {
    return std::distance(vec.begin(), it);
  }
}

RunHyPERAlg::RunHyPERAlg(const std::string &name, ISvcLocator *pSvcLocator)
    : EL::AnaAlgorithm(name, pSvcLocator),
      m_topology("SetMe"),
      m_btagger("SetMe"),
      m_fullLogEventNumber(0) {
  declareProperty(
      "topology", m_topology,
      " HyPER topology. Choose between: 'TtbarLJets', 'TtbarAllHadronic'.");
  declareProperty("btagger", m_btagger = "ftag_select_GN2v01_FixedCutBEff_77",
                  "Name of the b-tagger and working point for jets");
  declareProperty("fullLogEventNumber", m_fullLogEventNumber = 0,
                  "Number of the event to log in full detail");
  declareProperty(
      "trainedOnEvenPath", m_trainedOnEvenPath,
      "Path to the ONNX model trained on events with even run number.");
  declareProperty(
      "trainedOnOddPath", m_trainedOnOddPath,
      "Path to the ONNX model trained on events with odd run number.");
}

StatusCode RunHyPERAlg::initialize() {
  ANA_MSG_INFO("Initializing RunHyPER " << name());
  ANA_MSG_INFO("  --> topology: " << m_topology);
  ANA_MSG_INFO("  --> btagger: " << m_btagger);
  ANA_MSG_INFO("  --> debug level: " << static_cast<int>(this->msg().level()));
  ANA_MSG_INFO("  --> full log event number: " << m_fullLogEventNumber);
  ANA_MSG_INFO("  --> trainedOnEvenPath: " << m_trainedOnEvenPath);
  ANA_MSG_INFO("  --> trainedOnOddPath: " << m_trainedOnOddPath);

  // Load the b-tagging decoration
  m_btagger = "ftag_quantile_" + m_btagger;
  m_bTagDecoAcc =
      std::make_unique<SG::AuxElement::ConstAccessor<int>>(m_btagger);

  // Parse topology
  HyPERTopology hyperTopology = strToHyPERTopology(m_topology);
  if (hyperTopology == top::HyPERTopology::NotSelected) {
    ANA_MSG_ERROR("Unrecognized HyPER topology: " << m_topology);
    return StatusCode::FAILURE;
  }

  // Initialise object containers
  ANA_CHECK(m_electronsHandle.initialize(m_systematicsList));
  ANA_CHECK(m_muonsHandle.initialize(m_systematicsList));
  ANA_CHECK(m_jetsHandle.initialize(m_systematicsList));
  ANA_CHECK(m_metHandle.initialize(m_systematicsList));
  ANA_CHECK(m_eventInfoHandle.initialize(m_systematicsList));
  ANA_CHECK(m_electronSelection.initialize(m_systematicsList, m_electronsHandle,
                                           SG::AllowEmpty));
  ANA_CHECK(m_muonSelection.initialize(m_systematicsList, m_muonsHandle,
                                       SG::AllowEmpty));
  ANA_CHECK(m_jetSelection.initialize(m_systematicsList, m_jetsHandle,
                                      SG::AllowEmpty));
  // Intialise pre-selection
  ANA_CHECK(m_selection.initialize(m_systematicsList, m_eventInfoHandle,
                                   SG::AllowEmpty));

  // Decorations (topology dependent)
  if (hyperTopology == top::HyPERTopology::TtbarAllHadronic) {
    ANA_CHECK(m_hyper_TtbarAllHadronic_Top1_Indices.initialize(
        m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));
    ANA_CHECK(m_hyper_TtbarAllHadronic_Top1_Score.initialize(
        m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));
    ANA_CHECK(m_hyper_TtbarAllHadronic_Top2_Indices.initialize(
        m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));
    ANA_CHECK(m_hyper_TtbarAllHadronic_Top2_Score.initialize(
        m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));
    ANA_CHECK(m_hyper_TtbarAllHadronic_W1_Indices.initialize(
        m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));
    ANA_CHECK(m_hyper_TtbarAllHadronic_W1_Score.initialize(
        m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));
    ANA_CHECK(m_hyper_TtbarAllHadronic_W2_Indices.initialize(
        m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));
    ANA_CHECK(m_hyper_TtbarAllHadronic_W2_Score.initialize(
        m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));
  }
  if (hyperTopology == top::HyPERTopology::TtbarLJets) {
    ANA_CHECK(m_hyper_TtbarLJets_TopHad_Indices.initialize(
        m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));
    ANA_CHECK(m_hyper_TtbarLJets_TopLep_Indices.initialize(
        m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));
    ANA_CHECK(m_hyper_TtbarLJets_WHad_Indices.initialize(
        m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));
    ANA_CHECK(m_hyper_TtbarLJets_WLep_Indices.initialize(
        m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));
    ANA_CHECK(m_hyper_TtbarLJets_TopHad_Score.initialize(
        m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));
    ANA_CHECK(m_hyper_TtbarLJets_TopLep_Score.initialize(
        m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));
    ANA_CHECK(m_hyper_TtbarLJets_WHad_Score.initialize(
        m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));
    ANA_CHECK(m_hyper_TtbarLJets_WLep_Score.initialize(
        m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));
    ANA_CHECK(m_hyper_TtbarLJets_TopHad_IDs.initialize(
        m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));
    ANA_CHECK(m_hyper_TtbarLJets_TopLep_IDs.initialize(
        m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));
  }

  // Initialise systematics
  ANA_CHECK(m_systematicsList.initialize());

  // Initialise the correct HyPER model
  if (hyperTopology == top::HyPERTopology::TtbarLJets) {
    ANA_MSG_INFO("Loading HyPER model.");
    m_hyperModel = std::make_unique<top::HyPERTtbarLJetsModel>(
        m_topology, m_trainedOnEvenPath, m_trainedOnOddPath);
    ANA_MSG_DEBUG("Loaded HyPER model!");

    ANA_MSG_DEBUG("Initialising HyPERParser");
    m_hyperParser = std::make_unique<top::HyPERTtbarLJetsParser>();
    ANA_MSG_DEBUG("Initialised HyPERParser");

  } else if (hyperTopology == top::HyPERTopology::TtbarAllHadronic) {
    ANA_MSG_INFO("Loading HyPER model.");
    m_hyperModel = std::make_unique<top::HyPERTtbarAllHadronicModel>(
        m_topology, m_trainedOnEvenPath, m_trainedOnOddPath);
    ANA_MSG_DEBUG("Loaded HyPER model!");

    ANA_MSG_DEBUG("Initialising HyPERParser");
    m_hyperParser = std::make_unique<top::HyPERTtbarAllHadronicParser>();
    ANA_MSG_DEBUG("Initialised HyPERParser");
  }

  // Initialisation of the HyPERGraph
  ANA_MSG_DEBUG("Initialising HyPERGraph");
  m_hyperGraph = std::make_unique<top::HyPERGraph>();
  ANA_MSG_DEBUG("Initialised HyPERGraph");

  return StatusCode::SUCCESS;
}

StatusCode RunHyPERAlg::execute() {
  // Loop over systematics
  for (const auto &sys : m_systematicsList.systematicsVector()) {
    // Check the event selection
    const xAOD::EventInfo *evtInfo = nullptr;
    ANA_CHECK(m_eventInfoHandle.retrieve(evtInfo, sys));

    if (m_fullLogEventNumber != 0) {  // User wants to log a particular event.
      if (evtInfo->eventNumber() == m_fullLogEventNumber) {
        this->msg().setLevel(MSG::VERBOSE);
        s_msgLevelHyPER = MSG::VERBOSE;
      } else {
        this->msg().setLevel(MSG::INFO);
        s_msgLevelHyPER = MSG::INFO;
      }
    }
    ANA_MSG_VERBOSE("Event number.... : " << evtInfo->eventNumber());

    // Default decoration values (topology dependent)
    HyPERTopology hyperTopology = strToHyPERTopology(m_topology);
    if (hyperTopology == top::HyPERTopology::TtbarAllHadronic) {
      m_hyper_TtbarAllHadronic_Top1_Indices.set(
          *evtInfo, std::vector<int64_t>{-1, -1, -1}, sys);
      m_hyper_TtbarAllHadronic_Top1_Score.set(*evtInfo, -1, sys);
      m_hyper_TtbarAllHadronic_Top2_Indices.set(
          *evtInfo, std::vector<int64_t>{-1, -1, -1}, sys);
      m_hyper_TtbarAllHadronic_Top2_Score.set(*evtInfo, -1, sys);
      m_hyper_TtbarAllHadronic_W1_Indices.set(
          *evtInfo, std::vector<int64_t>{-1, -1, -1}, sys);
      m_hyper_TtbarAllHadronic_W1_Score.set(*evtInfo, -1, sys);
      m_hyper_TtbarAllHadronic_W2_Indices.set(
          *evtInfo, std::vector<int64_t>{-1, -1, -1}, sys);
      m_hyper_TtbarAllHadronic_W2_Score.set(*evtInfo, -1, sys);
    }
    if (hyperTopology == top::HyPERTopology::TtbarLJets) {
      m_hyper_TtbarLJets_TopHad_Indices.set(
          *evtInfo, std::vector<int64_t>{-1, -1, -1}, sys);
      m_hyper_TtbarLJets_TopLep_Indices.set(
          *evtInfo, std::vector<int64_t>{-1, -1, -1}, sys);
      m_hyper_TtbarLJets_WHad_Indices.set(*evtInfo,
                                          std::vector<int64_t>{-1, -1}, sys);
      m_hyper_TtbarLJets_WLep_Indices.set(*evtInfo,
                                          std::vector<int64_t>{-1, -1}, sys);
      m_hyper_TtbarLJets_TopHad_Score.set(*evtInfo, -1, sys);
      m_hyper_TtbarLJets_TopLep_Score.set(*evtInfo, -1, sys);
      m_hyper_TtbarLJets_WHad_Score.set(*evtInfo, -1, sys);
      m_hyper_TtbarLJets_WLep_Score.set(*evtInfo, -1, sys);
      m_hyper_TtbarLJets_TopHad_IDs.set(*evtInfo,
                                        std::vector<int64_t>{-1, -1, -1}, sys);
      m_hyper_TtbarLJets_TopLep_IDs.set(*evtInfo,
                                        std::vector<int64_t>{-1, -1, -1}, sys);
    }

    if (m_selection && !m_selection.getBool(*evtInfo, sys))
      continue;

    // Read the containers
    const xAOD::ElectronContainer *electrons = nullptr;
    ANA_CHECK(m_electronsHandle.retrieve(electrons, sys));
    const xAOD::MuonContainer *muons = nullptr;
    ANA_CHECK(m_muonsHandle.retrieve(muons, sys));
    const xAOD::JetContainer *jets = nullptr;
    ANA_CHECK(m_jetsHandle.retrieve(jets, sys));
    const xAOD::MissingETContainer *met = nullptr;
    ANA_CHECK(m_metHandle.retrieve(met, sys));

    // Apply object-wise selection
    ConstDataVector<xAOD::ElectronContainer> selected_electrons(
        SG::VIEW_ELEMENTS);
    ConstDataVector<xAOD::MuonContainer> selected_muons(SG::VIEW_ELEMENTS);
    ConstDataVector<xAOD::JetContainer> selected_jets(SG::VIEW_ELEMENTS);
    std::vector<int> selected_jets_btag;

    ANA_MSG_DEBUG("Building particle containers.");

    ANA_MSG_DEBUG("Building electrons container.");
    for (const xAOD::Electron *el : *electrons) {
      if (m_electronSelection.getBool(*el, sys))
        selected_electrons.push_back(el);
    }

    ANA_MSG_DEBUG("Building muons container.");
    for (const xAOD::Muon *mu : *muons) {
      if (m_muonSelection.getBool(*mu, sys))
        selected_muons.push_back(mu);
    }

    ANA_MSG_DEBUG("Building jets container.");
    for (const xAOD::Jet *jet : *jets) {
      if (m_jetSelection.getBool(*jet, sys))
        selected_jets.push_back(jet);
    }

    // Build the HyPER graph
    m_hyperInputs.m_electrons = selected_electrons;
    m_hyperInputs.m_muons = selected_muons;
    m_hyperInputs.m_jets = selected_jets;
    m_hyperInputs.m_met = met;
    ANA_MSG_DEBUG("Building graph.");
    ANA_CHECK(this->buildGraph());
    ANA_MSG_DEBUG("Graph building successful.");

    if (this->msg().level() == MSG::VERBOSE) {
      ANA_MSG_VERBOSE("Printing the graph...");
      m_hyperGraph->printGraph();
    }

    // Parse the inputs to the HyPER algorithm
    ANA_MSG_DEBUG("Building the ONNX inputs.");
    m_hyperParser->buildONNXInputs(*m_hyperGraph, *m_hyperModel);
    ANA_MSG_DEBUG("ONNX inputs building successful.");

    // Run the HyPER algorithm
    ANA_MSG_DEBUG("Evaluating model.");
    unsigned int modelToUse;
    // The model TRAINED on even should always be at position [0] in the
    // m_session vector so, m_session[1] should be used for even events
    modelToUse = evtInfo->eventNumber() % 2 == 0 ? 1 : 0;

    ANA_MSG_DEBUG("Using model: " << modelToUse);
    m_hyperModel->evaluate(modelToUse);
    ANA_MSG_DEBUG("Model evaluation successful.");

    if (this->msg().level() == MSG::VERBOSE) {
      ANA_MSG_INFO("Printing the ONNX inputs:");
      m_hyperModel->printInputInfo(true);
      ANA_MSG_INFO("Printing the ONNX outputs:");
      m_hyperModel->printOutputInfo(true);
    }

    // Process the outputs
    ANA_MSG_DEBUG("Extracting outputs from model.");
    m_hyperParser->getONNXOutputs(*m_hyperModel);
    ANA_MSG_DEBUG("Outputs extraction successful.");

    ANA_MSG_DEBUG("Performing output reconstruction.");
    m_hyperParser->reconstructOutputs(*m_hyperGraph);
    ANA_MSG_DEBUG("Output reconstruction successful.");

    // Print the inputs/outputs for PyTorch/TCT validation
    if (m_fullLogEventNumber != 0 &&
        evtInfo->eventNumber() == m_fullLogEventNumber) {
      m_hyperGraph->printGraphInputsForValidation();
      m_hyperParser->printOutputsForValidation();
    }

    // Write the outputs
    ANA_MSG_DEBUG("Writing the outputs.");

    if (hyperTopology == top::HyPERTopology::TtbarAllHadronic) {
      std::vector<std::string> recoLabels = m_hyperParser->getLabels();
      std::vector<std::vector<int64_t>> recoIndices =
          m_hyperParser->getIndices();
      std::vector<float> recoScores = m_hyperParser->getScores();
      // HyPER_Reco_Top1
      std::size_t index = getIndexFromLabel(recoLabels, "HyPER_Reco_Top1");
      m_hyper_TtbarAllHadronic_Top1_Indices.set(*evtInfo, recoIndices[index],
                                                sys);
      m_hyper_TtbarAllHadronic_Top1_Score.set(*evtInfo, recoScores[index], sys);
      // HyPER_Reco_Top2
      index = getIndexFromLabel(recoLabels, "HyPER_Reco_Top2");
      m_hyper_TtbarAllHadronic_Top2_Indices.set(*evtInfo, recoIndices[index],
                                                sys);
      m_hyper_TtbarAllHadronic_Top2_Score.set(*evtInfo, recoScores[index], sys);
      // HyPER_Reco_W1
      index = getIndexFromLabel(recoLabels, "HyPER_Reco_W1");
      m_hyper_TtbarAllHadronic_W1_Indices.set(*evtInfo, recoIndices[index],
                                              sys);
      m_hyper_TtbarAllHadronic_W1_Score.set(*evtInfo, recoScores[index], sys);
      // HyPER_Reco_W2
      index = getIndexFromLabel(recoLabels, "HyPER_Reco_W2");
      m_hyper_TtbarAllHadronic_W2_Indices.set(*evtInfo, recoIndices[index],
                                              sys);
      m_hyper_TtbarAllHadronic_W2_Score.set(*evtInfo, recoScores[index], sys);
    }
    if (hyperTopology == top::HyPERTopology::TtbarLJets) {
      std::vector<std::string> recoLabels = m_hyperParser->getLabels();
      std::vector<std::vector<int64_t>> recoIndices =
          m_hyperParser->getIndices();
      std::vector<float> recoScores = m_hyperParser->getScores();
      std::vector<std::vector<int64_t>> recoIDs = m_hyperParser->getIds();
      // HyPER_Reco_TopHad
      std::size_t index = getIndexFromLabel(recoLabels, "HyPER_Reco_TopHad");
      ANA_MSG_DEBUG("Index of HyPER_Reco_TopHad: " << index);
      m_hyper_TtbarLJets_TopHad_Indices.set(*evtInfo, recoIndices[index], sys);
      m_hyper_TtbarLJets_TopHad_Score.set(*evtInfo, recoScores[index], sys);
      m_hyper_TtbarLJets_TopHad_IDs.set(*evtInfo, recoIDs[index], sys);
      // HyPER_Reco_TopLep
      index = getIndexFromLabel(recoLabels, "HyPER_Reco_TopLep");
      ANA_MSG_DEBUG("Index of HyPER_Reco_TopLep: " << index);
      m_hyper_TtbarLJets_TopLep_Indices.set(*evtInfo, recoIndices[index], sys);
      m_hyper_TtbarLJets_TopLep_Score.set(*evtInfo, recoScores[index], sys);
      m_hyper_TtbarLJets_TopLep_IDs.set(*evtInfo, recoIDs[index], sys);
      // HyPER_Reco_WHad
      index = getIndexFromLabel(recoLabels, "HyPER_Reco_WHad");
      ANA_MSG_DEBUG("Index of HyPER_Reco_WHad: " << index);
      m_hyper_TtbarLJets_WHad_Indices.set(*evtInfo, recoIndices[index], sys);
      m_hyper_TtbarLJets_WHad_Score.set(*evtInfo, recoScores[index], sys);
      // HyPER_Reco_WLep
      index = getIndexFromLabel(recoLabels, "HyPER_Reco_WLep");
      ANA_MSG_DEBUG("Index of HyPER_Reco_WLep: " << index);
      m_hyper_TtbarLJets_WLep_Indices.set(*evtInfo, recoIndices[index], sys);
      m_hyper_TtbarLJets_WLep_Score.set(*evtInfo, recoScores[index], sys);
    }

    // Clear the graph and other relevant objects for the next event
    ANA_MSG_DEBUG("Cleaning HyPER objects for next event.");
    m_hyperGraph->clearGraph();
    m_hyperModel->clearInputs();
    m_hyperModel->clearOutputs();
    m_hyperParser->clear();
    m_hyperInputs.clear();
    ANA_MSG_DEBUG("Clean successful.");
  }
  return StatusCode::SUCCESS;
}

StatusCode RunHyPERAlg::buildGraph() {
  HyPERTopology topology = m_hyperModel->getTopology();
  if (topology == top::HyPERTopology::NotSelected) {
    ANA_MSG_ERROR("Bad topology while building HyPER graph");
    return StatusCode::FAILURE;
  }

  using Features = std::vector<float>;

  // Remember... For HyPER we use the following particle IDs -> jet=1 e=2 mu=3
  // met=4 tau=5 Also... VERY important! The order of filling the particles for
  // the graph is important! We use this later for the reconstruction. The order
  // is: jets -> electrons -> muons -> met -> taus
  // TODO: Write a checker function for the ordering.

  if (topology == top::HyPERTopology::TtbarLJets)
    return this->buildTtbarLJetsGraph();
  else if (topology == top::HyPERTopology::TtbarAllHadronic)
    return this->buildTtbarAllHadronicGraph();
  return StatusCode::FAILURE;  // No topology selected, this is bad behaviour.
}

StatusCode RunHyPERAlg::buildTtbarLJetsGraph() {
  // For this topology we use jets, electrons and muons:
  // [E, eta, phi, pT, charge, particleID]
  // And Edges with the following features:
  // [dEta, dPhi, dR, m(i,j)]
  // And, the following globals:
  // [nJets, nBTagJets]

  // Build particles
  std::vector<Features> particles;
  float nBJets90 =
      0;  // Store number of bjets per-quantile for the global features.
  float nBJets85 = 0;
  float nBJets77 = 0;
  float nBJets70 = 0;
  float nBJets65 = 0;
  float nJets = 0;  // Store number of jets for the global features.
  for (const xAOD::Jet *jet : m_hyperInputs.m_jets) {
    nJets += 1.0f;
    std::vector<double> jetDoubleFeatures;
    if (m_topology == "TtbarLJets") {
      // Get b-tagging decision.
      if (!m_bTagDecoAcc->isAvailable(*jet)) {
        ANA_MSG_ERROR("HyPERAlg:: the jets do not have " << m_btagger
                                                         << " aux variable!");
        return StatusCode::FAILURE;
      }
      int bTagQuantile = (*m_bTagDecoAcc)(*jet);
      if (bTagQuantile >= 2)
        nBJets90 += 1;
      if (bTagQuantile >= 3)
        nBJets85 += 1;
      if (bTagQuantile >= 4)
        nBJets77 += 1;
      if (bTagQuantile >= 5)
        nBJets70 += 1;
      if (bTagQuantile >= 6)
        nBJets65 += 1;
      jetDoubleFeatures = {jet->e() / 1000,
                           jet->eta(),
                           jet->phi(),
                           jet->pt() / 1000,
                           static_cast<double>(bTagQuantile),
                           0,
                           top::HyPERParticleID::jet,
                           1};  // TODO: This is needed because the training was
                                // done with new HyPER dataset.
    }
    if (m_topology == "TtbarLJetsNoBTag")
      jetDoubleFeatures = {jet->e() / 1000,
                           jet->eta(),
                           jet->phi(),
                           jet->pt() / 1000,
                           0,
                           top::HyPERParticleID::jet,
                           1};  // TODO: This is needed because the training was
                                // done with new HyPER dataset.

    Features feats = vector1DTypeConverter<float, double>(jetDoubleFeatures);
    particles.push_back(feats);
  }
  for (const xAOD::Electron *el : m_hyperInputs.m_electrons) {
    std::vector<double> elDoubleFeatures;
    if (m_topology == "TtbarLJetsNoBTag")
      elDoubleFeatures = {el->e() / 1000,
                          el->eta(),
                          el->phi(),
                          el->pt() / 1000,
                          el->charge(),
                          top::HyPERParticleID::e,
                          2};  // TODO: This is needed because the training was
                               // done with new HyPER dataset.
    if (m_topology == "TtbarLJets")
      elDoubleFeatures = {el->e() / 1000,          el->eta(), el->phi(),
                          el->pt() / 1000,         0,         el->charge(),
                          top::HyPERParticleID::e, 2};

    Features feats = vector1DTypeConverter<float, double>(elDoubleFeatures);
    particles.push_back(feats);
  }
  for (const xAOD::Muon *mu : m_hyperInputs.m_muons) {
    std::vector<double> muDoubleFeatures;
    if (m_topology == "TtbarLJetsNoBTag")
      muDoubleFeatures = {mu->e() / 1000,
                          mu->eta(),
                          mu->phi(),
                          mu->pt() / 1000,
                          mu->charge(),
                          top::HyPERParticleID::mu,
                          2};  // TODO: This is needed because the training was
                               // done with new HyPER dataset.
    if (m_topology == "TtbarLJets")
      muDoubleFeatures = {
          mu->e() / 1000,           mu->eta(), mu->phi(),
          mu->pt() / 1000,          0,         mu->charge(),
          top::HyPERParticleID::mu, 2};  // TODO: This is needed because the
                                         // training was done with new HyPER
                                         // dataset.

    Features feats = vector1DTypeConverter<float, double>(muDoubleFeatures);
    particles.push_back(feats);
  }
  std::vector<double> metDoubleFeatures;
  if (m_topology == "TtbarLJetsNoBTag")
    metDoubleFeatures = {(*m_hyperInputs.m_met)["Final"]->met() / 1000,
                         0,
                         (*m_hyperInputs.m_met)["Final"]->phi(),
                         (*m_hyperInputs.m_met)["Final"]->met() / 1000,
                         0,
                         top::HyPERParticleID::met,
                         3};  // TODO: This is needed because the training was
                              // done with new HyPER dataset.
  if (m_topology == "TtbarLJets")
    metDoubleFeatures = {(*m_hyperInputs.m_met)["Final"]->met() / 1000,
                         0,
                         (*m_hyperInputs.m_met)["Final"]->phi(),
                         (*m_hyperInputs.m_met)["Final"]->met() / 1000,
                         0,
                         0,
                         top::HyPERParticleID::met,
                         3};  // TODO: This is needed because the training was
                              // done with new HyPER dataset.

  Features metFeats = vector1DTypeConverter<float, double>(metDoubleFeatures);
  particles.push_back(metFeats);

  // Add the nodes
  for (std::size_t i = 0; i < particles.size(); i++) {
    m_hyperGraph->addNode(particles[i]);
  }

  // Add the globals, scaling already applied here.
  Features globalFeats;
  if (m_topology == "TtbarLJets")
    globalFeats = {nJets / 6.0f,    nBJets90 / 2.0f, nBJets85 / 2.0f,
                   nBJets77 / 2.0f, nBJets70 / 2.0f, nBJets65 / 2.0f};
  if (m_topology == "TtbarLJetsNoBTag")
    globalFeats = {nJets / 6.0f};
  m_hyperGraph->addGlobal(globalFeats);

  // Build graph edges and hyperedges
  m_hyperGraph->buildEdges();
  m_hyperGraph->buildHyperEdges(3);

  // Add the edges
  // Loop over the edge indices
  for (const auto &edge : m_hyperGraph->getEdgeIndices()) {
    // Get the source and target nodes
    int64_t source = edge.first;
    int64_t target = edge.second;
    // Get the features
    Features firstNodeFeats = m_hyperGraph->getNodeFeats(source);
    Features secondNodeFeats = m_hyperGraph->getNodeFeats(target);

    // Calculate the edge features
    float dEta = secondNodeFeats[1] - firstNodeFeats[1];
    float dPhi = deltaPhi(secondNodeFeats[2], firstNodeFeats[2]);
    float dR = sqrt(dEta * dEta + dPhi * dPhi);
    ROOT::Math::PtEtaPhiEVector particle1;
    ROOT::Math::PtEtaPhiEVector particle2;
    particle1.SetCoordinates(firstNodeFeats[3], firstNodeFeats[1],
                             firstNodeFeats[2], firstNodeFeats[0]);
    particle2.SetCoordinates(secondNodeFeats[3], secondNodeFeats[1],
                             secondNodeFeats[2], secondNodeFeats[0]);
    float m = (particle1 + particle2).M();

    Features edgeFeats = {dEta, dPhi, dR, log(m)};
    m_hyperGraph->addEdge(source, target, edgeFeats);
  }

  // Scaling the node inputs
  for (std::size_t i{0}; i < static_cast<std::size_t>(m_hyperGraph->nNodes());
       i++) {
    Features &nodeFeats = m_hyperGraph->getNodeFeats(i);
    nodeFeats.at(0) = log(nodeFeats.at(0));
    nodeFeats.at(3) = log(nodeFeats.at(3));
  }
  return StatusCode::SUCCESS;
}

StatusCode RunHyPERAlg::buildTtbarAllHadronicGraph() {
  // For this topology we only use jets:
  // [E, eta, phi, pT, bTag, particleID = 1]
  // And Edges with the following features:
  // [dEta, dPhi, dR, m(i,j)]
  // And, the following globals:
  // [nJets, nBTagJets]

  // Build particles
  std::vector<Features> particles;
  float nBJets90 =
      0;  // Store number of bjets per-quantile for the global features.
  float nBJets85 = 0;
  float nBJets77 = 0;
  float nBJets70 = 0;
  float nBJets65 = 0;
  float nJets = 0;  // Store number of jets for the global features.
  for (const xAOD::Jet *jet : m_hyperInputs.m_jets) {
    nJets += 1.0f;
    // Get b-tagging decision
    if (!m_bTagDecoAcc->isAvailable(*jet)) {
      ANA_MSG_ERROR("HyPERAlg:: the jets do not have " << m_btagger
                                                       << " aux variable!");
      return StatusCode::FAILURE;
    }

    int bTagQuantile = (*m_bTagDecoAcc)(*jet);
    if (bTagQuantile >= 2)
      nBJets90 += 1;
    if (bTagQuantile >= 3)
      nBJets85 += 1;
    if (bTagQuantile >= 4)
      nBJets77 += 1;
    if (bTagQuantile >= 5)
      nBJets70 += 1;
    if (bTagQuantile >= 6)
      nBJets65 += 1;
    std::vector<double> jetDoubleFeatures{jet->e() / 1000,
                                          jet->eta(),
                                          jet->phi(),
                                          jet->pt() / 1000,
                                          static_cast<float>(bTagQuantile),
                                          top::HyPERParticleID::jet};
    Features feats = vector1DTypeConverter<float, double>(jetDoubleFeatures);
    particles.push_back(feats);
  }

  // Add the nodes
  for (std::size_t i = 0; i < particles.size(); i++) {
    m_hyperGraph->addNode(particles[i]);
  }

  // Add the globals, scaling already applied here.
  Features globalFeats = {nJets / 6.0f,    nBJets90 / 2.0f, nBJets85 / 2.0f,
                          nBJets77 / 2.0f, nBJets70 / 2.0f, nBJets65 / 2.0f};
  m_hyperGraph->addGlobal(globalFeats);

  // Build graph edges and hyperedges
  m_hyperGraph->buildEdges();
  m_hyperGraph->buildHyperEdges(3);

  // Add the edges
  // Loop over the edge indices
  for (const auto &edge : m_hyperGraph->getEdgeIndices()) {
    // Get the source and target nodes
    int64_t source = edge.first;
    int64_t target = edge.second;
    // Get the features
    Features firstNodeFeats = m_hyperGraph->getNodeFeats(source);
    Features secondNodeFeats = m_hyperGraph->getNodeFeats(target);

    // Calculate the edge features
    float dEta = secondNodeFeats[1] - firstNodeFeats[1];
    float dPhi = deltaPhi(secondNodeFeats[2], firstNodeFeats[2]);
    float dR = sqrt(dEta * dEta + dPhi * dPhi);
    ROOT::Math::PtEtaPhiEVector particle1;
    ROOT::Math::PtEtaPhiEVector particle2;
    particle1.SetCoordinates(firstNodeFeats[3], firstNodeFeats[1],
                             firstNodeFeats[2], firstNodeFeats[0]);
    particle2.SetCoordinates(secondNodeFeats[3], secondNodeFeats[1],
                             secondNodeFeats[2], secondNodeFeats[0]);
    float m = (particle1 + particle2).M();

    Features edgeFeats = {dEta, dPhi, dR, log(m)};
    m_hyperGraph->addEdge(source, target, edgeFeats);
  }

  // Scaling the node inputs
  for (std::size_t i{0}; i < static_cast<std::size_t>(m_hyperGraph->nNodes());
       i++) {
    Features &nodeFeats = m_hyperGraph->getNodeFeats(i);
    nodeFeats.at(0) = log(nodeFeats.at(0));
    nodeFeats.at(3) = log(nodeFeats.at(3));
  }
  return StatusCode::SUCCESS;
}

StatusCode RunHyPERAlg::finalize() {
  ANA_MSG_INFO("Finalizing RunHyPER");
  return StatusCode::SUCCESS;
}
}  // namespace top
