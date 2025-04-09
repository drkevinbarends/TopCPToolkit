#include "TopCPToolkit/SinglelepHadronicChi2RecoAlg.h"

#include "AthContainers/ConstDataVector.h"

#include "VectorHelpers/LorentzHelper.h"

#include <algorithm>

namespace top {
  using ROOT::Math::PtEtaPhiEVector;
  using ROOT::Math::PtEtaPhiMVector;
  using ROOT::Math::VectorUtil::DeltaR;

  SinglelepHadronicChi2RecoAlg::SinglelepHadronicChi2RecoAlg(const std::string &name, ISvcLocator *pSvcLocator)
    : EL::AnaAlgorithm(name, pSvcLocator)
  {
    declareProperty("topMass", m_topMass, "Mass of the top quark for chi^2 (in GeV)");
    declareProperty("maxBJets", m_maxBJets, "Maximum number of b-jets to consider");
    declareProperty("maxLightJets", m_maxLightJets, "Maximum number of light (non-b) jets to consider");
    declareProperty("topResolution", m_topResolution, "The sigma parameter for the top quark in the chi^2 (in GeV)");
    declareProperty("wResolution", m_wResolution, "The sigma parameter for the W boson in the chi^2 (in GeV)");
    declareProperty("bTagDecoration", m_bTagDecoration, "Decoration used for b-tagging decision");
  }

  StatusCode SinglelepHadronicChi2RecoAlg::initialize() {
    ANA_MSG_INFO("Initializing PartonToJetsMatchAlg " << name() );

    if (m_maxBJets < 1) {
      ANA_MSG_ERROR("Max number of b-jets < 1");
      return StatusCode::FAILURE;
    }
    if (m_maxLightJets < 2) {
      ANA_MSG_ERROR("Max number of light jets < 2");
      return StatusCode::FAILURE;
    }
    if (m_bTagDecoration.empty()) {
      ANA_MSG_ERROR("bTagDecoration is not set");
      return StatusCode::FAILURE;
    }

    m_bTagDecoAcc = std::make_unique<SG::AuxElement::ConstAccessor<char>>(m_bTagDecoration);

    ANA_CHECK(m_jetsHandle.initialize(m_systematicsList));
    ANA_CHECK(m_jetSelection.initialize(m_systematicsList, m_jetsHandle, SG::AllowEmpty));
    ANA_CHECK(m_eventInfoHandle.initialize(m_systematicsList));
    ANA_CHECK(m_selection.initialize(m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));


    ANA_CHECK(m_had_b_idx_decor.initialize(m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));
    ANA_CHECK(m_light_1_idx_decor.initialize(m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));
    ANA_CHECK(m_light_2_idx_decor.initialize(m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));
    ANA_CHECK(m_best_chi2_decor.initialize(m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));

    ANA_CHECK(m_systematicsList.initialize());

    return StatusCode::SUCCESS;
  }

  StatusCode SinglelepHadronicChi2RecoAlg::execute() {
    for (const auto &sys : m_systematicsList.systematicsVector()) {
      ANA_CHECK(execute_syst(sys));
    }
    return StatusCode::SUCCESS;
  }

  StatusCode SinglelepHadronicChi2RecoAlg::finalize() {
    ANA_MSG_INFO("Finalizing SinglelepHadroicChi2RecoAlg");

    return StatusCode::SUCCESS;
  }

  StatusCode SinglelepHadronicChi2RecoAlg::execute_syst(const CP::SystematicSet &sys) {
    const xAOD::EventInfo *evtInfo = nullptr;
    ANA_CHECK(m_eventInfoHandle.retrieve(evtInfo, sys));

    // default-decorate EventInfo
    m_had_b_idx_decor.set(*evtInfo, -1, sys);
    m_light_1_idx_decor.set(*evtInfo, -1, sys);
    m_light_2_idx_decor.set(*evtInfo, -1, sys);
    m_best_chi2_decor.set(*evtInfo, -1, sys);

    m_best_chi2 = 999999.;
    m_had_b_idx = -1;
    m_light_1_idx = -1;
    m_light_2_idx = -1;

    // skip events not selected
    if (m_selection && !m_selection.getBool(*evtInfo, sys))
      return StatusCode::SUCCESS;

    const xAOD::JetContainer *jets = nullptr;
    ANA_CHECK(m_jetsHandle.retrieve(jets, sys));

    // get only selected jets
    ConstDataVector<xAOD::JetContainer> selected_jets(SG::VIEW_ELEMENTS);
    for (const xAOD::Jet *jet : *jets) {
      if (m_jetSelection.getBool(*jet, sys)) {
        selected_jets.push_back(jet);
      }
    }

    std::vector<std::pair<int, const xAOD::Jet*> > orderedJets;
    for (std::size_t i = 0; i < selected_jets.size(); ++i) {
      orderedJets.emplace_back(std::make_pair(i, selected_jets.at(i)));
    }

    // sort them based on pT
    std::sort(orderedJets.begin(), orderedJets.end(),
      [](const auto& el1, const auto& el2){
        return el1.second->pt() > el2.second->pt();
      }
      );

    // split them based on b-tagging
    std::vector<int> bIndices;
    std::vector<int> lIndices;

    for (const auto& ipair : orderedJets) {
      const bool isTagged = (*m_bTagDecoAcc)(*(ipair.second));
      if (isTagged) {
        bIndices.emplace_back(ipair.first);
      } else {
        lIndices.emplace_back(ipair.first);
      }
    }

    // skip the matching if we dont have enough jets
    if (bIndices.empty() || lIndices.size() < 2) return StatusCode::SUCCESS;

    // consider only N jets based on the settings
    if ((int)bIndices.size() > m_maxBJets) {
      bIndices.resize(m_maxBJets);
    }
    if ((int)lIndices.size() > m_maxLightJets) {
      lIndices.resize(m_maxLightJets);
    }

    std::vector<std::tuple<int, int, int> > consideredIndices;

    // run the permutations
    // b-jet permutations
    do {
      // light jet permutations
      do {
        auto indices = std::make_tuple(bIndices.at(0), lIndices.at(0), lIndices.at(1));
        auto itr = std::find(consideredIndices.begin(), consideredIndices.end(), indices);
        if (itr == consideredIndices.end()) {
          auto indicesReversed = std::make_tuple(bIndices.at(0), lIndices.at(1), lIndices.at(0));
          consideredIndices.emplace_back(std::move(indices));
          consideredIndices.emplace_back(std::move(indicesReversed));
        } else {
          continue;
        }

        // evaluate this permutation
        this->runSinglePermutation(selected_jets, bIndices.at(0), lIndices.at(0), lIndices.at(1));
      } while (std::next_permutation(lIndices.begin(), lIndices.end()));
    } while (std::next_permutation(bIndices.begin(), bIndices.end()));

    // store the results
    m_best_chi2_decor.set(*evtInfo, m_best_chi2, sys);
    m_had_b_idx_decor.set(*evtInfo, m_had_b_idx, sys);
    m_light_1_idx_decor.set(*evtInfo, m_light_1_idx, sys);
    m_light_2_idx_decor.set(*evtInfo, m_light_2_idx, sys);

    return StatusCode::SUCCESS;
  }
    
  double SinglelepHadronicChi2RecoAlg::chi2(const float recoWmass, const float recoTopMass) const {
    constexpr double wMass = 80.38;
    const double a = (recoWmass - wMass)*(recoWmass - wMass)/(m_wResolution*m_wResolution);
    const double b = (recoTopMass - m_topMass)*(recoTopMass - m_topMass)/(m_topResolution*m_topResolution);

    return a + b;
  }

  void SinglelepHadronicChi2RecoAlg::runSinglePermutation(const ConstDataVector<xAOD::JetContainer>& jets,
                                                          const int bIndex,
                                                          const int lIndex1,
                                                          const int lIndex2) {

    const auto& bjet = jets.at(bIndex);
    const auto& ljet1 = jets.at(lIndex1);
    const auto& ljet2 = jets.at(lIndex2);

    ROOT::Math::PtEtaPhiEVector bHad(bjet->pt()/1e3,
                                     bjet->eta(),
                                     bjet->phi(),
                                     bjet->e()/1e3);

    ROOT::Math::PtEtaPhiEVector l1(ljet1->pt()/1e3,
                                   ljet1->eta(),
                                   ljet1->phi(),
                                   ljet1->e()/1e3);

    ROOT::Math::PtEtaPhiEVector l2(ljet2->pt()/1e3,
                                   ljet2->eta(),
                                   ljet2->phi(),
                                   ljet2->e()/1e3);
    const double mW = (l1+l2).M();
    const double mT = (l1+l2+bHad).M();
    const double currentChi2 = this->chi2(mW, mT);

    if (currentChi2 < m_best_chi2) {
        m_best_chi2 = currentChi2;
        m_light_1_idx = lIndex1;
        m_light_2_idx = lIndex2;
        m_had_b_idx = bIndex;
    }

  }

}
