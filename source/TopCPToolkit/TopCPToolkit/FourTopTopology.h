#ifndef FOURTOPTOPOLOGY_H
#define FOURTOPTOPOLOGY_H

#include "AthContainers/AuxElement.h"
#include "TopCPToolkit/BaseTopology.h"
#include "VectorHelpers/AccessorHelpers.h"

namespace top {

class FourTopTopology : public BaseTopology {
 public:
  FourTopTopology();
  virtual ~FourTopTopology() = default;

 private:
  bool m_t1_isHadronic = false;
  bool m_t2_isHadronic = false;
  bool m_tbar1_isHadronic = false;
  bool m_tbar2_isHadronic = false;
  bool m_is_up_type;
  std::string m_up_source;
  std::string m_down_source;
  std::string m_lep_source;
  int m_lepton_pdgId = 0;
  int m_MC_Wdecay1_afterFSR_from_t1_pdgId = 0;
  int m_MC_Wdecay2_afterFSR_from_t1_pdgId = 0;
  int m_MC_Wdecay1_afterFSR_from_tbar1_pdgId = 0;
  int m_MC_Wdecay2_afterFSR_from_tbar1_pdgId = 0;
  int m_MC_Wdecay1_afterFSR_from_t2_pdgId = 0;
  int m_MC_Wdecay2_afterFSR_from_t2_pdgId = 0;
  int m_MC_Wdecay1_afterFSR_from_tbar2_pdgId = 0;
  int m_MC_Wdecay2_afterFSR_from_tbar2_pdgId = 0;

  PtEtaPhiMVector m_Wp1_up, m_Wp1_down, m_Wm1_up, m_Wm1_down, m_b1, m_bbar1;
  PtEtaPhiMVector m_Wp2_up, m_Wp2_down, m_Wm2_up, m_Wm2_down, m_b2, m_bbar2;

  PartonAccessor m_partonAcc;

 protected:
  virtual bool checkTopology() override;

  virtual bool fillTopology(std::vector<TruthObject>& truth_jets,
                            std::vector<TruthObject>& truth_electrons,
                            std::vector<TruthObject>& truth_muons) override;
};

}  // namespace top

#endif  // FOURTOPTOPOLOGY_H
