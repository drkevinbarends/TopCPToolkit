#ifndef TTHTOPOLOGY_H
#define TTHTOPOLOGY_H

#include "TopCPToolkit/BaseTopology.h"
#include "VectorHelpers/AccessorHelpers.h"

namespace top {

class TthTopology : public BaseTopology {
 public:
  TthTopology();
  virtual ~TthTopology() = default;

 private:
  std::string m_up_source;
  std::string m_down_source;
  std::string m_lep_source;
  bool m_t_isHadronic = false;
  bool m_tbar_isHadronic = false;
  bool m_is_up_type;
  bool m_Hdecay1_isHadronic = false;
  bool m_Hdecay2_isHadronic = false;
  bool m_Hdecay1_decay1_isHadronic = false;
  bool m_Hdecay1_decay2_isHadronic = false;
  bool m_Hdecay2_decay1_isHadronic = false;
  bool m_Hdecay2_decay2_isHadronic = false;
  int m_lepton_pdgId = 0;
  int m_MC_Wdecay1_afterFSR_from_t_pdgId = 0;
  int m_MC_Wdecay2_afterFSR_from_t_pdgId = 0;
  int m_MC_Wdecay1_afterFSR_from_tbar_pdgId = 0;
  int m_MC_Wdecay2_afterFSR_from_tbar_pdgId = 0;
  int m_MC_Hdecay1_afterFSR_pdgId = 0;
  int m_MC_Hdecay2_afterFSR_pdgId = 0;
  int m_MC_Hdecay1_decay1_afterFSR_pdgId = 0;
  int m_MC_Hdecay1_decay2_afterFSR_pdgId = 0;
  int m_MC_Hdecay2_decay2_afterFSR_pdgId = 0;
  int m_MC_Hdecay2_decay1_afterFSR_pdgId = 0;
  PtEtaPhiMVector m_b, m_bbar;
  PtEtaPhiMVector m_Hdecay1, m_Hdecay2;
  PtEtaPhiMVector m_Hdecay1_decay1, m_Hdecay1_decay2;
  PtEtaPhiMVector m_Hdecay2_decay1, m_Hdecay2_decay2;

  PartonAccessor m_partonAcc;

 protected:
  virtual bool checkTopology() override;

  virtual bool fillTopology(std::vector<TruthObject>& truth_jets,
                            std::vector<TruthObject>& truth_electrons,
                            std::vector<TruthObject>& truth_muons) override;
};

}  // namespace top

#endif  // TTHTOPOLOGY_H
