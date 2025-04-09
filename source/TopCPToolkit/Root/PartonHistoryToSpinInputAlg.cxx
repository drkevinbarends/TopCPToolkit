#include "TopCPToolkit/PartonHistoryToSpinInputAlg.h"

namespace top {
  using ROOT::Math::PtEtaPhiMVector;

  PartonHistoryToSpinInputAlg::PartonHistoryToSpinInputAlg(const std::string &name,
                                                           ISvcLocator *pSvcLocator)
    : EL::AnaAlgorithm(name, pSvcLocator) {}

  StatusCode PartonHistoryToSpinInputAlg::initialize() {

    ANA_CHECK(m_eventInfoHandle.initialize(m_systematicsList));
    ANA_CHECK(m_partonHistoryHandle.initialize(m_systematicsList));

    ANA_CHECK(m_top.initialize(m_systematicsList, m_eventInfoHandle));
    ANA_CHECK(m_tbar.initialize(m_systematicsList, m_eventInfoHandle));
    ANA_CHECK(m_top_decay.initialize(m_systematicsList, m_eventInfoHandle));
    ANA_CHECK(m_tbar_decay.initialize(m_systematicsList, m_eventInfoHandle));

    ANA_CHECK(m_systematicsList.initialize());

    return StatusCode::SUCCESS;
  }

  StatusCode PartonHistoryToSpinInputAlg::execute() {

    for (const auto &sys : m_systematicsList.systematicsVector()) {

      // retrieve the EventInfo and parton history
      const xAOD::EventInfo *evtInfo = nullptr;
      ANA_CHECK(m_eventInfoHandle.retrieve(evtInfo, sys));
      const xAOD::PartonHistory *history = nullptr;
      ANA_CHECK(m_partonHistoryHandle.retrieve(history, sys));

      // top
      static const SG::AuxElement::Accessor<float> acc_top_pt("MC_t_afterFSR_pt");
      static const SG::AuxElement::Accessor<float> acc_top_eta("MC_t_afterFSR_eta");
      static const SG::AuxElement::Accessor<float> acc_top_phi("MC_t_afterFSR_phi");
      static const SG::AuxElement::Accessor<float> acc_top_m("MC_t_afterFSR_m");
      // anti-top
      static const SG::AuxElement::Accessor<float> acc_tbar_pt("MC_tbar_afterFSR_pt");
      static const SG::AuxElement::Accessor<float> acc_tbar_eta("MC_tbar_afterFSR_eta");
      static const SG::AuxElement::Accessor<float> acc_tbar_phi("MC_tbar_afterFSR_phi");
      static const SG::AuxElement::Accessor<float> acc_tbar_m("MC_tbar_afterFSR_m");
      // first top decay product
      static const SG::AuxElement::Accessor<float> acc_top_dec1_pt("MC_Wdecay1_from_t_pt");
      static const SG::AuxElement::Accessor<float> acc_top_dec1_eta("MC_Wdecay1_from_t_eta");
      static const SG::AuxElement::Accessor<float> acc_top_dec1_phi("MC_Wdecay1_from_t_phi");
      static const SG::AuxElement::Accessor<float> acc_top_dec1_m("MC_Wdecay1_from_t_m");
      static const SG::AuxElement::Accessor<int>   acc_top_dec1_pdgid("MC_Wdecay1_from_t_pdgId");
      // second top decay product
      static const SG::AuxElement::Accessor<float> acc_top_dec2_pt("MC_Wdecay2_from_t_pt");
      static const SG::AuxElement::Accessor<float> acc_top_dec2_eta("MC_Wdecay2_from_t_eta");
      static const SG::AuxElement::Accessor<float> acc_top_dec2_phi("MC_Wdecay2_from_t_phi");
      static const SG::AuxElement::Accessor<float> acc_top_dec2_m("MC_Wdecay2_from_t_m");
      static const SG::AuxElement::Accessor<int>   acc_top_dec2_pdgid("MC_Wdecay2_from_t_pdgId");
      // first anti-top decay product
      static const SG::AuxElement::Accessor<float> acc_tbar_dec1_pt("MC_Wdecay1_from_tbar_pt");
      static const SG::AuxElement::Accessor<float> acc_tbar_dec1_eta("MC_Wdecay1_from_tbar_eta");
      static const SG::AuxElement::Accessor<float> acc_tbar_dec1_phi("MC_Wdecay1_from_tbar_phi");
      static const SG::AuxElement::Accessor<float> acc_tbar_dec1_m("MC_Wdecay1_from_tbar_m");
      static const SG::AuxElement::Accessor<int>   acc_tbar_dec1_pdgid("MC_Wdecay1_from_tbar_pdgId");
      // second anti-top decay product
      static const SG::AuxElement::Accessor<float> acc_tbar_dec2_pt("MC_Wdecay2_from_tbar_pt");
      static const SG::AuxElement::Accessor<float> acc_tbar_dec2_eta("MC_Wdecay2_from_tbar_eta");
      static const SG::AuxElement::Accessor<float> acc_tbar_dec2_phi("MC_Wdecay2_from_tbar_phi");
      static const SG::AuxElement::Accessor<float> acc_tbar_dec2_m("MC_Wdecay2_from_tbar_m");
      static const SG::AuxElement::Accessor<int>   acc_tbar_dec2_pdgid("MC_Wdecay2_from_tbar_pdgId");

      // build the PtEtaPhiMVectors
      PtEtaPhiMVector top, tbar, top_decay, tbar_decay;
      top.SetCoordinates(acc_top_pt(*history), acc_top_eta(*history), acc_top_phi(*history), acc_top_m(*history));
      tbar.SetCoordinates(acc_tbar_pt(*history), acc_tbar_eta(*history), acc_tbar_phi(*history), acc_tbar_m(*history));
      // ignore neutrinos and up-type quarks amongst the decay products
      std::set<int> ignored = {2, 4, 12, 14, 16};
      if (ignored.find(std::abs(acc_top_dec1_pdgid(*history))) != ignored.end())
        top_decay.SetCoordinates(acc_top_dec2_pt(*history), acc_top_dec2_eta(*history), acc_top_dec2_phi(*history), acc_top_dec2_m(*history));
      else
        top_decay.SetCoordinates(acc_top_dec1_pt(*history), acc_top_dec1_eta(*history), acc_top_dec1_phi(*history), acc_top_dec1_m(*history));
      if (ignored.find(std::abs(acc_tbar_dec1_pdgid(*history))) != ignored.end())
        tbar_decay.SetCoordinates(acc_tbar_dec2_pt(*history), acc_tbar_dec2_eta(*history), acc_tbar_dec2_phi(*history), acc_tbar_dec2_m(*history));
      else
        tbar_decay.SetCoordinates(acc_tbar_dec1_pt(*history), acc_tbar_dec1_eta(*history), acc_tbar_dec1_phi(*history), acc_tbar_dec1_m(*history));

      // decorate onto EventInfo
      m_top.set(*evtInfo, top, sys);
      m_tbar.set(*evtInfo, tbar, sys);
      m_top_decay.set(*evtInfo, top_decay, sys);
      m_tbar_decay.set(*evtInfo, tbar_decay, sys);
    }

    return StatusCode::SUCCESS;
  }

} //namespace top
