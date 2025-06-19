#pragma once

#include <string>
#include <vector>

#include "TopHyPER/HyPERParser.h"

namespace top {
class HyPERTtbarLJetsParser : public HyPERParser {
 public:
  HyPERTtbarLJetsParser() = default;
  ~HyPERTtbarLJetsParser() = default;

  void buildONNXInputs(const HyPERGraph& hyperGraph,
                       HyPERModel& hyperModel) override;

  void getONNXOutputs(HyPERModel& hyperModel) override;

  void reconstructOutputs(const HyPERGraph& hyperGraph) override;

  std::vector<std::string> getLabels() override { return m_reco_labels_out; }
  std::vector<float> getScores() override { return m_reco_scores_out; }
  std::vector<std::vector<int64_t>> getIndices() override {
    return m_reco_indices_out;
  }
  std::vector<std::vector<int64_t>> getIds() override { return m_reco_ids_out; }

  void clear() override;
  virtual void printOutputsForValidation() const override;

 public:
  // Outputs
  std::vector<int64_t> m_edge_out_shape{};
  std::vector<int64_t> m_hyperedge_out_shape{};
  std::vector<std::vector<float>> m_edge_out{};
  std::vector<std::vector<float>> m_hyperedge_out{};

  std::vector<std::vector<int64_t>> m_reco_indices_out = {};
  std::vector<float> m_reco_scores_out = {};
  std::vector<std::string> m_reco_labels_out = {};
  std::vector<std::vector<int64_t>> m_reco_ids_out = {};

 private:
  // Input tensors for the ONNX model including shapes and names.
  std::vector<int64_t> m_l_edge_index_ = {{}, {}};
  std::vector<int64_t> m_l_edge_index_shape;
  const std::string m_l_edge_index_name = "edge_index";
  std::vector<float> m_l_edge_attr_s_ = {};
  std::vector<int64_t> m_l_edge_attr_s_shape;
  const std::string m_l_edge_attr_s_name = "edge_attr_s";
  std::vector<float> m_l_x_s_ = {};
  std::vector<int64_t> m_l_x_s_shape;
  const std::string m_l_x_s_name = "x_s";
  std::vector<int64_t> m_l_batch_ = {};
  std::vector<int64_t> m_l_batch_shape;
  const std::string m_l_batch_name = "batch";
  std::vector<int64_t> m_l_edge_index_h_ = {};
  std::vector<int64_t> m_l_edge_index_h_shape;
  const std::string m_l_edge_index_h_name = "edge_index_h";
  std::vector<int64_t> m_l_edge_index_h_batch_ = {};
  std::vector<int64_t> m_l_edge_index_h_batch_shape;
  const std::string m_l_edge_index_h_batch_name = "edge_index_h_batch";
  std::vector<float> m_l_u_s_;
  std::vector<int64_t> m_l_u_s_shape;
  const std::string m_l_u_s_name = "u_s";
};

}  // namespace top
