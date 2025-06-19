from AnalysisAlgorithmsConfig.ConfigBlock import ConfigBlock


class SoftMuonSelectorConfig(ConfigBlock):
    """Config Block for the SoftMuonSelector Algorithm"""

    def __init__(self):
        super(SoftMuonSelectorConfig, self).__init__()
        self.addOption("jets", None, type=str)
        self.addOption("softmuons", None, type=str)  # Soft muons
        self.addOption("selection", "", type=str)
        self.addOption("softMuonDRJet", 0.4, type=float)
        self.addOption("saveSoftMuonAdditionalInfo", False, type=bool)
        self.addOption("saveSoftMuonNearestJetInfo", False, type=bool)
        self.addOption("softMuonDRJetUseRapidity", False, type=bool)
        self.addOption("saveSoftMuonTruthInfo", False, type=bool)

    def makeAlgs(self, config):
        alg = config.createAlgorithm("top::SoftMuonSelectorAlg", "SoftMuonSelectorAlg")

        alg.softmuons, alg.softmuonSelection = config.readNameAndSelection(
            self.softmuons
        )
        alg.jets, alg.jetSelection = config.readNameAndSelection(self.jets)
        alg.eventPreselection = self.selection

        # give appropriate names for the handles to decorate
        alg.SoftMuonDRJet = self.softMuonDRJet
        alg.SaveSoftMuonAdditionalInfo = self.saveSoftMuonAdditionalInfo
        alg.SaveSoftMuonNearestJetInfo = self.saveSoftMuonNearestJetInfo
        alg.SoftMuonDRJetUseRapidity = self.softMuonDRJetUseRapidity
        from AnalysisAlgorithmsConfig.ConfigAccumulator import DataType

        isMC = not (config.dataType() == DataType.Data)
        alg.SaveSoftMuonTruthInfo = isMC and self.saveSoftMuonTruthInfo

        from AthenaConfiguration.Enums import LHCPeriod

        alg.isRun3 = config.geometry() == LHCPeriod.Run3

        config.addPrivateTool("SoftMuonSelTool_Tight", "CP::MuonSelectionTool")
        alg.SoftMuonSelTool_Tight.MaxEta = 2.7
        alg.SoftMuonSelTool_Tight.MuQuality = 0

        alg.SoftMuonSelTool_Tight.IsRun3Geo = alg.isRun3

        config.addOutputVar(
            self.softmuons.split(".")[0], "SoftMuonJetDRmin_%SYS%", "SoftMuonJetDRmin"
        )
        config.addSelection(
            self.softmuons.split(".")[0],
            self.softmuons.split(".")[0],
            "SoftMuonPassDRJetcut_%SYS%",
        )

        config.addOutputVar(
            "EventInfo", "pass_SoftMuonPassDRJetcut_%SYS%", "pass_SoftMuonPassDRJetcut"
        )
        # Additional information to save whether some flags are activated or not in the YAML file

        if self.saveSoftMuonAdditionalInfo:
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "author_%SYS%",
                "author",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "allAuthors_%SYS%",
                "allAuthors",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "resolutionCategory_%SYS%",
                "resolutionCategory",
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "passedIDCuts_%SYS%",
                "passedIDCuts",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "passedHighPtCuts_%SYS%",
                "passedHighPtCuts",
            )
            config.addOutputVar(
                self.softmuons.split(".")[0], "passedLowPtCuts_%SYS%", "passedLowPtCuts"
            )
            config.addOutputVar(
                self.softmuons.split(".")[0], "isBadMuon_%SYS%", "isBadMuon"
            )
            config.addOutputVar(self.softmuons.split(".")[0], "pt_id_%SYS%", "pt_id")
            config.addOutputVar(
                self.softmuons.split(".")[0], "eta_id_%SYS%", "eta_id", noSys=True
            )
            config.addOutputVar(
                self.softmuons.split(".")[0], "phi_id_%SYS%", "phi_id", noSys=True
            )
            config.addOutputVar(self.softmuons.split(".")[0], "e_id_%SYS%", "e_id")
            config.addOutputVar(self.softmuons.split(".")[0], "pt_me_%SYS%", "pt_me")
            config.addOutputVar(
                self.softmuons.split(".")[0], "eta_me_%SYS%", "eta_me", noSys=True
            )
            config.addOutputVar(
                self.softmuons.split(".")[0], "phi_me_%SYS%", "phi_me", noSys=True
            )
            config.addOutputVar(self.softmuons.split(".")[0], "e_me_%SYS%", "e_me")

            config.addOutputVar(self.softmuons.split(".")[0], "qOverP_%SYS%", "qOverP")
            config.addOutputVar(
                self.softmuons.split(".")[0], "qOverP_ID_%SYS%", "qOverP_ID"
            )
            config.addOutputVar(
                self.softmuons.split(".")[0], "qOverP_ME_%SYS%", "qOverP_ME"
            )
            config.addOutputVar(
                self.softmuons.split(".")[0], "qOverPerr_%SYS%", "qOverPerr"
            )
            config.addOutputVar(
                self.softmuons.split(".")[0], "qOverPerr_ID_%SYS%", "qOverPerr_ID"
            )
            config.addOutputVar(
                self.softmuons.split(".")[0], "qOverPerr_ME_%SYS%", "qOverPerr_ME"
            )
            config.addOutputVar(
                self.softmuons.split(".")[0], "d0_%SYS%", "d0", noSys=True
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "reducedChi2_%SYS%",
                "reducedChi2",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "reducedChi2_ID_%SYS%",
                "reducedChi2_ID",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "reducedChi2_ME_%SYS%",
                "reducedChi2_ME",
                noSys=True,
            )
            # not currently available in DAOD_PHYS
            # config.addOutputVar(
            #    self.softmuons.split(".")[0],
            #    "reducedSegmentChi2_%SYS%",
            #    "reducedSegmentChi2",
            #    noSys=True,
            # )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "momentumBalanceSignificance_%SYS%",
                "momentumBalanceSignificance",
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "scatteringCurvatureSignificance_%SYS%",
                "scatteringCurvatureSignificance",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "scatteringNeighbourSignificance_%SYS%",
                "scatteringNeighbourSignificance",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "qOverPSignificance_%SYS%",
                "qOverPSignificance",
            )
            config.addOutputVar(
                self.softmuons.split(".")[0], "rhoPrime_%SYS%", "rhoPrime"
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "segmentDeltaEta_%SYS%",
                "segmentDeltaEta",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "spectrometerFieldIntegral_%SYS%",
                "spectrometerFieldIntegral",
                noSys=True,
            )

            config.addOutputVar(
                self.softmuons.split(".")[0],
                "nprecisionLayers_%SYS%",
                "nprecisionLayers",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "nprecisionHoleLayers_%SYS%",
                "nprecisionHoleLayers",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "nGoodPrecLayers_%SYS%",
                "nGoodPrecLayers",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "innerSmallHits_%SYS%",
                "innerSmallHits",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "innerLargeHits_%SYS%",
                "innerLargeHits",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "innerSmallHoles_%SYS%",
                "innerSmallHoles",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "innerLargeHoles_%SYS%",
                "innerLargeHoles",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "middleSmallHits_%SYS%",
                "middleSmallHits",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "middleLargeHits_%SYS%",
                "middleLargeHits",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "middleSmallHoles_%SYS%",
                "middleSmallHoles",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "middleLargeHoles_%SYS%",
                "middleLargeHoles",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "outerSmallHits_%SYS%",
                "outerSmallHits",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "outerLargeHits_%SYS%",
                "outerLargeHits",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "outerSmallHoles_%SYS%",
                "outerSmallHoles",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "outerLargeHoles_%SYS%",
                "outerLargeHoles",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "extendedSmallHits_%SYS%",
                "extendedSmallHits",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "extendedLargeHits_%SYS%",
                "extendedLargeHits",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "extendedSmallHoles_%SYS%",
                "extendedSmallHoles",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "extendedLargeHoles_%SYS%",
                "extendedLargeHoles",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "innerClosePrecisionHits_%SYS%",
                "innerClosePrecisionHits",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "middleClosePrecisionHits_%SYS%",
                "middleClosePrecisionHits",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "outerClosePrecisionHits_%SYS%",
                "outerClosePrecisionHits",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "extendedClosePrecisionHits_%SYS%",
                "extendedClosePrecisionHits",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "innerOutBoundsPrecisionHits_%SYS%",
                "innerOutBoundsPrecisionHits",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "middleOutBoundsPrecisionHits_%SYS%",
                "middleOutBoundsPrecisionHits",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "outerOutBoundsPrecisionHits_%SYS%",
                "outerOutBoundsPrecisionHits",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "extendedOutBoundsPrecisionHits_%SYS%",
                "extendedOutBoundsPrecisionHits",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "combinedTrackOutBoundsPrecisionHits_%SYS%",
                "combinedTrackOutBoundsPrecisionHits",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "isSmallGoodSectors_%SYS%",
                "isSmallGoodSectors",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "isEndcapGoodLayers_%SYS%",
                "isEndcapGoodLayers",
                noSys=True,
            )
            # Run-2 specific variables
            if not config.geometry() == LHCPeriod.Run3:
                config.addOutputVar(
                    self.softmuons.split(".")[0],
                    "cscUnspoiledEtaHits_%SYS%",
                    "cscUnspoiledEtaHits",
                    noSys=True,
                )
                # end of Run-2 specific variables
            # Run-3 specific variables
            if config.geometry() == LHCPeriod.Run3:
                config.addOutputVar(
                    self.softmuons.split(".")[0],
                    "etaLayer1STGCHits_%SYS%",
                    "etaLayer1STGCHits",
                    noSys=True,
                )
                config.addOutputVar(
                    self.softmuons.split(".")[0],
                    "etaLayer2STGCHits_%SYS%",
                    "etaLayer2STGCHits",
                    noSys=True,
                )
                config.addOutputVar(
                    self.softmuons.split(".")[0],
                    "phiLayer1STGCHits_%SYS%",
                    "phiLayer1STGCHits",
                    noSys=True,
                )
                config.addOutputVar(
                    self.softmuons.split(".")[0],
                    "phiLayer2STGCHits_%SYS%",
                    "phiLayer2STGCHits",
                    noSys=True,
                )
                config.addOutputVar(
                    self.softmuons.split(".")[0],
                    "MMHits_%SYS%",
                    "MMHits",
                    noSys=True,
                )
                # end of Run-3 specific variables
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "numberOfPixelHits_%SYS%",
                "numberOfPixelHits",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "numberOfPixelDeadSensors_%SYS%",
                "numberOfPixelDeadSensors",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "numberOfPixelHoles_%SYS%",
                "numberOfPixelHoles",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "numberOfSCTHits_%SYS%",
                "numberOfSCTHits",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "numberOfSCTDeadSensors_%SYS%",
                "numberOfSCTDeadSensors",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "numberOfSCTHoles_%SYS%",
                "numberOfSCTHoles",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "numberOfTRTHits_%SYS%",
                "numberOfTRTHits",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "numberOfTRTOutliers_%SYS%",
                "numberOfTRTOutliers",
                noSys=True,
            )
            # this is currently not available in PHYS derivation
            # config.addOutputVar(
            #    self.softmuons.split(".")[0],
            #    "numberOfTRTHoles_%SYS%",
            #    "numberOfTRTHoles",
            #    noSys=True,
            # )

            config.addOutputVar(
                self.softmuons.split(".")[0],
                "isBIS78_%SYS%",
                "isBIS78",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "isBEE_%SYS%",
                "isBEE",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "isBMG_%SYS%",
                "isBMG",
                noSys=True,
            )

            config.addOutputVar(
                self.softmuons.split(".")[0],
                "ptcone20_Nonprompt_All_MaxWeightTTVA_pt500_%SYS%",
                "ptcone20_Nonprompt_All_MaxWeightTTVA_pt500",
                noSys=True,
            )
            # is it right that we have no unc here?
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt500_%SYS%",
                "ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt500",
                noSys=True,
            )
            # is it right that we have no unc here?
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "ptcone20_Nonprompt_All_MaxWeightTTVA_pt1000_%SYS%",
                "ptcone20_Nonprompt_All_MaxWeightTTVA_pt1000",
                noSys=True,
            )
            # is it right that we have no unc here?
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt1000_%SYS%",
                "ptvarcone30_Nonprompt_All_MaxWeightTTVA_pt1000",
                noSys=True,
            )
            # is it right that we have no unc here?
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "neflowisol20_%SYS%",
                "neflowisol20",
                noSys=True,
            )
            # is it right that we have no unc here?
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "EnergyLoss_%SYS%",
                "EnergyLoss",
                noSys=True,
            )
            # currently not available in DAOD_PHYS
            # config.addOutputVar(
            # self.softmuons.split(".")[0],
            # "EnergyLossSigma_%SYS%",
            # "EnergyLossSigma",
            # noSys=True,
            # )
            # is it right that we have no unc here?
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "EnergyLossType_%SYS%",
                "EnergyLossType",
                noSys=True,
            )
            # currently not available in DAOD_PHYS
            # config.addOutputVar(
            # self.softmuons.split(".")[0],
            # "ParamEnergyLoss_%SYS%",
            # "ParamEnergyLoss",
            # noSys=True,
            # )
            # config.addOutputVar(
            # self.softmuons.split(".")[0],
            # "MeasEnergyLoss_%SYS%",
            # "MeasEnergyLoss",
            # noSys=True,
            # )
            # config.addOutputVar(
            # self.softmuons.split(".")[0],
            # "ParamEnergyLossSigmaPlus_%SYS%",
            # "ParamEnergyLossSigmaPlus",
            # noSys=True,
            # )
            # config.addOutputVar(
            # self.softmuons.split(".")[0],
            # "ParamEnergyLossSigmaMinus_%SYS%",
            # "ParamEnergyLossSigmaMinus",
            # noSys=True,
            # )
            # config.addOutputVar(
            # self.softmuons.split(".")[0],
            # "MeasEnergyLossSigma_%SYS%",
            # "MeasEnergyLossSigma",
            # noSys=True,
            # )
            # is it right that we have no unc here?
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "nSegments_%SYS%",
                "nSegments",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "Seg1ChamberId_%SYS%",
                "Seg1ChamberId",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "Seg2ChamberId_%SYS%",
                "Seg2ChamberId",
                noSys=True,
            )
        # end of if self.saveSoftMuonAdditionalInfo
        if self.saveSoftMuonNearestJetInfo:
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "nearestJet_Index_%SYS%",
                "nearestJet_Index",
            )
            config.addOutputVar(
                self.softmuons.split(".")[0], "nearestJet_pt_%SYS%", "nearestJet_pt"
            )
            config.addOutputVar(
                self.softmuons.split(".")[0], "nearestJet_eta_%SYS%", "nearestJet_eta"
            )
            config.addOutputVar(
                self.softmuons.split(".")[0], "nearestJet_phi_%SYS%", "nearestJet_phi"
            )
            config.addOutputVar(
                self.softmuons.split(".")[0], "nearestJet_e_%SYS%", "nearestJet_e"
            )
            config.addOutputVar(
                self.softmuons.split(".")[0], "nearestJet_dR_%SYS%", "nearestJet_dR"
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "nearestJet_EMFrac_%SYS%",
                "nearestJet_EMFrac",
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "nearestJet_NumTrkPt500_%SYS%",
                "nearestJet_NumTrkPt500",
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "nearestJet_SumPtTrkPt500_%SYS%",
                "nearestJet_SumPtTrkPt500",
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "nearestJet_NumTrkPt1000_%SYS%",
                "nearestJet_NumTrkPt1000",
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "nearestJet_SumPtTrkPt1000_%SYS%",
                "nearestJet_SumPtTrkPt1000",
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "nearestJet_N90Constituents_%SYS%",
                "nearestJet_N90Constituents",
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "nearestJet_TrackWidthPt500_%SYS%",
                "nearestJet_TrackWidthPt500",
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "nearestJet_TrackWidthPt1000_%SYS%",
                "nearestJet_TrackWidthPt1000",
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "nearestJet_Width_%SYS%",
                "nearestJet_Width",
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "nearestJet_Charge_%SYS%",
                "nearestJet_Charge",
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "nearestJet_ChargedPFOWidthPt500_%SYS%",
                "nearestJet_ChargedPFOWidthPt500",
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "nearestJet_ChargedPFOWidthPt1000_%SYS%",
                "nearestJet_ChargedPFOWidthPt1000",
            )
        # end of if self.saveSoftMuonNearestJetInfo:
        if self.saveSoftMuonTruthInfo:
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "truthmuon_barcode_%SYS%",
                "truthmuon_barcode_pdgId",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "truthBAncestor_pdgId_%SYS%",
                "truthBAncestor_pdgId",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "truthBAncestor_pt_%SYS%",
                "truthBAncestor_pt",
                noSys=True,
            )  # eta, phi, e also available but not written out at the moment
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "truthCAncestor_pdgId_%SYS%",
                "truthCAncestor_pdgId",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "truthCAncestor_pt_%SYS%",
                "truthCAncestor_pt",
                noSys=True,
            )  # eta, phi, e also available but not written out at the moment
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "truthTauAncestor_pdgId_%SYS%",
                "truthTauAncestor_pdgId",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "truthFirstBAncestor_pdgId_%SYS%",
                "truthFirstBAncestor_pdgId",
                noSys=True,
            )
            config.addOutputVar(
                self.softmuons.split(".")[0],
                "truthFirstCAncestor_pdgId_%SYS%",
                "truthFirstCAncestor_pdgId",
                noSys=True,
            )
        # end of if self.saveSoftMuonTruthInfo:
