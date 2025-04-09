#ifndef TOPSPANET_TOPOLOGY_BASE_H
#define TOPSPANET_TOPOLOGY_BASE_H

// useful
#include <vector>
#include <iostream>
#include <math.h>

// Framework includes
#include <xAODEgamma/ElectronContainer.h>
#include <xAODMuon/MuonContainer.h>
#include <xAODJet/JetContainer.h>

#include "TopCPToolkit/ONNXWrapper.h"

namespace top {
  class TopSpaNetTopology: public ONNXWrapper {
  public:
    TopSpaNetTopology(const std::string& name, const std::string& model_even, const std::string& model_odd);

    virtual StatusCode execute() { return StatusCode::SUCCESS; }; // we would need to refactor things a bit in order to let the tool know about the selected objects, then we could use execute() instead of Predict()

    virtual void Predict(ConstDataVector<xAOD::ElectronContainer>& electrons,
			 ConstDataVector<xAOD::MuonContainer>& muons,
			 ConstDataVector<xAOD::JetContainer>& jets,
			 float met_met, float met_phi,
			 unsigned long long eventNumber) = 0;

    virtual std::vector<int> GetOutputIndices() = 0;
    virtual std::vector<float> GetOutputScores() = 0;
    virtual std::vector<float> GetRegressedValues() = 0;

    void setBtagger(const std::string& algorithm) { m_btagger = algorithm; };

    unsigned getSessionIndex(unsigned long long eventNumber) override;

  protected:
    std::string m_btagger;
    int m_NUM_FEATURES;
    int m_MAX_JETS;
    int m_NUM_JET_FEATURES;
    int m_MAX_LEPTONS;
    int m_NUM_LEPTON_FEATURES;
    int m_MAX_GLOBALS;
    int m_NUM_GLOBAL_FEATURES;

  };
} // namespace top

#endif

