from AnalysisAlgorithmsConfig.ConfigBlock import ConfigBlock

class SoftMuonSelectorConfig(ConfigBlock):
    """Config Block for the SoftMuonSelector Algorithm"""

    def __init__(self):
        super(SoftMuonSelectorConfig, self).__init__()
        self.addOption('jets', None, type=str)
        self.addOption('softmuons', None, type=str) #Soft muons

        self.addOption('selection', "", type=str)

        self.addOption('softMuonDRJet', 0.4, type=float)

        self.addOption('saveSoftMuonAdditionalInfo', False, type=bool)
        self.addOption('saveSoftMuonNearestJetInfo', False, type=bool)

        self.addOption('softMuonDRJetUseRapidity', False, type=bool)

    def makeAlgs(self, config):
##        electrons = config.readName(self.electrons)
##        muons = config.readName(self.muons)
#        jets  = config.readName(self.jets)
##        met   = config.readName(self.met)
#        softmuons = config.readName(self.softmuons) #Soft muons


        alg = config.createAlgorithm('top::SoftMuonSelectorAlg', 'SoftMuonSelectorAlg')

        alg.softmuons, alg.softmuonSelection = config.readNameAndSelection(self.softmuons)
        alg.jets, alg.jetSelection = config.readNameAndSelection(self.jets)

##        alg.electrons = electrons
##        alg.muons = muons
#        alg.jets  = jets
##        alg.met   = met
#        alg.softmuons = softmuons #Soft muons
        alg.eventPreselection = self.selection

        # give appropriate names for the handles to decorate
        alg.SoftMuonDRJet              = self.softMuonDRJet
        alg.SaveSoftMuonAdditionalInfo = self.saveSoftMuonAdditionalInfo
        alg.SaveSoftMuonNearestJetInfo = self.saveSoftMuonNearestJetInfo
        alg.SoftMuonDRJetUseRapidity   = self.softMuonDRJetUseRapidity
#        alg.SoftMuonJetDRmin     = 'SoftMuonJetDRmin_%SYS%'
#        alg.SoftMuonPassDRJetcut = 'SoftMuonPassDRJetcut_%SYS%'

        config.addOutputVar(self.softmuons.split('.')[0], 'SoftMuonJetDRmin_%SYS%', 'SoftMuonJetDRmin')
        config.addOutputVar(self.softmuons.split('.')[0], 'SoftMuonPassDRJetcut_%SYS%', 'SoftMuonPassDRJetcut')

        config.addOutputVar('EventInfo', 'pass_SoftMuonPassDRJetcut_%SYS%', 'pass_SoftMuonPassDRJetcut' )


        # Additional information to save whether some flags are activated or not in the YAML file

        if self.saveSoftMuonAdditionalInfo:

          # config.addOutputVar(self.softmuons.split('.')[0], "softmu_pass_tight_%SYS%", "softmu_pass_tight");
          # config.addOutputVar(self.softmuons.split('.')[0], "softmu_resolutionCategory_%SYS%", "softmu_resolutionCategory");

          config.addOutputVar(self.softmuons.split('.')[0], "softmu_pt_id_%SYS%", "softmu_pt_id");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_eta_id_%SYS%", "softmu_eta_id");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_phi_id_%SYS%", "softmu_phi_id");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_e_id_%SYS%", "softmu_e_id");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_pt_me_%SYS%", "softmu_pt_me");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_eta_me_%SYS%", "softmu_eta_me");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_phi_me_%SYS%", "softmu_phi_me");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_e_me_%SYS%", "softmu_e_me");

          config.addOutputVar(self.softmuons.split('.')[0], "softmu_qOverP_%SYS%", "softmu_qOverP");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_qOverP_ID_%SYS%", "softmu_qOverP_ID");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_qOverP_ME_%SYS%", "softmu_qOverP_ME");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_qOverPerr_%SYS%", "softmu_qOverPerr");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_qOverPerr_ID_%SYS%", "softmu_qOverPerr_ID");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_qOverPerr_ME_%SYS%", "softmu_qOverPerr_ME");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_z0_sintheta_%SYS%", "softmu_z0_sintheta");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_d0_%SYS%", "softmu_d0");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_d0sig_%SYS%", "softmu_d0sig");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_reducedChi2_%SYS%", "softmu_reducedChi2");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_reducedChi2_ID_%SYS%", "softmu_reducedChi2_ID");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_reducedChi2_ME_%SYS%", "softmu_reducedChi2_ME");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_reducedSegmentChi2_%SYS%", "softmu_reducedSegmentChi2");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_momentumBalanceSignificance_%SYS%", "softmu_momentumBalanceSignificance");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_scatteringCurvatureSignificance_%SYS%", "softmu_scatteringCurvatureSignificance");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_scatteringNeighbourSignificance_%SYS%", "softmu_scatteringNeighbourSignificance");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_qOverPSignificance_%SYS%", "softmu_qOverPSignificance");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_rhoPrime_%SYS%", "softmu_rhoPrime");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_spectrometerFieldIntegral_%SYS%", "softmu_spectrometerFieldIntegral");

          config.addOutputVar(self.softmuons.split('.')[0], "softmu_nprecisionLayers_%SYS%", "softmu_nprecisionLayers");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_nprecisionHoleLayers_%SYS%", "softmu_nprecisionHoleLayers");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_nGoodPrecLayers_%SYS%", "softmu_nGoodPrecLayers");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_innerSmallHits_%SYS%", "softmu_innerSmallHits");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_innerLargeHits_%SYS%", "softmu_innerLargeHits");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_middleSmallHits_%SYS%", "softmu_middleSmallHits");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_middleLargeHits_%SYS%", "softmu_middleLargeHits");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_outerSmallHits_%SYS%", "softmu_outerSmallHits");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_outerLargeHits_%SYS%", "softmu_outerLargeHits");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_extendedSmallHits_%SYS%", "softmu_extendedSmallHits");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_extendedLargeHits_%SYS%", "softmu_extendedLargeHits");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_extendedSmallHoles_%SYS%", "softmu_extendedSmallHoles");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_isSmallGoodSectors_%SYS%", "softmu_isSmallGoodSectors");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_cscUnspoiledEtaHits_%SYS%", "softmu_cscUnspoiledEtaHits");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_numberOfPixelHits_%SYS%", "softmu_numberOfPixelHits");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_numberOfPixelDeadSensors_%SYS%", "softmu_numberOfPixelDeadSensors");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_numberOfPixelHoles_%SYS%", "softmu_numberOfPixelHoles");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_numberOfSCTHits_%SYS%", "softmu_numberOfSCTHits");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_numberOfSCTDeadSensors_%SYS%", "softmu_numberOfSCTDeadSensors");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_numberOfSCTHoles_%SYS%", "softmu_numberOfSCTHoles");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_numberOfTRTHits_%SYS%", "softmu_numberOfTRTHits");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_numberOfTRTOutliers_%SYS%", "softmu_numberOfTRTOutliers");

          config.addOutputVar(self.softmuons.split('.')[0], "softmu_ptcone20_Nonprompt_All_MaxWeightTTVA_pt500_%SYS%", "softmu_ptcone20_Nonprompt_All_MaxWeightTTVA_pt500");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt500_%SYS%", "softmu_ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt500");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_ptcone20_Nonprompt_All_MaxWeightTTVA_pt1000_%SYS%", "softmu_ptcone20_Nonprompt_All_MaxWeightTTVA_pt1000");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt1000_%SYS%", "softmu_ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt1000");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_neflowisol20_%SYS%", "softmu_neflowisol20");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_EnergyLoss_%SYS%", "softmu_EnergyLoss");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_EnergyLossSigma_%SYS%", "softmu_EnergyLossSigma");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_EnergyLossType_%SYS%", "softmu_EnergyLossType");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_ParamEnergyLoss_%SYS%", "softmu_ParamEnergyLoss");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_MeasEnergyLoss_%SYS%", "softmu_MeasEnergyLoss");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_ParamEnergyLossSigmaPlus_%SYS%", "softmu_ParamEnergyLossSigmaPlus");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_ParamEnergyLossSigmaMinus_%SYS%", "softmu_ParamEnergyLossSigmaMinus");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_MeasEnergyLossSigma_%SYS%", "softmu_MeasEnergyLossSigma");

          config.addOutputVar(self.softmuons.split('.')[0], "IdMsPt_%SYS%", "IdMsPt");
          config.addOutputVar(self.softmuons.split('.')[0], "qOverPsignificance_%SYS%", "qOverPsignificance");
          config.addOutputVar(self.softmuons.split('.')[0], "rhoPrime_%SYS%", "rhoPrime");

        if self.saveSoftMuonNearestJetInfo:
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_nearestJet_Index_%SYS%", "softmu_nearestJet_Index");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_nearestJet_pt_%SYS%", "softmu_nearestJet_pt");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_nearestJet_eta_%SYS%", "softmu_nearestJet_eta");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_nearestJet_phi_%SYS%", "softmu_nearestJet_phi");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_nearestJet_e_%SYS%", "softmu_nearestJet_e");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_nearestJet_dR_%SYS%", "softmu_nearestJet_dR");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_nearestJet_EMFrac_%SYS%", "softmu_nearestJet_EMFrac");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_nearestJet_NumTrkPt500_%SYS%", "softmu_nearestJet_NumTrkPt500");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_nearestJet_SumPtTrkPt500_%SYS%", "softmu_nearestJet_SumPtTrkPt500");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_nearestJet_NumTrkPt1000_%SYS%", "softmu_nearestJet_NumTrkPt1000");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_nearestJet_SumPtTrkPt1000_%SYS%", "softmu_nearestJet_SumPtTrkPt1000");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_nearestJet_N90Constituents_%SYS%", "softmu_nearestJet_N90Constituents");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_nearestJet_TrackWidthPt500_%SYS%", "softmu_nearestJet_TrackWidthPt500");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_nearestJet_TrackWidthPt1000_%SYS%", "softmu_nearestJet_TrackWidthPt1000");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_nearestJet_Width_%SYS%", "softmu_nearestJet_Width");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_nearestJet_Charge_%SYS%", "softmu_nearestJet_Charge");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_nearestJet_ChargedPFOWidthPt500_%SYS%", "softmu_nearestJet_ChargedPFOWidthPt500");
          config.addOutputVar(self.softmuons.split('.')[0], "softmu_nearestJet_ChargedPFOWidthPt1000_%SYS%", "softmu_nearestJet_ChargedPFOWidthPt1000");
