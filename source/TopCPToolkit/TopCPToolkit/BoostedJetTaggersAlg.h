#ifndef BOOSTEDJETTAGGERSALG_H
#define BOOSTEDJETTAGGERSALG_H

#include <AnaAlgorithm/AnaAlgorithm.h>
#include <SystematicsHandles/SysListHandle.h>
#include <SystematicsHandles/SysReadHandle.h>
#include <SystematicsHandles/SysWriteDecorHandle.h>
#include <xAODJet/JetContainer.h>

// Top taggers
#include <memory>

#include "AsgTools/AnaToolHandle.h"
#include "BoostedJetTaggers/JSSWTopTaggerANN.h"
#include "BoostedJetTaggers/JSSWTopTaggerDNN.h"

namespace top {

class BoostedJetTaggersAlg final : public EL::AnaAlgorithm {

 public:
  BoostedJetTaggersAlg(const std::string &name, ISvcLocator *pSvcLocator);
  virtual StatusCode initialize() override;
  virtual StatusCode execute() override;

 private:
  bool m_isMC;
  std::string m_taggerTypeStr;
  std::string m_taggerCalibArea;
  std::string m_taggerConfigFile;
  std::string m_taggerDecoration;

  enum class TaggerType { JSSWTopTaggerDNN, JSSWTopTaggerANN };
  TaggerType m_taggerType;
  asg::AnaToolHandle<JSSWTopTaggerDNN> m_taggerDNN;
  asg::AnaToolHandle<JSSWTopTaggerANN> m_taggerANN;

  CP::SysListHandle m_systematicsList{this};

  CP::SysReadHandle<xAOD::JetContainer> m_ljetsHandle{
      this, "ljets", "", "the input large-R jet container"};

  std::unique_ptr<CP::SysWriteDecorHandle<int>> m_ljetsTopTagHandle;
  std::unique_ptr<CP::SysWriteDecorHandle<float>> m_ljetsTopTagScoreHandle;
  std::unique_ptr<CP::SysWriteDecorHandle<int>> m_ljetsTopTagPassKinRangeHandle;
  std::unique_ptr<CP::SysWriteDecorHandle<int>> m_ljetsTopTagPassMassHandle;
  std::unique_ptr<CP::SysWriteDecorHandle<float>> m_ljetsTopTagSFHandle;
  std::unique_ptr<CP::SysWriteDecorHandle<float>> m_ljetsTopTagEfficiencyHandle;
  std::unique_ptr<CP::SysWriteDecorHandle<float>> m_ljetsTopTagEffSFHandle;
};

}  // namespace top

#endif
