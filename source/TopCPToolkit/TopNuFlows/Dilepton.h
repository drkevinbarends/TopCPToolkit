#ifndef TOPNUFlLOWS_DILEPTON
#define TOPNUFlLOWS_DILEPTON

// Standard includes
#include <TLorentzVector.h>
#include <math.h>

#include <iostream>
#include <numeric>
#include <vector>

// Framework includes
#include <xAODEgamma/ElectronContainer.h>
#include <xAODJet/JetContainer.h>
#include <xAODMuon/MuonContainer.h>

// Parent class
#include "TopCPToolkit/ONNXWrapper.h"

using std::string;
using std::vector;

namespace top {

class TopNuFlowsDilepton : public ONNXWrapper {
  using ONNXWrapper::ONNXWrapper;

 public:
  // Vectors for the input and outputs to the model
  float m_loglik{0};
  vector<float> m_nu_out;
  vector<vector<float>> m_input_lep;
  vector<vector<float>> m_input_jet;
  vector<vector<float>> m_input_met;
  vector<vector<float>> m_input_misc;

  TopNuFlowsDilepton(const string &name, string model_even, string model_odd);

  // Placeholder, simpler to use custon Sample method
  virtual StatusCode execute() { return StatusCode::SUCCESS; };

  // Does not return, sets the internal state
  virtual void Sample(ConstDataVector<xAOD::ElectronContainer> &electrons,
                      ConstDataVector<xAOD::MuonContainer> &muons,
                      ConstDataVector<xAOD::JetContainer> &jets, float met_mpx,
                      float met_mpy, float met_sumet,
                      unsigned long long eventNumber);

  // The methods to set/return the values from the internal state
  virtual vector<float> GetSample() { return m_nu_out; };
  void setBtagger(const string &algorithm) { m_btagger = algorithm; };

  // Variable to work out which model to use
  unsigned m_model_idx{0};

 protected:
  string m_btagger;
};

}  // namespace top

#endif
