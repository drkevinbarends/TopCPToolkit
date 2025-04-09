#include "TopCPToolkit/RunSpaNetAlg.h"
#include "AthContainers/ConstDataVector.h"

namespace top {

  RunSpaNetAlg::RunSpaNetAlg(const std::string &name, ISvcLocator *pSvcLocator)
    : EL::AnaAlgorithm(name, pSvcLocator)
    , m_topology("SetMe")
    , m_btagger("SetMe")
    , m_topologyEnum(SpaNetEnums::Topology::TtbarLjetsNu)
  {
    declareProperty("topology", m_topology, "SPA-Net topology to use. Choose between: 'TtbarLjetsNu'.");
    declareProperty("btagger", m_btagger, "Btagging algorithm to use. Must support the 'Continuous' WP in order to retrieve the quantile bin.");
  }

  StatusCode RunSpaNetAlg::initialize() {
    ANA_MSG_INFO("Initializing RunSpaNet " << name() );
    ANA_MSG_INFO("  --> topology: " << m_topology);
    ANA_MSG_INFO("  --> btagger:  " << m_btagger);

    // parse topology
    try {
      m_topologyEnum = SpaNetEnums::strToTopology.at(m_topology);
    } catch (std::out_of_range&) {
      ANA_MSG_ERROR("Unrecognized SpaNet topology: " << m_topology
		    << ". Available options: " << SpaNetEnums::printEnumOptions(SpaNetEnums::strToTopology));
      return StatusCode::FAILURE;
    }

    ANA_CHECK(m_electronsHandle.initialize(m_systematicsList));
    ANA_CHECK(m_muonsHandle.initialize(m_systematicsList));
    ANA_CHECK(m_jetsHandle.initialize(m_systematicsList));
    ANA_CHECK(m_metHandle.initialize(m_systematicsList));
    ANA_CHECK(m_eventInfoHandle.initialize(m_systematicsList));
    ANA_CHECK(m_electronSelection.initialize(m_systematicsList, m_electronsHandle, SG::AllowEmpty));
    ANA_CHECK(m_muonSelection.initialize(m_systematicsList, m_muonsHandle, SG::AllowEmpty));
    ANA_CHECK(m_jetSelection.initialize(m_systematicsList, m_jetsHandle, SG::AllowEmpty));

    ANA_CHECK(m_selection.initialize(m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));

    ANA_CHECK(m_lep_b_idx_decor.initialize(m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));
    ANA_CHECK(m_had_b_idx_decor.initialize(m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));
    ANA_CHECK(m_down_idx_decor.initialize(m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));
    ANA_CHECK(m_up_idx_decor.initialize(m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));
    ANA_CHECK(m_lep_top_assignment_decor.initialize(m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));
    ANA_CHECK(m_had_top_assignment_decor.initialize(m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));
    ANA_CHECK(m_lep_top_detection_decor.initialize(m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));
    ANA_CHECK(m_had_top_detection_decor.initialize(m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));
    ANA_CHECK(m_reg_nu_eta_decor.initialize(m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));
    ANA_CHECK(m_reg_nu_px_decor.initialize(m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));
    ANA_CHECK(m_reg_nu_py_decor.initialize(m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));
    ANA_CHECK(m_reg_nu_pz_decor.initialize(m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));
    ANA_CHECK(m_reg_ttbar_m_decor.initialize(m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));

    ANA_CHECK(m_systematicsList.initialize());

    if (m_topologyEnum == SpaNetEnums::Topology::TtbarLjetsNu) {
      m_spanet_reco = std::unique_ptr<top::TopSpaNetTopology> (new top::TopSpaNetTtbarLjetsNu("AsgSpaNetTool"+m_topology,
											    "dev/AnalysisTop/SpaNetModels/spanet_ttbarljetsnu_trainedoneven.onnx",
											    "dev/AnalysisTop/SpaNetModels/spanet_ttbarljetsnu_trainedonodd.onnx")
							       );
    }
    else {
      ANA_MSG_ERROR("  ==> topology is not recognised! aborting.");
      return StatusCode::FAILURE;
    }

    if ( this->msg().level()==MSG::DEBUG || this->msg().level()==MSG::VERBOSE ) {
      m_spanet_reco->makeVerbose(true);
      ANA_CHECK(m_spanet_reco->setProperty("OutputLevel", MSG::VERBOSE));
    }
    m_spanet_reco->setBtagger( m_btagger );

    return StatusCode::SUCCESS;
  }

  StatusCode RunSpaNetAlg::execute() {
    for (const auto &sys : m_systematicsList.systematicsVector()) {
      ANA_CHECK(execute_syst(sys));
    }
    return StatusCode::SUCCESS;
  }

  StatusCode RunSpaNetAlg::execute_syst(const CP::SystematicSet &sys) {
    // check the event selection
    const xAOD::EventInfo *evtInfo = nullptr;
    ANA_CHECK(m_eventInfoHandle.retrieve(evtInfo, sys));

    // default-decorate EventInfo
    m_lep_b_idx_decor.set(*evtInfo, -1, sys);
    m_had_b_idx_decor.set(*evtInfo, -1, sys);
    m_down_idx_decor.set(*evtInfo, -1, sys);
    m_up_idx_decor.set(*evtInfo, -1, sys);
    m_had_top_assignment_decor.set(*evtInfo, -999., sys);
    m_lep_top_assignment_decor.set(*evtInfo, -999., sys);
    m_had_top_detection_decor.set(*evtInfo, -999., sys);
    m_lep_top_detection_decor.set(*evtInfo, -999., sys);
    m_reg_nu_eta_decor.set(*evtInfo, -999., sys);
    m_reg_nu_px_decor.set(*evtInfo, -999., sys);
    m_reg_nu_py_decor.set(*evtInfo, -999., sys);
    m_reg_nu_pz_decor.set(*evtInfo, -999., sys);
    m_reg_ttbar_m_decor.set(*evtInfo, -999., sys);

    if (m_selection && !m_selection.getBool(*evtInfo, sys))
      return StatusCode::SUCCESS;

    // retrieve objects
    const xAOD::ElectronContainer *electrons = nullptr;
    ANA_CHECK(m_electronsHandle.retrieve(electrons, sys));
    const xAOD::MuonContainer *muons = nullptr;
    ANA_CHECK(m_muonsHandle.retrieve(muons, sys));
    const xAOD::JetContainer *jets = nullptr;
    ANA_CHECK(m_jetsHandle.retrieve(jets, sys));
    const xAOD::MissingETContainer *met = nullptr;
    ANA_CHECK(m_metHandle.retrieve(met, sys));

    // apply object-wise selection
    ConstDataVector<xAOD::ElectronContainer> selected_electrons(SG::VIEW_ELEMENTS);
    ConstDataVector<xAOD::MuonContainer> selected_muons(SG::VIEW_ELEMENTS);
    ConstDataVector<xAOD::JetContainer> selected_jets(SG::VIEW_ELEMENTS);

    for (const xAOD::Electron *el : *electrons) {
      if (m_electronSelection.getBool(*el, sys))
        selected_electrons.push_back(el);
    }

    for (const xAOD::Muon *mu : *muons) {
      if (m_muonSelection.getBool(*mu, sys))
        selected_muons.push_back(mu);
    }

    for (const xAOD::Jet *jet : *jets) {
      if (m_jetSelection.getBool(*jet, sys))
        selected_jets.push_back(jet);
    }

    // run SPA-Net with the selected inputs
    m_spanet_reco->Predict(selected_electrons, selected_muons, selected_jets,
			   (*met)["Final"]->met(), (*met)["Final"]->phi(),
			   evtInfo->eventNumber()
			   );

    // decorate EventInfo depending on the topology
    if (m_topologyEnum == SpaNetEnums::Topology::TtbarLjetsNu) {
      const std::vector<int>& best_indices = m_spanet_reco->GetOutputIndices();
      m_lep_b_idx_decor.set(*evtInfo, best_indices[0], sys);
      m_had_b_idx_decor.set(*evtInfo, best_indices[1], sys);
      m_down_idx_decor.set( *evtInfo, best_indices[2], sys);
      m_up_idx_decor.set(   *evtInfo, best_indices[3], sys);
      const std::vector<float>& best_scores = m_spanet_reco->GetOutputScores();
      m_had_top_assignment_decor.set(*evtInfo, best_scores[0], sys);
      m_lep_top_assignment_decor.set(*evtInfo, best_scores[1], sys);
      m_had_top_detection_decor.set(*evtInfo, best_scores[2], sys);
      m_lep_top_detection_decor.set(*evtInfo, best_scores[3], sys);
      const std::vector<float>& regressions = m_spanet_reco->GetRegressedValues();
      m_reg_nu_eta_decor.set(*evtInfo, regressions[0], sys);
      m_reg_nu_px_decor.set(*evtInfo, regressions[1], sys);
      m_reg_nu_py_decor.set(*evtInfo, regressions[2], sys);
      m_reg_nu_pz_decor.set(*evtInfo, regressions[3], sys);
      m_reg_ttbar_m_decor.set(*evtInfo, regressions[4], sys);
    }

    return StatusCode::SUCCESS;
  }

  StatusCode RunSpaNetAlg::finalize() {
    ANA_MSG_INFO("Finalizing RunSpaNet");

    return StatusCode::SUCCESS;
  }

} // namespace top
