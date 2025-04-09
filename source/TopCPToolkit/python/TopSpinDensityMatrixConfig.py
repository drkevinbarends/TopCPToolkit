from AnalysisAlgorithmsConfig.ConfigBlock import ConfigBlock

class TopSpinDensityMatrixConfig(ConfigBlock):
    """Compute the top spin density matrix observables"""

    def __init__(self):
        super(TopSpinDensityMatrixConfig, self).__init__()
        self.addOption('setup', "", type=str)
        self.addOption('eventSelection', "", type=str)
        self.addOption('top', None, type=str)
        self.addOption('tbar', None, type=str)
        self.addOption('top_decay', None, type=str)
        self.addOption('tbar_decay', None, type=str)
        self.addOption('doHelicityBasis', False, type=bool)
        self.addOption('doBeamBasis', False, type=bool)
        self.addOption('doEntanglement', False, type=bool)

    def makeAlgs(self, config):
        if not (self.top and self.tbar and self.top_decay and self.tbar_decay):
           raise ValueError("You must specify the names of the 4-vectors for top/tbar and their spin analysers!")

        alg = config.createAlgorithm('top::TopSpinDensityMatrixAlg', 'TopSpinDensityMatrix'+self.setup)
        # event selection
        alg.eventSelection = self.eventSelection
        # inputs
        alg.top_4vec = self.top
        alg.tbar_4vec = self.tbar
        alg.top_analyser_4vec = self.top_decay
        alg.tbar_analyser_4vec = self.tbar_decay
        # outputs
        alg.ckp = '_'.join([self.setup, 'cos_theta_helicity_p_%SYS%'])
        alg.ckm = '_'.join([self.setup, 'cos_theta_helicity_m_%SYS%'])
        alg.cnp = '_'.join([self.setup, 'cos_theta_transverse_p_%SYS%'])
        alg.cnm = '_'.join([self.setup, 'cos_theta_transverse_m_%SYS%'])
        alg.crp = '_'.join([self.setup, 'cos_theta_raxis_p_%SYS%'])
        alg.crm = '_'.join([self.setup, 'cos_theta_raxis_m_%SYS%'])
        alg.cxp = '_'.join([self.setup, 'cos_theta_x_p_%SYS%'])
        alg.cxm = '_'.join([self.setup, 'cos_theta_x_m_%SYS%'])
        alg.cyp = '_'.join([self.setup, 'cos_theta_y_p_%SYS%'])
        alg.cym = '_'.join([self.setup, 'cos_theta_y_m_%SYS%'])
        alg.czp = '_'.join([self.setup, 'cos_theta_z_p_%SYS%'])
        alg.czm = '_'.join([self.setup, 'cos_theta_z_m_%SYS%'])        
        alg.D   = '_'.join([self.setup, 'cos_phi_%SYS%'])

        observables = []
        if self.doHelicityBasis:
            observables += [alg.ckp, alg.ckm, alg.cnp, alg.cnm, alg.crp, alg.crm]
        if self.doBeamBasis:
            observables += [alg.cxp, alg.cxm, alg.cyp, alg.cym, alg.czp, alg.czm]
        if self.doEntanglement:
            observables += [alg.D]
        for observable in observables:
          config.addOutputVar('EventInfo', observable, 'spin_' + observable.split("_%SYS%")[0])
