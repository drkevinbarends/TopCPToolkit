from AnalysisAlgorithmsConfig.ConfigBlock import ConfigBlock


class TopNuFlowsConfig(ConfigBlock):
    """ConfigBlock, for the NuFlows Algorithms"""

    def __init__(self) -> None:
        super().__init__()
        self.addOption("btagger", "Dl1dv01", type=str)
        self.addOption("onnxfilepath", "SetMe", type=str)
        self.addOption("write_inputs", False, type=bool)
        self.addOption("electrons", None, type=str)
        self.addOption("muons", None, type=str)
        self.addOption("jets", None, type=str)
        self.addOption("met", None, type=str)
        self.addOption("eventSelection", None, type=str)

    def makeAlgs(self, config):
        alg = config.createAlgorithm("top::RunNuFlowsAlg", "RunNuFlowsAlg")

        # All the inputs to initialize the algorithm
        alg.btagger = self.btagger
        alg.onnxfilepath = self.onnxfilepath
        alg.write_inputs = self.write_inputs
        alg.electrons, alg.electronSelection = config.readNameAndSelection(
            self.electrons
        )
        alg.muons, alg.muonSelection = config.readNameAndSelection(self.muons)
        alg.jets, alg.jetSelection = config.readNameAndSelection(self.jets)
        alg.met = config.readName(self.met)
        alg.eventSelection = self.eventSelection

        # Appropriate names for the handles to decorate
        alg.nu_out = "nuflows_nu_out_%SYS%"
        alg.loglik = "nuflows_loglik_%SYS%"

        # Add the output variables
        config.addOutputVar("EventInfo", alg.nu_out, "nuflows_nu_out")
        config.addOutputVar("EventInfo", alg.loglik, "nuflows_loglik")

        # Add all of the inputs to the neural network
        if self.write_inputs:
            alg.input_lep = "nuflows_input_lep_%SYS%"
            alg.input_jet = "nuflows_input_jet_%SYS%"
            alg.input_met = "nuflows_input_met_%SYS%"
            alg.input_misc = "nuflows_input_misc_%SYS%"
            config.addOutputVar("EventInfo", alg.input_lep, "nuflows_input_lep")
            config.addOutputVar("EventInfo", alg.input_jet, "nuflows_input_jet")
            config.addOutputVar("EventInfo", alg.input_met, "nuflows_input_met")
            config.addOutputVar("EventInfo", alg.input_misc, "nuflows_input_misc")
