from AnalysisAlgorithmsConfig.ConfigBlock import ConfigBlock


class ToponiumDileptonReconstructionConfig(ConfigBlock):
    """ConfigBlock for toponium reconstruction in dileptonic final states"""

    def __init__(self):
        super(ToponiumDileptonReconstructionConfig, self).__init__()
        self.addOption(
            "jets",
            "",
            type=str,
            info="the input jet container, with a possible selection, in the format `container` or `container.selection`.",
        )
        self.addOption(
            "electrons",
            "",
            type=str,
            info="the input electron container, with a possible selection, in the format `container` or `container.selection`.",
        )
        self.addOption(
            "muons",
            "",
            type=str,
            info="the input muon container, with a possible selection, in the format `container` or `container.selection`.",
        )
        self.addOption("met", "", type=str, info="the input MET container.")
        self.addOption(
            "metTerm",
            "Final",
            type=str,
            info="the MET term name to use. The default is `'Final'` (suited for detector-level analysis; at particle-level, you need `'NonInt'`).",
        )
        self.addOption(
            "selection", "", type=str, info="a possible event selection to apply."
        )
        self.addOption(
            "btagDecor", "", type=str, info="the b-tagging selection string."
        )

    def makeAlgs(self, config):
        alg = config.createAlgorithm(
            "top::ToponiumDileptonReconstructionAlg", "ToponiumDileptonReco"
        )

        alg.jets, alg.jetSelection = config.readNameAndSelection(self.jets)
        alg.electrons, alg.electronSelection = config.readNameAndSelection(
            self.electrons
        )
        alg.muons, alg.muonSelection = config.readNameAndSelection(self.muons)
        alg.met = config.readName(self.met)
        alg.metTerm = self.metTerm
        alg.selection = self.selection
        alg.bTagDecoration = self.btagDecor

        for var in [
            "toponium_top_4vect",
            "toponium_tbar_4vect",
            "toponium_top_decay_4vect",
            "toponium_tbar_decay_4vect",
        ]:
            config.addOutputVar("EventInfo", var + "_%SYS%", var)
