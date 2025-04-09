#ifndef TOP_SPIN_DENSITY_MATRIX__ALG_H
#define TOP_SPIN_DENSITY_MATRIX__ALG_H

#include <AnaAlgorithm/AnaAlgorithm.h>
#include <SystematicsHandles/SysListHandle.h>
#include <SystematicsHandles/SysReadHandle.h>
#include <SystematicsHandles/SysReadDecorHandle.h>
#include <SystematicsHandles/SysWriteDecorHandle.h>
#include <SelectionHelpers/SysReadSelectionHandle.h>

#include <xAODEventInfo/EventInfo.h>
#include "Math/Vector4D.h"

using ROOT::Math::PtEtaPhiMVector;

namespace top {

  class TopSpinDensityMatrixAlg final : public EL::AnaAlgorithm {

  public:
    TopSpinDensityMatrixAlg(const std::string &name, ISvcLocator *pSvcLocator);
    virtual StatusCode initialize() override;
    virtual StatusCode execute() override;

  private:
    CP::SysListHandle m_systematicsList {this};
    CP::SysReadHandle<xAOD::EventInfo> m_eventInfoHandle {
      this, "eventInfo", "EventInfo", "the EventInfo container to read from"
    };
    CP::SysReadSelectionHandle m_selection {
      this, "eventSelection", "SetMe", "the event selection to apply"
    };
    CP::SysReadDecorHandle<PtEtaPhiMVector> m_top {
      this, "top_4vec", "", "the 4-vector of the top"
    };
    CP::SysReadDecorHandle<PtEtaPhiMVector> m_tbar {
      this, "tbar_4vec", "", "the 4-vector of the anti-top"
    };
    CP::SysReadDecorHandle<PtEtaPhiMVector> m_top_analyser {
      this, "top_analyser_4vec", "", "the 4-vector of the spin analyser from the top"
    };
    CP::SysReadDecorHandle<PtEtaPhiMVector> m_tbar_analyser {
      this, "tbar_analyser_4vec", "", "the 4-vector of the spin analyser from the anti-top"
    };
    CP::SysWriteDecorHandle<float> m_cos_theta_helicity_p {
      this, "ckp", "", "cos(theta)+ along the helicity axis"
    };
    CP::SysWriteDecorHandle<float> m_cos_theta_helicity_m {
      this, "ckm", "", "cos(theta)- along the helicity axis"
    };
    CP::SysWriteDecorHandle<float> m_cos_theta_transverse_p {
      this, "cnp", "", "cos(theta)+ along the transverse axis"
    };
    CP::SysWriteDecorHandle<float> m_cos_theta_transverse_m {
      this, "cnm", "", "cos(theta)- along the transverse axis"
    };
    CP::SysWriteDecorHandle<float> m_cos_theta_raxis_p {
      this, "crp", "", "cos(theta)+ along the r-axis"
    };
    CP::SysWriteDecorHandle<float> m_cos_theta_raxis_m {
      this, "crm", "", "cos(theta)- along the r-axis"
    };
    CP::SysWriteDecorHandle<float> m_cos_phi {
      this, "D", "", "cos(phi) between the two spin analysers"
    };
    CP::SysWriteDecorHandle<float> m_cos_theta_x_p {
      this, "cxp", "", "cos(theta)+ along the x-axis"
    };  
    CP::SysWriteDecorHandle<float> m_cos_theta_x_m {
      this, "cxm", "", "cos(theta)- along the x-axis"
    };        
    CP::SysWriteDecorHandle<float> m_cos_theta_y_p {
      this, "cyp", "", "cos(theta)+ along the y-axis"
    };  
    CP::SysWriteDecorHandle<float> m_cos_theta_y_m {
      this, "cym", "", "cos(theta)- along the y-axis"
    };        
    CP::SysWriteDecorHandle<float> m_cos_theta_z_p {
      this, "czp", "", "cos(theta)+ along the z-axis"
    };  
    CP::SysWriteDecorHandle<float> m_cos_theta_z_m {
      this, "czm", "", "cos(theta)- along the z-axis"
    };                
  };

} // namespace top

#endif
