/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

/// @author Nils Krumnack

//
// includes
//

#include <TopCPToolkit/ComponentFactoryPreloader.h>

#include <AsgTools/AsgComponentFactories.h>
#include <AsgTools/MessageCheckAsgTools.h>

#include <AssociationUtils/DeltaROverlapTool.h>
#include <AssociationUtils/EleJetOverlapTool.h>
#include <AssociationUtils/EleMuSharedTrkOverlapTool.h>
#include <AssociationUtils/MuJetOverlapTool.h>
#include <AssociationUtils/OverlapRemovalTool.h>
#include <AsgAnalysisAlgorithms/AsgFlagSelectionTool.h>
#include <AsgAnalysisAlgorithms/AsgLeptonTrackSelectionAlg.h>
#include <AsgAnalysisAlgorithms/AsgOriginalObjectLinkAlg.h>
#include <AsgAnalysisAlgorithms/AsgPriorityDecorationAlg.h>
#include <AsgAnalysisAlgorithms/AsgPtEtaSelectionTool.h>
#include <AsgAnalysisAlgorithms/AsgSelectionAlg.h>
#include <AsgAnalysisAlgorithms/AsgShallowCopyAlg.h>
#include <AsgAnalysisAlgorithms/AsgUnionPreselectionAlg.h>
#include <AsgAnalysisAlgorithms/AsgUnionSelectionAlg.h>
#include <AsgAnalysisAlgorithms/AsgViewFromSelectionAlg.h>
#include <AsgAnalysisAlgorithms/AsgxAODMetNTupleMakerAlg.h>
#include <AsgAnalysisAlgorithms/AsgxAODNTupleMakerAlg.h>
#include <AsgAnalysisAlgorithms/AsgEnergyDecoratorAlg.h>
#include <AsgAnalysisAlgorithms/BootstrapGeneratorAlg.h>
#include <AsgAnalysisAlgorithms/CopyNominalSelectionAlg.h>
#include <AsgAnalysisAlgorithms/EventFlagSelectionAlg.h>
#include <AsgAnalysisAlgorithms/EventSelectionByObjectFlagAlg.h>
#include <AsgAnalysisAlgorithms/EventStatusSelectionAlg.h>
#include <AsgAnalysisAlgorithms/FakeBkgCalculatorAlg.h>
#include <AsgAnalysisAlgorithms/IOStatsAlg.h>
#include <AsgAnalysisAlgorithms/KinematicHistAlg.h>
#include <AsgAnalysisAlgorithms/LeptonSFCalculatorAlg.h>
#include <AsgAnalysisAlgorithms/ObjectCutFlowHistAlg.h>
#include <AsgAnalysisAlgorithms/OverlapRemovalAlg.h>
#include <AsgAnalysisAlgorithms/PileupReweightingAlg.h>
#include <AsgAnalysisAlgorithms/SysListDumperAlg.h>
#include <AsgAnalysisAlgorithms/SystObjectLinkerAlg.h>
#include <AsgAnalysisAlgorithms/TreeFillerAlg.h>
#include <AsgAnalysisAlgorithms/TreeMakerAlg.h>
#include <EgammaAnalysisAlgorithms/EgammaCalibrationAndSmearingAlg.h>
#include <EgammaAnalysisAlgorithms/EgammaIsGoodOQSelectionTool.h>
#include <EgammaAnalysisAlgorithms/EgammaIsolationCorrectionAlg.h>
#include <EgammaAnalysisAlgorithms/EgammaIsolationSelectionAlg.h>
#include <EgammaAnalysisAlgorithms/PhotonOriginCorrectionAlg.h>
#include <ElectronPhotonFourMomentumCorrection/EgammaCalibrationAndSmearingTool.h>
#include <ElectronPhotonSelectorTools/AsgDeadHVCellRemovalTool.h>
#include <EventSelectionAlgorithms/MissingETPlusTransverseMassSelectorAlg.h>
#include <EventSelectionAlgorithms/MissingETSelectorAlg.h>
#include <EventSelectionAlgorithms/NObjectPtSelectorAlg.h>
#include <EventSelectionAlgorithms/SaveFilterAlg.h>
#include <EventSelectionAlgorithms/TransverseMassSelectorAlg.h>
#include <GoodRunsLists/GoodRunsListSelectionTool.h>
#include <GoodRunsLists/GRLSelectorAlg.h>
#include <IsolationCorrections/IsolationCorrectionTool.h>
#include <IsolationSelection/IsolationSelectionTool.h>
#include <JetAnalysisAlgorithms/JetCalibrationAlg.h>
#include <JetAnalysisAlgorithms/JetDecoratorAlg.h>
#include <JetAnalysisAlgorithms/JetGhostMuonAssociationAlg.h>
#include <JetAnalysisAlgorithms/JetUncertaintiesAlg.h>
#include <JetCalibTools/JetCalibrationTool.h>
#include <JetJvtEfficiency/NNJvtSelectionTool.h>
#include <JetMomentTools/JetVertexNNTagger.h>
#include <JetUncertainties/JetUncertaintiesTool.h>
#include <METUtilities/METMaker.h>
#include <METUtilities/METSignificance.h>
#include <MetAnalysisAlgorithms/MetBuilderAlg.h>
#include <MetAnalysisAlgorithms/MetMakerAlg.h>
#include <MetAnalysisAlgorithms/MetSignificanceAlg.h>
#include <MuonAnalysisAlgorithms/MuonCalibrationAndSmearingAlg.h>
#include <MuonAnalysisAlgorithms/MuonIsolationAlg.h>
#include <MuonAnalysisAlgorithms/MuonSelectionAlg.h>
#include <MuonMomentumCorrections/MuonCalibIntSagittaTool.h>
#include <MuonMomentumCorrections/MuonCalibIntScaleSmearTool.h>
#include <MuonMomentumCorrections/MuonCalibTool.h>
#include <MuonSelectorTools/MuonSelectionTool.h>
#include <PileupReweighting/PileupReweightingTool.h>
#include <SelectionHelpers/SelectionNameSvc.h>
#include <SystematicsHandles/SystematicsSvc.h>
#include <TauAnalysisAlgorithms/TauSmearingAlg.h>
#include <TauAnalysisTools/CommonSmearingTool.h>
#include <TauAnalysisTools/TauSelectionTool.h>
#include <TauAnalysisTools/TauSmearingTool.h>
#include <TrackingAnalysisAlgorithms/VertexSelectionAlg.h>
#include <TrigConfxAOD/xAODConfigTool.h>
#include <TrigGlobalEfficiencyCorrection/TrigGlobalEfficiencyCorrectionTool.h>
#include <TriggerAnalysisAlgorithms/TrigEventSelectionAlg.h>
#include <TriggerAnalysisAlgorithms/TrigGlobalEfficiencyAlg.h>
#include <TriggerMatchingTool/DRScoringTool.h>
#include <TriggerMatchingTool/MatchFromCompositeTool.h>
#include <TrigDecisionTool/TrigDecisionTool.h>
#include <egammaMVACalib/egammaMVACalibTool.h>
#include <egammaMVACalib/egammaMVASvc.h>
#include <tauRecTools/TauCombinedTES.h>
#include <xAODBTaggingEfficiency/BTaggingSelectionTool.h>

#include <ParticleJetTools/JetPileupLabelingTool.h>
#include <JetAnalysisAlgorithms/JvtEfficiencyAlg.h>
#include <JetJvtEfficiency/NNJvtEfficiencyTool.h>
#include <AsgAnalysisAlgorithms/AsgEventScaleFactorAlg.h>
#include <FTagAnalysisAlgorithms/BTaggingEfficiencyAlg.h>
#include <xAODBTaggingEfficiency/BTaggingEfficiencyTool.h>
#include <FTagAnalysisAlgorithms/BTaggingInformationDecoratorAlg.h>
#include <EgammaAnalysisAlgorithms/ElectronEfficiencyCorrectionAlg.h>
#include <ElectronEfficiencyCorrection/AsgElectronEfficiencyCorrectionTool.h>
#include <AsgAnalysisAlgorithms/AsgClassificationDecorationAlg.h>
#include <TruthClassification/TruthClassificationTool.h>
#include <MuonAnalysisAlgorithms/MuonEfficiencyScaleFactorAlg.h>
#include <MuonEfficiencyCorrections/MuonEfficiencyScaleFactors.h>
#include <AsgAnalysisAlgorithms/AsgCutBookkeeperAlg.h>
#include <PMGTools/PMGTruthWeightTool.h>
#include <AsgAnalysisAlgorithms/PMGTruthWeightAlg.h>
#include <METUtilities/METSystematicsTool.h>
#include <MuonEfficiencyCorrections/MuonTriggerScaleFactors.h>
#include <AsgAnalysisAlgorithms/EventCutFlowHistAlg.h>
#include <EventSelectionAlgorithms/DileptonInvariantMassSelectorAlg.h>
#include <EventSelectionAlgorithms/ChargeSelectorAlg.h>
#include <EventSelectionAlgorithms/DileptonInvariantMassWindowSelectorAlg.h>
#include <TopCPToolkit/CARAlg.h>
#include <TopCPToolkit/JetMatchingAlg.h>
#include <TopCPToolkit/KLFitterAlg.h>
#include <TopCPToolkit/KLFitterFinalizeOutputAlg.h>
#include <TopCPToolkit/PartonHistoryToSpinInputAlg.h>
#include <TopCPToolkit/PartonToJetsMatchAlg.h>
#include <TopCPToolkit/ParticleLevelAlg.h>
#include <TopCPToolkit/RunPartonHistoryAlg.h>
#include <TopCPToolkit/RunSpaNetAlg.h>
#include <TopCPToolkit/RunTtbarNNLORecursiveRewAlg.h>
#include <TopCPToolkit/SinglelepHadronicChi2RecoAlg.h>
#include <TopCPToolkit/SoftMuonSelectorAlg.h>
#include <TopCPToolkit/TopSpinDensityMatrixAlg.h>
#include <TopCPToolkit/TutorialAlg.h>

//
// method implementations
//

namespace top
{
  bool preloadComponentFactories ()
  {
    using namespace asg::msgComponentConfig;
    ANA_CHECK_SET_TYPE (bool);

    ANA_MSG_INFO ("preloading component factories");

    ANA_CHECK (asg::registerServiceFactory<egammaMVASvc> ("egammaMVASvc"));
    ANA_CHECK (asg::registerServiceFactory<CP::SystematicsSvc> ("CP::SystematicsSvc"));
    ANA_CHECK (asg::registerServiceFactory<CP::SelectionNameSvc> ("CP::SelectionNameSvc"));

    ANA_CHECK (asg::registerAlgorithmFactory<GRLSelectorAlg>("GRLSelectorAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::AsgLeptonTrackSelectionAlg>("CP::AsgLeptonTrackSelectionAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::AsgSelectionAlg>("CP::AsgSelectionAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::AsgShallowCopyAlg>("CP::AsgShallowCopyAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::AsgUnionPreselectionAlg>("CP::AsgUnionPreselectionAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::AsgUnionSelectionAlg>("CP::AsgUnionSelectionAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::AsgViewFromSelectionAlg>("CP::AsgViewFromSelectionAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::AsgxAODMetNTupleMakerAlg>("CP::AsgxAODMetNTupleMakerAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::AsgxAODNTupleMakerAlg>("CP::AsgxAODNTupleMakerAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::AsgEnergyDecoratorAlg>("CP::AsgEnergyDecoratorAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::AsgOriginalObjectLinkAlg>("CP::AsgOriginalObjectLinkAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::AsgPriorityDecorationAlg>("CP::AsgPriorityDecorationAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::EventSelectionByObjectFlagAlg>("CP::EventSelectionByObjectFlagAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::KinematicHistAlg>("CP::KinematicHistAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::CopyNominalSelectionAlg>("CP::CopyNominalSelectionAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::IOStatsAlg>("CP::IOStatsAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::FakeBkgCalculatorAlg>("CP::FakeBkgCalculatorAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::BootstrapGeneratorAlg>("CP::BootstrapGeneratorAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::EgammaCalibrationAndSmearingAlg>("CP::EgammaCalibrationAndSmearingAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::EgammaIsolationCorrectionAlg>("CP::EgammaIsolationCorrectionAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::EgammaIsolationSelectionAlg>("CP::EgammaIsolationSelectionAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::EventFlagSelectionAlg>("CP::EventFlagSelectionAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::EventStatusSelectionAlg>("CP::EventStatusSelectionAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::JetCalibrationAlg>("CP::JetCalibrationAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::JetDecoratorAlg>("CP::JetDecoratorAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::JetGhostMuonAssociationAlg>("CP::JetGhostMuonAssociationAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::JetUncertaintiesAlg>("CP::JetUncertaintiesAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::LeptonSFCalculatorAlg> ("CP::LeptonSFCalculatorAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::MetBuilderAlg>("CP::MetBuilderAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::MetMakerAlg>("CP::MetMakerAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::MetSignificanceAlg>("CP::MetSignificanceAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::MissingETPlusTransverseMassSelectorAlg>("CP::MissingETPlusTransverseMassSelectorAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::MissingETSelectorAlg>("CP::MissingETSelectorAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::MuonCalibrationAndSmearingAlg>("CP::MuonCalibrationAndSmearingAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::MuonIsolationAlg>("CP::MuonIsolationAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::MuonSelectionAlgV2>("CP::MuonSelectionAlgV2"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::NObjectPtSelectorAlg>("CP::NObjectPtSelectorAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::ObjectCutFlowHistAlg>("CP::ObjectCutFlowHistAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::OverlapRemovalAlg>("CP::OverlapRemovalAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::PhotonOriginCorrectionAlg>("CP::PhotonOriginCorrectionAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::PileupReweightingAlg> ("CP::PileupReweightingAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::SaveFilterAlg>("CP::SaveFilterAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::SystObjectLinkerAlg>("CP::SystObjectLinkerAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::TauSmearingAlg>("CP::TauSmearingAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::TransverseMassSelectorAlg>("CP::TransverseMassSelectorAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::TreeFillerAlg>("CP::TreeFillerAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::TreeMakerAlg>("CP::TreeMakerAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::TrigEventSelectionAlg>("CP::TrigEventSelectionAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::TrigGlobalEfficiencyAlg>("CP::TrigGlobalEfficiencyAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::VertexSelectionAlg>("CP::VertexSelectionAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::JvtEfficiencyAlg> ("CP::JvtEfficiencyAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::AsgEventScaleFactorAlg> ("CP::AsgEventScaleFactorAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::BTaggingEfficiencyAlg> ("CP::BTaggingEfficiencyAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::BTaggingInformationDecoratorAlg> ("CP::BTaggingInformationDecoratorAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::ElectronEfficiencyCorrectionAlg> ("CP::ElectronEfficiencyCorrectionAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::AsgClassificationDecorationAlg> ("CP::AsgClassificationDecorationAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::MuonEfficiencyScaleFactorAlg> ("CP::MuonEfficiencyScaleFactorAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::AsgCutBookkeeperAlg> ("CP::AsgCutBookkeeperAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::PMGTruthWeightAlg> ("CP::PMGTruthWeightAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::EventCutFlowHistAlg> ("CP::EventCutFlowHistAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::DileptonInvariantMassSelectorAlg> ("CP::DileptonInvariantMassSelectorAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::ChargeSelectorAlg> ("CP::ChargeSelectorAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::DileptonInvariantMassWindowSelectorAlg> ("CP::DileptonInvariantMassWindowSelectorAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<CP::SysListDumperAlg> ("CP::SysListDumperAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<top::CARAlg> ("top::CARAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<top::JetMatchingAlg> ("top::JetMatchingAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<top::KLFitterAlg> ("top::KLFitterAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<top::KLFitterFinalizeOutputAlg> ("top::KLFitterFinalizeOutputAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<top::PartonHistoryToSpinInputAlg> ("top::PartonHistoryToSpinInputAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<top::PartonToJetsMatchAlg> ("top::PartonToJetsMatchAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<top::ParticleLevelAlg> ("top::ParticleLevelAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<top::RunPartonHistoryAlg> ("top::RunPartonHistoryAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<top::RunSpaNetAlg> ("top::RunSpaNetAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<top::RunTtbarNNLORecursiveRewAlg> ("top::RunTtbarNNLORecursiveRewAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<top::SinglelepHadronicChi2RecoAlg> ("top::SinglelepHadronicChi2RecoAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<top::SoftMuonSelectorAlg> ("top::SoftMuonSelectorAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<top::TopSpinDensityMatrixAlg> ("top::TopSpinDensityMatrixAlg"));
    ANA_CHECK (asg::registerAlgorithmFactory<top::TutorialAlg> ("top::TutorialAlg"));

    ANA_CHECK (asg::registerToolFactory<AsgDeadHVCellRemovalTool> ("AsgDeadHVCellRemovalTool"));
    ANA_CHECK (asg::registerToolFactory<BTaggingSelectionTool> ("BTaggingSelectionTool"));
    ANA_CHECK (asg::registerToolFactory<GoodRunsListSelectionTool> ("GoodRunsListSelectionTool"));
    ANA_CHECK (asg::registerToolFactory<JetCalibrationTool> ("JetCalibrationTool"));
    ANA_CHECK (asg::registerToolFactory<JetUncertaintiesTool> ("JetUncertaintiesTool"));
    ANA_CHECK (asg::registerToolFactory<TauCombinedTES> ("TauCombinedTES"));
    ANA_CHECK (asg::registerToolFactory<CP::MuonTriggerScaleFactors> ("CP::MuonTriggerScaleFactors"));
    ANA_CHECK (asg::registerToolFactory<TrigGlobalEfficiencyCorrectionTool> ("TrigGlobalEfficiencyCorrectionTool"));
    ANA_CHECK (asg::registerToolFactory<egammaMVACalibTool> ("egammaMVACalibTool"));
    ANA_CHECK (asg::registerToolFactory<CP::AsgFlagSelectionTool> ("CP::AsgFlagSelectionTool"));
    ANA_CHECK (asg::registerToolFactory<CP::AsgPtEtaSelectionTool> ("CP::AsgPtEtaSelectionTool"));
    ANA_CHECK (asg::registerToolFactory<CP::EgammaIsGoodOQSelectionTool> ("CP::EgammaIsGoodOQSelectionTool"));
    ANA_CHECK (asg::registerToolFactory<CP::EgammaCalibrationAndSmearingTool> ("CP::EgammaCalibrationAndSmearingTool"));
    ANA_CHECK (asg::registerToolFactory<CP::IsolationCorrectionTool> ("CP::IsolationCorrectionTool"));
    ANA_CHECK (asg::registerToolFactory<CP::IsolationSelectionTool> ("CP::IsolationSelectionTool"));
    ANA_CHECK (asg::registerToolFactory<CP::MuonCalibIntSagittaTool> ("CP::MuonCalibIntSagittaTool"));
    ANA_CHECK (asg::registerToolFactory<CP::MuonCalibIntScaleSmearTool> ("CP::MuonCalibIntScaleSmearTool"));
    ANA_CHECK (asg::registerToolFactory<CP::MuonCalibTool> ("CP::MuonCalibTool"));
    ANA_CHECK (asg::registerToolFactory<CP::MuonSelectionTool> ("CP::MuonSelectionTool"));
    ANA_CHECK (asg::registerToolFactory<CP::NNJvtSelectionTool> ("CP::NNJvtSelectionTool"));
    ANA_CHECK (asg::registerToolFactory<CP::PileupReweightingTool> ("CP::PileupReweightingTool"));
    ANA_CHECK (asg::registerToolFactory<JetPileupTag::JetVertexNNTagger> ("JetPileupTag::JetVertexNNTagger"));
    ANA_CHECK (asg::registerToolFactory<ORUtils::DeltaROverlapTool> ("ORUtils::DeltaROverlapTool"));
    ANA_CHECK (asg::registerToolFactory<ORUtils::EleJetOverlapTool> ("ORUtils::EleJetOverlapTool"));
    ANA_CHECK (asg::registerToolFactory<ORUtils::EleMuSharedTrkOverlapTool> ("ORUtils::EleMuSharedTrkOverlapTool"));
    ANA_CHECK (asg::registerToolFactory<ORUtils::MuJetOverlapTool> ("ORUtils::MuJetOverlapTool"));
    ANA_CHECK (asg::registerToolFactory<ORUtils::OverlapRemovalTool> ("ORUtils::OverlapRemovalTool"));
    ANA_CHECK (asg::registerToolFactory<TauAnalysisTools::CommonSmearingTool> ("TauAnalysisTools::CommonSmearingTool"));
    ANA_CHECK (asg::registerToolFactory<TauAnalysisTools::TauSelectionTool> ("TauAnalysisTools::TauSelectionTool"));
    ANA_CHECK (asg::registerToolFactory<TauAnalysisTools::TauSmearingTool> ("TauAnalysisTools::TauSmearingTool"));
    ANA_CHECK (asg::registerToolFactory<Trig::DRScoringTool> ("Trig::DRScoringTool"));
    ANA_CHECK (asg::registerToolFactory<Trig::MatchFromCompositeTool> ("Trig::MatchFromCompositeTool"));
    ANA_CHECK (asg::registerToolFactory<Trig::TrigDecisionTool> ("Trig::TrigDecisionTool"));
    ANA_CHECK (asg::registerToolFactory<TrigConf::xAODConfigTool> ("TrigConf::xAODConfigTool"));
    ANA_CHECK (asg::registerToolFactory<met::METMaker> ("met::METMaker"));
    ANA_CHECK (asg::registerToolFactory<met::METSignificance> ("met::METSignificance"));
    ANA_CHECK (asg::registerToolFactory<JetPileupLabelingTool> ("JetPileupLabelingTool"));
    ANA_CHECK (asg::registerToolFactory<CP::NNJvtEfficiencyTool> ("CP::NNJvtEfficiencyTool"));
    ANA_CHECK (asg::registerToolFactory<BTaggingEfficiencyTool> ("BTaggingEfficiencyTool"));
    ANA_CHECK (asg::registerToolFactory<AsgElectronEfficiencyCorrectionTool> ("AsgElectronEfficiencyCorrectionTool"));
    ANA_CHECK (asg::registerToolFactory<TruthClassificationTool> ("TruthClassificationTool"));
    ANA_CHECK (asg::registerToolFactory<CP::MuonEfficiencyScaleFactors> ("CP::MuonEfficiencyScaleFactors"));
    ANA_CHECK (asg::registerToolFactory<PMGTools::PMGTruthWeightTool> ("PMGTools::PMGTruthWeightTool"));
    ANA_CHECK (asg::registerToolFactory<met::METSystematicsTool> ("met::METSystematicsTool"));

    return true;
  }
}
