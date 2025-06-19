#include <AsgMessaging/MessageCheck.h>  // To access ANA_MSG

#include <string>

#include "TopCPToolkit/GraphBase.h"
#include "TopCPToolkit/RunHyPERAlg.h"
#include "TopHyPER/HyPERGraph.h"
#include "TopHyPER/HyPERUtils.h"

namespace top {
void HyPERGraph::addNode(const Features& attributes) {
  top::Node newNode(attributes, m_nNodes);
  m_nodes.push_back(newNode);
  m_nNodes += 1;
}

void HyPERGraph::addEdge(int64_t source, int64_t target,
                         const Features& attributes) {
  top::Edge newEdge(source, target, attributes);
  m_edges.push_back(newEdge);
  m_nEdges += 1;
}

void HyPERGraph::addGlobal(const Features& attributes) {
  m_global = top::Global(attributes);
}

void HyPERGraph::buildEdges() {
  if (m_nNodes < 2) {
    return;  // There are no edges when the number of nodes is less than 2.
  }
  std::vector<int64_t> edgeIndices = range(m_nNodes);
  for (const auto& index1 : edgeIndices) {
    for (const auto& index2 : edgeIndices) {
      if (index1 != index2)
        m_edgeIndices.push_back(std::make_pair(index1, index2));
    }
  }
}

void HyPERGraph::buildHyperEdges(int64_t order) {
  m_hyperEdgeOrder = order;
  if (m_nNodes < order) {
    return;  // There are no hyperedges when the number of nodes is less than
             // the hyperedge order.
  }
  std::vector<int64_t> edgeIndices = range(m_nNodes);
  m_hyperEdgeIndices = buildCombinations(edgeIndices, order);
  m_nHyperEdges = m_hyperEdgeIndices.size();
}

void HyPERGraph::printGraph() const {
  using namespace asg::msgUserCode;
  setMsgLevel(MSG::INFO);
  ANA_MSG_INFO("Printing graph...");
  // Print the nodes
  for (const auto& node : m_nodes) {
    ANA_MSG_INFO("Node Index: " << node.getNodeIndex());
    for (const auto& feat : node.getFeatures()) {
      ANA_MSG_INFO("Feature:  " << feat);
    }
  }
  // Print the edges
  for (const auto& edge : m_edges) {
    ANA_MSG_INFO("Edge Index: " << edge.getIndices().first << " -> "
                                << edge.getIndices().second);
    for (const auto& feat : edge.getFeatures()) {
      ANA_MSG_INFO("Feature:  " << feat);
    }
  }
  // Print the global
  ANA_MSG_INFO("Global Features: ");
  for (const auto& feat : m_global.getFeatures()) {
    ANA_MSG_INFO("Feature:  " << feat);
  }
  // Print the hyperedges
  for (const auto& hyperEdge : m_hyperEdgeIndices) {
    std::string indices{""};
    for (const auto& index : hyperEdge) {
      indices += std::to_string(index) + " ";
    }
    ANA_MSG_INFO("HyperEdge indices: " << indices);
  }
  ANA_MSG_INFO("Finished graph printing...");
}

void HyPERGraph::printGraphInputsForValidation() const {
  using namespace asg::msgUserCode;
  setMsgLevel(MSG::INFO);
  ANA_MSG_INFO("Printing graph inputs in validation mode...");

  // Print the nodes
  ANA_MSG_INFO("NODES:");
  for (const auto& node : m_nodes) {
    std::string row = "[";
    for (const auto& feat : node.getFeatures()) {
      row += std::to_string(feat) + ", ";
    }
    row += "],";
    ANA_MSG_INFO(row);
  }
  // Print the edges
  ANA_MSG_INFO("EDGES:");
  for (const auto& edge : m_edges) {
    std::string row = "[";
    for (const auto& feat : edge.getFeatures()) {
      row += std::to_string(feat) + ", ";
    }
    row += "],";
    ANA_MSG_INFO(row);
  }
  // Print the edge indices
  ANA_MSG_INFO("EDGE INDICES:");
  std::string row1 = "[";
  std::string row2 = "[";
  for (const auto& edge : m_edges) {
    row1 += std::to_string(edge.getIndices().first) + ", ";
    row2 += std::to_string(edge.getIndices().second) + ", ";
  }
  row1 += "],";
  row2 += "],";
  ANA_MSG_INFO(row1);
  ANA_MSG_INFO(row2);
  // Print the globals
  ANA_MSG_INFO("GLOBAL:");
  std::string row = "[[";
  for (const auto& feat : m_global.getFeatures()) {
    row += std::to_string(feat) + ", ";
  }
  row += "]],";
  ANA_MSG_INFO(row);
  // Print batch
  ANA_MSG_INFO("BATCH:");
  row = "[[";
  for (auto i{0}; i < this->nNodes(); i++) {
    row += "0, ";
  }
  row += "]],";
  ANA_MSG_INFO(row);
  // Print the hyperedges
  ANA_MSG_INFO("HYPEREDGE INDICES:");
  row1 = "[";
  row2 = "[";
  std::string row3 = "[";
  for (const auto& hyperEdge : m_hyperEdgeIndices) {
    row1 += std::to_string(hyperEdge[0]) + ", ";
    row2 += std::to_string(hyperEdge[1]) + ", ";
    row3 += std::to_string(hyperEdge[2]) + ", ";
  }
  row1 += "],";
  row2 += "],";
  row3 += "],";
  ANA_MSG_INFO(row1);
  ANA_MSG_INFO(row2);
  ANA_MSG_INFO(row3);
  // Print batch hyperedges
  ANA_MSG_INFO("BATCH HYPEREDGES:");
  row = "[[";
  for (auto i{0}; i < this->nHyperEdges(); i++) {
    row += "0, ";
  }
  row += "]],";
  ANA_MSG_INFO(row);

  ANA_MSG_INFO("Finished printing graph inputs for validation...");
}

void HyPERGraph::clearGraph() {
  m_nNodes = 0;
  m_nEdges = 0;
  m_nodes.clear();
  m_edges.clear();
  m_global = {};
  m_edgeIndices = {};
  m_hyperEdgeIndices = {};
  m_hyperEdgeOrder = 0;
  m_nHyperEdges = 0;
}

}  // namespace top
