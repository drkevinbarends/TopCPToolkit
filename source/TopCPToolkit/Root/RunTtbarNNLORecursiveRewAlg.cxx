#include "TopCPToolkit/RunTtbarNNLORecursiveRewAlg.h"

namespace top {

    RunTtbarNNLORecursiveRewAlg::RunTtbarNNLORecursiveRewAlg(const std::string &name, ISvcLocator *pSvcLocator)
        : EL::AnaAlgorithm(name, pSvcLocator)
	, m_sampleID(-1)
	, m_reweightType(-1)
        , m_user_sampleID("SetMe")
        , m_user_reweightType("SetMe")
        , m_reweightPath("SetMe")
        , m_reweightSuffix("SetMe")
        {
            declareProperty("sampleID", m_user_sampleID,
                "Type of ttbar MC sample to use as the basis for reweighting. Choose between: "
                "PH7new,aMCH7,PH7,aMCP8,PP8hdamp,PP8muRup,PP8muRdown,PP8muFu,PP8muFdown,PP8FSRup,PP8FSRdown,PP8pthard1,MECoff"
            );
            declareProperty("reweightingType", m_user_reweightType, "Type of reweighting to perform: 2D, 3D.");
            declareProperty("reweightingPath", m_reweightPath, "Path to the directory containing the histograms for reweighting ");
            declareProperty("reweightingSuffix", m_reweightSuffix, "Suffix to apply to ROOT filenames, in order to access specific variations.");
        }

    StatusCode RunTtbarNNLORecursiveRewAlg::initialize() {
        ANA_MSG_INFO("Initializing RunTtbarNNLORecursiveRew " << name() );

        // parse properties
        m_sampleID     = NNLORewEnums::strToSampleId.at(m_user_sampleID);
        m_reweightType = NNLORewEnums::strToRewType.at(m_user_reweightType);

        ANA_MSG_INFO("  --> sample: " << m_user_sampleID );
        ANA_MSG_INFO("  --> type:   " << m_user_reweightType );
        ANA_MSG_INFO("  --> path:   " << m_reweightPath );
        ANA_MSG_INFO("  --> suffix: " << m_reweightSuffix );

        // initialise and configure the tool
        m_nnlo_reweighter = std::unique_ptr<TtbarNNLORecursiveRew> (new TtbarNNLORecursiveRew("AsgTtbarNNLORecursiveRewTool"+m_user_sampleID));
        ANA_CHECK(m_nnlo_reweighter->setProperty("SampleID", m_sampleID));
        ANA_CHECK(m_nnlo_reweighter->setProperty("Type", m_reweightType));
        ANA_CHECK(m_nnlo_reweighter->setProperty("InputDirectory", m_reweightPath));
        ANA_CHECK(m_nnlo_reweighter->setProperty("InputSuffix", m_reweightSuffix));
        ANA_CHECK(m_nnlo_reweighter->initialize());

        ANA_CHECK(m_systematicsList.addSystematics(*m_nnlo_reweighter));
        ANA_CHECK(m_eventInfoHandle.initialize(m_systematicsList));
        ANA_CHECK(m_NNLO_weight_decor.initialize(m_systematicsList, m_eventInfoHandle));
        ANA_CHECK(m_systematicsList.initialize());

        return StatusCode::SUCCESS;
    }

    StatusCode RunTtbarNNLORecursiveRewAlg::execute() {
        for (const auto &sys : m_systematicsList.systematicsVector()) {
            ANA_CHECK(execute_syst(sys));
        }
        return StatusCode::SUCCESS;
    }

    StatusCode RunTtbarNNLORecursiveRewAlg::execute_syst(const CP::SystematicSet &sys) {
        // retrieve the event info
        const xAOD::EventInfo *evtInfo = nullptr;
        ANA_CHECK(m_eventInfoHandle.retrieve(evtInfo, sys));

        // retrieve the parton history
        const xAOD::PartonHistory* ttbarPartonHistory(nullptr);
        if ( evtStore()->contains<xAOD::PartonHistory>("PartonHistoryTtbar_NOSYS") ) {
            ANA_CHECK(evtStore()->retrieve(ttbarPartonHistory, "PartonHistoryTtbar_NOSYS"));
        }
        else {
            ANA_MSG_ERROR("Attempting to run the ttbar NNLO reweighter without a valid PartonHistoryTtbar!");
        }

        // define accessors
        static const SG::AuxElement::ConstAccessor<float> acc_top_pt("MC_t_afterFSR_pt");
        static const SG::AuxElement::ConstAccessor<float> acc_antitop_pt("MC_tbar_afterFSR_pt");
        static const SG::AuxElement::ConstAccessor<float> acc_ttbar_m("MC_ttbar_afterFSR_m");
        static const SG::AuxElement::ConstAccessor<float> acc_ttbar_pt("MC_ttbar_afterFSR_pt");

        // retrieve necessary top kinematics here --> they're floats!
        float top_pt     = acc_top_pt.isAvailable(*ttbarPartonHistory)     ? acc_top_pt(*ttbarPartonHistory) : -1;
        float antitop_pt = acc_antitop_pt.isAvailable(*ttbarPartonHistory) ? acc_antitop_pt(*ttbarPartonHistory) : -1;
        float ttbar_m    = acc_ttbar_m.isAvailable(*ttbarPartonHistory)    ? acc_ttbar_m(*ttbarPartonHistory) : -1;
        float ttbar_pt   = acc_ttbar_pt.isAvailable(*ttbarPartonHistory)   ? acc_ttbar_pt(*ttbarPartonHistory) : -1;

        // then call the reweight method - watch out, we need numbers in GeV not in MeV (as in the ttbar parton history)!
        double result = m_nnlo_reweighter->GetSysWeight(sys, top_pt/1.e3, antitop_pt/1.e3, ttbar_m/1.e3, ttbar_pt/1.e3);

        // decorate the result onto EventInfo
        m_NNLO_weight_decor.set(*evtInfo, result, sys);

        return StatusCode::SUCCESS;
    }

    StatusCode RunTtbarNNLORecursiveRewAlg::finalize() {
        ANA_MSG_INFO("Finalizing RunTtbarNNLORecursiveRew");

        return StatusCode::SUCCESS;
    }

} // namespace top
