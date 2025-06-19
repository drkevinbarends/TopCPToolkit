#ifndef SINGLELEP_HADRONIC_CHI2_ALG_H
#define SINGLELEP_HADRONIC_CHI2_ALG_H

// Algorithm includes
#include <AnaAlgorithm/AnaAlgorithm.h>
#include <SelectionHelpers/SysReadSelectionHandle.h>
#include <SystematicsHandles/SysReadDecorHandle.h>
#include <SystematicsHandles/SysReadHandle.h>
#include <SystematicsHandles/SysWriteDecorHandle.h>
#include <SystematicsHandles/SysWriteHandle.h>
#include <xAODEventInfo/EventInfo.h>
#include <xAODJet/JetContainer.h>

namespace top {

class SinglelepHadronicChi2RecoAlg final : public EL::AnaAlgorithm {

 public:
  SinglelepHadronicChi2RecoAlg(const std::string &name,
                               ISvcLocator *pSvcLocator);
  virtual StatusCode initialize() override final;
  virtual StatusCode execute() override final;
  virtual StatusCode finalize() override final;

 private:
  struct PermutationParam {
    int bIndex;
    int lIndex1;
    int lIndex2;
    double chi2;
  };

  StatusCode execute_syst(const CP::SystematicSet &sys);

  // systematics
  CP::SysListHandle m_systematicsList{this};

  CP::SysReadHandle<xAOD::JetContainer> m_jetsHandle{
      this, "jets", "", "the jet container to use"};
  CP::SysReadSelectionHandle m_jetSelection{this, "jetSelection", "",
                                            "the selection on the input jets"};

  CP::SysReadHandle<xAOD::EventInfo> m_eventInfoHandle{
      this, "eventInfo", "EventInfo",
      "the EventInfo container to read selection decisions from"};

  CP::SysReadSelectionHandle m_selection{
      this, "eventSelection", "",
      "Name of the selection on which this SPA-Net instance is allowed to run"};

  CP::SysWriteDecorHandle<std::vector<int>> m_had_b_idx_decor{
      this, "hadronic_b_index", "chi2_had_b_index_%SYS%",
      "Index of the hadronic b-jet found by the chi^2 algorithm"};
  CP::SysWriteDecorHandle<std::vector<int>> m_light_1_idx_decor{
      this, "light_1_index", "chi2_light_1_index_%SYS%",
      "Index of the first jet from W decay found by the chi^2 algorithm"};
  CP::SysWriteDecorHandle<std::vector<int>> m_light_2_idx_decor{
      this, "light_2_index", "chi2_light_2_index_%SYS%",
      "Index of the second jet from W decay found by the chi^2 algorithm"};
  CP::SysWriteDecorHandle<std::vector<float>> m_best_chi2_decor{
      this, "best_chi2", "best_chi2_%SYS%",
      "best chi^2 value for the reconstruction"};

  double chi2(const float recoWmass, const float recoTopMass) const;

  double runSinglePermutation(const ConstDataVector<xAOD::JetContainer> &jets,
                              const int bIndex, const int lIndex1,
                              const int lIndex2);

  float m_topMass = 172.5;
  int m_maxBJets = 2;
  int m_maxLightJets = 4;
  float m_topResolution = 35;
  float m_wResolution = 25;
  std::string m_bTagDecoration = "";
  bool m_storeFullPermutations = false;

  std::unique_ptr<SG::AuxElement::ConstAccessor<char>> m_bTagDecoAcc;

  std::vector<int> m_had_b_idx;
  std::vector<int> m_light_1_idx;
  std::vector<int> m_light_2_idx;
  std::vector<float> m_best_chi2;
};
}  // namespace top

#endif
