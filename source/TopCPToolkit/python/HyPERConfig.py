from AnalysisAlgorithmsConfig.ConfigBlock import ConfigBlock
from AthenaConfiguration.Enums import LHCPeriod


class HyPERConfig(ConfigBlock):
    """ConfigBlock for HyPER algorithms"""

    def __init__(self):
        super(HyPERConfig, self).__init__()
        self.addOption("btagger", "GN2v01_FixedCutBEff_77", type=str)
        self.addOption("electrons", "", type=str)
        self.addOption("muons", "", type=str)
        self.addOption("jets", "", type=str)
        self.addOption("met", "", type=str)
        self.addOption("eventSelection", "", type=str)
        self.addOption("topology", "", type=str)
        self.addOption("OutputLevel", 3, type=int)  # 3: INFO 2: DEBUG 1: VERBOSE
        self.addOption("fullLogEventNumber", 0, type=int)

    def makeAlgs(self, config):
        alg = config.createAlgorithm("top::RunHyPERAlg", "RunHyPERAlg" + self.topology)
        alg.btagger = self.btagger
        alg.electrons, alg.electronSelection = config.readNameAndSelection(
            self.electrons
        )
        alg.muons, alg.muonSelection = config.readNameAndSelection(self.muons)
        alg.jets, alg.jetSelection = config.readNameAndSelection(self.jets)
        alg.met = config.readName(self.met)
        alg.eventSelection = self.eventSelection
        alg.topology = self.topology
        # Check that config is ok if a particular event was passed for full log
        if self.fullLogEventNumber > 0:
            if self.OutputLevel != 3:
                raise ValueError(
                    "OutputLevel must be 3 when tracing only one particular event!"
                )

        alg.OutputLevel = self.OutputLevel
        alg.fullLogEventNumber = self.fullLogEventNumber

        def form_even_odd_path(topology, run):
            base_path = "/TopCPToolkit/HyPER/" + topology
            even_path = base_path + "_" + run + "_trained_on_even.onnx"
            odd_path = base_path + "_" + run + "_trained_on_odd.onnx"
            return even_path, odd_path

        # Confifigure model onnx paths
        if config.geometry() == LHCPeriod.Run2:
            if self.topology == "TtbarAllHadronic":
                alg.trainedOnEvenPath, alg.trainedOnOddPath = form_even_odd_path(
                    "TtbarAllHadronic", "run2"
                )
            elif self.topology == "TtbarLJets":
                alg.trainedOnEvenPath, alg.trainedOnOddPath = form_even_odd_path(
                    "TtbarLJets", "run2"
                )
            elif self.topology == "TtbarLJetsNoBTag":
                alg.trainedOnEvenPath, alg.trainedOnOddPath = form_even_odd_path(
                    "TtbarLJetsNoBTag", "run2"
                )
            else:
                print("Not being able to set the model paths for the given topology.")
                raise ValueError("Unknown topology: " + self.topology)
        elif config.geometry() == LHCPeriod.Run3:
            if self.topology == "TtbarAllHadronic":
                alg.trainedOnEvenPath, alg.trainedOnOddPath = form_even_odd_path(
                    "TtbarAllHadronic", "run3"
                )
            elif self.topology == "TtbarLJets":
                alg.trainedOnEvenPath, alg.trainedOnOddPath = form_even_odd_path(
                    "TtbarLJets", "run3"
                )
            elif self.topology == "TtbarLJetsNoBTag":
                alg.trainedOnEvenPath, alg.trainedOnOddPath = form_even_odd_path(
                    "TtbarLJetsNoBTag", "run3"
                )
            else:
                print("Not being able to set the model paths for the given topology.")
                raise ValueError("Unknown topology: " + self.topology)
        else:
            print("Not being able to set the model paths for the given run period.")
            raise ValueError(
                "HyPER models not available for run period: " + config.geometry()
            )

        # give appropriate names for the handles to decorate
        if self.topology == "TtbarAllHadronic":
            config.addOutputVar(
                "EventInfo",
                "hyper_TtbarAllHadronic_Top1_Indices_%SYS%",
                "hyper_TtbarAllHadronic_Top1_Indices",
            )
            config.addOutputVar(
                "EventInfo",
                "hyper_TtbarAllHadronic_Top1_Score_%SYS%",
                "hyper_TtbarAllHadronic_Top1_Score",
            )
            config.addOutputVar(
                "EventInfo",
                "hyper_TtbarAllHadronic_Top2_Indices_%SYS%",
                "hyper_TtbarAllHadronic_Top2_Indices",
            )
            config.addOutputVar(
                "EventInfo",
                "hyper_TtbarAllHadronic_Top2_Score_%SYS%",
                "hyper_TtbarAllHadronic_Top2_Score",
            )
            config.addOutputVar(
                "EventInfo",
                "hyper_TtbarAllHadronic_W1_Indices_%SYS%",
                "hyper_TtbarAllHadronic_W1_Indices",
            )
            config.addOutputVar(
                "EventInfo",
                "hyper_TtbarAllHadronic_W1_Score_%SYS%",
                "hyper_TtbarAllHadronic_W1_Score",
            )
            config.addOutputVar(
                "EventInfo",
                "hyper_TtbarAllHadronic_W2_Indices_%SYS%",
                "hyper_TtbarAllHadronic_W2_Indices",
            )
            config.addOutputVar(
                "EventInfo",
                "hyper_TtbarAllHadronic_W2_Score_%SYS%",
                "hyper_TtbarAllHadronic_W2_Score",
            )
        if self.topology == "TtbarLJets" or self.topology == "TtbarLJetsNoBTag":
            config.addOutputVar(
                "EventInfo",
                "hyper_TtbarLJets_TopHad_Indices_%SYS%",
                "hyper_TtbarLJets_TopHad_Indices",
            )
            config.addOutputVar(
                "EventInfo",
                "hyper_TtbarLJets_TopHad_Score_%SYS%",
                "hyper_TtbarLJets_TopHad_Score",
            )
            config.addOutputVar(
                "EventInfo",
                "hyper_TtbarLJets_TopHad_IDs_%SYS%",
                "hyper_TtbarLJets_TopHad_IDs",
            )
            config.addOutputVar(
                "EventInfo",
                "hyper_TtbarLJets_TopLep_Indices_%SYS%",
                "hyper_TtbarLJets_TopLep_Indices",
            )
            config.addOutputVar(
                "EventInfo",
                "hyper_TtbarLJets_TopLep_Score_%SYS%",
                "hyper_TtbarLJets_TopLep_Score",
            )
            config.addOutputVar(
                "EventInfo",
                "hyper_TtbarLJets_TopLep_IDs_%SYS%",
                "hyper_TtbarLJets_TopLep_IDs",
            )
            config.addOutputVar(
                "EventInfo",
                "hyper_TtbarLJets_WHad_Indices_%SYS%",
                "hyper_TtbarLJets_WHad_Indices",
            )
            config.addOutputVar(
                "EventInfo",
                "hyper_TtbarLJets_WHad_Score_%SYS%",
                "hyper_TtbarLJets_WHad_Score",
            )
            config.addOutputVar(
                "EventInfo",
                "hyper_TtbarLJets_WLep_Indices_%SYS%",
                "hyper_TtbarLJets_WLep_Indices",
            )
            config.addOutputVar(
                "EventInfo",
                "hyper_TtbarLJets_WLep_Score_%SYS%",
                "hyper_TtbarLJets_WLep_Score",
            )
