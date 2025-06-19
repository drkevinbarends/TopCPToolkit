#ifndef TTBARTOPOLOGY_H
#define TTBARTOPOLOGY_H

#include "TopCPToolkit/BaseTopology.h"
#include "VectorHelpers/AccessorHelpers.h"

namespace top {

class TtbarTopology : public virtual BaseTopology {
 public:
  TtbarTopology();
  virtual ~TtbarTopology() = default;

 private:
  std::string m_up_source;
  std::string m_down_source;
  std::string m_lep_source;
  bool m_t_isHadronic = false;
  bool m_tbar_isHadronic = false;
  bool m_is_up_type;
  int m_lepton_pdgId = 0;
  int m_MC_Wdecay1_afterFSR_from_t_pdgId = 0;
  int m_MC_Wdecay2_afterFSR_from_t_pdgId = 0;
  int m_MC_Wdecay1_afterFSR_from_tbar_pdgId = 0;
  int m_MC_Wdecay2_afterFSR_from_tbar_pdgId = 0;
  PtEtaPhiMVector m_b, m_bbar;

  PartonAccessor m_partonAcc;

 protected:
  virtual bool checkTopology() override;

  virtual bool fillTopology(std::vector<TruthObject>& truth_jets,
                            std::vector<TruthObject>& truth_electrons,
                            std::vector<TruthObject>& truth_muons) override;
};

}  // namespace top

#endif  // TTBARTOPOLOGY_H
