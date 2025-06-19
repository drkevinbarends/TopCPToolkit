#include "AthContainers/ConstDataVector.h"
#include "TopNuFlows/Dilepton.h"

namespace top {

TopNuFlowsDilepton::TopNuFlowsDilepton(const std::string &name,
                                       std::string model_even,
                                       std::string model_odd)
    : ONNXWrapper(name, {model_even, model_odd}) {}

void TopNuFlowsDilepton::Sample(
    ConstDataVector<xAOD::ElectronContainer> &electrons,
    ConstDataVector<xAOD::MuonContainer> &muons,
    ConstDataVector<xAOD::JetContainer> &jets, float met_mpx, float met_mpy,
    float met_sumet, unsigned long long eventNumber) {

  const SG::AuxElement::ConstAccessor<int> acc_btagging_quantile(
      "ftag_quantile_" + m_btagger + "_Continuous");
  // Clear all inputs + outputs to the model
  m_input_lep.clear();
  m_input_jet.clear();
  m_input_met.clear();
  m_input_misc.clear();
  m_nu_out.clear();
  m_loglik = 0.0;

  // Fill in the leptons
  for (const auto &l : electrons) {
    std::vector<float> v = {
        static_cast<float>(l->p4().Px() / 1000.0),
        static_cast<float>(l->p4().Py() / 1000.0),
        static_cast<float>(l->p4().Pz() / 1000.0),
        static_cast<float>(std::log(1.0 + l->p4().E() / 1000.0)),
        static_cast<float>(l->charge()),
        0.0};  // Flavor = 0 for electrons
    m_input_lep.push_back(v);
  }
  for (const auto &l : muons) {
    std::vector<float> v = {
        static_cast<float>(l->p4().Px() / 1000.0),
        static_cast<float>(l->p4().Py() / 1000.0),
        static_cast<float>(l->p4().Pz() / 1000.0),
        static_cast<float>(std::log(1.0 + l->p4().E() / 1000.0)),
        static_cast<float>(l->charge()),
        1.0};  // Flavor = 1 for muons
    m_input_lep.push_back(v);
  }

  // Fill in the jets
  for (const auto &j : jets) {
    std::vector<float> v = {
        static_cast<float>(j->p4().Px() / 1000.0),
        static_cast<float>(j->p4().Py() / 1000.0),
        static_cast<float>(j->p4().Pz() / 1000.0),
        static_cast<float>(std::log(1.0 + j->p4().E() / 1000.0)),
        static_cast<float>(std::log(1.0 + j->p4().M() / 1000.0)),
        static_cast<float>(acc_btagging_quantile(*j))};
    m_input_jet.push_back(v);
  }

  // MET information
  std::vector<float> met_v = {met_mpx / 1000.0f, met_mpy / 1000.0f,
                              met_sumet / 1000.0f};
  m_input_met.push_back(met_v);

  // Misc information (particle multiplicities)
  std::vector<float> misc_v = {static_cast<float>(electrons.size()),
                               static_cast<float>(muons.size()),
                               static_cast<float>(jets.size())};
  m_input_misc.push_back(misc_v);

  // Clear the inputs and outputs
  this->clearInputs();
  this->clearOutputs();

  // Add the inputs (appends to m_input_tensors)
  this->addInputs(m_input_jet);
  this->addInputs(m_input_lep);
  this->addInputs(m_input_met);
  this->addInputs(m_input_misc);

  // Select the appropriate network and run using ONNX
  m_model_idx = this->getSessionIndex(eventNumber);
  this->evaluate(m_model_idx);

  // Insert the neutrinos (first output)
  auto out_array = this->getOutputs<float>(0);
  int out_array_size = this->m_output_shapes[0][0];
  m_nu_out.insert(m_nu_out.end(), &out_array[0], &out_array[out_array_size]);

  // Insert the log-likelihood (second output)
  m_loglik = this->getOutputs<float>(1)[0];
}

}  // namespace top
