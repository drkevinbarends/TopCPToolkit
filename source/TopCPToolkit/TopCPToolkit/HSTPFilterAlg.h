/*
    Copyright (C) 2002-2025 CERN for the benefit of the ATLAS collaboration
    Implemented by Zackary Alegria following instructions from the JETM
    twiki recommendation on Dijet sample normalization
    https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/JetEtMissMCSamples#Dijet_normalization_procedure_HS
*/

#ifndef HSTPFILTERALG_H
#define HSTPFILTERALG_H

#include <AnaAlgorithm/AnaAlgorithm.h>
#include <AsgTools/PropertyWrapper.h>
#include <EventBookkeeperTools/FilterReporterParams.h>

namespace CP {
class HSTPFilterAlg final : public EL::AnaAlgorithm {
 public:
  HSTPFilterAlg(const std::string& name, ISvcLocator* pSvcLocator);
  virtual StatusCode initialize() override;
  virtual StatusCode execute() override;
  virtual StatusCode finalize() override;

 private:
  Gaudi::Property<std::string> m_truthHSCollection{
      this, "truthHSCollection", "", "The name of the truth HS jet collection"};

  Gaudi::Property<std::string> m_truthPUCollection{
      this, "truthPUCollection", "", "The name of the truth PU jet collection"};

  FilterReporterParams m_filterParams{this, "HSTPFilterSelection",
                                      "HS softer than PU event filter"};
};
}  // namespace CP

#endif
