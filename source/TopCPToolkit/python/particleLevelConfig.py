from AnalysisAlgorithmsConfig.ConfigBlock import ConfigBlock
from AnalysisAlgorithmsConfig.ConfigAccumulator import DataType

particlelevel_branch_mappings = {
    "electrons": [
        'pt_dressed               -> pt',
        'eta_dressed              -> eta',
        'phi_dressed              -> phi',
        'e                        -> e',
        'charge                   -> charge',
        'classifierParticleType   -> type',
        'classifierParticleOrigin -> origin',
    ],
    "muons": [
        'pt_dressed               -> pt',
        'eta_dressed              -> eta',
        'phi_dressed              -> phi',
        'e                        -> e',
        'charge                   -> charge',
        'classifierParticleType   -> type',
        'classifierParticleOrigin -> origin',
    ],
    "photons": [
        'pt                       -> pt',
        'eta                      -> eta',
        'phi                      -> phi',
        'e                        -> e',
        'classifierParticleType   -> type',
        'classifierParticleOrigin -> origin',
    ],
    "taus": [
        'pt                       -> pt',
        'eta                      -> eta',
        'phi                      -> phi',
        'e                        -> e',
        'classifierParticleType   -> type',
        'classifierParticleOrigin -> origin',
        'IsHadronicTau            -> IsHadronicTau',
    ],
    "jets": [
        'pt                       -> pt',
        'eta                      -> eta',
        'phi                      -> phi',
        'e                        -> e',
        'GhostBHadronsFinalCount  -> nGhosts_bHadron',
        'GhostCHadronsFinalCount  -> nGhosts_cHadron',
    ],
    "ljets": [
        'pt                       -> pt',
        'eta                      -> eta',
        'phi                      -> phi',
        'e                        -> e',
    ],
    "neutrinos": [
        'pt                       -> pt',
        'eta                      -> eta',
        'phi                      -> phi',
        'e                        -> e',
    ],
    "met": [
        'met_met                  -> met',
        'met_phi                  -> phi',
    ],
}

class particleLevelConfig(ConfigBlock):
    """ConfigBlock for particle-level objects"""

    def __init__(self):
        super(particleLevelConfig, self).__init__()
        self.addOption('useTruthElectrons', True, type=bool)
        self.addOption('useTruthMuons', True, type=bool)
        self.addOption('useTruthPhotons', False, type=bool)
        self.addOption('useTruthTaus', False, type=bool)
        self.addOption('useTruthJets', True, type=bool)
        self.addOption('useTruthLargeRJets', False, type=bool)
        self.addOption('useTruthNeutrinos', False, type=bool)
        self.addOption('useTruthMET', True, type=bool)
        self.addOption('doOverlapRemoval', True, type=bool)
        self.addOption('elPtMin', None, type=float)
        self.addOption('elEtaMax', None, type=float)
        self.addOption('elNotFromHadron', True, type=bool)
        self.addOption('elTauIsHadron', False, type=bool)
        self.addOption('muPtMin', None, type=float)
        self.addOption('muEtaMax', None, type=float)
        self.addOption('muNotFromHadron', True, type=bool)
        self.addOption('muTauIsHadron', False, type=bool)
        self.addOption('phPtMin', None, type=float)
        self.addOption('phEtaMax', None, type=float)
        self.addOption('phOrigin', '', type=str)
        self.addOption('phIsolation', '', type=str)
        self.addOption('tauPtMin', None, type=float)
        self.addOption('tauEtaMax', None, type=float)
        self.addOption('jetPtMin', None, type=float)
        self.addOption('jetEtaMax', None, type=float)
        self.addOption('ljetPtMin', None, type=float)
        self.addOption('ljetEtaMax', None, type=float)
        self.addOption('ljetCollection', None, type=str)
        self.addOption('nuPtMin', None, type=float)
        self.addOption('nuEtaMax', None, type=float)

    def makeAlgs(self, config):

        if config.dataType() is DataType.Data: return

        alg = config.createAlgorithm("top::ParticleLevelAlg", "TopParticleLevel")
        alg.useTruthElectrons  = self.useTruthElectrons
        alg.useTruthMuons      = self.useTruthMuons
        alg.useTruthPhotons    = self.useTruthPhotons
        alg.useTruthTaus       = self.useTruthTaus
        alg.useTruthJets       = self.useTruthJets
        alg.useTruthLargeRJets = self.useTruthLargeRJets
        alg.useTruthNeutrinos  = self.useTruthNeutrinos
        alg.useTruthMET        = self.useTruthMET
        alg.doOverlapRemoval   = self.doOverlapRemoval

        if self.useTruthElectrons:
            container = "ParticleLevelElectrons"
            config = self.createAndFillOutputContainer(config, container, "electrons")
            if self.elPtMin: alg.el_ptMin = self.elPtMin
            if self.elEtaMax: alg.el_etaMax = self.elEtaMax
            alg.el_notFromHadron = self.elNotFromHadron
            alg.el_tauIsHadron = self.elTauIsHadron
        if self.useTruthMuons:
            container = "ParticleLevelMuons"
            config = self.createAndFillOutputContainer(config, container, "muons")
            if self.muPtMin: alg.mu_ptMin = self.muPtMin
            if self.muEtaMax: alg.mu_etaMax = self.muEtaMax
            alg.mu_notFromHadron = self.muNotFromHadron
            alg.mu_tauIsHadron = self.muTauIsHadron
        if self.useTruthPhotons:
            container = "ParticleLevelPhotons"
            config = self.createAndFillOutputContainer(config, container, "photons")
            if self.phPtMin: alg.ph_ptMin = self.phPtMin
            if self.phEtaMax: alg.ph_etaMax = self.phEtaMax
            alg.ph_origin = self.phOrigin
            alg.ph_isolation = self.phIsolation
        if self.useTruthTaus:
            container = "ParticleLevelTaus"
            config = self.createAndFillOutputContainer(config, container, "taus")
            if self.tauPtMin: alg.tau_ptMin = self.tauPtMin
            if self.tauEtaMax: alg.tau_etaMax = self.tauEtaMax
        if self.useTruthJets:
            container = "ParticleLevelJets"
            config = self.createAndFillOutputContainer(config, container, "jets")
            if self.jetPtMin: alg.jet_ptMin = self.jetPtMin
            if self.jetEtaMax: alg.jet_etaMax = self.jetEtaMax
            config.addOutputVar("EventInfo","num_truth_bjets_nocuts","num_truth_bjets_nocuts",noSys=True)
            config.addOutputVar("EventInfo","num_truth_cjets_nocuts","num_truth_cjets_nocuts",noSys=True)
        if self.useTruthLargeRJets:
            container = "ParticleLevelLargeRJets"
            config = self.createAndFillOutputContainer(config, container, "ljets")
            if self.ljetCollection: alg.ljet_collection = self.ljetCollection
            elif config.isPhyslite(): alg.ljet_collection = 'AntiKt10TruthSoftDropBeta100Zcut10Jets'
            if self.ljetPtMin: alg.ljet_ptMin = self.ljetPtMin
            if self.ljetEtaMax: alg.ljet_etaMax = self.ljetEtaMax
        if self.useTruthNeutrinos:
            container = "ParticleLevelNeutrinos"
            config = self.createAndFillOutputContainer(config, container, "neutrinos")
            if self.nuPtMin: alg.nu_ptMin = self.nuPtMin
            if self.nuEtaMax: alg.nu_etaMax = self.nuEtaMax
        if self.useTruthMET:
            container = "ParticleLevelMissingET"
            config = self.createAndFillOutputContainer(config, container, "met", True)

        return

    def createAndFillOutputContainer(self, config, container, map_key, isMET=False):
        # create the output container for that object collection
        _ = config.writeName(container, isMet=isMET)

        # loop over branch mappings
        for mapping in particlelevel_branch_mappings[map_key]:
            inputname  = mapping.split("->")[0].rstrip()
            outputname = mapping.split("->")[1].lstrip()
            config.addOutputVar(container, inputname, outputname, noSys=True)

        return config

    def getOutputContainers(self):
        # return a dictionary of containers that can be added to the OutputAnalysisConfig
        containerDict = {}
        if self.useTruthElectrons:
            containerDict['el_']   = 'ParticleLevelElectrons'
        if self.useTruthMuons:
            containerDict['mu_']   = 'ParticleLevelMuons'
        if self.useTruthPhotons:
            containerDict['ph_']   = 'ParticleLevelPhotons'
        if self.useTruthTaus:
            containerDict['tau_']  = 'ParticleLevelTaus'
        if self.useTruthJets:
            containerDict['jet_']  = 'ParticleLevelJets'
        if self.useTruthLargeRJets:
            containerDict['ljet_'] = 'ParticleLevelLargeRJets'
        if self.useTruthNeutrinos:
            containerDict['nu_']   = 'ParticleLevelNeutrinos'
        if self.useTruthMET:
            containerDict['met_']  = 'ParticleLevelMissingET'

        return containerDict
