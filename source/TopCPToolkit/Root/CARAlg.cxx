#include "TopCPToolkit/CARAlg.h"

#include <TRandom3.h>

#include <cmath>

using ROOT::Math::Boost;
using ROOT::Math::PtEtaPhiMVector;
using ROOT::Math::PxPyPzEVector;
using ROOT::Math::XYZVector;

// This code implements the "custom angle replacement" (CAR)
// [algorithm](https://journals.aps.org/prd/pdf/10.1103/PhysRevD.106.115021). An
// example can be found
// [here](https://journals.aps.org/prd/pdf/10.1103/PhysRevD.107.076016).

namespace top {

CARAlg::CARAlg(const std::string& name, ISvcLocator* pSvcLocator)
    : EL::AnaAlgorithm(name, pSvcLocator) {
  declareProperty("history", m_partonhistory, "Name of the parton history");
  declareProperty("partonContainerName", m_partonContainerName,
                  "Name of the parton container");
  declareProperty(
      "seed", m_seed,
      "Seed (int) for the random number generator for drawing angles");
}

StatusCode CARAlg::initialize() {
  ANA_MSG_INFO("Initializing CAR algorithm " << name());
  ANA_CHECK(m_eventInfoHandle.initialize(m_systematicsList));

  if (m_partonhistory == "HWW") {
    ANA_MSG_INFO("Using the following W-Polarisation parameters:");
    ANA_MSG_INFO("Parameter 0: " << m_Polarisations[0]);
    ANA_MSG_INFO("Parameter 1: " << m_Polarisations[1]);
    ANA_MSG_INFO("Parameter 2: " << m_Polarisations[2]);
    m_PDF_costheta->SetParameter(0, m_Polarisations[0]);
    m_PDF_costheta->SetParameter(1, m_Polarisations[1]);
    m_PDF_costheta->SetParameter(2, m_Polarisations[2]);
    initializeHVVdecorators();
    initializeHVVaccessors();
  } else if (m_partonhistory == "HZZ") {
    ANA_MSG_INFO("Using the following Z-Polarisation parameters:");
    ANA_MSG_INFO("Parameter 0: " << m_Polarisations[0]);
    ANA_MSG_INFO("Parameter 1: " << m_Polarisations[1]);
    ANA_MSG_INFO("Parameter 2: " << m_Polarisations[2]);
    m_PDF_costheta->SetParameter(0, m_Polarisations[0]);
    m_PDF_costheta->SetParameter(1, m_Polarisations[1]);
    m_PDF_costheta->SetParameter(2, m_Polarisations[2]);
    initializeHVVdecorators();
    initializeHVVaccessors();
  }

  SetSeed(m_seed);
  ANA_CHECK(m_systematicsList.initialize());

  return StatusCode::SUCCESS;
}

void CARAlg::initializeHVVaccessors() {
  m_CAR_acc.initializePtEtaPhiMAccessor("MC_Hdecay1_decay1_beforeFSR");
  m_CAR_acc.initializePtEtaPhiMAccessor("MC_Hdecay1_decay2_beforeFSR");
  m_CAR_acc.initializePtEtaPhiMAccessor("MC_Hdecay2_decay1_beforeFSR");
  m_CAR_acc.initializePtEtaPhiMAccessor("MC_Hdecay2_decay2_beforeFSR");
  m_CAR_acc.initializePtEtaPhiMAccessor("MC_Hdecay1_decay1_afterFSR");
  m_CAR_acc.initializePtEtaPhiMAccessor("MC_Hdecay1_decay2_afterFSR");
  m_CAR_acc.initializePtEtaPhiMAccessor("MC_Hdecay2_decay1_afterFSR");
  m_CAR_acc.initializePtEtaPhiMAccessor("MC_Hdecay2_decay2_afterFSR");
  m_CAR_acc.initializeIntAccessor("MC_Hdecay1_decay1_beforeFSR_pdgId");
  m_CAR_acc.initializeIntAccessor("MC_Hdecay1_decay2_beforeFSR_pdgId");
  m_CAR_acc.initializeIntAccessor("MC_Hdecay2_decay1_beforeFSR_pdgId");
  m_CAR_acc.initializeIntAccessor("MC_Hdecay2_decay2_beforeFSR_pdgId");
  m_CAR_acc.initializeIntAccessor("MC_Hdecay1_decay1_afterFSR_pdgId");
  m_CAR_acc.initializeIntAccessor("MC_Hdecay1_decay2_afterFSR_pdgId");
  m_CAR_acc.initializeIntAccessor("MC_Hdecay2_decay1_afterFSR_pdgId");
  m_CAR_acc.initializeIntAccessor("MC_Hdecay2_decay2_afterFSR_pdgId");
}

void CARAlg::initializeHVVdecorators() {
  m_CAR_dec.initializePtEtaPhiMDecorator("MC_Hdecay1_decay1_beforeFSR_CAR");
  m_CAR_dec.initializeIntDecorator("MC_Hdecay1_decay1_beforeFSR_CAR_pdgId");
  m_CAR_dec.initializePtEtaPhiMDecorator("MC_Hdecay1_decay1_afterFSR_CAR");
  m_CAR_dec.initializeIntDecorator("MC_Hdecay1_decay1_afterFSR_CAR_pdgId");

  m_CAR_dec.initializePtEtaPhiMDecorator("MC_Hdecay1_decay2_beforeFSR_CAR");
  m_CAR_dec.initializeIntDecorator("MC_Hdecay1_decay2_beforeFSR_CAR_pdgId");
  m_CAR_dec.initializePtEtaPhiMDecorator("MC_Hdecay1_decay2_afterFSR_CAR");
  m_CAR_dec.initializeIntDecorator("MC_Hdecay1_decay2_afterFSR_CAR_pdgId");

  m_CAR_dec.initializePtEtaPhiMDecorator("MC_Hdecay2_decay1_beforeFSR_CAR");
  m_CAR_dec.initializeIntDecorator("MC_Hdecay2_decay1_beforeFSR_CAR_pdgId");
  m_CAR_dec.initializePtEtaPhiMDecorator("MC_Hdecay2_decay1_afterFSR_CAR");
  m_CAR_dec.initializeIntDecorator("MC_Hdecay2_decay1_afterFSR_CAR_pdgId");

  m_CAR_dec.initializePtEtaPhiMDecorator("MC_Hdecay2_decay2_beforeFSR_CAR");
  m_CAR_dec.initializeIntDecorator("MC_Hdecay2_decay2_beforeFSR_CAR_pdgId");
  m_CAR_dec.initializePtEtaPhiMDecorator("MC_Hdecay2_decay2_afterFSR_CAR");
  m_CAR_dec.initializeIntDecorator("MC_Hdecay2_decay2_afterFSR_CAR_pdgId");
}

void CARAlg::prepareCARHVVInputs() {
  // Negatively charged fermions are always decay1!
  if (m_partonhistory == "HWW") {
    // We are looking for l+ from W+
    // Hdecay1 is W+:
    // beforeFSR
    m_decay_map["V1_L1_beforeFSR"] = m_Hdecay1_decay2_beforeFSR_CAR;  // l+
    m_decay_map["V1_L2_beforeFSR"] = m_Hdecay1_decay1_beforeFSR_CAR;  // nu
    // afterFSR
    m_decay_map["V1_L1_afterFSR"] = m_Hdecay1_decay2_afterFSR_CAR;  // l+
    m_decay_map["V1_L2_afterFSR"] = m_Hdecay1_decay1_afterFSR_CAR;  // nu
    // We are looking for l- from W-
    // Hdecay2 is W-:
    // beforeFSR
    m_decay_map["V2_L1_beforeFSR"] = m_Hdecay2_decay1_beforeFSR_CAR;  // l-
    m_decay_map["V2_L2_beforeFSR"] = m_Hdecay2_decay2_beforeFSR_CAR;  // nubar
    // afterFSR
    m_decay_map["V2_L1_afterFSR"] = m_Hdecay2_decay1_afterFSR_CAR;  // l-
    m_decay_map["V2_L2_afterFSR"] = m_Hdecay2_decay2_afterFSR_CAR;  // nubar
  } else if (m_partonhistory == "HZZ") {
    // We are looking for l- from both Z
    // beforeFSR
    m_decay_map["V1_L1_beforeFSR"] = m_Hdecay1_decay1_beforeFSR_CAR;  // l-
    m_decay_map["V1_L2_beforeFSR"] = m_Hdecay1_decay2_beforeFSR_CAR;  // l+
    m_decay_map["V2_L1_beforeFSR"] = m_Hdecay2_decay1_beforeFSR_CAR;  // l-
    m_decay_map["V2_L2_beforeFSR"] = m_Hdecay2_decay2_beforeFSR_CAR;  // l+
    // afterFSR
    m_decay_map["V1_L1_afterFSR"] = m_Hdecay1_decay1_afterFSR_CAR;  // l-
    m_decay_map["V1_L2_afterFSR"] = m_Hdecay1_decay2_afterFSR_CAR;  // l+
    m_decay_map["V2_L1_afterFSR"] = m_Hdecay2_decay1_afterFSR_CAR;  // l-
    m_decay_map["V2_L2_afterFSR"] = m_Hdecay2_decay2_afterFSR_CAR;  // l+
  }
}

void CARAlg::prepareCARHVVOutputs() {
  if (m_partonhistory == "HWW") {
    // We are looking for l+ from W+
    // Hdecay1 is W+:
    // beforeFSR
    m_Hdecay1_decay2_beforeFSR_CAR = m_decay_map["V1_L1_beforeFSR"];  // l+
    m_Hdecay1_decay1_beforeFSR_CAR = m_decay_map["V1_L2_beforeFSR"];  // nu
    // afterFSR
    m_Hdecay1_decay2_afterFSR_CAR = m_decay_map["V1_L1_afterFSR"];  // l+
    m_Hdecay1_decay1_afterFSR_CAR = m_decay_map["V1_L2_afterFSR"];  // nu
    // We are looking for l- from W-
    // Hdecay2 is W-:
    // beforeFSR
    m_Hdecay2_decay1_beforeFSR_CAR = m_decay_map["V2_L1_beforeFSR"];  // l-
    m_Hdecay2_decay2_beforeFSR_CAR = m_decay_map["V2_L2_beforeFSR"];  // nubar
    // afterFSR
    m_Hdecay2_decay1_afterFSR_CAR = m_decay_map["V2_L1_afterFSR"];  // l-
    m_Hdecay2_decay2_afterFSR_CAR = m_decay_map["V2_L2_afterFSR"];  // nubar
  } else if (m_partonhistory == "HZZ") {
    // We are looking for l- from both Z
    // beforeFSR
    m_Hdecay1_decay1_beforeFSR_CAR = m_decay_map["V1_L1_beforeFSR"];  // l-
    m_Hdecay1_decay2_beforeFSR_CAR = m_decay_map["V1_L2_beforeFSR"];  // l+
    m_Hdecay2_decay1_beforeFSR_CAR = m_decay_map["V2_L1_beforeFSR"];  // l-
    m_Hdecay2_decay2_beforeFSR_CAR = m_decay_map["V2_L2_beforeFSR"];  // l+
    // afterFSR
    m_Hdecay1_decay1_afterFSR_CAR = m_decay_map["V1_L1_afterFSR"];  // l-
    m_Hdecay1_decay2_afterFSR_CAR = m_decay_map["V1_L2_afterFSR"];  // l+
    m_Hdecay2_decay1_afterFSR_CAR = m_decay_map["V2_L1_afterFSR"];  // l-
    m_Hdecay2_decay2_afterFSR_CAR = m_decay_map["V2_L2_afterFSR"];  // l+
  }
}

void CARAlg::performDefaultHVVDecoration(const xAOD::EventInfo* evtInfo) {
  m_CAR_dec.decorateDefault("MC_Hdecay1_decay1_beforeFSR_CAR", evtInfo);
  m_CAR_dec.decorateDefault("MC_Hdecay1_decay1_afterFSR_CAR", evtInfo);

  m_CAR_dec.decorateDefault("MC_Hdecay1_decay2_beforeFSR_CAR", evtInfo);
  m_CAR_dec.decorateDefault("MC_Hdecay1_decay2_afterFSR_CAR", evtInfo);

  m_CAR_dec.decorateDefault("MC_Hdecay2_decay1_beforeFSR_CAR", evtInfo);
  m_CAR_dec.decorateDefault("MC_Hdecay2_decay1_afterFSR_CAR", evtInfo);

  m_CAR_dec.decorateDefault("MC_Hdecay2_decay2_beforeFSR_CAR", evtInfo);
  m_CAR_dec.decorateDefault("MC_Hdecay2_decay2_afterFSR_CAR", evtInfo);
}

void CARAlg::performHVVDecoration(const xAOD::EventInfo* evtInfo,
                                  const xAOD::PartonHistory* history) {
  m_CAR_dec.decorateParticle(
      "MC_Hdecay1_decay1_beforeFSR_CAR", m_Hdecay1_decay1_beforeFSR_CAR,
      m_CAR_acc.getInt("MC_Hdecay1_decay1_beforeFSR_pdgId", history), evtInfo);
  m_CAR_dec.decorateParticle(
      "MC_Hdecay1_decay1_afterFSR_CAR", m_Hdecay1_decay1_afterFSR_CAR,
      m_CAR_acc.getInt("MC_Hdecay1_decay1_afterFSR_pdgId", history), evtInfo);

  m_CAR_dec.decorateParticle(
      "MC_Hdecay1_decay2_beforeFSR_CAR", m_Hdecay1_decay2_beforeFSR_CAR,
      m_CAR_acc.getInt("MC_Hdecay1_decay2_beforeFSR_pdgId", history), evtInfo);
  m_CAR_dec.decorateParticle(
      "MC_Hdecay1_decay2_afterFSR_CAR", m_Hdecay1_decay2_afterFSR_CAR,
      m_CAR_acc.getInt("MC_Hdecay1_decay2_afterFSR_pdgId", history), evtInfo);

  m_CAR_dec.decorateParticle(
      "MC_Hdecay2_decay1_beforeFSR_CAR", m_Hdecay2_decay1_beforeFSR_CAR,
      m_CAR_acc.getInt("MC_Hdecay2_decay1_beforeFSR_pdgId", history), evtInfo);
  m_CAR_dec.decorateParticle(
      "MC_Hdecay2_decay1_afterFSR_CAR", m_Hdecay2_decay1_afterFSR_CAR,
      m_CAR_acc.getInt("MC_Hdecay2_decay1_afterFSR_pdgId", history), evtInfo);

  m_CAR_dec.decorateParticle(
      "MC_Hdecay2_decay2_beforeFSR_CAR", m_Hdecay2_decay2_beforeFSR_CAR,
      m_CAR_acc.getInt("MC_Hdecay2_decay2_beforeFSR_pdgId", history), evtInfo);
  m_CAR_dec.decorateParticle(
      "MC_Hdecay2_decay2_afterFSR_CAR", m_Hdecay2_decay2_afterFSR_CAR,
      m_CAR_acc.getInt("MC_Hdecay2_decay2_afterFSR_pdgId", history), evtInfo);
}

void CARAlg::prepareHVVInputVectors(const xAOD::PartonHistory* history) {
  // Filling PtEtaPhiVectors for beforeFSR
  m_CAR_acc.getPtEtaPhiMVector("MC_Hdecay1_decay1_beforeFSR",
                               m_Hdecay1_decay1_beforeFSR_CAR, history);
  m_CAR_acc.getPtEtaPhiMVector("MC_Hdecay1_decay2_beforeFSR",
                               m_Hdecay1_decay2_beforeFSR_CAR, history);
  m_CAR_acc.getPtEtaPhiMVector("MC_Hdecay2_decay1_beforeFSR",
                               m_Hdecay2_decay1_beforeFSR_CAR, history);
  m_CAR_acc.getPtEtaPhiMVector("MC_Hdecay2_decay2_beforeFSR",
                               m_Hdecay2_decay2_beforeFSR_CAR, history);

  // Filling PtEtaPhiVectors for afterFSR
  m_CAR_acc.getPtEtaPhiMVector("MC_Hdecay1_decay1_afterFSR",
                               m_Hdecay1_decay1_afterFSR_CAR, history);
  m_CAR_acc.getPtEtaPhiMVector("MC_Hdecay1_decay2_afterFSR",
                               m_Hdecay1_decay2_afterFSR_CAR, history);
  m_CAR_acc.getPtEtaPhiMVector("MC_Hdecay2_decay1_afterFSR",
                               m_Hdecay2_decay1_afterFSR_CAR, history);
  m_CAR_acc.getPtEtaPhiMVector("MC_Hdecay2_decay2_afterFSR",
                               m_Hdecay2_decay2_afterFSR_CAR, history);
}

StatusCode CARAlg::execute() {
  for (const auto& sys : m_systematicsList.systematicsVector()) {
    // retrieve the EventInfo and parton history
    const xAOD::EventInfo* evtInfo = nullptr;
    ANA_CHECK(m_eventInfoHandle.retrieve(evtInfo, sys));
    const xAOD::PartonHistory* history = nullptr;
    ANA_CHECK(evtStore()->retrieve(history, m_partonContainerName));

    performDefaultHVVDecoration(evtInfo);
    prepareHVVInputVectors(history);
    prepareCARHVVInputs();
    performCAR();
    prepareCARHVVOutputs();
    performHVVDecoration(evtInfo, history);
  }
  return StatusCode::SUCCESS;
}

void CARAlg::SetSeed(unsigned int seed) {
  ANA_MSG_INFO("Using the following CAR seed: " + seed);
  m_randGen.SetSeed(seed);
}

float CARAlg::getRandPhi() {
  return m_randGen.Uniform(-TMath::Pi(), TMath::Pi());
}

float CARAlg::getRandCosth() {
  float randomValue;
  do {
    randomValue = m_randGen.Uniform(-1.0, 1.0);
  } while (m_randGen.Uniform(0.0, 1.0) >= m_PDF_costheta->Eval(randomValue));
  return randomValue;
}

void CARAlg::SetHBasis(const XYZVector& PV_CM3, const XYZVector& Pp3,
                       XYZVector& V1_x, XYZVector& V1_y, XYZVector& V1_z,
                       XYZVector& V2_x, XYZVector& V2_y, XYZVector& V2_z) {
  V1_z = PV_CM3.Unit();
  const float yp = V1_z.Dot(Pp3);
  const float SIGNyp = (yp >= 0.0f) ? 1.0f : -1.0f;

  V1_x = (Pp3 - yp * V1_z).Unit();
  V1_x *= SIGNyp;

  V1_y = Pp3.Cross(V1_z).Unit();
  V1_y *= SIGNyp;

  // Use the same basis for both V1 and V2, do the sign flipping later
  V2_x = V1_x;
  V2_y = V1_y;
  V2_z = V1_z;
}

void CARAlg::SetTransformedMomentum(PxPyPzEVector& result,
                                    const PtEtaPhiMVector& originalMomentum,
                                    const XYZVector& Vx, const XYZVector& Vy,
                                    const XYZVector& Vz,
                                    const double& thetaStar,
                                    const double& phiStar, const double& dir) {
  // Calculate the magnitude of the original momentum
  const double OP = originalMomentum.P();

  // Precompute common trigonometric terms
  const double cosTheta = std::cos(thetaStar);
  const double sinTheta = std::sin(thetaStar);
  const double cosPhi = std::cos(phiStar);
  const double sinPhi = std::sin(phiStar);

  // Compute the components of the transformed momentum, conserving total
  // momentum. The direction is defined via the 'dir' variable (1.0 or -1.0)
  const double px = dir * OP *
                    (Vz.X() * cosTheta + Vx.X() * sinTheta * cosPhi +
                     Vy.X() * sinTheta * sinPhi);
  const double py = dir * OP *
                    (Vz.Y() * cosTheta + Vx.Y() * sinTheta * cosPhi +
                     Vy.Y() * sinTheta * sinPhi);
  const double pz = dir * OP *
                    (Vz.Z() * cosTheta + Vx.Z() * sinTheta * cosPhi +
                     Vy.Z() * sinTheta * sinPhi);

  // Set the transformed momentum
  result.SetPxPyPzE(px, py, pz, originalMomentum.E());
}

void CARAlg::performCAR() {
  if (m_partonhistory == "HWW") {
    // performing CAR for beforeFSR
    performCAR(m_decay_map["V1_L1_beforeFSR"], m_decay_map["V1_L2_beforeFSR"],
               m_decay_map["V2_L1_beforeFSR"], m_decay_map["V2_L2_beforeFSR"]);

    // performing CAR for afterFSR
    performCAR(m_decay_map["V1_L1_afterFSR"], m_decay_map["V1_L2_afterFSR"],
               m_decay_map["V2_L1_afterFSR"], m_decay_map["V2_L2_afterFSR"]);
  }
  // TODO implement HZZ and ttbar
}

void CARAlg::performCAR(PtEtaPhiMVector& PV1_L1, PtEtaPhiMVector& PV1_L2,
                        PtEtaPhiMVector& PV2_L1, PtEtaPhiMVector& PV2_L2) {

  // At some point boosting replaces the mass with sqrt(E^2 - P^2), keep the
  // true masses here so we can always change back
  const float PV1_L1_m = PV1_L1.M();
  const float PV1_L2_m = PV1_L2.M();
  const float PV2_L1_m = PV2_L1.M();
  const float PV2_L2_m = PV2_L2.M();
  PtEtaPhiMVector PV1 = PV1_L1 + PV1_L2;
  PtEtaPhiMVector PV2 = PV2_L1 + PV2_L2;
  Boost LabtoVVCM = Boost(PV1.BoostToCM(PV2));

  PtEtaPhiMVector PV1_VVCM = LabtoVVCM * PV1;
  PtEtaPhiMVector PV2_VVCM = LabtoVVCM * PV2;

  // proton beam
  const XYZVector Pb(0.0, 0.0, 1.0);  // We don't care about the energy
                                      // component

  // defining the helicity basis
  XYZVector V1_x, V1_y, V1_z, V2_x, V2_y, V2_z;
  SetHBasis(PV1_VVCM.Vect(), Pb, V1_x, V1_y, V1_z, V2_x, V2_y,
            V2_z);  // Use same basis, derived from V1, for both V1 and V2

  Boost VVCMtoV1CM = Boost(PV1_VVCM.BoostToCM());
  Boost VVCMtoV2CM = Boost(PV2_VVCM.BoostToCM());

  PtEtaPhiMVector PV1_L1R = VVCMtoV1CM * (LabtoVVCM * PV1_L1);
  PtEtaPhiMVector PV1_L2R = VVCMtoV1CM * (LabtoVVCM * PV1_L2);
  PtEtaPhiMVector PV2_L1R = VVCMtoV2CM * (LabtoVVCM * PV2_L1);
  PtEtaPhiMVector PV2_L2R = VVCMtoV2CM * (LabtoVVCM * PV2_L2);

  // generate cos(theta)* and phi* from PDF
  const float costhstar1 = this->getRandCosth();
  const float costhstar2 = this->getRandCosth();
  const float phistar1 = this->getRandPhi();
  const float phistar2 = this->getRandPhi();

  const float thstar1 = std::acos(costhstar1);
  const float thstar2 = std::acos(costhstar2);

  // Recalculate the momenta based on the new angles
  auto transformMomentum = [&](PtEtaPhiMVector& pL1, PtEtaPhiMVector& pL2,
                               const XYZVector& Vx, const XYZVector& Vy,
                               const XYZVector& Vz, float thetaStar,
                               float phiStar) {
    PxPyPzEVector pL1RT, pL2RT;

    SetTransformedMomentum(pL1RT, pL1, Vx, Vy, Vz, thetaStar, phiStar, 1.0);
    SetTransformedMomentum(pL2RT, pL2, Vx, Vy, Vz, thetaStar, phiStar, -1.0);

    return std::make_pair(pL1RT, pL2RT);
  };

  auto [PV1_L1RT, PV1_L2RT] =
      transformMomentum(PV1_L1R, PV1_L2R, V1_x, V1_y, V1_z, thstar1, phistar1);
  auto [PV2_L1RT, PV2_L2RT] =
      transformMomentum(PV2_L1R, PV2_L2R, V2_x, V2_y, V2_z, thstar2, phistar2);

  // Transform back to the original frame
  auto transformBack = [&](const PxPyPzEVector& pRT, const Boost& boost) {
    return LabtoVVCM.Inverse() * (boost.Inverse() * pRT);
  };

  PV1_L1 = transformBack(PV1_L1RT, VVCMtoV1CM);
  PV1_L2 = transformBack(PV1_L2RT, VVCMtoV1CM);
  PV2_L1 = transformBack(PV2_L1RT, VVCMtoV2CM);
  PV2_L2 = transformBack(PV2_L2RT, VVCMtoV2CM);

  // Restore the original masses
  PV1_L1.SetM(PV1_L1_m);
  PV1_L2.SetM(PV1_L2_m);
  PV2_L1.SetM(PV2_L1_m);
  PV2_L2.SetM(PV2_L2_m);
  return;
}

StatusCode CARAlg::finalize() {
  ANA_MSG_INFO("Finalizing CAR");

  return StatusCode::SUCCESS;
}
}  // namespace top
