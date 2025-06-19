#ifndef ZTOPOLOGY_H
#define ZTOPOLOGY_H

#include "TopCPToolkit/BaseTopology.h"
#include "VectorHelpers/AccessorHelpers.h"

namespace top {

class ZTopology : public virtual BaseTopology {
 public:
  ZTopology();
  virtual ~ZTopology() = default;

 private:
  std::string m_up_source;
  std::string m_down_source;
  std::string m_lep_source;
  bool m_Z_isHadronic = false;
  bool m_is_up_type;
  int m_lepton_pdgId = 0;
  int m_MC_Zdecay1_afterFSR_pdgId = 0;
  int m_MC_Zdecay2_afterFSR_pdgId = 0;

  PartonAccessor m_partonAcc;

 protected:
  virtual bool checkTopology() override;

  virtual bool fillTopology(std::vector<TruthObject>& truth_jets,
                            std::vector<TruthObject>& truth_electrons,
                            std::vector<TruthObject>& truth_muons) override;
};

}  // namespace top

#endif  // ZTOPOLOGY_H
