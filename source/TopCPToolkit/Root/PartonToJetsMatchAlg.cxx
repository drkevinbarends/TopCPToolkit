#include "TopCPToolkit/PartonToJetsMatchAlg.h"
#include "AthContainers/ConstDataVector.h"

#include "PartonHistory/PartonHistory.h"
#include "VectorHelpers/LorentzHelper.h"

namespace top {
  using ROOT::Math::PtEtaPhiEVector;
  using ROOT::Math::PtEtaPhiMVector;
  using ROOT::Math::VectorUtil::DeltaR;

  PartonToJetsMatchAlg::PartonToJetsMatchAlg(const std::string &name, ISvcLocator *pSvcLocator)
    : EL::AnaAlgorithm(name, pSvcLocator)
  {
    declareProperty("criticalDR", m_criticalDR, "Maximum delta R for matching");
    declareProperty("partonContainerName", m_partonContainerName, "Name of the parton container");
  }

  StatusCode PartonToJetsMatchAlg::initialize() {
    ANA_MSG_INFO("Initializing PartonToJetsMatchAlg " << name() );

    ANA_CHECK(m_jetsHandle.initialize(m_systematicsList));
    ANA_CHECK(m_jetSelection.initialize(m_systematicsList, m_jetsHandle, SG::AllowEmpty));
    ANA_CHECK(m_eventInfoHandle.initialize(m_systematicsList));
    ANA_CHECK(m_selection.initialize(m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));


    ANA_CHECK(m_lep_b_idx_decor.initialize(m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));
    ANA_CHECK(m_had_b_idx_decor.initialize(m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));
    ANA_CHECK(m_down_idx_decor.initialize(m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));
    ANA_CHECK(m_up_idx_decor.initialize(m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));
    ANA_CHECK(m_event_is_dilepton_decor.initialize(m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));

    ANA_CHECK(m_systematicsList.initialize());

    return StatusCode::SUCCESS;
  }

  StatusCode PartonToJetsMatchAlg::execute() {
    for (const auto &sys : m_systematicsList.systematicsVector()) {
      ANA_CHECK(execute_syst(sys));
    }
    return StatusCode::SUCCESS;
  }

  StatusCode PartonToJetsMatchAlg::finalize() {
    ANA_MSG_INFO("Finalizing PartonToJetsMatchAlg");

    return StatusCode::SUCCESS;
  }

  StatusCode PartonToJetsMatchAlg::execute_syst(const CP::SystematicSet &sys) {
    const xAOD::EventInfo *evtInfo = nullptr;
    ANA_CHECK(m_eventInfoHandle.retrieve(evtInfo, sys));

    // default-decorate EventInfo
    m_lep_b_idx_decor.set(*evtInfo, -1, sys);
    m_had_b_idx_decor.set(*evtInfo, -1, sys);
    m_down_idx_decor.set(*evtInfo, -1, sys);
    m_up_idx_decor.set(*evtInfo, -1, sys);
    m_event_is_dilepton_decor.set(*evtInfo, -1, sys);

    if (m_selection && !m_selection.getBool(*evtInfo, sys))
      return StatusCode::SUCCESS;

    const xAOD::JetContainer *jets = nullptr;
    ANA_CHECK(m_jetsHandle.retrieve(jets, sys));

    ConstDataVector<xAOD::JetContainer> selected_jets(SG::VIEW_ELEMENTS);
    for (const xAOD::Jet *jet : *jets) {
      if (m_jetSelection.getBool(*jet, sys))
        selected_jets.push_back(jet);
    }

    const xAOD::PartonHistory* topParton = nullptr;
    ANA_CHECK(evtStore()->retrieve(topParton, m_partonContainerName));

    int b_had_index(-1);
    int b_lep_index(-1);
    int up_index(-1);
    int down_index(-1);
    bool isDilepton(false);

    float MC_Wdecay1_from_t_pt = 0;
    float MC_Wdecay1_from_t_eta = 0;
    float MC_Wdecay1_from_t_phi = 0;
    float MC_Wdecay1_from_t_m = 0;
    int   MC_Wdecay1_from_t_pdgId = 0;
    float MC_Wdecay2_from_t_pt = 0;
    float MC_Wdecay2_from_t_eta = 0;
    float MC_Wdecay2_from_t_phi = 0;
    float MC_Wdecay2_from_t_m = 0;
    int   MC_Wdecay2_from_t_pdgId = 0;
    float MC_Wdecay1_from_tbar_pt = 0;
    float MC_Wdecay1_from_tbar_eta = 0;
    float MC_Wdecay1_from_tbar_phi = 0;
    float MC_Wdecay1_from_tbar_m = 0;
    int   MC_Wdecay1_from_tbar_pdgId = 0;
    float MC_Wdecay2_from_tbar_pt = 0;
    float MC_Wdecay2_from_tbar_eta = 0;
    float MC_Wdecay2_from_tbar_phi = 0;
    float MC_Wdecay2_from_tbar_m = 0;
    int   MC_Wdecay2_from_tbar_pdgId = 0;
    float MC_b_from_t_pt = 0;
    float MC_b_from_t_eta = 0;
    float MC_b_from_t_phi = 0;
    float MC_b_from_t_m = 0;
    float MC_bbar_from_tbar_pt = 0;
    float MC_bbar_from_tbar_eta = 0;
    float MC_bbar_from_tbar_phi = 0;
    float MC_bbar_from_tbar_m = 0;

    if(topParton->auxdata<float>("MC_Wdecay1_from_t_pt") > 0) {
      MC_Wdecay1_from_t_pt    = topParton->auxdata<float>("MC_Wdecay1_from_t_pt");
      MC_Wdecay1_from_t_eta   = topParton->auxdata<float>("MC_Wdecay1_from_t_eta");
      MC_Wdecay1_from_t_phi   = topParton->auxdata<float>("MC_Wdecay1_from_t_phi");
      MC_Wdecay1_from_t_m     = topParton->auxdata<float>("MC_Wdecay1_from_t_m");
      MC_Wdecay1_from_t_pdgId = topParton->auxdata<int>("MC_Wdecay1_from_t_pdgId");
    } else {
      // something is wrong with the truth information
      return StatusCode::SUCCESS;
    }

    if(topParton->auxdata<float>("MC_Wdecay2_from_t_pt") > 0) {
      MC_Wdecay2_from_t_pt    = topParton->auxdata<float>("MC_Wdecay2_from_t_pt");
      MC_Wdecay2_from_t_eta   = topParton->auxdata<float>("MC_Wdecay2_from_t_eta");
      MC_Wdecay2_from_t_phi   = topParton->auxdata<float>("MC_Wdecay2_from_t_phi");
      MC_Wdecay2_from_t_m     = topParton->auxdata<float>("MC_Wdecay2_from_t_m");
      MC_Wdecay2_from_t_pdgId = topParton->auxdata<int>("MC_Wdecay2_from_t_pdgId");
    } else {
      // something is wrong with the truth information
      return StatusCode::SUCCESS;
    }

    if(topParton->auxdata<float>("MC_Wdecay1_from_tbar_pt") > 0) {
      MC_Wdecay1_from_tbar_pt    = topParton->auxdata<float>("MC_Wdecay1_from_tbar_pt");
      MC_Wdecay1_from_tbar_eta   = topParton->auxdata<float>("MC_Wdecay1_from_tbar_eta");
      MC_Wdecay1_from_tbar_phi   = topParton->auxdata<float>("MC_Wdecay1_from_tbar_phi");
      MC_Wdecay1_from_tbar_m     = topParton->auxdata<float>("MC_Wdecay1_from_tbar_m");
      MC_Wdecay1_from_tbar_pdgId = topParton->auxdata<int>("MC_Wdecay1_from_tbar_pdgId");
    } else {
      // something is wrong with the truth information
      return StatusCode::SUCCESS;
    }

    if(topParton->auxdata<float>("MC_Wdecay2_from_tbar_pt") > 0) {
      MC_Wdecay2_from_tbar_pt    = topParton->auxdata<float>("MC_Wdecay2_from_tbar_pt");
      MC_Wdecay2_from_tbar_eta   = topParton->auxdata<float>("MC_Wdecay2_from_tbar_eta");
      MC_Wdecay2_from_tbar_phi   = topParton->auxdata<float>("MC_Wdecay2_from_tbar_phi");
      MC_Wdecay2_from_tbar_m     = topParton->auxdata<float>("MC_Wdecay2_from_tbar_m");
      MC_Wdecay2_from_tbar_pdgId = topParton->auxdata<int>("MC_Wdecay2_from_tbar_pdgId");
    } else {
      // something is wrong with the truth information
      return StatusCode::SUCCESS;
    }
    if(topParton->auxdata<float>("MC_b_from_t_pt") > 0) {
      MC_b_from_t_pt    = topParton->auxdata<float>("MC_b_from_t_pt");
      MC_b_from_t_eta   = topParton->auxdata<float>("MC_b_from_t_eta");
      MC_b_from_t_phi   = topParton->auxdata<float>("MC_b_from_t_phi");
      MC_b_from_t_m     = topParton->auxdata<float>("MC_b_from_t_m");
    } else {
      return StatusCode::SUCCESS;
    }

    if(topParton->auxdata<float>("MC_bbar_from_tbar_pt") > 0) {
      MC_bbar_from_tbar_pt    = topParton->auxdata<float>("MC_bbar_from_tbar_pt");
      MC_bbar_from_tbar_eta   = topParton->auxdata<float>("MC_bbar_from_tbar_eta");
      MC_bbar_from_tbar_phi   = topParton->auxdata<float>("MC_bbar_from_tbar_phi");
      MC_bbar_from_tbar_m     = topParton->auxdata<float>("MC_bbar_from_tbar_m");
    } else {
      return StatusCode::SUCCESS;
    }

    // the parton truth information should be correct here
    bool t_isHadronic(false);
    bool tbar_isHadronic(false);

    // check for weird values
    if (MC_Wdecay1_from_t_pdgId == 0) return StatusCode::SUCCESS;
    if (MC_Wdecay2_from_t_pdgId == 0) return StatusCode::SUCCESS;
    if (MC_Wdecay1_from_tbar_pdgId == 0) return StatusCode::SUCCESS;
    if (MC_Wdecay2_from_tbar_pdgId == 0) return StatusCode::SUCCESS;

    // if top has a hadronic decay of W
    if ((std::abs(MC_Wdecay1_from_t_pdgId) < 6) || (std::abs(MC_Wdecay2_from_t_pdgId) < 6)) t_isHadronic = true;
    if ((std::abs(MC_Wdecay1_from_tbar_pdgId) < 6) || (std::abs(MC_Wdecay2_from_tbar_pdgId) < 6)) tbar_isHadronic = true;

    if (!t_isHadronic && !tbar_isHadronic) isDilepton = true;
    else isDilepton = false;

    if (isDilepton) {
      m_event_is_dilepton_decor.set(*evtInfo, 1, sys);
      return StatusCode::SUCCESS;
    }

    PtEtaPhiMVector W_quark_up, W_quark_down, b_had, b_lep;
  
    if (t_isHadronic) {
      b_had.SetCoordinates(MC_b_from_t_pt, MC_b_from_t_eta, MC_b_from_t_phi, MC_b_from_t_m);
      b_lep.SetCoordinates(MC_bbar_from_tbar_pt, MC_bbar_from_tbar_eta, MC_bbar_from_tbar_phi, MC_bbar_from_tbar_m);
      if (std::abs(MC_Wdecay1_from_t_pdgId) == 2 || std::abs(MC_Wdecay1_from_t_pdgId) == 4) {
        W_quark_up.SetCoordinates(MC_Wdecay1_from_t_pt, MC_Wdecay1_from_t_eta, MC_Wdecay1_from_t_phi, MC_Wdecay1_from_t_m);
        W_quark_down.SetCoordinates(MC_Wdecay2_from_t_pt, MC_Wdecay2_from_t_eta, MC_Wdecay2_from_t_phi, MC_Wdecay2_from_t_m);
      } else {
        W_quark_down.SetCoordinates(MC_Wdecay1_from_t_pt, MC_Wdecay1_from_t_eta, MC_Wdecay1_from_t_phi, MC_Wdecay1_from_t_m);
        W_quark_up.SetCoordinates(MC_Wdecay2_from_t_pt, MC_Wdecay2_from_t_eta, MC_Wdecay2_from_t_phi, MC_Wdecay2_from_t_m);
      }

    } else {
      b_lep.SetCoordinates(MC_b_from_t_pt, MC_b_from_t_eta, MC_b_from_t_phi, MC_b_from_t_m);
      b_had.SetCoordinates(MC_bbar_from_tbar_pt, MC_bbar_from_tbar_eta, MC_bbar_from_tbar_phi, MC_bbar_from_tbar_m);
      if (std::abs(MC_Wdecay1_from_tbar_pdgId) == 2 || std::abs(MC_Wdecay1_from_tbar_pdgId) == 4) {
        W_quark_up.SetCoordinates(MC_Wdecay1_from_tbar_pt, MC_Wdecay1_from_tbar_eta, MC_Wdecay1_from_tbar_phi, MC_Wdecay1_from_tbar_m);
        W_quark_down.SetCoordinates(MC_Wdecay2_from_tbar_pt, MC_Wdecay2_from_tbar_eta, MC_Wdecay2_from_tbar_phi, MC_Wdecay2_from_tbar_m);
      } else {
        W_quark_down.SetCoordinates(MC_Wdecay1_from_tbar_pt, MC_Wdecay1_from_tbar_eta, MC_Wdecay1_from_tbar_phi, MC_Wdecay1_from_tbar_m);
        W_quark_up.SetCoordinates(MC_Wdecay2_from_tbar_pt, MC_Wdecay2_from_tbar_eta, MC_Wdecay2_from_tbar_phi, MC_Wdecay2_from_tbar_m);
      }
    }

    std::vector<double> dr_up_vec;
    std::vector<double> dr_down_vec;
    std::vector<double> dr_b_had_vec;
    std::vector<double> dr_b_lep_vec;

    // loop over truth jets and try to identify
    for (std::size_t ijet = 0; ijet < selected_jets.size(); ++ijet) {
      PtEtaPhiEVector truth_jet;
      truth_jet.SetCoordinates(selected_jets.at(ijet)->pt(), selected_jets.at(ijet)->eta(), selected_jets.at(ijet)->phi(), selected_jets.at(ijet)->e());

      const double dr_up    = DeltaR(truth_jet, W_quark_up); 
      const double dr_down  = DeltaR(truth_jet, W_quark_down); 
      const double dr_b_had = DeltaR(truth_jet, b_had); 
      const double dr_b_lep = DeltaR(truth_jet, b_lep); 

      dr_up_vec.emplace_back(dr_up);
      dr_down_vec.emplace_back(dr_down);
      dr_b_had_vec.emplace_back(dr_b_had);
      dr_b_lep_vec.emplace_back(dr_b_lep);
    }

    up_index    = this->FindIndex(dr_up_vec);
    down_index  = this->FindIndex(dr_down_vec);
    b_had_index = this->FindIndex(dr_b_had_vec);
    b_lep_index = this->FindIndex(dr_b_lep_vec);

    // check multimatch
    if (up_index == down_index) {
      up_index = -1;
      down_index = -1;
    }
    if (up_index == b_had_index) {
      up_index = -1;
      b_had_index = -1;
    }
    if (up_index == b_lep_index) {
      up_index = -1;
      b_lep_index = -1;
    }
    if (down_index == b_lep_index) {
      down_index = -1;
      b_lep_index = -1;
    }
    if (down_index == b_had_index) {
      down_index = -1;
      b_had_index = -1;
    }
    if (b_lep_index == b_had_index) {
      b_lep_index = -1;
      b_had_index = -1;
    }

    m_lep_b_idx_decor.set(*evtInfo, b_lep_index, sys);
    m_had_b_idx_decor.set(*evtInfo, b_had_index, sys);
    m_down_idx_decor.set(*evtInfo, down_index, sys);
    m_up_idx_decor.set(*evtInfo, up_index, sys);
    m_event_is_dilepton_decor.set(*evtInfo, 0, sys);

    return StatusCode::SUCCESS;
  }

  int PartonToJetsMatchAlg::FindIndex(const std::vector<double>& dr) const {

    std::size_t n_hits(0);
    int best(-1);

    for (std::size_t i = 0; i < dr.size(); ++i) {
      if (dr.at(i) < m_criticalDR) {
        ++n_hits;
        best = i;
      }
    }

    if (n_hits > 1) {
      return -1;
    }

    return best;
 }

}
