#ifndef TTZTOPOLOGY_H
#define TTZTOPOLOGY_H

#include "TopCPToolkit/TtbarTopology.h"
#include "TopCPToolkit/ZTopology.h"
#include "VectorHelpers/AccessorHelpers.h"

namespace top {

class TtzTopology : public TtbarTopology, public ZTopology {
 public:
  TtzTopology();
  virtual ~TtzTopology() = default;

 private:
 protected:
  virtual bool checkTopology() override;

  virtual bool fillTopology(std::vector<TruthObject>& truth_jets,
                            std::vector<TruthObject>& truth_electrons,
                            std::vector<TruthObject>& truth_muons) override;
};

}  // namespace top

#endif  // TTZTOPOLOGY_H
