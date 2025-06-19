#ifndef TOPHYPER_HYPER_GRAPH_H
#define TOPHYPER_HYPER_GRAPH_H

#include <math.h>

#include <iostream>
#include <list>
#include <map>
#include <vector>

#include "TopCPToolkit/GraphBase.h"

namespace top {
// @class HyPERGraph
// @brief This class is in charge of storing the graph structure and features of
// the HyPER model. It inherits from the GraphBase class.
class HyPERGraph : public GraphBase {
  using HyperEdgeIndex = std::vector<int64_t>;

 public:
  HyPERGraph() : GraphBase() {};

  virtual void addNode(const Features& attributes) override;
  virtual void addEdge(int64_t source, int64_t target,
                       const Features& attributes) override;
  virtual void addGlobal(const Features& attributes) override;

  void buildEdges();
  void buildHyperEdges(int64_t order);
  const std::vector<EdgeIndex>& getEdgeIndices() const { return m_edgeIndices; }
  HyperEdgeIndex getHyperEdgeIndices(std::size_t index) const {
    return m_hyperEdgeIndices[index];
  }
  int64_t nHyperEdges() const { return m_nHyperEdges; }
  int64_t hyperEdgeOrder() const { return m_hyperEdgeOrder; }

  void printGraph() const;
  void printGraphInputsForValidation() const;
  void clearGraph();

 private:
  std::vector<EdgeIndex> m_edgeIndices = {};
  std::vector<HyperEdgeIndex> m_hyperEdgeIndices = {};
  int64_t m_nHyperEdges = 0;
  int64_t m_hyperEdgeOrder = 0;
};
}  // namespace top

#endif
