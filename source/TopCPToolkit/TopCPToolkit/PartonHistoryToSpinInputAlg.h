#ifndef PARTON_HISTORY_TO_SPIN_INPUT__ALG_H
#define PARTON_HISTORY_TO_SPIN_INPUT__ALG_H

#include <AnaAlgorithm/AnaAlgorithm.h>
#include <SystematicsHandles/SysListHandle.h>
#include <SystematicsHandles/SysReadHandle.h>
#include <SystematicsHandles/SysWriteDecorHandle.h>

#include <PartonHistory/CalcPartonHistory.h>
#include <xAODEventInfo/EventInfo.h>

namespace top {
  using ROOT::Math::PtEtaPhiMVector;

  class PartonHistoryToSpinInputAlg final : public EL::AnaAlgorithm {

    public:
      PartonHistoryToSpinInputAlg(const std::string &name, ISvcLocator *pSvcLocator);
      virtual StatusCode initialize() override;
      virtual StatusCode execute() override;

    private:
      CP::SysListHandle m_systematicsList {this};
      CP::SysReadHandle<xAOD::EventInfo> m_eventInfoHandle {
        this, "eventInfo", "EventInfo", "the EventInfo container to decorate"
      };
      CP::SysReadHandle<xAOD::PartonHistory> m_partonHistoryHandle {
        this, "partonHistory", "", "the parton history to read from"
      };
      CP::SysWriteDecorHandle<PtEtaPhiMVector> m_top {
        this, "truth_top", "truth_top_4vect_%SYS%", "the 4-vector of the top"
      };
      CP::SysWriteDecorHandle<PtEtaPhiMVector> m_tbar {
        this, "truth_tbar", "truth_tbar_4vect_%SYS%", "the 4-vector of the anti-top"
      };
      CP::SysWriteDecorHandle<PtEtaPhiMVector> m_top_decay {
        this, "truth_top_decay", "truth_top_decay_4vect_%SYS%", "the 4-vector of the decay product (spin analyser) from the top"
      };
      CP::SysWriteDecorHandle<PtEtaPhiMVector> m_tbar_decay {
        this, "truth_tbar_decay", "truth_tbar_decay_4vect_%SYS%", "the 4-vector of the decay product (spin analyser) from the anti-top"
      };
  };

} // namespace top

#endif
