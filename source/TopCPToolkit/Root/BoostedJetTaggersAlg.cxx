#include "TopCPToolkit/BoostedJetTaggersAlg.h"

#include "xAODCore/ShallowAuxContainer.h"
#include "xAODCore/ShallowCopy.h"

namespace top {

BoostedJetTaggersAlg::BoostedJetTaggersAlg(const std::string& name,
                                           ISvcLocator* pSvcLocator)
    : EL::AnaAlgorithm(name, pSvcLocator) {

  declareProperty("isMC", m_isMC = true, "True for MC sample, False for data.");
  declareProperty("taggerType", m_taggerTypeStr = "", "Define tagger type");
  declareProperty("taggerCalibArea", m_taggerCalibArea = "",
                  "Identifies directory with top-tagger config file");
  declareProperty("taggerConfigFile", m_taggerConfigFile = "",
                  "Config file name inside calib area directory");
  declareProperty("taggerDecoration", m_taggerDecoration = "",
                  "Decoration name");
}

StatusCode BoostedJetTaggersAlg::initialize() {
  ANA_MSG_INFO("Initialising the BoostedJetTaggersAlg tool: "
               << m_taggerTypeStr + "/" + m_taggerDecoration);

  ANA_CHECK(m_ljetsHandle.initialize(m_systematicsList));

  if (m_taggerTypeStr == "JSSWTopTaggerDNN") {
    m_taggerType = TaggerType::JSSWTopTaggerDNN;
  } else if (m_taggerTypeStr == "JSSWTopTaggerANN") {
    m_taggerType = TaggerType::JSSWTopTaggerANN;
  } else {
    ANA_MSG_ERROR("Unrecognized boosted jet tagger type: "
                  << m_taggerTypeStr << ". Available options: "
                  << "JSSWTopTaggerDNN" << ", " << "JSSWTopTaggerANN");
    return StatusCode::FAILURE;
  }

  auto initTagger = [this]<typename T>(T& tagger) -> StatusCode {
    tagger.setTypeAndName(m_taggerTypeStr + "/" + m_taggerDecoration);
    ANA_CHECK(tagger.setProperty("CalibArea", m_taggerCalibArea));
    ANA_CHECK(tagger.setProperty("ConfigFile", m_taggerConfigFile));
    ANA_CHECK(tagger.setProperty("IsMC", m_isMC));
    ANA_CHECK(tagger.retrieve());
    return StatusCode::SUCCESS;
  };

  // Tagger configuration
  if (m_taggerType == TaggerType::JSSWTopTaggerDNN) {
    ANA_CHECK(initTagger(m_taggerDNN));
  }
  if (m_taggerType == TaggerType::JSSWTopTaggerANN) {
    ANA_CHECK(initTagger(m_taggerANN));
  }

  // Initialize output handles
  m_ljetsTopTagHandle = std::make_unique<CP::SysWriteDecorHandle<int>>(
      this, m_taggerDecoration + "_tagged",
      m_taggerDecoration + "_tagged_%SYS%",
      "Decoration name for tagging result");
  m_ljetsTopTagScoreHandle = std::make_unique<CP::SysWriteDecorHandle<float>>(
      this, m_taggerDecoration + "_score", m_taggerDecoration + "_score_%SYS%",
      "Decoration name for tagging score");
  m_ljetsTopTagPassMassHandle = std::make_unique<CP::SysWriteDecorHandle<int>>(
      this, m_taggerDecoration + "_passMass",
      m_taggerDecoration + "_passMass_%SYS%",
      "Indicates that jet passes mass cuts");

  m_ljetsTopTagPassKinRangeHandle =
      std::make_unique<CP::SysWriteDecorHandle<int>>(
          this, m_taggerDecoration + "_validKinRange",
          m_taggerDecoration + "_validKinRange_%SYS%",
          "Indicates that jet passes pT, eta cuts");

  m_ljetsTopTagSFHandle = std::make_unique<CP::SysWriteDecorHandle<float>>(
      this, m_taggerDecoration + "_SF", m_taggerDecoration + "_SF_%SYS%",
      "Decoration name for tagging scale factor");
  m_ljetsTopTagEfficiencyHandle =
      std::make_unique<CP::SysWriteDecorHandle<float>>(
          this, m_taggerDecoration + "_efficiency",
          m_taggerDecoration + "_efficiency_%SYS%",
          "Decoration name for tagging efficiency");
  m_ljetsTopTagEffSFHandle = std::make_unique<CP::SysWriteDecorHandle<float>>(
      this, m_taggerDecoration + "_effSF", m_taggerDecoration + "_effSF_%SYS%",
      "Decoration name for tagging efficiency scale factor");
  ANA_CHECK(m_ljetsTopTagHandle->initialize(m_systematicsList, m_ljetsHandle));
  ANA_CHECK(
      m_ljetsTopTagScoreHandle->initialize(m_systematicsList, m_ljetsHandle));
  ANA_CHECK(m_ljetsTopTagPassMassHandle->initialize(m_systematicsList,
                                                    m_ljetsHandle));
  ANA_CHECK(m_ljetsTopTagPassKinRangeHandle->initialize(m_systematicsList,
                                                        m_ljetsHandle));
  ANA_CHECK(
      m_ljetsTopTagSFHandle->initialize(m_systematicsList, m_ljetsHandle));
  ANA_CHECK(m_ljetsTopTagEfficiencyHandle->initialize(m_systematicsList,
                                                      m_ljetsHandle));
  ANA_CHECK(
      m_ljetsTopTagEffSFHandle->initialize(m_systematicsList, m_ljetsHandle));

  ANA_CHECK(m_systematicsList.initialize());

  return StatusCode::SUCCESS;
}

StatusCode BoostedJetTaggersAlg::execute() {

  const SG::ConstAccessor<bool> acc_Tagged(m_taggerDecoration + "_Tagged");
  const SG::ConstAccessor<float> acc_Score(m_taggerDecoration + "_Score");
  const SG::ConstAccessor<bool> acc_PassMass(m_taggerDecoration + "_PassMass");
  const SG::ConstAccessor<bool> acc_ValidKinRange(m_taggerDecoration +
                                                  "_ValidKinRange");
  const SG::ConstAccessor<float> acc_SF(m_taggerDecoration + "_SF");
  const SG::ConstAccessor<float> acc_efficiency(m_taggerDecoration +
                                                "_efficiency");
  const SG::ConstAccessor<float> acc_effSF(m_taggerDecoration + "_effSF");

  for (const auto& sys : m_systematicsList.systematicsVector()) {
    const xAOD::JetContainer* ljets = nullptr;

    ANA_CHECK(m_ljetsHandle.retrieve(ljets, sys));

    std::pair<xAOD::JetContainer*, xAOD::ShallowAuxContainer*>
        ljets_shallowCopy = xAOD::shallowCopyContainer(*ljets);
    std::unique_ptr<xAOD::JetContainer> shallowJets(ljets_shallowCopy.first);
    std::unique_ptr<xAOD::ShallowAuxContainer> shallowAux(
        ljets_shallowCopy.second);

    // ANA_CHECK(m_taggerDNN->decorate(*shallowJets));

    for (size_t ijet = 0; ijet < shallowJets->size(); ijet++) {

      xAOD::Jet* scJet = shallowJets->at(ijet);
      const xAOD::Jet* ljet = ljets->at(ijet);

      if (m_taggerType == TaggerType::JSSWTopTaggerDNN) {
        ANA_CHECK(m_taggerDNN->tag(*scJet));
      }
      if (m_taggerType == TaggerType::JSSWTopTaggerANN) {
        ANA_CHECK(m_taggerANN->tag(*scJet));
      }

      const int isTagged =
          acc_Tagged.isAvailable(*scJet) ? acc_Tagged(*scJet) : 0;
      const float score =
          acc_Score.isAvailable(*scJet) ? acc_Score(*scJet) : -666.;
      const int passMass =
          acc_PassMass.isAvailable(*scJet) ? acc_PassMass(*scJet) : 0;
      const int validKinRange =
          acc_ValidKinRange.isAvailable(*scJet) ? acc_ValidKinRange(*scJet) : 0;
      const float sf = acc_SF.isAvailable(*scJet) ? acc_SF(*scJet) : 1.;
      const float efficiency =
          acc_efficiency.isAvailable(*scJet) ? acc_efficiency(*scJet) : 1.;
      const float effSF =
          acc_effSF.isAvailable(*scJet) ? acc_effSF(*scJet) : 1.;

      m_ljetsTopTagHandle->set(*ljet, isTagged, sys);
      m_ljetsTopTagScoreHandle->set(*ljet, score, sys);
      m_ljetsTopTagPassMassHandle->set(*ljet, passMass, sys);
      m_ljetsTopTagPassKinRangeHandle->set(*ljet, validKinRange, sys);
      m_ljetsTopTagSFHandle->set(*ljet, sf, sys);
      m_ljetsTopTagEfficiencyHandle->set(*ljet, efficiency, sys);
      m_ljetsTopTagEffSFHandle->set(*ljet, effSF, sys);
    }
  }

  return StatusCode::SUCCESS;
}

}  // namespace top
