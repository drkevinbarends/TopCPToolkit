#include "TopCPToolkit/RunPartonHistoryAlg.h"

namespace top {
  RunPartonHistoryAlg::RunPartonHistoryAlg(const std::string &name, ISvcLocator *pSvcLocator)
    : EL::AnaAlgorithm(name, pSvcLocator),
      m_PartonScheme("Ttbar")
  {
    declareProperty("partonScheme", m_PartonScheme, "the parton history to run");
  }

  StatusCode RunPartonHistoryAlg::initialize() {
    ANA_MSG_INFO("Initializing PartonHistory " << name() );
    ANA_MSG_INFO("  - partonScheme: " << m_PartonScheme);

    if (m_PartonScheme == "Ttbar") m_PartonHistory = std::make_unique<CalcTtbarPartonHistory>("top::CalcTtbarPartonHistory");
    // else if (m_PartonScheme == "TtbarLight") m_PartonHistory = std::make_unique<CalcTtbarLightPartonHistory>("top::CalcTtbarLightPartonHistory");
    else if (m_PartonScheme == "Ttz") m_PartonHistory = std::make_unique<CalcTtzPartonHistory>("top::CalcTtzPartonHistory");
    else if (m_PartonScheme == "Tth") m_PartonHistory = std::make_unique<CalcTthPartonHistory>("top::CalcTthPartonHistory");
    else if (m_PartonScheme == "Tzq") m_PartonHistory = std::make_unique<CalcTzqPartonHistory>("top::CalcTzqPartonHistory");
    else if (m_PartonScheme == "Thq") m_PartonHistory = std::make_unique<CalcThqPartonHistory>("top::CalcThqPartonHistory");
    else if (m_PartonScheme == "Tqgamma") m_PartonHistory = std::make_unique<CalcTqgammaPartonHistory>("top::CalcTqgammaPartonHistory");
    else if (m_PartonScheme == "Ttgamma") m_PartonHistory = std::make_unique<CalcTtgammaPartonHistory>("top::CalcTtgammaPartonHistory");
    else if (m_PartonScheme == "HWW") m_PartonHistory = std::make_unique<CalcHWWPartonHistory>("top::CalcHWWPartonHistory");
    else if (m_PartonScheme == "HZZ") m_PartonHistory = std::make_unique<CalcHZZPartonHistory>("top::CalcHZZPartonHistory");
    else {
      ANA_MSG_ERROR("  ==> PartonScheme " << m_PartonScheme << " is not recognised! aborting.");
      return StatusCode::FAILURE;
    }
    ANA_CHECK(m_PartonHistory->setProperty("outputSGKey", "PartonHistory" + m_PartonScheme + "_NOSYS"));
    return StatusCode::SUCCESS;
  }

  StatusCode RunPartonHistoryAlg::execute() {
    ANA_CHECK(m_PartonHistory->execute());
    return StatusCode::SUCCESS;
  }

  StatusCode RunPartonHistoryAlg::finalize() {
    ANA_MSG_INFO("Finalizing PartonHistory");

    return StatusCode::SUCCESS;
  }

} // namespace top
