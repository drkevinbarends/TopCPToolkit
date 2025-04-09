from AnalysisAlgorithmsConfig.ConfigBlock import ConfigBlock

class SinglelepHadronicChi2RecoConfig(ConfigBlock):
    """ConfigBlock for hadronic chi2 reconstruction algorithm"""

    def __init__(self):
        super(SinglelepHadronicChi2RecoConfig, self).__init__()
        self.addOption('jets', '', type=str)
        self.addOption('eventSelection', '', type=str)
        self.addOption('topMass', 172.5, type=float)
        self.addOption('maxBJets', 2, type=int)
        self.addOption('maxLightJets', 4, type=int)
        self.addOption('topResolution', 35., type=float)
        self.addOption('wResolution', 25., type=float)
        self.addOption('btagger', '', type=str)
        self.addOption('btagWP', '', type=str)

    def makeAlgs(self, config):
        alg = config.createAlgorithm('top::SinglelepHadronicChi2RecoAlg', 'SinglelepHadronicChi2RecoAlg')

        alg.jets, alg.jetSelection = config.readNameAndSelection(self.jets)

        alg.eventSelection = self.eventSelection
        alg.topMass = self.topMass
        alg.maxBJets = self.maxBJets
        alg.maxLightJets = self.maxLightJets
        alg.topResolution = self.topResolution
        alg.wResolution = self.wResolution
        alg.bTagDecoration = 'ftag_select_' + self.btagger + '_' + self.btagWP

        alg.hadronic_b_index  = 'chi2_had_b_index_%SYS%'
        alg.light_1_index     = 'chi2_light_1_index_%SYS%'
        alg.light_2_index     = 'chi2_light_2_index_%SYS%'
        alg.best_chi2         = 'best_chi2_%SYS%'

        config.addOutputVar('EventInfo', alg.hadronic_b_index, 'chi2_had_b_index')
        config.addOutputVar('EventInfo', alg.light_1_index, 'chi2_light_1_index')
        config.addOutputVar('EventInfo', alg.light_2_index, 'chi2_light_2_index')
        config.addOutputVar('EventInfo', alg.best_chi2, 'best_chi2')
