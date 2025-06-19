#ifndef TOPNUFLOWS_RUNNUFLOWSALG_H
#define TOPNUFLOWS_RUNNUFLOWSALG_H

// Algorithm includes
#include <AnaAlgorithm/AnaAlgorithm.h>
#include <SelectionHelpers/SysReadSelectionHandle.h>
#include <SystematicsHandles/SysReadDecorHandle.h>
#include <SystematicsHandles/SysReadHandle.h>
#include <SystematicsHandles/SysWriteDecorHandle.h>
#include <SystematicsHandles/SysWriteHandle.h>

// Framework includes
#include <xAODEgamma/ElectronContainer.h>
#include <xAODEventInfo/EventInfo.h>
#include <xAODJet/JetContainer.h>
#include <xAODMissingET/MissingETContainer.h>
#include <xAODMuon/MuonContainer.h>

// NuFlows model
#include "TopNuFlows/Dilepton.h"

namespace top {

class RunNuFlowsAlg final : public EL::AnaAlgorithm {
 public:
  RunNuFlowsAlg(const std::string &name, ISvcLocator *pSvcLocator);
  virtual StatusCode initialize() override;
  virtual StatusCode execute() override;
  virtual StatusCode finalize() override;

 private:
  // Function to run in the event loop per systematic
  StatusCode execute_syst(const CP::SystematicSet &sys);

  // configurable properties
  std::string m_onnxFilePath;
  std::string m_btagger;
  bool m_write_inputs;

  // the nuflows object
  std::unique_ptr<top::TopNuFlowsDilepton> m_nuflows;

  // Systematics
  CP::SysListHandle m_systematicsList{this};

  // Inputs needed for reconstruction
  CP::SysReadHandle<xAOD::ElectronContainer> m_electronsHandle{
      this, "electrons", "", "the electron container to use"};
  CP::SysReadSelectionHandle m_electronSelection{
      this, "electronSelection", "", "the selection on the input electrons"};
  CP::SysReadHandle<xAOD::MuonContainer> m_muonsHandle{
      this, "muons", "", "the muon container to use"};
  CP::SysReadSelectionHandle m_muonSelection{
      this, "muonSelection", "", "the selection on the input muons"};
  CP::SysReadHandle<xAOD::JetContainer> m_jetsHandle{
      this, "jets", "", "the jet container to use"};
  CP::SysReadSelectionHandle m_jetSelection{this, "jetSelection", "",
                                            "the selection on the input jets"};
  CP::SysReadHandle<xAOD::MissingETContainer> m_metHandle{
      this, "met", "", "the MET container to use"};
  CP::SysReadHandle<xAOD::EventInfo> m_eventInfoHandle{
      this, "eventInfo", "EventInfo",
      "the EventInfo container to read selection decisions from"};

  // Output decorations
  CP::SysWriteDecorHandle<vector<float>> m_nu_out_decor{
      this, "nu_out", "nu_out_%SYS%", "Sampled value of the unfolded vector"};
  CP::SysWriteDecorHandle<float> m_loglik_decor{
      this, "loglik", "loglik_%SYS%",
      "Log likelihood of the unfolded solution"};

  // Inputs to the conditional normalizing flow
  CP::SysWriteDecorHandle<vector<vector<float>>> m_input_lep_decor{
      this, "input_lep", "input_lep_%SYS%", "The input vector for the leptons"};
  CP::SysWriteDecorHandle<vector<vector<float>>> m_input_jet_decor{
      this, "input_jet", "input_jet_%SYS%", "The input vector for the jets"};
  CP::SysWriteDecorHandle<vector<vector<float>>> m_input_met_decor{
      this, "input_met", "input_met_%SYS%", "The input vector for the met"};
  CP::SysWriteDecorHandle<vector<vector<float>>> m_input_misc_decor{
      this, "input_misc", "input_misc_%SYS%",
      "The input vector for the misc variables"};

  // Misc
  CP::SysReadSelectionHandle m_eventselection{
      this, "eventSelection", "",
      "event preselection to check before running this algorithm"};
};

}  // namespace top

#endif
