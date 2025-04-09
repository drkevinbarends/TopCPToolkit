#ifndef TOPCPTOOLKIT_KLFITTERFINALIZEOUTPUTALG_H_
#define TOPCPTOOLKIT_KLFITTERFINALIZEOUTPUTALG_H_

#include <AnaAlgorithm/AnaAlgorithm.h>
#include <SystematicsHandles/SysReadHandle.h>
#include <SystematicsHandles/SysWriteHandle.h>

#include "TopCPToolkit/KLFitterResultContainer.h"
#include "TopCPToolkit/KLFitterResultAuxContainer.h"

namespace top {
  class KLFitterFinalizeOutputAlg final : public EL::AnaAlgorithm {
  public:
    KLFitterFinalizeOutputAlg(const std::string &name, ISvcLocator *pSvcLocator);
    virtual StatusCode initialize() final;
    virtual StatusCode execute() final;
    virtual StatusCode finalize() final;

  private:
    CP::SysListHandle m_systematicsList {this};

    CP::SysReadHandle<xAOD::KLFitterResultContainer> m_inHandle {
      this, "resultContainerToCheck", "KLFitterResult_%SYS%", "the input KLFitterResultContainer to check for existence"
    };
    CP::SysWriteHandle<xAOD::KLFitterResultContainer, xAOD::KLFitterResultAuxContainer> m_outHandle {
      this, "resultContainerToWrite", "KLFitterResult_%SYS%", "the output KLFitterResultContainer"
    };
  };
}

#endif
