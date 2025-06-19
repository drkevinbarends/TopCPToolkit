#ifndef TOPONIUM_DILEPTON_RECONSTRUCTIONALG_H
#define TOPONIUM_DILEPTON_RECONSTRUCTIONALG_H

// Algorithm includes
#include <AnaAlgorithm/AnaAlgorithm.h>
#include <AsgTools/PropertyWrapper.h>
#include <SelectionHelpers/SysReadSelectionHandle.h>
#include <SystematicsHandles/SysReadHandle.h>
#include <SystematicsHandles/SysWriteDecorHandle.h>

// Framework includes
#include <xAODBase/IParticle.h>
#include <xAODBase/IParticleContainer.h>
#include <xAODEventInfo/EventInfo.h>
#include <xAODMissingET/MissingETContainer.h>

#include "VectorHelpers/LorentzHelper.h"

namespace top {
using ROOT::Math::PtEtaPhiEVector;
using ROOT::Math::PtEtaPhiMVector;

class ToponiumDileptonReconstructionAlg final : public EL::AnaAlgorithm {

 public:
  /// \brief the standard constructor
  using EL::AnaAlgorithm::AnaAlgorithm;
  virtual StatusCode initialize() override;
  virtual StatusCode execute() override;

 private:
  Gaudi::Property<std::string> m_bTagDecoration{this, "bTagDecoration", "",
                                                "the b-tagging selection flag"};
  Gaudi::Property<std::string> m_metTerm{this, "metTerm", "",
                                         "the MET term name to use"};
  std::unique_ptr<SG::AuxElement::ConstAccessor<char>> m_bTagDecoAcc;

  CP::SysListHandle m_systematicsList{this};

  CP::SysReadHandle<xAOD::IParticleContainer> m_jetsHandle{
      this, "jets", "", "the jet container to use"};
  CP::SysReadSelectionHandle m_jetSelection{this, "jetSelection", "",
                                            "the selection on the input jets"};
  CP::SysReadHandle<xAOD::IParticleContainer> m_electronsHandle{
      this, "electrons", "", "the electron container to use"};
  CP::SysReadSelectionHandle m_electronSelection{
      this, "electronSelection", "", "the selection on the input electrons"};
  CP::SysReadHandle<xAOD::IParticleContainer> m_muonsHandle{
      this, "muons", "", "the muon container to use"};
  CP::SysReadSelectionHandle m_muonSelection{
      this, "muonSelection", "", "the selection on the input muons"};

  CP::SysReadHandle<xAOD::MissingETContainer> m_metHandle{
      this, "met", "", "the MET container to use"};
  CP::SysReadHandle<xAOD::EventInfo> m_eventInfoHandle{
      this, "eventInfo", "EventInfo", "the EventInfo container to use"};
  CP::SysReadSelectionHandle m_selection{this, "selection", "",
                                         "the event selection to apply"};

  CP::SysWriteDecorHandle<PtEtaPhiMVector> m_top{this, "toponium_top",
                                                 "toponium_top_4vect_%SYS%",
                                                 "the 4-vector of the top"};
  CP::SysWriteDecorHandle<PtEtaPhiMVector> m_tbar{
      this, "toponium_tbar", "toponium_tbar_4vect_%SYS%",
      "the 4-vector of the anti-top"};
  CP::SysWriteDecorHandle<PtEtaPhiMVector> m_top_decay{
      this, "toponium_top_decay", "toponium_top_decay_4vect_%SYS%",
      "the 4-vector of the decay product (spin analyser) from the top"};
  CP::SysWriteDecorHandle<PtEtaPhiMVector> m_tbar_decay{
      this, "toponium_tbar_decay", "toponium_tbar_decay_4vect_%SYS%",
      "the 4-vector of the decay product (spin analyser) from the anti-top"};

  std::vector<PtEtaPhiEVector> compute_neutrinos(const PtEtaPhiEVector&,
                                                 const PtEtaPhiEVector&,
                                                 const PtEtaPhiEVector&,
                                                 const PtEtaPhiEVector&,
                                                 const float, const float);
  std::vector<float> compute_neutrino_pz(const PtEtaPhiEVector&, const float,
                                         const float);
};

}  // namespace top

#endif
