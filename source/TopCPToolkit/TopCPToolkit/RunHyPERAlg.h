#ifndef HYPER_RUNHYPERALG_H
#define HYPER_RUNHYPERALG_H

/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/
/// @author Diego Baron
/// @author Zihan Zhang

#include <string>

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

#include "AthContainers/ConstDataVector.h"

// HyPER includes
#include "TopHyPER/HyPERGraph.h"
#include "TopHyPER/HyPERModel.h"
#include "TopHyPER/HyPERParser.h"

namespace top {
/**
 * @struct InputsPack
 * @brief This struct stores the input objects needed to build the graph after
 * reading and filtering the xAOD containers.
 */
struct InputsPack {
  ConstDataVector<xAOD::ElectronContainer> m_electrons;
  ConstDataVector<xAOD::MuonContainer> m_muons;
  ConstDataVector<xAOD::JetContainer> m_jets;
  const xAOD::MissingETContainer *m_met = nullptr;

  InputsPack() = default;

  void clear() {
    m_electrons.clear();
    m_muons.clear();
    m_jets.clear();
  }
};

/**
 *  @class RunHyPERAlg
 *  @brief This class is in charge of building the HyPER graph and running the
 * HyPER algorithm: The user-inputs are:
 *    - The HyPER topology
 *    - The b-tagger to use
 *    - The object containers to use
 *    - The event selection
 *    - The algortithm debug level.
 *  Apart from the usual handles to the xAOD containers and tools, the class
 * stores pointers to:
 *    - The HyPER ONNX model to be used -> Implemented in HyPER<TOPOLOGY>Model.h
 *    - The graph class -> Implemented in HyPERGraph.h
 *    - A parser object -> Implemented in HyPER<TOPOLOGY>Parser.h
 */
class RunHyPERAlg final : public EL::AnaAlgorithm {
 public:
  RunHyPERAlg(const std::string &name, ISvcLocator *pSvcLocator);
  virtual StatusCode initialize() override;
  virtual StatusCode execute() override;
  virtual StatusCode finalize() override;

  /**
   * @brief These methods build the HyPER graph based on the user-input
   * topology.
   */
  StatusCode buildGraph();
  StatusCode buildTtbarAllHadronicGraph();
  StatusCode buildTtbarLJetsGraph();

  // Message level for auxiliary code.
  static MSG::Level s_msgLevelHyPER;

 private:
  // Configurable properties from python/yml
  std::string m_topology;
  std::string m_btagger;
  std::string m_trainedOnEvenPath;
  std::string m_trainedOnOddPath;

  // The onnxruntime model
  std::unique_ptr<HyPERModel> m_hyperModel;
  // The HyPER Graph object
  std::unique_ptr<HyPERGraph> m_hyperGraph;
  // The inputs from xAOD containers
  InputsPack m_hyperInputs{};
  // The HyPER parser object
  std::unique_ptr<HyPERParser> m_hyperParser;
  // Number for full log event
  long unsigned int m_fullLogEventNumber;

  // systematics
  CP::SysListHandle m_systematicsList{this};

  // Input objects needed for reconstruction
  // Electrons
  CP::SysReadHandle<xAOD::ElectronContainer> m_electronsHandle{
      this, "electrons", "", "The electron container to use."};
  CP::SysReadSelectionHandle m_electronSelection{
      this, "electronSelection", "", "The selection on the input electrons."};
  // Muons
  CP::SysReadHandle<xAOD::MuonContainer> m_muonsHandle{
      this, "muons", "", "The muon container to use."};
  CP::SysReadSelectionHandle m_muonSelection{
      this, "muonSelection", "", "The selection on the input muons."};
  // Jets
  CP::SysReadHandle<xAOD::JetContainer> m_jetsHandle{
      this, "jets", "", "The jet container to use."};
  CP::SysReadSelectionHandle m_jetSelection{this, "jetSelection", "",
                                            "The selection on the input jets."};
  // MET
  CP::SysReadHandle<xAOD::MissingETContainer> m_metHandle{
      this, "met", "", "The MET container to use."};
  // EventInfo
  CP::SysReadHandle<xAOD::EventInfo> m_eventInfoHandle{
      this, "eventInfo", "EventInfo",
      "The EventInfo container to read selection decisions from."};
  // Event pre-selection
  CP::SysReadSelectionHandle m_selection{
      this, "eventSelection", "",
      "Name of the selection on which this HyPER instance is allowed to run."};

  // Output decorations

  // For TtbarAllHadronic
  // Indices
  CP::SysWriteDecorHandle<std::vector<int64_t>>
      m_hyper_TtbarAllHadronic_Top1_Indices{
          this, "hyper_TtbarAllHadronic_Top1_Indices",
          "hyper_TtbarAllHadronic_Top1_Indices_%SYS%",
          "Indices of the reconstructed top1 in the ttbar all-hadronic "
          "topology"};
  CP::SysWriteDecorHandle<std::vector<int64_t>>
      m_hyper_TtbarAllHadronic_Top2_Indices{
          this, "hyper_TtbarAllHadronic_Top2_Indices",
          "hyper_TtbarAllHadronic_Top2_Indices_%SYS%",
          "Indices of the reconstructed top2 in the ttbar all-hadronic "
          "topology"};
  CP::SysWriteDecorHandle<std::vector<int64_t>>
      m_hyper_TtbarAllHadronic_W1_Indices{
          this, "hyper_TtbarAllHadronic_W1_Indices",
          "hyper_TtbarAllHadronic_W1_Indices_%SYS%",
          "Indices of the reconstructed W1 in the ttbar all-hadronic topology"};
  CP::SysWriteDecorHandle<std::vector<int64_t>>
      m_hyper_TtbarAllHadronic_W2_Indices{
          this, "hyper_TtbarAllHadronic_W2_Indices",
          "hyper_TtbarAllHadronic_W2_Indices_%SYS%",
          "Indices of the reconstructed W2 in the ttbar all-hadronic topology"};
  // Scores
  CP::SysWriteDecorHandle<float> m_hyper_TtbarAllHadronic_Top1_Score{
      this, "hyper_TtbarAllHadronic_Top1_Score",
      "hyper_TtbarAllHadronic_Top1_Score_%SYS%",
      "Score of the reconstructed top1 in the ttbar all-hadronic topology"};
  CP::SysWriteDecorHandle<float> m_hyper_TtbarAllHadronic_Top2_Score{
      this, "hyper_TtbarAllHadronic_Top2_Score",
      "hyper_TtbarAllHadronic_Top2_Score_%SYS%",
      "Score of the reconstructed top2 in the ttbar all-hadronic topology"};
  CP::SysWriteDecorHandle<float> m_hyper_TtbarAllHadronic_W1_Score{
      this, "hyper_TtbarAllHadronic_W1_Score",
      "hyper_TtbarAllHadronic_W1_Score_%SYS%",
      "Score of the reconstructed W1 in the ttbar all-hadronic topology"};
  CP::SysWriteDecorHandle<float> m_hyper_TtbarAllHadronic_W2_Score{
      this, "hyper_TtbarAllHadronic_W2_Score",
      "hyper_TtbarAllHadronic_W2_Score_%SYS%",
      "Score of the reconstructed W2 in the ttbar all-hadronic topology"};

  // For TtbarLJets
  // Indices
  CP::SysWriteDecorHandle<std::vector<int64_t>>
      m_hyper_TtbarLJets_TopHad_Indices{
          this, "hyper_TtbarLJets_TopHad_Indices",
          "hyper_TtbarLJets_TopHad_Indices_%SYS%",
          "Indices of the reconstructed hadronic top in the ttbar single "
          "lepton topology"};
  CP::SysWriteDecorHandle<std::vector<int64_t>>
      m_hyper_TtbarLJets_TopLep_Indices{
          this, "hyper_TtbarLJets_TopLep_Indices",
          "hyper_TtbarLJets_TopLep_Indices_%SYS%",
          "Indices of the reconstructed leptonic top in the ttbar single "
          "lepton topology"};
  CP::SysWriteDecorHandle<std::vector<int64_t>> m_hyper_TtbarLJets_WHad_Indices{
      this, "hyper_TtbarLJets_WHad_Indices",
      "hyper_TtbarLJets_WHad_Indices_%SYS%",
      "Indices of the reconstructed hadronic W in the ttbar single lepton "
      "topology"};
  CP::SysWriteDecorHandle<std::vector<int64_t>> m_hyper_TtbarLJets_WLep_Indices{
      this, "hyper_TtbarLJets_WLep_Indices",
      "hyper_TtbarLJets_WLep_Indices_%SYS%",
      "Indices of the reconstructed leptonic W in the ttbar single lepton "
      "topology"};
  // Scores
  CP::SysWriteDecorHandle<float> m_hyper_TtbarLJets_TopHad_Score{
      this, "hyper_TtbarLJets_TopHad_Score",
      "hyper_TtbarLJets_TopHad_Score_%SYS%",
      "Score of the reconstructed hadronic top in the ttbar single lepton "
      "topology"};
  CP::SysWriteDecorHandle<float> m_hyper_TtbarLJets_TopLep_Score{
      this, "hyper_TtbarLJets_TopLep_Score",
      "hyper_TtbarLJets_TopLep_Score_%SYS%",
      "Score of the reconstructed leptonic top in the ttbar single lepton "
      "topology"};
  CP::SysWriteDecorHandle<float> m_hyper_TtbarLJets_WHad_Score{
      this, "hyper_TtbarLJets_WHad_Score", "hyper_TtbarLJets_WHad_Score_%SYS%",
      "Score of the reconstructed hadronic W in the ttbar single lepton "
      "topology"};
  CP::SysWriteDecorHandle<float> m_hyper_TtbarLJets_WLep_Score{
      this, "hyper_TtbarLJets_WLep_Score", "hyper_TtbarLJets_WLep_Score_%SYS%",
      "Score of the reconstructed leptonic W in the ttbar single lepton "
      "topology"};
  // IDs
  CP::SysWriteDecorHandle<std::vector<int64_t>> m_hyper_TtbarLJets_TopHad_IDs{
      this, "hyper_TtbarLJets_TopHad_IDs", "hyper_TtbarLJets_TopHad_IDs_%SYS%",
      "IDs of the reconstructed hadronic top in the ttbar single lepton "
      "topology"};
  CP::SysWriteDecorHandle<std::vector<int64_t>> m_hyper_TtbarLJets_TopLep_IDs{
      this, "hyper_TtbarLJets_TopLep_IDs", "hyper_TtbarLJets_TopLep_IDs_%SYS%",
      "IDs of the reconstructed leptonic top in the ttbar single lepton "
      "topology"};

  // Tools and functions for btagging
  std::unique_ptr<SG::AuxElement::ConstAccessor<int>> m_bTagDecoAcc;
};

}  // namespace top

#endif
