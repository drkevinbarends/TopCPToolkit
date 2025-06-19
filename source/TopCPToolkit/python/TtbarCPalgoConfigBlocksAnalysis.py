from AnalysisAlgorithmsConfig.ConfigAccumulator import ConfigAccumulator


def makeRecoConfiguration(
    flags, algSeq, configSeq, factory, noSystematics=False, noFilter=False
):

    makeConfig = factory.makeConfig

    use_electrons = True
    use_muons = True
    use_jets = True
    use_photons = False
    use_taus = False
    use_MET = True
    use_track_jets = False
    use_largeR_jets = False

    outputContainers = {"": "EventInfo"}  # for output NTuple config block
    reco_branches = []
    met_branches = []

    # primary vertex ,event cleaning (jet clean loosebad) and GoodRunsList selection
    configSeq += makeConfig("EventCleaning")
    configSeq.setOptionValue(".runEventCleaning", True)

    # run PMG TruthWeightTool on MC only
    configSeq += makeConfig("GeneratorLevelAnalysis")

    # PRW
    if not flags.Input.isPHYSLITE:
        configSeq += makeConfig("PileupReweighting")

    # electrons
    if use_electrons:
        configSeq += makeConfig("Electrons", containerName="AnaElectrons")
        configSeq.setOptionValue(".crackVeto", True)

        WPLoose = ["TightLH", "NonIso"]  # no isolation
        WPTight = ["TightLH", "Tight_VarRad"]
        configSeq += makeConfig(
            "Electrons.WorkingPoint",
            containerName="AnaElectrons",
            selectionName="loose",
        )
        configSeq.setOptionValue(".identificationWP", WPLoose[0])
        configSeq.setOptionValue(".isolationWP", WPLoose[1])
        configSeq.setOptionValue(".noEffSF", True)
        configSeq += makeConfig(
            "Electrons.WorkingPoint",
            containerName="AnaElectrons",
            selectionName="tight",
        )
        configSeq.setOptionValue(".identificationWP", WPTight[0])
        configSeq.setOptionValue(".isolationWP", WPTight[1])
        configSeq.setOptionValue(".noEffSF", True)

        configSeq += makeConfig(
            "Electrons.PtEtaSelection", containerName="AnaElectrons"
        )
        configSeq.setOptionValue(".selectionDecoration", "selectPtEta")
        configSeq.setOptionValue(".minPt", 25e3)
        configSeq.setOptionValue(".maxEta", 2.47)
        configSeq.setOptionValue(".useClusterEta", True)

        outputContainers["el_"] = "OutElectrons"

    # muons
    if use_muons:
        configSeq += makeConfig("Muons", containerName="AnaMuons")

        WPLoose = ["Medium", "NonIso"]
        WPTight = ["Medium", "Tight_VarRad"]
        configSeq += makeConfig(
            "Muons.WorkingPoint", containerName="AnaMuons", selectionName="loose"
        )
        configSeq.setOptionValue(".quality", WPLoose[0])
        configSeq.setOptionValue(".isolation", WPLoose[1])
        configSeq += makeConfig(
            "Muons.WorkingPoint", containerName="AnaMuons", selectionName="tight"
        )
        configSeq.setOptionValue(".quality", WPTight[0])
        configSeq.setOptionValue(".isolation", WPTight[1])
        configSeq.setOptionValue(".systematicBreakdown", True)

        configSeq += makeConfig("Muons.PtEtaSelection", containerName="AnaMuons")
        configSeq.setOptionValue(".selectionDecoration", "selectPtEta")
        configSeq.setOptionValue(".minPt", 25e3)
        configSeq.setOptionValue(".maxEta", 2.5)

        outputContainers["mu_"] = "OutMuons"

    # small-R jets
    WPs = [
        "FixedCutBEff_90",
        "FixedCutBEff_85",
        "FixedCutBEff_77",
        "FixedCutBEff_70",
        "FixedCutBEff_65",
        "Continuous",
    ]
    btagger = "GN2v01"
    if use_jets:
        jetContainer = "AntiKt4EMPFlowJets"
        configSeq += makeConfig(
            "Jets", containerName="AnaJets", jetCollection=jetContainer
        )
        configSeq.setOptionValue(".runGhostMuonAssociation", True)
        configSeq.setOptionValue(".runNNJvtUpdate", True)
        configSeq.setOptionValue(".systematicsModelJER", "Full")
        configSeq.setOptionValue(".systematicsModelJES", "Category")

        configSeq += makeConfig("Jets.JVT", containerName="AnaJets")

        # b-tagging
        for WP in WPs:
            configSeq += makeConfig("Jets.FlavourTagging", containerName="AnaJets")
            configSeq.setOptionValue(".btagger", btagger)
            configSeq.setOptionValue(".btagWP", WP)
            # Scale factors are only available for the Continuous WP
            if WP == "Continuous":
                configSeq += makeConfig(
                    "Jets.FlavourTaggingEventSF", containerName="AnaJets.baselineJvt"
                )
                configSeq.setOptionValue(".btagger", btagger)
                configSeq.setOptionValue(".btagWP", WP)

        outputContainers["jet_"] = "OutJets"

        configSeq += makeConfig("Jets.PtEtaSelection", containerName="AnaJets")
        configSeq.setOptionValue(".selectionDecoration", "selectPtEta")
        configSeq.setOptionValue(".minPt", 25e3)
        configSeq.setOptionValue(".maxEta", 2.5)

    # TODO VR track jets b-tagging -- not yet in CDI ?
    if use_track_jets:
        jetContainer = "AntiKtVR30Rmax4Rmin02PV0TrackJets"
        configSeq += makeConfig(
            "Jets",
            containerName="AnaTrackJets",
            jetCollection=jetContainer,
            postfix="track_jets",
        )

        configSeq += makeConfig("Jets.PtEtaSelection", containerName="AnaTrackJets")
        configSeq.setOptionValue(".selectionDecoration", "selectPtEta")
        configSeq.setOptionValue(".selectionName", "track_jets")
        configSeq.setOptionValue(".minPt", 10e3)
        configSeq.setOptionValue(".maxEta", 2.5)

        outputContainers["track_jet_"] = "OutTrackJets"

    if use_largeR_jets:
        jetContainer = "AntiKt10UFOCSSKSoftDropBeta100Zcut10Jets"
        configSeq += makeConfig(
            "Jets",
            containerName="LargeRJets",
            jetCollection=jetContainer,
            postfix="largeR_jets",
        )

        configSeq += makeConfig("Jets.PtEtaSelection", containerName="LargeRJets")
        configSeq.setOptionValue(".selectionDecoration", "selectPtEta")
        configSeq.setOptionValue(".selectionName", "largeR_jets")
        configSeq.setOptionValue(".minPt", 350e3)
        configSeq.setOptionValue(".maxEta", 2.0)

        outputContainers["larger_jet_"] = "OutLargeRJets"

    # photons
    if use_photons:
        configSeq += makeConfig("Photons", containerName="AnaPhotons")

        WPLoose = ["Loose", "NonIso"]
        WPTight = ["Tight", "FixedCutTight"]
        configSeq += makeConfig(
            "Photons.WorkingPoint", containerName="AnaPhotons", selectionName="loose"
        )
        configSeq.setOptionValue(".qualityWP", WPLoose[0])
        configSeq.setOptionValue(".isolationWP", WPLoose[1])
        configSeq += makeConfig(
            "Photons.WorkingPoint", containerName="AnaPhotons", selectionName="tight"
        )
        configSeq.setOptionValue(".qualityWP", WPTight[0])
        configSeq.setOptionValue(".isolationWP", WPTight[1])

        configSeq += makeConfig("Photons.PtEtaSelection", containerName="AnaPhotons")
        configSeq.setOptionValue(".selectionDecoration", "selectPtEta")
        configSeq.setOptionValue(".minPt", 25e3)
        configSeq.setOptionValue(".maxEta", 2.5)
        configSeq.setOptionValue(".useClusterEta", True)

        outputContainers["ph_"] = "OutPhotons"

    # Taus
    if use_taus:
        configSeq += makeConfig("TauJets", containerName="AnaTauJets")

        configSeq += makeConfig(
            "TauJets.WorkingPoint", containerName="AnaTauJets", selectionName="tight"
        )
        configSeq.setOptionValue(".quality", "Tight")

        configSeq += makeConfig("TauJets.PtEtaSelection", containerName="AnaTauJets")
        configSeq.setOptionValue(".selectionDecoration", "selectPtEta")
        configSeq.setOptionValue(".minPt", 25e3)
        configSeq.setOptionValue(".maxEta", 2.5)

        outputContainers["tau_"] = "OutTauJets"

    # MET
    if use_MET:
        configSeq += makeConfig("MissingET", containerName="AnaMET")
        configSeq.setOptionValue(
            ".electrons", "AnaElectrons.tight" if use_electrons else ""
        )
        configSeq.setOptionValue(".muons", "AnaMuons.tight" if use_muons else "")
        configSeq.setOptionValue(
            ".jets", "AnaJets" if use_jets else ""
        )  # use all jets for MET, it does it's own OR
        configSeq.setOptionValue(
            ".photons", "AnaPhotons.tight&&selectPtEta" if use_photons else ""
        )
        configSeq.setOptionValue(".taus", "AnaTauJets.tight" if use_taus else "")

        outputContainers["met_"] = "AnaMET"

    # overlap removal
    configSeq += makeConfig("OverlapRemoval")
    configSeq.setOptionValue(
        ".electrons", "AnaElectrons.tight" if use_electrons else ""
    )
    configSeq.setOptionValue(".muons", "AnaMuons.tight" if use_muons else "")
    configSeq.setOptionValue(
        ".photons", "AnaPhotons.tight&&selectPtEta" if use_photons else ""
    )
    configSeq.setOptionValue(".jets", "AnaJets.baselineJvt" if use_jets else "")
    configSeq.setOptionValue(".taus", "AnaTauJets.tight" if use_taus else "")
    configSeq.setOptionValue(".inputLabel", "preselectOR")
    configSeq.setOptionValue(".outputLabel", "passesOR")

    # global lepton trigger
    triggerChainsPerYear = {
        "2015": [
            "HLT_e24_lhmedium_L1EM20VH || HLT_e60_lhmedium || HLT_e120_lhloose",
            "HLT_mu20_iloose_L1MU15 || HLT_mu40",
        ],
        "2016": [
            "HLT_e26_lhtight_nod0_ivarloose || HLT_e60_lhmedium_nod0 || HLT_e140_lhloose_nod0",
            "HLT_mu26_ivarmedium || HLT_mu50",
        ],
        "2017": [
            "HLT_e26_lhtight_nod0_ivarloose || HLT_e60_lhmedium_nod0 || HLT_e140_lhloose_nod0",
            "HLT_mu26_ivarmedium || HLT_mu50",
        ],
        "2018": [
            "HLT_e26_lhtight_nod0_ivarloose || HLT_e60_lhmedium_nod0 || HLT_e140_lhloose_nod0",
            "HLT_mu26_ivarmedium || HLT_mu50",
        ],
        "2022": [
            "HLT_e26_lhtight_ivarloose_L1EM22VHI || HLT_e60_lhmedium_L1EM22VHI || HLT_e140_lhloose_L1EM22VHI",
            "HLT_mu24_ivarmedium_L1MU14FCH || HLT_mu50_L1MU14FCH",
        ],
    }
    # individual_triggers = list(
    #     set(
    #         substring.strip()
    #         for chains_list in triggerChainsPerYear.values()
    #         for chain in chains_list
    #         for substring in chain.split("||")
    #     )
    # )
    configSeq += makeConfig("Trigger")
    configSeq.setOptionValue(".triggerChainsPerYear", triggerChainsPerYear)
    configSeq.setOptionValue(".noFilter", noFilter)
    configSeq.setOptionValue(".electronID", "Tight")
    configSeq.setOptionValue(".electronIsol", "Tight_VarRad")
    configSeq.setOptionValue(".muonID", "Medium")
    configSeq.setOptionValue(".electrons", "AnaElectrons.tight")
    configSeq.setOptionValue(".muons", "AnaMuons.tight")

    # object-based cutflow
    if use_electrons:
        configSeq += makeConfig(
            "ObjectCutFlow", containerName="AnaElectrons", selectionName="tight"
        )
    if use_muons:
        configSeq += makeConfig(
            "ObjectCutFlow", containerName="AnaMuons", selectionName="tight"
        )
    if use_taus:
        configSeq += makeConfig(
            "ObjectCutFlow", containerName="AnaTauJets", selectionName="tight"
        )
    if use_photons:
        configSeq += makeConfig(
            "ObjectCutFlow", containerName="AnaPhotons", selectionName="tight"
        )
    if use_jets:
        configSeq += makeConfig(
            "ObjectCutFlow", containerName="AnaJets", selectionName="baselineJvt"
        )
    if use_largeR_jets:
        configSeq += makeConfig(
            "ObjectCutFlow", containerName="LargeRJets", selectionName=""
        )
    if use_track_jets:
        configSeq += makeConfig(
            "ObjectCutFlow", containerName="AnaTrackJets", selectionName=""
        )

    # a single lepton SF
    configSeq += makeConfig("LeptonSF")
    # configSeq.setOptionValue ('.electrons', 'AnaElectrons.tight')
    configSeq.setOptionValue(".muons", "AnaMuons.tight")
    # configSeq.setOptionValue ('.photons', 'AnaPhotons.tight')
    configSeq.setOptionValue(".lepton_postfix", "tight")

    # the IFF lepton classification
    if use_electrons:
        configSeq += makeConfig(
            "Electrons.IFFClassification", containerName="AnaElectrons"
        )
    if use_muons:
        configSeq += makeConfig("Muons.IFFClassification", containerName="AnaMuons")

    # object thinning
    if use_electrons:
        # write out single electron container containg all leptons passing loose or tight and at the same time pass selectPtEta
        # i.e. there is a logical && between selection and selectionName
        configSeq += makeConfig("Thinning", containerName="AnaElectrons")
        configSeq.setOptionValue(".selectionName", "tight||loose")
        configSeq.setOptionValue(".outputName", "OutElectrons")
    if use_muons:
        configSeq += makeConfig("Thinning", containerName="AnaMuons")
        configSeq.setOptionValue(".selectionName", "tight||loose")
        configSeq.setOptionValue(".outputName", "OutMuons")
    if use_jets:
        configSeq += makeConfig("Thinning", containerName="AnaJets")
        configSeq.setOptionValue(".selectionName", "baselineJvt")
        configSeq.setOptionValue(".outputName", "OutJets")
    if use_photons:
        configSeq += makeConfig("Thinning", containerName="AnaPhotons")
        configSeq.setOptionValue(".selectionName", "tight||loose")
        configSeq.setOptionValue(".outputName", "OutPhotons")
    if use_taus:
        configSeq += makeConfig("Thinning", containerName="AnaTauJets")
        configSeq.setOptionValue(".selectionName", "tight")
        configSeq.setOptionValue(".outputName", "OutTauJets")
    if use_largeR_jets:
        configSeq += makeConfig("Thinning", containerName="LargeRJets")
        configSeq.setOptionValue(".selectionName", "largeR_jets")
        configSeq.setOptionValue(".outputName", "OutLargeRJets")
    if use_track_jets:
        configSeq += makeConfig("Thinning", containerName="AnaTrackJets")
        configSeq.setOptionValue(".selectionName", "track_jets")
        configSeq.setOptionValue(".outputName", "OutTrackJets")

    # event selection
    mycuts = {
        "SUBcommon": """
JET_N_BTAG >= 2
JET_N_BTAG GN2v01:FixedCutBEff_70 >= 1
JET_N 25000 >= 4
MET >= 20000
SAVE
""",
        "ejets": """
IMPORT SUBcommon
EL_N 25000 >= 1
EL_N tight 25000 == 1
MU_N 25000 == 0
MWT < 170000
MET+MWT > 40000
SAVE
""",
        "mujets": """
EL_N 25000 == 0
MU_N 25000 > 0
MU_N 25000 == 1
IMPORT SUBcommon
SAVE
""",
        "dilep": """
EL_N 25000 == 2
MU_N 25000 == 0
MLL >= 10000
OS
MLLWINDOW 100000 80000
SAVE
""",
        "supertight": """
JET_N 1000000 >= 1
SAVE
""",
    }
    configSeq += makeConfig(
        "EventSelection",
        electrons="AnaElectrons.loose",
        muons="AnaMuons.tight",
        met="AnaMET",
        jets="AnaJets.baselineJvt",
        btagDecoration="ftag_select_GN2v01_FixedCutBEff_85",
        selectionCutsDict=mycuts,
        noFilter=noFilter,
        cutFlowHistograms=True,
    )

    # from KLFitterAnalysisAlgorithms.KLFitterConfig import KLFitterBlock

    # cfg = KLFitterBlock("KLFitterResult")
    # cfg.setOptionValue("electrons", "AnaElectrons.tight")
    # cfg.setOptionValue("muons", "AnaMuons.tight")
    # cfg.setOptionValue("jets", "AnaJets.baselineJvt")
    # cfg.setOptionValue("met", "AnaMET")
    # cfg.setOptionValue("likelihoodType", "ttbar")
    # cfg.setOptionValue("jetSelectionMode", "kBtagPriorityFourJets")
    # cfg.setOptionValue("btaggingMethod", "kVetoBoth")
    # cfg.setOptionValue("btagger", "GN2v01")
    # cfg.setOptionValue("btagWP", "FixedCutBEff_77")
    # selectionRegionsConfig = (
    #     "selectionName:pass_ejets, leptonType: kElectron;"
    #     "selectionName:pass_mujets, leptonType: kMuon"
    # )
    # cfg.setOptionValue("selectionRegionsConfig", selectionRegionsConfig)
    # configSeq.append(cfg)

    # outputContainers["klfitter_"] = "KLFitterResult"

    # TODO: give it a factory when moving to Athena
    from TopCPToolkit.TopSpaNetConfig import TopSpaNetConfig

    for topology in ["TtbarLjetsNu"]:
        cfg = TopSpaNetConfig()
        cfg.setOptionValue("electrons", "AnaElectrons.tight")
        cfg.setOptionValue("muons", "AnaMuons.tight")
        cfg.setOptionValue("jets", "AnaJets.baselineJvt")
        cfg.setOptionValue("btagger", "GN2v01")
        cfg.setOptionValue("met", "AnaMET")
        cfg.setOptionValue(
            "eventSelection", "pass_ejets_%SYS%,as_char||pass_mujets_%SYS%,as_char"
        )
        cfg.setOptionValue("topology", topology)
        configSeq.append(cfg)

    if use_taus:
        configSeq += makeConfig("DiTauMMC")
        configSeq.setOptionValue(".electrons", "AnaElectrons.tight")
        configSeq.setOptionValue(".muons", "AnaMuons.tight")
        configSeq.setOptionValue(".taus", "AnaTauJets.tight")
        configSeq.setOptionValue(".jets", "AnaJets.baselineJvt")
        configSeq.setOptionValue(".met", "AnaMET")
        configSeq.setOptionValue(".eventSelection", "")
        configSeq.setOptionValue(".saveExtraVariables", True)
        configSeq.setOptionValue(".doMLNU3P", True)
        configSeq.setOptionValue(",doMAXW", True)

    # bootstraps
    configSeq += makeConfig("Bootstraps")
    configSeq.setOptionValue(".nReplicas", 2000)
    configSeq.setOptionValue(".skipOnMC", False)

    # add NTuple output config
    configSeq += makeConfig("Output")
    configSeq.setOptionValue(".treeName", "reco")
    configSeq.setOptionValue(".vars", reco_branches)
    configSeq.setOptionValue(".metVars", met_branches)
    configSeq.setOptionValue(".containers", outputContainers)
    configSeq.setOptionValue(
        ".commands",
        [
            "disable jet_jvtEfficiency.*",
            "disable trigPassed_HLT.*",
            "enable trigPassed_HLT_e.*",
        ],
    )

    # put everything together
    configAccumulator = ConfigAccumulator(
        algSeq,
        flags.Input.DataType,
        isPhyslite=flags.Input.isPHYSLITE,
        geometry=flags.Input.LHCPeriod,
        autoconfigFromFlags=flags,
        noSystematics=noSystematics,
    )
    configSeq.fullConfigure(configAccumulator)

    from AnaAlgorithm.DualUseConfig import isAthena, useComponentAccumulator

    if isAthena and useComponentAccumulator:
        return configAccumulator.CA


def makeTruthConfiguration(flags, algSeq, configSeq, factory, noSystematics=False):

    makeConfig = factory.makeConfig

    truth_branches = []
    outputContainers = {"": "EventInfo"}

    # primary vertex
    configSeq += makeConfig("EventCleaning")

    # PMG TruthWeightTool
    configSeq += makeConfig("GeneratorLevelAnalysis")

    # add all three Number variables by hand, since we don't run PRW
    truth_branches += [
        "EventInfo.runNumber       -> runNumber",
        "EventInfo.eventNumber     -> eventNumber",
        "EventInfo.mcChannelNumber -> mcChannelNumber",
    ]

    # TODO: give it a factory when moving to Athena
    from TopCPToolkit.truthConfig import truthConfig

    cfg = truthConfig()
    cfg.setOptionValue("histories", "Ttbar")
    configSeq.append(cfg)
    outputContainers.update(cfg.getOutputContainers())

    # NNLO reweighting
    # TODO: give it a factory when moving to Athena
    from TopCPToolkit.TtbarNNLORecursiveRewConfig import TtbarNNLORecursiveRewConfig

    cfg = TtbarNNLORecursiveRewConfig()
    # cfg.setOptionValue ('reweightType','3D')
    # cfg.setOptionValue ('sampleID', 'aMCH7')
    configSeq.append(cfg)

    # example spin analysis
    from TopCPToolkit.PartonHistoryToSpinInputConfig import (
        PartonHistoryToSpinInputConfig,
    )

    cfg = PartonHistoryToSpinInputConfig()
    cfg.setOptionValue("history", "Ttbar")
    configSeq.append(cfg)
    from TopCPToolkit.TopSpinDensityMatrixConfig import TopSpinDensityMatrixConfig

    cfg = TopSpinDensityMatrixConfig()
    cfg.setOptionValue("setup", "QEttbarExample")
    cfg.setOptionValue("top", "truth_top_4vect_%SYS%")
    cfg.setOptionValue("tbar", "truth_tbar_4vect_%SYS%")
    cfg.setOptionValue("top_decay", "truth_top_decay_4vect_%SYS%")
    cfg.setOptionValue("tbar_decay", "truth_tbar_decay_4vect_%SYS%")
    cfg.setOptionValue("doHelicityBasis", True)
    cfg.setOptionValue("doEntanglement", True)
    configSeq.append(cfg)

    # from TopCPToolkit.CARConfig import CARConfig
    # cfg = CARConfig()
    # cfg.setOptionValue ('history', 'HWW')
    # configSeq.append(cfg)

    # add NTuple output config
    configSeq += makeConfig("Output")
    configSeq.setOptionValue(".treeName", "truth")
    configSeq.setOptionValue(".vars", truth_branches)
    configSeq.setOptionValue(".containers", outputContainers)

    # put everything together
    configAccumulator = ConfigAccumulator(
        algSeq,
        flags.Input.DataType,
        isPhyslite=flags.Input.isPHYSLITE,
        geometry=flags.Input.LHCPeriod,
        autoconfigFromFlags=flags,
        noSystematics=noSystematics,
    )
    configSeq.fullConfigure(configAccumulator)

    from AnaAlgorithm.DualUseConfig import isAthena, useComponentAccumulator

    if isAthena and useComponentAccumulator:
        return configAccumulator.CA


def makeParticleLevelConfiguration(
    flags, algSeq, configSeq, factory, noSystematics=False, noFilter=False
):

    makeConfig = factory.makeConfig

    particleLevel_branches = []
    outputContainers = {"PL_": "EventInfo"}

    # primary vertex
    configSeq += makeConfig("EventCleaning")

    # PMG TruthWeightTool
    configSeq += makeConfig("GeneratorLevelAnalysis")

    # add all three Number variables by hand, since we don't run PRW
    particleLevel_branches += [
        "EventInfo.runNumber       -> runNumber",
        "EventInfo.eventNumber     -> eventNumber",
        "EventInfo.mcChannelNumber -> mcChannelNumber",
    ]

    # particle-level object definitions
    configSeq += makeConfig("PL_Electrons")
    configSeq.setOptionValue(".notFromTau", False)
    configSeq += makeConfig(
        "PL_Electrons.PtEtaSelection", containerName="TruthElectrons"
    )
    configSeq.setOptionValue(".useDressedProperties", True)
    configSeq.setOptionValue(".minPt", 25000.0)
    configSeq.setOptionValue(".maxEta", 2.5)
    outputContainers["PL_el_"] = "OutTruthElectrons"

    configSeq += makeConfig("PL_Muons")
    configSeq.setOptionValue(".notFromTau", False)
    configSeq += makeConfig("PL_Muons.PtEtaSelection", containerName="TruthMuons")
    configSeq.setOptionValue(".useDressedProperties", True)
    configSeq.setOptionValue(".minPt", 25000.0)
    configSeq.setOptionValue(".maxEta", 2.5)
    outputContainers["PL_mu_"] = "OutTruthMuons"

    # configSeq += makeConfig ('PL_Jets', containerName='AntiKt4TruthDressedWZJets')
    configSeq += makeConfig("PL_Jets")
    configSeq.setOptionValue("containerName", "AntiKt4TruthDressedWZJets")
    configSeq += makeConfig(
        "PL_Jets.PtEtaSelection", containerName="AntiKt4TruthDressedWZJets"
    )
    configSeq.setOptionValue(".minPt", 25000.0)
    configSeq.setOptionValue(".maxEta", 2.5)
    outputContainers["PL_jet_"] = "OutTruthJets"
    configSeq += makeConfig("PL_Jets")
    configSeq.setOptionValue("containerName", "AntiKt10TruthTrimmedPtFrac5SmallR20Jets")
    configSeq += makeConfig(
        "PL_Jets.PtEtaSelection",
        containerName="AntiKt10TruthTrimmedPtFrac5SmallR20Jets",
    )
    configSeq.setOptionValue(".minPt", 25000.0)
    configSeq.setOptionValue(".maxEta", 2.5)
    outputContainers["PL_ljet_"] = "OutTruthLargeRJets"

    configSeq += makeConfig("PL_Neutrinos")
    configSeq.setOptionValue(".isolated", False)
    configSeq.setOptionValue(".notFromTau", False)
    outputContainers["PL_nu_"] = "OutTruthNeutrinos"

    configSeq += makeConfig("PL_Taus")
    configSeq += makeConfig("PL_Taus.PtEtaSelection", containerName="TruthTaus")
    configSeq.setOptionValue(".minPt", 25000.0)
    configSeq.setOptionValue(".maxEta", 2.5)
    outputContainers["PL_tau_"] = "OutTruthTaus"

    configSeq += makeConfig("PL_Photons")
    configSeq.setOptionValue(".isolated", False)
    configSeq += makeConfig("PL_Photons.PtEtaSelection", containerName="TruthPhotons")
    configSeq.setOptionValue(".minPt", 25000.0)
    configSeq.setOptionValue(".maxEta", 2.5)
    outputContainers["PL_ph_"] = "OutTruthPhotons"

    configSeq += makeConfig("PL_MissingET")
    outputContainers["PL_met_"] = "TruthMET"

    configSeq += makeConfig("PL_OverlapRemoval")
    configSeq.setOptionValue(".electrons", "TruthElectrons")
    configSeq.setOptionValue(".muons", "TruthMuons")
    configSeq.setOptionValue(".photons", "TruthPhotons")
    configSeq.setOptionValue(".jets", "AntiKt4TruthDressedWZJets")
    configSeq.setOptionValue(".useRapidityForDeltaR", False)

    # event selection
    mycuts = {
        "SUBcommon": """
JET_N 25000 >= 4
MET >= 20000
SAVE
""",
        "ejets": """
IMPORT SUBcommon
EL_N 25000 >= 1
MU_N 25000 == 0
MWT < 170000
MET+MWT > 40000
SAVE
""",
    }

    configSeq += makeConfig(
        "EventSelection",
        electrons="TruthElectrons",
        muons="TruthMuons",
        met="TruthMET",
        metTerm="NonInt",
        jets="AntiKt4TruthDressedWZJets",
        selectionCutsDict=mycuts,
        noFilter=noFilter,
        cutFlowHistograms=True,
    )

    # thinning
    configSeq += makeConfig("Thinning", containerName="TruthElectrons")
    configSeq.setOptionValue(".outputName", "OutTruthElectrons")
    configSeq += makeConfig("Thinning", containerName="TruthMuons")
    configSeq.setOptionValue(".outputName", "OutTruthMuons")
    configSeq += makeConfig("Thinning", containerName="TruthNeutrinos")
    configSeq.setOptionValue(".outputName", "OutTruthNeutrinos")
    configSeq += makeConfig("Thinning", containerName="TruthTaus")
    configSeq.setOptionValue(".outputName", "OutTruthTaus")
    configSeq += makeConfig("Thinning", containerName="TruthPhotons")
    configSeq.setOptionValue(".outputName", "OutTruthPhotons")
    configSeq += makeConfig("Thinning", containerName="AntiKt4TruthDressedWZJets")
    configSeq.setOptionValue(".outputName", "OutTruthJets")
    configSeq += makeConfig(
        "Thinning", containerName="AntiKt10TruthTrimmedPtFrac5SmallR20Jets"
    )
    configSeq.setOptionValue(".outputName", "OutTruthLargeRJets")

    # add NTuple output config
    configSeq += makeConfig("Output")
    configSeq.setOptionValue(".treeName", "particleLevel")
    configSeq.setOptionValue(".vars", particleLevel_branches)
    configSeq.setOptionValue(".containers", outputContainers)

    # put everything together
    configAccumulator = ConfigAccumulator(
        algSeq,
        flags.Input.DataType,
        isPhyslite=flags.Input.isPHYSLITE,
        geometry=flags.Input.LHCPeriod,
        autoconfigFromFlags=flags,
        noSystematics=noSystematics,
    )
    configSeq.fullConfigure(configAccumulator)

    from AnaAlgorithm.DualUseConfig import isAthena, useComponentAccumulator

    if isAthena and useComponentAccumulator:
        return configAccumulator.CA
