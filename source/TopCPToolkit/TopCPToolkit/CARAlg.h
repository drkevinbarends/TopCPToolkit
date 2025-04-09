#ifndef CARALG_H
#define CARALG_H

#include <AnaAlgorithm/AnaAlgorithm.h>
#include <SystematicsHandles/SysListHandle.h>
#include <SystematicsHandles/SysReadHandle.h>
#include <SystematicsHandles/SysWriteDecorHandle.h>
#include <SelectionHelpers/SysReadSelectionHandle.h>

#include <PartonHistory/CalcPartonHistory.h>
#include "PartonHistory/PartonHistoryUtils.h"

#include <xAODEventInfo/EventInfo.h>

#include "TF1.h"
#include "TRandom3.h"
#include "Math/Vector4D.h"
#include "Math/Vector3D.h"
#include "Math/GenVector/Boost.h"

using ROOT::Math::PtEtaPhiMVector;
using ROOT::Math::PxPyPzEVector;
using ROOT::Math::XYZVector;

namespace top {

  class CARAlg final : public EL::AnaAlgorithm {

    public:
      CARAlg(const std::string &name, ISvcLocator *pSvcLocator);
      virtual StatusCode initialize() override;
      virtual StatusCode execute() override;
      virtual StatusCode finalize() override;
      void prepareCARInputsHVV(const std::string &decaytype);
      void performCAR(ROOT::Math::PtEtaPhiMVector& PV1_L1,
		      ROOT::Math::PtEtaPhiMVector& PV1_L2,
		      ROOT::Math::PtEtaPhiMVector& PV2_L1,
		      ROOT::Math::PtEtaPhiMVector& PV2_L2);
      void SetSeed(unsigned int);

    private:
      float getRandPhi();
      float getRandCosth();
      void SetHBasis(const ROOT::Math::XYZVector& PV_CM3,
		     const ROOT::Math::XYZVector& Pp3,
		     ROOT::Math::XYZVector &V1_x,
		     ROOT::Math::XYZVector &V1_y,
		     ROOT::Math::XYZVector &V1_z,
		     ROOT::Math::XYZVector &V2_x,
		     ROOT::Math::XYZVector &V2_y,
		     ROOT::Math::XYZVector &V2_z);
      void SetTransformedMomentum(PxPyPzEVector& result,
				  const PtEtaPhiMVector& originalMomentum,
				  const XYZVector& Vx,
				  const XYZVector& Vy,
				  const XYZVector& Vz,
				  const double& thetaStar,
				  const double& phiStar,
				  const double& dir);

      int m_seed;
      std::string m_partonContainerName;
      std::string m_partonhistory;
      std::map<std::string, PtEtaPhiMVector> m_decay_map;
      PtEtaPhiMVector m_Hdecay1_decay1_CAR, m_Hdecay1_decay2_CAR, m_Hdecay2_decay1_CAR, m_Hdecay2_decay2_CAR;

      std::unique_ptr<TF1> m_PDF_costheta = std::make_unique<TF1>("CARPDF_costheta",
    "1/4*[1]*(1+x)**2 + [0]*(1-x**2) + 1/4*[2]*(1-x)**2", -1.0, 1.0);
      std::vector<float> m_Polarisations{1.0, 0.0, 0.0}; //F_{L}, F_{+}, F_{-}
      TRandom3 m_randGen;

      CP::SysListHandle m_systematicsList {this};

      CP::SysReadHandle<xAOD::EventInfo> m_eventInfoHandle {
	this, "eventInfo", "EventInfo", "the EventInfo container to decorate"
      };
      CP::SysReadHandle<xAOD::PartonHistory> m_partonHistoryHandle {
        this, "partonHistory", "", "the parton history to read from"
      };
  };
} // namsepace top

#endif
