from AnalysisAlgorithmsConfig.ConfigBlock import ConfigBlock
from AnalysisAlgorithmsConfig.ConfigAccumulator import DataType

CAR_branches = {
    "HWW" : [
        'MC_Hdecay1_decay1_CAR_pt',
        'MC_Hdecay1_decay1_CAR_eta',
        'MC_Hdecay1_decay1_CAR_phi',
        'MC_Hdecay1_decay1_CAR_m',
        'MC_Hdecay1_decay1_CAR_pdgId',
        'MC_Hdecay1_decay2_CAR_pt',
        'MC_Hdecay1_decay2_CAR_eta',
        'MC_Hdecay1_decay2_CAR_phi',
        'MC_Hdecay1_decay2_CAR_m',
        'MC_Hdecay1_decay2_CAR_pdgId',
        'MC_Hdecay2_decay1_CAR_pt',
        'MC_Hdecay2_decay1_CAR_eta',
        'MC_Hdecay2_decay1_CAR_phi',
        'MC_Hdecay2_decay1_CAR_m',
        'MC_Hdecay2_decay1_CAR_pdgId',
        'MC_Hdecay2_decay2_CAR_pt',
        'MC_Hdecay2_decay2_CAR_eta',
        'MC_Hdecay2_decay2_CAR_phi',
        'MC_Hdecay2_decay2_CAR_m',
        'MC_Hdecay2_decay2_CAR_pdgId'
        ],
    "HZZ" : [
        'MC_Hdecay1_decay1_CAR_pt',
        'MC_Hdecay1_decay1_CAR_eta',
        'MC_Hdecay1_decay1_CAR_phi',
        'MC_Hdecay1_decay1_CAR_m',
        'MC_Hdecay1_decay1_CAR_pdgId',
        'MC_Hdecay1_decay2_CAR_pt',
        'MC_Hdecay1_decay2_CAR_eta',
        'MC_Hdecay1_decay2_CAR_phi',
        'MC_Hdecay1_decay2_CAR_m',
        'MC_Hdecay1_decay2_CAR_pdgId',
        'MC_Hdecay2_decay1_CAR_pt',
        'MC_Hdecay2_decay1_CAR_eta',
        'MC_Hdecay2_decay1_CAR_phi',
        'MC_Hdecay2_decay1_CAR_m',
        'MC_Hdecay2_decay1_CAR_pdgId',
        'MC_Hdecay2_decay2_CAR_pt',
        'MC_Hdecay2_decay2_CAR_eta',
        'MC_Hdecay2_decay2_CAR_phi',
        'MC_Hdecay2_decay2_CAR_m',
        'MC_Hdecay2_decay2_CAR_pdgId'
        ]
}

class CARConfig(ConfigBlock):

    def __init__(self):
        super(CARConfig, self).__init__()
        self.addOption('history', "HWW", type=str)
        self.addOption('seed', 42, type=int)

    def makeAlgs(self, config):
        if config.dataType() is DataType.Data: return

        if not self.history in ["HWW", "HZZ"]:
            raise ValueError("Invalid history option for CAR! Allowed are HWW, HZZ")

        if self.seed < 0:
            raise ValueError("CAR seed must be positive!")

        alg = config.createAlgorithm('top::CARAlg', 'CARAlg_'+self.history)

        alg.history = self.history
        alg.partonContainerName = 'PartonHistory' + self.history + '_NOSYS'
        alg.seed = self.seed

        for branch in CAR_branches[self.history]:
            config.addOutputVar('EventInfo', branch, branch, noSys=True)
