from AnalysisAlgorithmsConfig.ConfigBlock import ConfigBlock


class PartonMatchConfig(ConfigBlock):
    """ConfigBlock for jet matching algorithms"""

    def __init__(self):
        super(PartonMatchConfig, self).__init__()
        self.addOption("jets", "AntiKt4TruthDressedWZJets", type=str)
        self.addOption("electrons", "", type=str)
        self.addOption("muons", "", type=str)
        self.addOption(
            "eventSelection",
            "pass_ejets_%SYS%,as_char||pass_mujets_%SYS%,as_char",
            type=str,
        )
        self.addOption("criticalJetDR", 0.3, type=float)
        self.addOption("criticalLeptonDR", 0.3, type=float)
        self.addOption("acceptDoubleMatches", False, type=bool)
        self.addOption("partonTopology", "Ttbar", type=str)
        self.addOption("partonContainerName", "PartonHistoryTtbar_NOSYS", type=str)

    def makeAlgs(self, config):
        from AnalysisAlgorithmsConfig.ConfigAccumulator import DataType

        if config.dataType() is DataType.Data:
            return

        alg = config.createAlgorithm("top::PartonMatchAlg", "PartonMatchAlg")

        if "Truth" in self.jets:
            config.setSourceName(self.jets.split(".")[0], self.jets.split(".")[0])
        if "Truth" in self.electrons:
            config.setSourceName(
                self.electrons.split(".")[0], self.electrons.split(".")[0]
            )
        if "Truth" in self.muons:
            config.setSourceName(self.muons.split(".")[0], self.muons.split(".")[0])
        alg.jets, alg.jetSelection = config.readNameAndSelection(self.jets)
        alg.electrons, alg.electronSelection = config.readNameAndSelection(
            self.electrons
        )
        alg.muons, alg.muonSelection = config.readNameAndSelection(self.muons)

        alg.eventSelection = self.eventSelection
        alg.criticalJetDR = self.criticalJetDR
        alg.criticalLeptonDR = self.criticalLeptonDR
        alg.acceptDoubleMatches = self.acceptDoubleMatches
        alg.partonContainerName = self.partonContainerName
        alg.partonTopology = self.partonTopology

        alg.event_jet_truth_candidates = "event_jet_truth_candidates_%SYS%"
        alg.event_jet_truth_idx = "event_jet_truth_index_%SYS%"
        alg.event_electron_truth_candidates = "event_electron_truth_candidates_%SYS%"
        alg.event_electron_truth_idx = "event_electron_truth_index_%SYS%"
        alg.event_muon_truth_candidates = "event_muon_truth_candidates_%SYS%"
        alg.event_muon_truth_idx = "event_muon_truth_index_%SYS%"
        alg.event_nLeptons = "event_nLeptons_%SYS%"
        config.addOutputVar(
            "EventInfo",
            alg.event_jet_truth_candidates,
            "event_jet_truth_candidates",
            noSys=True,
        )
        config.addOutputVar(
            "EventInfo", alg.event_jet_truth_idx, "event_jet_truth_idx", noSys=True
        )
        config.addOutputVar(
            "EventInfo",
            alg.event_electron_truth_candidates,
            "event_electron_truth_candidates",
            noSys=True,
        )
        config.addOutputVar(
            "EventInfo",
            alg.event_electron_truth_idx,
            "event_electron_truth_idx",
            noSys=True,
        )
        config.addOutputVar(
            "EventInfo",
            alg.event_muon_truth_candidates,
            "event_muon_truth_candidates",
            noSys=True,
        )
        config.addOutputVar(
            "EventInfo", alg.event_muon_truth_idx, "event_muon_truth_idx", noSys=True
        )
        config.addOutputVar(
            "EventInfo", alg.event_nLeptons, "event_nLeptons", noSys=True
        )
