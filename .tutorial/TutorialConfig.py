from AnalysisAlgorithmsConfig.ConfigBlock import ConfigBlock

class TutorialConfig(ConfigBlock):
    """Skeleton of a Config Block, for the TopCPToolkit tutorial"""

    def __init__(self):
        super(TutorialConfig, self).__init__()
        self.addOption('electrons', None, type=str)
        self.addOption('silliness', False, type=bool)
        self.addOption('muons', None, type=str)
        self.addOption('jets', None, type=str)
        self.addOption('met', None, type=str)
        self.addOption('selection', "", type=str)
    
    def makeAlgs(self, config):
        electrons = config.readName(self.electrons)
        muons = config.readName(self.muons)
        jets  = config.readName(self.jets)
        met   = config.readName(self.met)

        alg = config.createAlgorithm('top::TutorialAlg', 'tutorialStuff')
        alg.electrons = electrons
        alg.muons     = muons
        alg.jets      = jets
        alg.met       = met
        alg.eventPreselection = self.selection

        if self.silliness:
            print("This is a pointless printout that has nothing to do with our tutorial.")

        config.addOutputVar('EventInfo', 'mtw_%SYS%', 'transverseWmass' )
        config.addOutputVar(self.jets, 'dphimet_%SYS%', 'deltaPhi_with_met')
