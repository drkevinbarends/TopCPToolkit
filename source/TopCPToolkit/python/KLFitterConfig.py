from AnalysisAlgorithmsConfig.ConfigBlock import ConfigBlock


class KLFitterConfig(ConfigBlock):
    """ConfigBlock for KLFitter algorithms"""

    def __init__(self, containerName):
        super(KLFitterConfig, self).__init__()
        self.containerName = containerName
        self.addOption('electrons', '', type=str)
        self.addOption('muons', '', type=str)
        self.addOption('jets', '', type=str)
        self.addOption('met', '', type=str)
        self.addOption('likelihoodType', '', type=str)
        self.addOption('leptonType', '', type=str)
        self.addOption('jetSelectionMode', '', type=str)
        self.addOption('btaggingMethod', 'kNoTag', type=str)
        self.addOption('bTagCDIFile',
                       'xAODBTaggingEfficiency/13TeV/2022-22-13TeV-MC20-CDI-2022-07-28_v1.root', type=str)
        self.addOption('btagger', 'DL1dv00', type=str)
        self.addOption('btagWP', 'FixedCutBEff_77', type=str)
        self.addOption('btagIgnoreOutOfValidityRange', False, type=bool)
        self.addOption('selectionRegionsConfig', '', type=str)
        self.addOption('saveAllPermutations', False, type=bool)
        # list of dictionaries for the per-region config options
        self.perRegionConfiguration = list()

    def parseSelectionRegionsConfig(self):
        regions = self.selectionRegionsConfig.split(';')
        if len(regions) == 0:
            raise Exception('KLFitterConfig: Could not determine any regions in your SelectionRegionsConfig')
        for reg in regions:
            regstrip = reg.replace(' ', '')
            regionopts = dict(tuple(option.split(':')) for option in regstrip.split(','))
            if 'selectionName' not in regionopts:
                raise Exception('KLFitterConfig: Could not parse SelectionRegionsConfig selectionName for region ', reg)
            if 'likelihoodType' in regionopts:
                raise Exception('KLFitterConfig: likelihoodType cannot be overriden per region. Create a separate instance of KLFitter block with different likelihoodType instead.')

            self.perRegionConfiguration.append(regionopts)

    def makeAlgs(self, config):
        self.parseSelectionRegionsConfig()
        for perRegionConfig in self.perRegionConfiguration:
            selectionName = perRegionConfig['selectionName']
            alg = config.createAlgorithm('top::KLFitterAlg',
                                         f'KLFitterAlg_{self.containerName}_{selectionName}')
            # input objects and their object selections
            alg.electrons, alg.electronSelection = config.readNameAndSelection(self.electrons)
            alg.muons, alg.muonSelection = config.readNameAndSelection(self.muons)
            alg.jets, alg.jetSelection = config.readNameAndSelection(self.jets)
            alg.met = config.readName(self.met)
            alg.result = self.containerName + '_%SYS%'

            # global settings, in future expect to expose more options for configuration
            alg.SaveAllPermutations = self.saveAllPermutations

            # these settings can be defined per-region, but if not, we fallback to global setting
            alg.selectionDecorationName = selectionName + '_%SYS%,as_char'
            alg.LHType = self.likelihoodType
            alg.LeptonType = perRegionConfig.get('leptonType', self.leptonType)
            alg.JetSelectionMode = perRegionConfig.get('jetSelectionMode', self.jetSelectionMode)
            btagAlgo = perRegionConfig.get('btagger', self.btagger)
            btagWP = perRegionConfig.get('btagWP', self.btagWP)
            alg.BTaggingDecoration = f'ftag_select_{btagAlgo}_{btagWP}'

            alg.BTaggingMethod = perRegionConfig.get('btaggingMethod', self.btaggingMethod)
            if alg.BTaggingMethod == 'kWorkingPoint':
                config.addPrivateTool('BTaggingEfficiencyTool', 'BTaggingEfficiencyTool')
                alg.BTaggingEfficiencyTool.TaggerName = self.btagger
                alg.BTaggingEfficiencyTool.OperatingPoint = self.btagWP
                jetCollection = config.originalName(self.jets.split('.')[0])
                alg.BTaggingEfficiencyTool.JetAuthor = jetCollection
                alg.BTaggingEfficiencyTool.ScaleFactorFileName = self.bTagCDIFile
                alg.BTaggingEfficiencyTool.IgnoreOutOfValidityRange = self.btagIgnoreOutOfValidityRange
                alg.BTaggingEfficiencyTool.MinPt = 20e3  # hardcoded to the recommendation for EMPFlow at the moment
                # NOTE the efficiency tool is simply set to the default generator,
                # meaning the results are not correct for alternative showering generators!!

        finalizeAlg = config.createAlgorithm('top::KLFitterFinalizeOutputAlg', 'KLFitterFinalizeOutputAlg_' + self.containerName)
        finalizeAlg.resultContainerToCheck = self.containerName + '_%SYS%'
        finalizeAlg.resultContainerToWrite = self.containerName + '_%SYS%'

        config.setSourceName(self.containerName, self.containerName)
        config.addOutputContainer(self.containerName, self.containerName + '_%SYS%')

        config.addOutputVar(self.containerName, 'eventProbability', 'eventProbability')
        config.addOutputVar(self.containerName, 'logLikelihood', 'logLikelihood')
        if self.saveAllPermutations:
            config.addOutputVar(self.containerName, 'selected', 'selected')

        if self.likelihoodType != 'ttbar_Allhad':
            config.addOutputVar(self.containerName, 'model_bhad_jetIndex', 'bhad_jetIndex')
            config.addOutputVar(self.containerName, 'model_blep_jetIndex', 'blep_jetIndex')
            config.addOutputVar(self.containerName, 'model_lq1_jetIndex', 'lq1_jetIndex')
            # boosted has only one light jet
            if self.likelihoodType != 'ttbar_BoostedLJets':
                config.addOutputVar(self.containerName, 'model_lq2_jetIndex', 'lq2_jetIndex')
            if self.likelihoodType == 'ttH':
                config.addOutputVar(self.containerName, 'model_Higgs_b1_jetIndex', 'Higgs_b1_jetIndex')
                config.addOutputVar(self.containerName, 'model_Higgs_b2_jetIndex', 'Higgs_b2_jetIndex')

            config.addOutputVar(self.containerName, 'model_nu_pt', 'nu_pt')
            config.addOutputVar(self.containerName, 'model_nu_eta', 'nu_eta')
            config.addOutputVar(self.containerName, 'model_nu_phi', 'nu_phi')
            config.addOutputVar(self.containerName, 'model_nu_E', 'nu_E')

            # users have to check if this is 3-el or 3-mu region to check index for appropriate lepton container
            if self.likelihoodType == 'ttZTrilepton':
                # lepton from W and neutrino
                # users have to take care to check if this is electron or muon region
                config.addOutputVar(self.containerName, 'model_lep_index', 'lep_index')
                config.addOutputVar(self.containerName, 'model_lepZ1_index', 'lepZ1_index')
                config.addOutputVar(self.containerName, 'model_lepZ2_index', 'lepZ2_index')
        else:  # all-had ttbar
            config.addOutputVar(self.containerName, 'model_b_from_top1_jetIndex', 'b_from_top1_jetIndex')
            config.addOutputVar(self.containerName, 'model_b_from_top2_jetIndex', 'b_from_top2_jetIndex')
            config.addOutputVar(self.containerName, 'model_lj1_from_top1_jetIndex', 'lj1_from_top1_jetIndex')
            config.addOutputVar(self.containerName, 'model_lj2_from_top1_jetIndex', 'lj2_from_top1_jetIndex')
            config.addOutputVar(self.containerName, 'model_lj1_from_top2_jetIndex', 'lj1_from_top2_jetIndex')
            config.addOutputVar(self.containerName, 'model_lj2_from_top2_jetIndex', 'lj2_from_top2_jetIndex')
