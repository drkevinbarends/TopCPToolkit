from AnalysisAlgorithmsConfig.ConfigBlock import ConfigBlock

class TutorialConfig(ConfigBlock):
    """Skeleton of a Config Block, for the TopCPToolkit tutorial"""

    def __init__(self):
        super(TutorialConfig, self).__init__()
        self.addOption('electrons', None, type=str)
        self.addOption('silliness', False, type=bool)

    def makeAlgs(self, config):
        electrons = config.readName(self.electrons)

        alg = config.createAlgorithm('top::TutorialAlg', 'tutorialStuff')
        alg.electrons = electrons

        if self.silliness:
            print("This is a pointless printout that has nothing to do with our tutorial.")
