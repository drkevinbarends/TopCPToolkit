#include "TopCPToolkit/RunNuFlowsAlg.h"

#include "AthContainers/ConstDataVector.h"

namespace top {

RunNuFlowsAlg::RunNuFlowsAlg(const std::string &name, ISvcLocator *pSvcLocator)
    : EL::AnaAlgorithm(name, pSvcLocator),
      m_onnxFilePath("SetMe"),
      m_btagger("SetMe"),
      m_write_inputs(false) {
  declareProperty("onnxfilepath", m_onnxFilePath,
                  "Path to folder containing even/odd ONNX files");
  declareProperty("btagger", m_btagger,
                  "Btagging algorithm to use. Must support the 'Continuous' WP "
                  "in order to retrieve the quantile bin.");
  declareProperty("write_inputs", m_write_inputs,
                  "Save the inputs to the network in the EventInfo object");
}

StatusCode RunNuFlowsAlg::initialize() {
  ANA_MSG_INFO("Initializing RunNuFlows " << name());
  ANA_MSG_INFO("  --> Using even/odd onnx files in: " << m_onnxFilePath);
  ANA_MSG_INFO("  --> Using btagger: " << m_btagger);
  ANA_MSG_INFO("  --> Saving inputs: " << m_write_inputs);

  // The preselection for the event
  ANA_CHECK(m_eventselection.initialize(m_systematicsList, m_eventInfoHandle,
                                        SG::AllowEmpty));

  // Initialise all input handles
  ANA_CHECK(m_electronsHandle.initialize(m_systematicsList));
  ANA_CHECK(m_muonsHandle.initialize(m_systematicsList));
  ANA_CHECK(m_jetsHandle.initialize(m_systematicsList));
  ANA_CHECK(m_metHandle.initialize(m_systematicsList));
  ANA_CHECK(m_eventInfoHandle.initialize(m_systematicsList));

  // Initialise all selection handles
  ANA_CHECK(m_electronSelection.initialize(m_systematicsList, m_electronsHandle,
                                           SG::AllowEmpty));
  ANA_CHECK(m_muonSelection.initialize(m_systematicsList, m_muonsHandle,
                                       SG::AllowEmpty));
  ANA_CHECK(m_jetSelection.initialize(m_systematicsList, m_jetsHandle,
                                      SG::AllowEmpty));

  // Initialise all output decorations
  ANA_CHECK(m_nu_out_decor.initialize(m_systematicsList, m_eventInfoHandle,
                                      SG::AllowEmpty));
  ANA_CHECK(m_loglik_decor.initialize(m_systematicsList, m_eventInfoHandle,
                                      SG::AllowEmpty));
  ANA_CHECK(m_input_lep_decor.initialize(m_systematicsList, m_eventInfoHandle,
                                         SG::AllowEmpty));
  ANA_CHECK(m_input_jet_decor.initialize(m_systematicsList, m_eventInfoHandle,
                                         SG::AllowEmpty));
  ANA_CHECK(m_input_met_decor.initialize(m_systematicsList, m_eventInfoHandle,
                                         SG::AllowEmpty));
  ANA_CHECK(m_input_misc_decor.initialize(m_systematicsList, m_eventInfoHandle,
                                          SG::AllowEmpty));

  // Initialise the systematics list
  ANA_CHECK(m_systematicsList.initialize());

  // Load the NuFlows model and set the btagger
  m_nuflows =
      std::unique_ptr<top::TopNuFlowsDilepton>(new top::TopNuFlowsDilepton(
          "AsgNuFlowsTool", m_onnxFilePath + "/even.onnx",
          m_onnxFilePath + "/odd.onnx"));
  m_nuflows->setBtagger(m_btagger);

  return StatusCode::SUCCESS;
}

StatusCode RunNuFlowsAlg::execute() {
  // Loop over all relevant systematics and execute the syst method
  for (const auto &sys : m_systematicsList.systematicsVector())
    ANA_CHECK(execute_syst(sys));

  return StatusCode::SUCCESS;
}

StatusCode RunNuFlowsAlg::execute_syst(const CP::SystematicSet &sys) {
  // Load the event information, especially with the run number
  const xAOD::EventInfo *evtInfo = nullptr;
  ANA_CHECK(m_eventInfoHandle.retrieve(evtInfo, sys));
  unsigned long long eventNumber = evtInfo->eventNumber();

  // default outputs
  m_nu_out_decor.set(*evtInfo, {}, sys);
  m_loglik_decor.set(*evtInfo, -999, sys);
  m_input_lep_decor.set(*evtInfo, {}, sys);
  m_input_jet_decor.set(*evtInfo, {}, sys);
  m_input_met_decor.set(*evtInfo, {}, sys);
  m_input_misc_decor.set(*evtInfo, {}, sys);

  // Skip the event if it does not pass the selection
  if (m_eventselection && !m_eventselection.getBool(*evtInfo, sys))
    return StatusCode::SUCCESS;

  // Load the objects
  const xAOD::ElectronContainer *electrons = nullptr;
  const xAOD::MuonContainer *muons = nullptr;
  const xAOD::JetContainer *jets = nullptr;
  const xAOD::MissingETContainer *met = nullptr;
  ANA_CHECK(m_electronsHandle.retrieve(electrons, sys));
  ANA_CHECK(m_muonsHandle.retrieve(muons, sys));
  ANA_CHECK(m_jetsHandle.retrieve(jets, sys));
  ANA_CHECK(m_metHandle.retrieve(met, sys));

  // Get the selected electrons
  ConstDataVector<xAOD::ElectronContainer> selected_electrons(
      SG::VIEW_ELEMENTS);
  for (const xAOD::Electron *el : *electrons) {
    if (m_electronSelection.getBool(*el, sys))
      selected_electrons.push_back(el);
  }

  // Get the selected muons
  ConstDataVector<xAOD::MuonContainer> selected_muons(SG::VIEW_ELEMENTS);
  for (const xAOD::Muon *mu : *muons) {
    if (m_muonSelection.getBool(*mu, sys))
      selected_muons.push_back(mu);
  }

  // Get the selected jets, the container will have to be sorted based on pt()
  ConstDataVector<xAOD::JetContainer> selected_jets(SG::VIEW_ELEMENTS);
  for (const xAOD::Jet *jet : *jets) {
    if (m_jetSelection.getBool(*jet, sys))
      selected_jets.push_back(jet);
  }
  auto ptsort = [](const auto *a, const auto *b) { return a->pt() > b->pt(); };
  std::sort(selected_jets.begin(), selected_jets.end(), ptsort);

  // Load the MET
  if ((*met)["Final"] == nullptr) {
    ANA_MSG_ERROR("The MET term " << "Final" << " doesn't exist! Aborting.");
    return StatusCode::FAILURE;
  }
  float met_mpx = (*met)["Final"]->mpx();
  float met_mpy = (*met)["Final"]->mpy();
  float met_sumet = (*met)["Final"]->sumet();

  // Run Nuflows with the selected object
  m_nuflows->Sample(selected_electrons, selected_muons, selected_jets, met_mpx,
                    met_mpy, met_sumet, eventNumber);

  // Decorate the event with the outputs of the network
  m_nu_out_decor.set(*evtInfo, m_nuflows->m_nu_out, sys);
  m_loglik_decor.set(*evtInfo, m_nuflows->m_loglik, sys);

  // Decorate with the inputs of the network if requested
  if (m_write_inputs) {
    m_input_lep_decor.set(*evtInfo, m_nuflows->m_input_lep, sys);
    m_input_jet_decor.set(*evtInfo, m_nuflows->m_input_jet, sys);
    m_input_met_decor.set(*evtInfo, m_nuflows->m_input_met, sys);
    m_input_misc_decor.set(*evtInfo, m_nuflows->m_input_misc, sys);
  }

  return StatusCode::SUCCESS;
}

StatusCode RunNuFlowsAlg::finalize() {
  ANA_MSG_INFO("Finalizing RunNuFlows");
  return StatusCode::SUCCESS;
}

}  // namespace top
