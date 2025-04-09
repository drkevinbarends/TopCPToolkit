#ifndef PARTON_TO_JETS_MATCH_ALG_H
#define PARTON_TO_JETS_MATCH_ALG_H

// Algorithm includes
#include <AnaAlgorithm/AnaAlgorithm.h>
#include <SystematicsHandles/SysReadHandle.h>
#include <SelectionHelpers/SysReadSelectionHandle.h>
#include <SystematicsHandles/SysWriteHandle.h>
#include <SystematicsHandles/SysReadDecorHandle.h>
#include <SystematicsHandles/SysWriteDecorHandle.h>

#include <xAODJet/JetContainer.h>
#include <xAODEventInfo/EventInfo.h>

namespace top {

  class PartonToJetsMatchAlg final : public EL::AnaAlgorithm {

    public:
      PartonToJetsMatchAlg(const std::string &name, ISvcLocator *pSvcLocator);
      virtual StatusCode initialize() override final;
      virtual StatusCode execute() override final;
      virtual StatusCode finalize() override final;

    private:
      StatusCode execute_syst(const CP::SystematicSet &sys);

      int FindIndex(const std::vector<double>& dr) const;

      // systematics
      CP::SysListHandle m_systematicsList {this};

      CP::SysReadHandle<xAOD::JetContainer> m_jetsHandle {
        this, "jets", "", "the jet container to use"
	  };
      CP::SysReadSelectionHandle m_jetSelection {
        this, "jetSelection", "", "the selection on the input jets"
	  };

      CP::SysReadHandle<xAOD::EventInfo> m_eventInfoHandle {
        this, "eventInfo", "EventInfo", "the EventInfo container to read selection decisions from"
	  };

      CP::SysReadSelectionHandle m_selection {
        this, "eventSelection", "", "Name of the selection on which this SPA-Net instance is allowed to run"
	  };

      CP::SysWriteDecorHandle<int> m_lep_b_idx_decor {
        this, "leptonic_b_index", "parton_truth_lep_b_index_%SYS%", "Index of the leptonic b-jet matched to the parton truth in lepton+jets"
    };
      CP::SysWriteDecorHandle<int> m_had_b_idx_decor {
        this, "hadronic_b_index", "parton_truth_had_b_index_%SYS%", "Index of the hadronic b-jet matched to the parton truth in lepton+jets"
    };
      CP::SysWriteDecorHandle<int> m_up_idx_decor {
        this, "up_index", "parton_truth_up_index_%SYS%", "Index of the up-type jet from W decay matched to the parton truth in lepton+jets"
    };
      CP::SysWriteDecorHandle<int> m_down_idx_decor {
        this, "down_index", "parton_truth_down_index_%SYS%", "Index of the down-type jet from W decay matched to the parton truth in lepton+jets"
    };
      CP::SysWriteDecorHandle<char> m_event_is_dilepton_decor {
        this, "event_is_dilepton", "parton_truth_event_is_dilepton_%SYS%", "Event is dilepton event (ttbar)"
    };

    float m_criticalDR = 0.3;
    std::string m_partonContainerName;
  };
}

#endif
