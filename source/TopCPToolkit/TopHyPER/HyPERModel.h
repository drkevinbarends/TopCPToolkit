#pragma once

#include <AsgTools/MessageCheckAsgTools.h>  // To access ANA_MSG
#include <math.h>

#include <iostream>

#include "TopCPToolkit/ONNXWrapper.h"

namespace top {
// @brief The enum class that holds the different topologies that the HyPER
// model can make predictions on.
enum class HyPERTopology {
  NotSelected = 0,
  TtbarLJets,
  TtbarAllHadronic,
  NumberOfTopologies
};
// @class HyPER
// @brief This class is in charge of loading the correct HyPER onnxruntime model
// based on the topology selected. This class also builds the graph object.
class HyPERModel : public ONNXWrapper {
 public:
  HyPERModel(const std::string& name, const std::string& onnxModelTrainedOnEven,
             const std::string& onnxModelTrainedOnOdd, HyPERTopology topology)
      : ONNXWrapper(name, {onnxModelTrainedOnEven, onnxModelTrainedOnOdd}),
        m_topology(topology) {}

  void setTopology(HyPERTopology topology) =
      delete;  // Not able to change the topology after construction
  HyPERTopology getTopology() const { return m_topology; }

  /**
   * @brief Model-specific output names
   * Output names varys according to the number of messgae-passing layers etc.
   * This must be defined for each and every topology.
   * @return std::vector<std::string> The output names of the model
   */
  virtual std::vector<std::string> getOutputNames() const = 0;

 protected:
  HyPERTopology m_topology{HyPERTopology::NotSelected};
};
}  // namespace top
