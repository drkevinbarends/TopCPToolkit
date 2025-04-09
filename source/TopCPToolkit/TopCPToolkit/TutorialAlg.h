#ifndef TUTORIALALG_H
#define TUTORIALALG_H

#include <AnaAlgorithm/AnaAlgorithm.h>
#include <SystematicsHandles/SysListHandle.h>
#include <SystematicsHandles/SysReadHandle.h>
#include <SystematicsHandles/SysWriteDecorHandle.h>
#include <SelectionHelpers/SysReadSelectionHandle.h>

#include <xAODEgamma/ElectronContainer.h>
#include <xAODMuon/MuonContainer.h>
#include <xAODJet/JetContainer.h>
#include <xAODMissingET/MissingETContainer.h>
#include <xAODEventInfo/EventInfo.h>

#include <TVector2.h>

namespace top {

  class TutorialAlg final : public EL::AnaAlgorithm {

    public:
      TutorialAlg(const std::string &name, ISvcLocator *pSvcLocator);
      virtual StatusCode initialize() override;
      virtual StatusCode execute() override;

    private:
      CP::SysListHandle m_systematicsList {this};
  };

} // namsepace top

#endif
