/*
    Copyright (C) 2002-2025 CERN for the benefit of the ATLAS collaboration
    Implemented by Zackary Alegria following instructions from the JETM
    twiki recommendation on Dijet sample normalization
    https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/JetEtMissMCSamples#Dijet_normalization_procedure_HS
*/

#include "TopCPToolkit/HSTPFilterAlg.h"

#include <EventBookkeeperTools/FilterReporter.h>
#include <xAODEventInfo/EventInfo.h>
#include <xAODJet/JetContainer.h>

namespace CP {

HSTPFilterAlg::HSTPFilterAlg(const std::string& name, ISvcLocator* pSvcLocator)
    : EL::AnaAlgorithm(name, pSvcLocator) {}

StatusCode HSTPFilterAlg::initialize() {

  if (m_truthHSCollection.empty()) {
    ANA_MSG_ERROR("The truth HS jet collection name is empty");
    return StatusCode::FAILURE;
  }

  if (m_truthPUCollection.empty()) {
    ANA_MSG_ERROR("The truth PU jet collection name is empty");
    return StatusCode::FAILURE;
  }

  ANA_CHECK(m_filterParams.initialize());

  return StatusCode::SUCCESS;
}  // initialize

StatusCode HSTPFilterAlg::execute() {

  FilterReporter filter(m_filterParams, false);

  const xAOD::JetContainer* truthHS_jets = nullptr;
  ANA_CHECK((*evtStore()).retrieve(truthHS_jets, m_truthHSCollection));
  const xAOD::JetContainer* truthPU_jets = nullptr;
  ANA_CHECK((*evtStore()).retrieve(truthPU_jets, m_truthPUCollection));

  double pT_j1_truthHS =
      5000.;  // In the rare case of no truth HS jets set to 5 GeV
  double pT_j1_truthPU = 0.;

  if (truthHS_jets->size()) {
    pT_j1_truthHS = truthHS_jets->at(0)->pt();
  }
  if (truthPU_jets->size()) {
    pT_j1_truthPU = truthPU_jets->at(0)->pt();
  }

  bool pass_HSTP_filter = pT_j1_truthHS > pT_j1_truthPU;
  filter.setPassed(pass_HSTP_filter);

  return StatusCode::SUCCESS;

}  // execute

StatusCode HSTPFilterAlg::finalize() {

  ANA_MSG_INFO(m_filterParams.summary());

  return StatusCode::SUCCESS;
}  // finalize

}  // namespace CP
