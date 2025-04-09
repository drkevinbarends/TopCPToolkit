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
    
    CP::SysReadHandle<xAOD::ElectronContainer> m_electronsHandle {
      this, "electrons", "", "the input electron container"
    };
    CP::SysReadHandle<xAOD::MuonContainer> m_muonsHandle {
      this, "muons", "", "the input muon container"
    };
    CP::SysReadHandle<xAOD::MissingETContainer> m_metHandle {
      this, "met", "", "the input MET container"
    };
    CP::SysReadHandle<xAOD::JetContainer> m_jetsHandle {
      this, "jets", "", "the input jet container"
    };
    CP::SysReadHandle<xAOD::EventInfo> m_eventInfoHandle {
      this, "eventInfo", "EventInfo", "the input EventInfo container"
    };
    CP::SysWriteDecorHandle<float> m_dphimetHandle {
      this, "dphimet", "dphimet_%SYS%", "decoration name for DeltaPhi between each jet and the MET"
    };
    CP::SysWriteDecorHandle<float> m_mtwHandle {
      this, "mtw", "mtw_%SYS%", "decoration name for the transverse W mass"
    };
    CP::SysReadSelectionHandle m_preselection {
      this, "eventPreselection", "", "event preselection to check before running this algorithm"
    };

  };

} // namsepace top

#endif
