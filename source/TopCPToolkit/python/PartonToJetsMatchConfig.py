from AnalysisAlgorithmsConfig.ConfigBlock import ConfigBlock

class PartonToJetsMatchConfig(ConfigBlock):
    """ConfigBlock for jet matching algorithms"""

    def __init__(self):
        super(PartonToJetsMatchConfig, self).__init__()
        self.addOption('jets', 'AntiKt4TruthDressedWZJets', type=str)
        self.addOption('eventSelection', 'pass_ejets_%SYS%,as_char||pass_mujets_%SYS%,as_char', type=str)
        self.addOption('criticalDR', 0.3, type=float)
        self.addOption('partonContainerName', 'PartonHistoryTtbar_NOSYS', type=str)

    def makeAlgs(self, config):
        from AnalysisAlgorithmsConfig.ConfigAccumulator import DataType
        if config.dataType() is DataType.Data: return

        alg = config.createAlgorithm('top::PartonToJetsMatchAlg', 'PartonToJetsMatchAlg')

        if 'Truth' in self.jets:
            config.setSourceName(self.jets.split(".")[0], self.jets.split(".")[0])
        alg.jets, alg.jetSelection = config.readNameAndSelection(self.jets)

        alg.eventSelection = self.eventSelection
        alg.criticalDR = self.criticalDR
        alg.partonContainerName = self.partonContainerName

        alg.leptonic_b_index  = 'parton_truth_lep_b_index_%SYS%'
        alg.hadronic_b_index  = 'parton_truth_had_b_index_%SYS%'
        alg.up_index          = 'parton_truth_up_index_%SYS%'
        alg.down_index        = 'parton_truth_down_index_%SYS%'
        alg.event_is_dilepton = 'event_is_dilepton_%SYS%'

        config.addOutputVar('EventInfo', alg.leptonic_b_index, 'parton_truth_lep_b_index', noSys=True)
        config.addOutputVar('EventInfo', alg.hadronic_b_index, 'parton_truth_had_b_index', noSys=True)
        config.addOutputVar('EventInfo', alg.up_index, 'parton_truth_up_index', noSys=True)
        config.addOutputVar('EventInfo', alg.down_index, 'parton_truth_down_index', noSys=True)
        config.addOutputVar('EventInfo', alg.event_is_dilepton, 'event_is_dilepton', noSys=True)
