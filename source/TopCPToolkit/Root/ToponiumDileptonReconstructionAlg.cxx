#include "TopCPToolkit/ToponiumDileptonReconstructionAlg.h"

namespace top {

using ROOT::Math::PtEtaPhiEVector;
using ROOT::Math::PtEtaPhiMVector;

StatusCode ToponiumDileptonReconstructionAlg::initialize() {

  ANA_CHECK(m_eventInfoHandle.initialize(m_systematicsList));
  ANA_CHECK(m_selection.initialize(m_systematicsList, m_eventInfoHandle,
                                   SG::AllowEmpty));

  ANA_CHECK(m_jetsHandle.initialize(m_systematicsList));
  ANA_CHECK(m_jetSelection.initialize(m_systematicsList, m_jetsHandle,
                                      SG::AllowEmpty));
  ANA_CHECK(m_electronsHandle.initialize(m_systematicsList));
  ANA_CHECK(m_electronSelection.initialize(m_systematicsList, m_electronsHandle,
                                           SG::AllowEmpty));
  ANA_CHECK(m_muonsHandle.initialize(m_systematicsList));
  ANA_CHECK(m_muonSelection.initialize(m_systematicsList, m_muonsHandle,
                                       SG::AllowEmpty));

  ANA_CHECK(m_metHandle.initialize(m_systematicsList));

  ANA_CHECK(m_top.initialize(m_systematicsList, m_eventInfoHandle));
  ANA_CHECK(m_tbar.initialize(m_systematicsList, m_eventInfoHandle));
  ANA_CHECK(m_top_decay.initialize(m_systematicsList, m_eventInfoHandle));
  ANA_CHECK(m_tbar_decay.initialize(m_systematicsList, m_eventInfoHandle));

  ANA_CHECK(m_systematicsList.initialize());

  m_bTagDecoAcc =
      std::make_unique<SG::AuxElement::ConstAccessor<char>>(m_bTagDecoration);

  return StatusCode::SUCCESS;
}

StatusCode ToponiumDileptonReconstructionAlg::execute() {

  for (const auto &sys : m_systematicsList.systematicsVector()) {
    // retrieve the EventInfo
    const xAOD::EventInfo *evtInfo = nullptr;
    ANA_CHECK(m_eventInfoHandle.retrieve(evtInfo, sys));

    // default-decorate
    PtEtaPhiMVector none(0, 0, 0, 0);
    m_top.set(*evtInfo, none, sys);
    m_tbar.set(*evtInfo, none, sys);
    m_top_decay.set(*evtInfo, none, sys);
    m_tbar_decay.set(*evtInfo, none, sys);

    // skip events not selected
    if (m_selection && !m_selection.getBool(*evtInfo, sys))
      continue;

    PtEtaPhiEVector lep1, lep2, bjet1, bjet2;
    int charge_lep1(0), charge_lep2(0);
    static const SG::AuxElement::ConstAccessor<float> acc_charge("charge");

    // load the candidate objects
    const xAOD::IParticleContainer *electrons = nullptr;
    const xAOD::IParticleContainer *muons = nullptr;
    const xAOD::IParticleContainer *jets = nullptr;
    const xAOD::MissingETContainer *met = nullptr;
    ANA_CHECK(m_electronsHandle.retrieve(electrons, sys));
    ANA_CHECK(m_muonsHandle.retrieve(muons, sys));
    ANA_CHECK(m_jetsHandle.retrieve(jets, sys));
    ANA_CHECK(m_metHandle.retrieve(met, sys));

    // count the number of leptons - we must have exactly 2!
    int nLeptons = 0;

    for (const xAOD::IParticle *el : *electrons) {
      if (m_electronSelection.getBool(*el, sys)) {
        if (lep1.Pt() < 1) {
          lep1 = GetPtEtaPhiE(el);
          charge_lep1 = acc_charge(*el);
          nLeptons++;
        } else {
          lep2 = GetPtEtaPhiE(el);
          charge_lep2 = acc_charge(*el);
          nLeptons++;
          break;
        }
      }
    }
    for (const xAOD::IParticle *mu : *muons) {
      if (m_muonSelection.getBool(*mu, sys)) {
        if (lep1.Pt() < 1) {
          lep1 = GetPtEtaPhiE(mu);
          charge_lep1 = acc_charge(*mu);
          nLeptons++;
        } else {
          lep2 = GetPtEtaPhiE(mu);
          charge_lep2 = acc_charge(*mu);
          nLeptons++;
          break;
        }
      }
    }
    for (const xAOD::IParticle *jet : *jets) {
      if (m_jetSelection.getBool(*jet, sys)) {
        const bool isTagged = (*m_bTagDecoAcc)(*jet);
        if (!isTagged)
          continue;
        if (bjet1.Pt() < 1) {
          bjet1 = GetPtEtaPhiE(jet);
        } else {
          bjet2 = GetPtEtaPhiE(jet);
          break;
        }
      }
    }

    if (nLeptons != 2) {
      ANA_MSG_ERROR(
          "Did not find two leptons in the event! You need to ensure exactly 2 "
          "opposite-sign leptons are available after preselection. Aborting.");
      return StatusCode::FAILURE;
    }
    if (bjet1.Pt() < 1 || bjet2.Pt() < 1) {
      ANA_MSG_ERROR(
          "Did not find two b-tagged jets in the event! you need to ensure at "
          "least 2 b-tagged jets are available after preselection. Aborting.");
      return StatusCode::FAILURE;
    }

    // sort the leptons and b-jets by pT
    if (lep1.Pt() < lep2.Pt()) {
      std::swap(lep1, lep2);
      std::swap(charge_lep1, charge_lep2);
    }
    if (bjet1.Pt() < bjet2.Pt()) {
      std::swap(bjet1, bjet2);
    }

    // get the MET
    if ((*met)[m_metTerm.value()] == nullptr) {
      ANA_MSG_ERROR("The MET term " << m_metTerm.value()
                                    << " doesn't exist! Aborting.");
      return StatusCode::FAILURE;
    }
    const float met_mpx = (*met)[m_metTerm.value()]->mpx();
    const float met_mpy = (*met)[m_metTerm.value()]->mpy();

    // compute the neutrino solutions
    std::vector<PtEtaPhiEVector> neutrinos =
        compute_neutrinos(lep1, lep2, bjet1, bjet2, met_mpx, met_mpy);

    // build the top and tbar, and assign them based on lepton charges
    PtEtaPhiMVector top, tbar, top_decay, tbar_decay;
    PtEtaPhiEVector t1 = lep1 + bjet1 + neutrinos[0];
    PtEtaPhiEVector t2 = lep2 + bjet2 + neutrinos[1];
    if (charge_lep1 > 0) {
      top = t1;
      tbar = t2;
      top_decay = lep1;
      tbar_decay = lep2;
    } else {
      top = t2;
      tbar = t1;
      top_decay = lep2;
      tbar_decay = lep1;
    }

    // save the necessary 4-vectors
    m_top.set(*evtInfo, top, sys);
    m_tbar.set(*evtInfo, tbar, sys);
    m_top_decay.set(*evtInfo, top_decay, sys);
    m_tbar_decay.set(*evtInfo, tbar_decay, sys);
  }
  return StatusCode::SUCCESS;
}

std::vector<PtEtaPhiEVector>
ToponiumDileptonReconstructionAlg::compute_neutrinos(const PtEtaPhiEVector &l1,
                                                     const PtEtaPhiEVector &l2,
                                                     const PtEtaPhiEVector &b1,
                                                     const PtEtaPhiEVector &b2,
                                                     const float mpx,
                                                     const float mpy) {

  constexpr float TOP_MASS = 172.5e3;

  // the two tops are assumed to be collinear
  const float DeltaPx = l1.Px() + b1.Px() - l2.Px() - b2.Px();
  const float DeltaPy = l1.Py() + b1.Py() - l2.Py() - b2.Py();

  // the neutrino transverse momenta are assumed to be equal to the MET
  const float nu1_px = (mpx - DeltaPx) / 2.0f;
  const float nu1_py = (mpy - DeltaPy) / 2.0f;
  const float nu2_px = nu1_px + DeltaPx;
  const float nu2_py = nu1_py + DeltaPy;

  // solve the longitudinal components of each neutrino separately
  const std::vector<float> vec_nu1_pz = compute_neutrino_pz(l1, nu1_px, nu1_py);
  const std::vector<float> vec_nu2_pz = compute_neutrino_pz(l2, nu2_px, nu2_py);

  // Helper lambda to select best pz based on top mass constraint
  auto select_best_pz =
      [&](const PtEtaPhiEVector &lep, const PtEtaPhiEVector &b, const float px,
          const float py, const std::vector<float> &pz_candidates) {
        if (pz_candidates.size() == 1)
          return pz_candidates[0];

        float pz1 = pz_candidates[0];
        float pz2 = pz_candidates[1];

        float energy1 = std::sqrt(px * px + py * py + pz1 * pz1);
        float energy2 = std::sqrt(px * px + py * py + pz2 * pz2);

        PtEtaPhiEVector nu_cand1, nu_cand2;
        nu_cand1.SetPxPyPzE(px, py, pz1, energy1);
        nu_cand2.SetPxPyPzE(px, py, pz2, energy2);

        // Compare mass difference to TOP_MASS and choose closest
        return (std::fabs((lep + b + nu_cand1).M() - TOP_MASS) <
                std::fabs((lep + b + nu_cand2).M() - TOP_MASS))
                   ? pz1
                   : pz2;
      };

  // Determine the best pz values
  float nu1_pz = select_best_pz(l1, b1, nu1_px, nu1_py, vec_nu1_pz);
  float nu2_pz = select_best_pz(l2, b2, nu2_px, nu2_py, vec_nu2_pz);

  // Construct final neutrino vectors
  PtEtaPhiEVector nu1, nu2;
  nu1.SetPxPyPzE(
      nu1_px, nu1_py, nu1_pz,
      std::sqrt(nu1_px * nu1_px + nu1_py * nu1_py + nu1_pz * nu1_pz));
  nu2.SetPxPyPzE(
      nu2_px, nu2_py, nu2_pz,
      std::sqrt(nu2_px * nu2_px + nu2_py * nu2_py + nu2_pz * nu2_pz));

  return {nu1, nu2};
}

std::vector<float> ToponiumDileptonReconstructionAlg::compute_neutrino_pz(
    const PtEtaPhiEVector &lep, const float nu_px, const float nu_py) {
  constexpr float W_MASS = 80.403e3;

  // the usual quadratic terms
  const double alpha = W_MASS * W_MASS - lep.M() * lep.M() +
                       2 * (lep.Px() * nu_px + lep.Py() * nu_py);
  const double A = lep.E() * lep.E() - lep.Pz() * lep.Pz();
  const double B = -alpha * lep.Pz();
  const double C =
      -alpha * alpha / 4. + lep.E() * lep.E() * (nu_px * nu_px + nu_py * nu_py);
  const double delta = B * B - 4 * A * C;

  // handle the various cases
  if (delta == 0) {
    return {static_cast<float>(-B / (2 * A))};
  } else if (delta > 0) {
    return {static_cast<float>((-B - std::sqrt(delta)) / (2 * A)),
            static_cast<float>((-B + std::sqrt(delta)) / (2 * A))};
  } else {
    // FIX-ME: we need to think of a smart way to handle negative discriminants!
    // For now, just take the real part
    return {static_cast<float>(-B / (2 * A))};
  }
}

}  // namespace top
