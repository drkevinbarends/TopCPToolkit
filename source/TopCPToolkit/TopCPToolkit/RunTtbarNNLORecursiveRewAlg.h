#ifndef TopNNLOReweighter_ALG_H
#define TopNNLOReweighter_ALG_H

// Algorithm includes
#include <AnaAlgorithm/AnaAlgorithm.h>
#include <SystematicsHandles/SysReadHandle.h>
#include <SelectionHelpers/SysReadSelectionHandle.h>
#include <SystematicsHandles/SysWriteHandle.h>
#include <SystematicsHandles/SysReadDecorHandle.h>
#include <SystematicsHandles/SysWriteDecorHandle.h>

// Framework includes
#include <xAODEventInfo/EventInfo.h>

// Custom includes
#include "TopNNLOReweighter/TtbarNNLORecursiveRew.h"
#include "PartonHistory/CalcTtbarPartonHistory.h"

namespace top {

    class RunTtbarNNLORecursiveRewAlg final : public EL::AnaAlgorithm {
    public:
        RunTtbarNNLORecursiveRewAlg(const std::string &name, ISvcLocator *pSvcLocator);
        virtual StatusCode initialize() override;
        virtual StatusCode execute() override;
        virtual StatusCode finalize() override;

    private:
        StatusCode execute_syst(const CP::SystematicSet &sys);

        // configurable properties
        int m_sampleID;
        int m_reweightType;
        std::string m_user_sampleID;
        std::string m_user_reweightType;
        std::string m_reweightPath;
        std::string m_reweightSuffix;

        // the reweighter tool
        std::unique_ptr<TtbarNNLORecursiveRew> m_nnlo_reweighter;

        // systematics
        CP::SysListHandle m_systematicsList {this};

        // inputs needed for NNLO reweighting
        CP::SysReadHandle<xAOD::EventInfo> m_eventInfoHandle {
            this, "eventInfo", "EventInfo", "the EventInfo container to read truth information from"
    	};

        // output decorations
        CP::SysWriteDecorHandle<double> m_NNLO_weight_decor {
            this, "NNLO_weight", "NNLO_weight_%SYS%", "Output weight of the Ttbar NNLO reweighter"
        };

    }; // class
} // namespace top

#endif // TopNNLOReweighter_ALG_H
