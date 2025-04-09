#include "TopCPToolkit/JetMatchingAlg.h"
#include "AthContainers/ConstDataVector.h"
#include "VectorHelpers/LorentzHelper.h"

namespace top {
  using ROOT::Math::PtEtaPhiMVector;
  using ROOT::Math::VectorUtil::DeltaR;

  JetMatchingAlg::JetMatchingAlg(const std::string &name, ISvcLocator *pSvcLocator)
    : EL::AnaAlgorithm(name, pSvcLocator)
  {
    declareProperty("criticalDR", m_criticalDR, "Maximum delta R for matching");
    declareProperty("criticalDR_leptons", m_criticalDR_leptons, "Minimum delta R required between reco jet and truth prompt lepton");
  }

  StatusCode JetMatchingAlg::initialize() {
    ANA_MSG_INFO("Initializing JetMatching " << name() );

    ANA_CHECK(m_eventInfoHandle.initialize(m_systematicsList));
    ANA_CHECK(m_jetsHandle.initialize(m_systematicsList));
    ANA_CHECK(m_truthJetsHandle.initialize(m_systematicsList));
    ANA_CHECK(m_truthElectronsHandle.initialize(m_systematicsList));
    ANA_CHECK(m_truthMuonsHandle.initialize(m_systematicsList));
    ANA_CHECK(m_jetSelection.initialize(m_systematicsList, m_jetsHandle, SG::AllowEmpty));
    ANA_CHECK(m_selection.initialize(m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));

    ANA_CHECK(m_truth_jet_paired_index.initialize(m_systematicsList, m_jetsHandle));
    ANA_CHECK(m_reco_to_reco_jet_closest_dR.initialize(m_systematicsList, m_jetsHandle));
    ANA_CHECK(m_truth_to_truth_jet_closest_dR.initialize(m_systematicsList, m_jetsHandle));
    ANA_CHECK(m_has_truth_lepton.initialize(m_systematicsList, m_jetsHandle));
    ANA_CHECK(m_overlapping_truth_lepton_pt.initialize(m_systematicsList, m_jetsHandle));

    ANA_CHECK(m_systematicsList.initialize());

    return StatusCode::SUCCESS;
  }

  StatusCode JetMatchingAlg::execute() {
    for (const auto &sys : m_systematicsList.systematicsVector()) {
      //retrieve objects
      const xAOD::EventInfo *evtInfo = nullptr;
      ANA_CHECK(m_eventInfoHandle.retrieve(evtInfo, sys));

      const xAOD::JetContainer *jets = nullptr;
      ANA_CHECK(m_jetsHandle.retrieve(jets, sys));

      const xAOD::JetContainer *truth_jets = nullptr;
      ANA_CHECK(m_truthJetsHandle.retrieve(truth_jets, sys));

      const xAOD::TruthParticleContainer *truth_electrons = nullptr;
      ANA_CHECK(m_truthElectronsHandle.retrieve(truth_electrons, sys));

      const xAOD::TruthParticleContainer *truth_muons = nullptr;
      ANA_CHECK(m_truthMuonsHandle.retrieve(truth_muons, sys));

      for (const xAOD::Jet *jet : *jets) {
        m_truth_jet_paired_index.set(*jet, -1, sys);
        m_reco_to_reco_jet_closest_dR.set(*jet, -1, sys);
        m_truth_to_truth_jet_closest_dR.set(*jet, -1, sys);
        m_has_truth_lepton.set(*jet, false, sys);
        m_overlapping_truth_lepton_pt.set(*jet, -1, sys);
      }

      if (m_selection && !m_selection.getBool(*evtInfo, sys))
        continue;

      ConstDataVector<xAOD::JetContainer> selected_jets(SG::VIEW_ELEMENTS);
      for (const xAOD::Jet *jet : *jets) {
        if (m_jetSelection.getBool(*jet, sys))
          selected_jets.push_back(jet);
      }

      std::vector<int> matched_truth_jet_indices;
      std::vector<float> reco_to_reco_jet_closest_dR;
      std::vector<float> truth_to_truth_jet_closest_dR;

      unsigned int ireco = 0;
      //loop over all selected reco jet
      for (const xAOD::Jet *recojet : selected_jets) {
        PtEtaPhiEVector reco_jet = GetPtEtaPhiE(recojet);
        //find deltaR to closest reco jet
        float minDR = JetMatchingAlg::get_minDR_reco(reco_jet, ireco, selected_jets);
        reco_to_reco_jet_closest_dR.push_back(minDR);
        //find index of truth jet matched to this reco jet (closest within critical deltaR)
        int truth_jet_index = JetMatchingAlg::get_matched_truth(reco_jet, *truth_jets);
        matched_truth_jet_indices.push_back(truth_jet_index);
        //for each matched truth jet, find deltaR to its closest truth jet
        if (truth_jet_index == -1) truth_to_truth_jet_closest_dR.push_back(-1); //if there was no truth jet matched to this reco
        else {
          minDR = JetMatchingAlg::get_minDR_truth(*truth_jets, truth_jet_index);
          truth_to_truth_jet_closest_dR.push_back(minDR);
        }
        ireco++;
      }

      //check if truth index was assigned more than once
      for (std::size_t i = 0; i < matched_truth_jet_indices.size()-1; i++) {
        for (std::size_t j = i+1; j < matched_truth_jet_indices.size(); j++) {
          if(matched_truth_jet_indices.at(j) == matched_truth_jet_indices.at(i)) {
            matched_truth_jet_indices.at(i) = -1;
            matched_truth_jet_indices.at(j) = -1;
            truth_to_truth_jet_closest_dR.at(i) = -1;
            truth_to_truth_jet_closest_dR.at(j) = -1;
          }
        }
      }

      //loop over selected reco jets and decorate them
      int ijet = 0;
      for (const xAOD::Jet *jet : selected_jets) {
        m_truth_jet_paired_index.set(*jet, matched_truth_jet_indices.at(ijet), sys);
        m_reco_to_reco_jet_closest_dR.set(*jet, reco_to_reco_jet_closest_dR.at(ijet), sys);
        m_truth_to_truth_jet_closest_dR.set(*jet, truth_to_truth_jet_closest_dR.at(ijet), sys);
        //is there a truth lepton within dR<0.4 from the reco jet?
        PtEtaPhiEVector reco_jet = GetPtEtaPhiE(jet);
        double overlapping_truth_lepton_pt = -1;
        m_has_truth_lepton.set(*jet, JetMatchingAlg::find_close_lepton(reco_jet, *truth_electrons, *truth_muons, overlapping_truth_lepton_pt), sys);
        m_overlapping_truth_lepton_pt.set(*jet, overlapping_truth_lepton_pt, sys);
        ijet++;
      }
    }
    return StatusCode::SUCCESS;
  }

  float JetMatchingAlg::get_minDR_reco(PtEtaPhiEVector jet1, unsigned int ijet1, ConstDataVector<xAOD::JetContainer> selected_jets) {
    float minDR = 9999;
    unsigned int ijet2 = 0;
    for (const xAOD::Jet *jet : selected_jets) {
      if (ijet1 == ijet2) {
        ijet2++;
        continue;
      }
      PtEtaPhiEVector jet2 = GetPtEtaPhiE(jet);
      if (DeltaR(jet1, jet2) < minDR) {
        minDR = DeltaR(jet1, jet2);
      }
      ijet2++;
    }
    return minDR;
  }
    
  int JetMatchingAlg::get_matched_truth(PtEtaPhiEVector reco_jet, const xAOD::JetContainer &truth_jets) {
    int truth_jet_index = -1;
    float minDR = 9999;
    int itruth = 0;
    for (const xAOD::Jet *truthjet : truth_jets) {
      PtEtaPhiMVector truth_jet = GetPtEtaPhiM(truthjet);
      if (DeltaR(reco_jet, truth_jet) < minDR) {
	minDR = DeltaR(reco_jet, truth_jet);
	truth_jet_index = itruth;
      }
      itruth++;
    }
    if (minDR > m_criticalDR) truth_jet_index = -1;
    return truth_jet_index;
  }

  bool JetMatchingAlg::find_close_lepton(PtEtaPhiEVector& reco_jet, const xAOD::TruthParticleContainer &truth_electrons, const xAOD::TruthParticleContainer &truth_muons, double& overlapping_truth_lepton_pt) {
    for (const xAOD::TruthParticle *electron : truth_electrons) {
      const PtEtaPhiMVector& TLVelectron = GetPtEtaPhiMfromTruth(electron);
      if (DeltaR(reco_jet, TLVelectron) < m_criticalDR_leptons) {
          overlapping_truth_lepton_pt = TLVelectron.Pt();
          return true;
      }
    }
    for (const xAOD::TruthParticle *muon : truth_muons) {
      const PtEtaPhiMVector& TLVmuon = GetPtEtaPhiMfromTruth(muon);
      if (DeltaR(reco_jet, TLVmuon) < m_criticalDR_leptons) {
        overlapping_truth_lepton_pt = TLVmuon.Pt();
        return true;
      }
    }
    return false;
  }

  float JetMatchingAlg::get_minDR_truth(const xAOD::JetContainer &truth_jets, int truth_jet_index) {
    float minDR = 9999;
    int itruth1 = 0;
    for (const xAOD::Jet *truthjet1 : truth_jets) {
      if (itruth1 == truth_jet_index) {
        PtEtaPhiMVector truth_jet1 = GetPtEtaPhiM(truthjet1);
        minDR = 9999;
        int itruth2 = 0;
        for (const xAOD::Jet *truthjet2 : truth_jets) {
          if (itruth1 == itruth2) {
              itruth2++;
              continue;
          }
          PtEtaPhiMVector truth_jet2 = GetPtEtaPhiM(truthjet2);
          if (DeltaR(truth_jet1, truth_jet2) < minDR) {
	    minDR = DeltaR(truth_jet1, truth_jet2);
          }
          itruth2++;
        }
        break;
      }
      itruth1++;
    }
    return minDR;
  }

} // namespace top

