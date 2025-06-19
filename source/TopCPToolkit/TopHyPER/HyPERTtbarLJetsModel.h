#pragma once

#include <string>
#include <vector>

// HyPER includes
#include "TopHyPER/HyPERModel.h"

namespace top {
/**
 * @class HyPERTtbarAllHadronicModel
 * @brief This class is in charge of loading the correct HyPER model based on
 * the Ttbar all hadronic topology.
 */
class HyPERTtbarLJetsModel : public HyPERModel {
 public:
  HyPERTtbarLJetsModel(const std::string& name,
                       const std::string& onnxModelTrainedOnEven,
                       const std::string& onnxModelTrainedOnOdd)
      : HyPERModel(name, onnxModelTrainedOnEven, onnxModelTrainedOnOdd,
                   HyPERTopology::TtbarLJets) {}
  std::vector<std::string> getOutputNames() const override {
    return std::vector<std::string>{"sigmoid_1", "sigmoid"};
  }
};
}  // namespace top
