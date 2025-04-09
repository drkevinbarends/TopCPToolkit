from AnalysisAlgorithmsConfig.ConfigBlock import ConfigBlock

class PartonHistoryToSpinInputConfig(ConfigBlock):
    """Turn the top parton histories into PtEtaPhiMVector suitable for use"""
    """in the TopSpinDensityMatrixAlg"""

    def __init__(self):
        super(PartonHistoryToSpinInputConfig, self).__init__()
        self.addOption('history', None, type=str)
        self.addOption('top', None, type=str)
        self.addOption('tbar', None, type=str)
        self.addOption('top_decay', None, type=str)
        self.addOption('tbar_decay', None, type=str)

    def makeAlgs(self, config):
        history = config.readName('PartonHistory' + self.history)

        alg = config.createAlgorithm('top::PartonHistoryToSpinInputAlg', self.history+'toSpinInputs')
        alg.partonHistory = history
        if self.top:
          alg.truth_top = self.top
        if self.tbar:
          alg.truth_tbar = self.tbar
        if self.top_decay:
          alg.truth_top_decay = self.top_decay
        if self.tbar_decay:
          alg.truth_tbar_decay = self.tbar_decay
