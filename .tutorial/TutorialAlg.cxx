#include "TopCPToolkit/TutorialAlg.h"
#include "VectorHelpers/LorentzHelper.h"

namespace top {

  using ROOT::Math::PtEtaPhiEVector;
  TutorialAlg::TutorialAlg(const std::string &name,
                           ISvcLocator *pSvcLocator)
    : EL::AnaAlgorithm(name, pSvcLocator) {}
  
  StatusCode TutorialAlg::initialize() {
    ANA_MSG_INFO("Initialising the algorithm for the TopCPToolkit tutorial");

    ANA_CHECK(m_electronsHandle.initialize(m_systematicsList));
    ANA_CHECK(m_muonsHandle.initialize(m_systematicsList));
    ANA_CHECK(m_jetsHandle.initialize(m_systematicsList));
    ANA_CHECK(m_metHandle.initialize(m_systematicsList));
    ANA_CHECK(m_eventInfoHandle.initialize(m_systematicsList));

    ANA_CHECK(m_preselection.initialize(m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));

    ANA_CHECK(m_dphimetHandle.initialize(m_systematicsList, m_jetsHandle));
    ANA_CHECK(m_mtwHandle.initialize(m_systematicsList, m_eventInfoHandle));

    ANA_CHECK(m_systematicsList.initialize());

    return StatusCode::SUCCESS;
  }

  StatusCode TutorialAlg::execute() {

    for (const auto &sys : m_systematicsList.systematicsVector()) {
      
      const xAOD::EventInfo *evtInfo = nullptr;
      const xAOD::ElectronContainer *electrons = nullptr;
      const xAOD::MuonContainer *muons = nullptr;
      const xAOD::JetContainer *jets = nullptr;
      const xAOD::MissingETContainer *met = nullptr;

      ANA_CHECK(m_eventInfoHandle.retrieve(evtInfo, sys));
      ANA_CHECK(m_electronsHandle.retrieve(electrons, sys));
      ANA_CHECK(m_muonsHandle.retrieve(muons, sys));
      ANA_CHECK(m_jetsHandle.retrieve(jets, sys));
      ANA_CHECK(m_metHandle.retrieve(met, sys));

      m_mtwHandle.set(*evtInfo, -1, sys);
      for (const xAOD::Jet *jet : *jets) {
	m_dphimetHandle.set(*jet, -99, sys);
      }
      
      if ( m_preselection && !m_preselection.getBool(*evtInfo, sys)) continue;
      
      PtEtaPhiEVector lepton;
      if ( electrons->size() > 0 ) {
	lepton = GetPtEtaPhiE(electrons->at(0));
      }
      else if ( muons->size() > 0) {
	lepton = GetPtEtaPhiE(muons->at(0));
      }

      float et_miss  = (*met)["Final"]->met();
      float phi_miss = (*met)["Final"]->phi();

      float dphi_lep_met = TVector2::Phi_mpi_pi( lepton.Phi() - phi_miss );
      float mtw = std::sqrt( 2 * lepton.Pt() * et_miss * (1-std::cos(dphi_lep_met)) );
      m_mtwHandle.set(*evtInfo, mtw, sys);

      for (const xAOD::Jet *jet : *jets) {
	float dphi_jet_met = TVector2::Phi_mpi_pi( jet->phi() - phi_miss );
	m_dphimetHandle.set(*jet, dphi_jet_met, sys);
      }
    }

    return StatusCode::SUCCESS;
  }

} //namespace top
