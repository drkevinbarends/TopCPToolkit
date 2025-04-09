from AnalysisAlgorithmsConfig.ConfigBlock import ConfigBlock


class TopSpaNetConfig(ConfigBlock):
    """ConfigBlock for SPA-Net algorithms"""

    def __init__(self):
        super(TopSpaNetConfig, self).__init__()
        self.addOption('btagger', 'Dl1dv01', type=str)
        self.addOption('electrons', '', type=str)
        self.addOption('muons', '', type=str)
        self.addOption('jets', '', type=str)
        self.addOption('met', '', type=str)
        self.addOption('eventSelection', '', type=str)
        self.addOption('topology', '', type=str)

    def makeAlgs(self, config):
        alg = config.createAlgorithm('top::RunSpaNetAlg', 'RunSpaNetAlg'+self.topology)
        alg.btagger = self.btagger
        alg.electrons, alg.electronSelection = config.readNameAndSelection(self.electrons)
        alg.muons, alg.muonSelection = config.readNameAndSelection(self.muons)
        alg.jets, alg.jetSelection = config.readNameAndSelection(self.jets)
        alg.met = config.readName(self.met)
        alg.eventSelection = self.eventSelection
        alg.topology = self.topology

        # give appropriate names for the handles to decorate
        alg.leptonic_b_index        = self.topology+'_spanet_lep_b_index_%SYS%'
        alg.hadronic_b_index        = self.topology+'_spanet_had_b_index_%SYS%'
        alg.down_index              = self.topology+'_spanet_down_index_%SYS%'
        alg.up_index                = self.topology+'_spanet_up_index_%SYS%'
        alg.leptonic_top_assignment = self.topology+'_spanet_lep_top_assignment_%SYS%'
        alg.hadronic_top_assignment = self.topology+'_spanet_had_top_assignment_%SYS%'
        alg.leptonic_top_detection  = self.topology+'_spanet_lep_top_detection_%SYS%'
        alg.hadronic_top_detection  = self.topology+'_spanet_had_top_detection_%SYS%'
        alg.regressed_neutrino_eta  = self.topology+'_spanet_reg_nu_eta_%SYS%'
        alg.regressed_neutrino_px   = self.topology+'_spanet_reg_nu_px_%SYS%'
        alg.regressed_neutrino_py   = self.topology+'_spanet_reg_nu_py_%SYS%'
        alg.regressed_neutrino_pz   = self.topology+'_spanet_reg_nu_pz_%SYS%'
        alg.regressed_ttbar_mass    = self.topology+'_spanet_reg_ttbar_m_%SYS%'

        if self.topology == 'TtbarLjetsNu':
            config.addOutputVar('EventInfo', alg.leptonic_b_index, self.topology+'_spanet_lep_b_index')
            config.addOutputVar('EventInfo', alg.hadronic_b_index, self.topology+'_spanet_had_b_index')
            config.addOutputVar('EventInfo', alg.down_index, self.topology+'_spanet_down_index')
            config.addOutputVar('EventInfo', alg.up_index, self.topology+'_spanet_up_index')
            config.addOutputVar('EventInfo', alg.leptonic_top_assignment, self.topology+'_spanet_lep_top_assignment')
            config.addOutputVar('EventInfo', alg.hadronic_top_assignment, self.topology+'_spanet_had_top_assignment')
            config.addOutputVar('EventInfo', alg.leptonic_top_detection, self.topology+'_spanet_lep_top_detection')
            config.addOutputVar('EventInfo', alg.hadronic_top_detection, self.topology+'_spanet_had_top_detection')
            config.addOutputVar('EventInfo', alg.regressed_neutrino_eta, self.topology+'_spanet_reg_nu_eta')
            config.addOutputVar('EventInfo', alg.regressed_neutrino_px, self.topology+'_spanet_reg_nu_px')
            config.addOutputVar('EventInfo', alg.regressed_neutrino_py, self.topology+'_spanet_reg_nu_py')
            config.addOutputVar('EventInfo', alg.regressed_neutrino_pz, self.topology+'_spanet_reg_nu_pz')
            config.addOutputVar('EventInfo', alg.regressed_ttbar_mass, self.topology+'_spanet_reg_ttbar_m')
