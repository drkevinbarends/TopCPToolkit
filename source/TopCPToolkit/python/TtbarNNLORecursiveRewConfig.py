from AnalysisAlgorithmsConfig.ConfigBlock import ConfigBlock
from AnalysisAlgorithmsConfig.ConfigAccumulator import DataType
from AthenaConfiguration.Enums import LHCPeriod


class TtbarNNLORecursiveRewConfig(ConfigBlock):
    """ConfigBlock for ttbar NNLO reweighting algorithms"""

    def __init__(self):
        super(TtbarNNLORecursiveRewConfig, self).__init__()
        self.addOption('sampleID', 'AutoConfig', type=str)
        self.addOption('reweightType', '2D', type=str)
        self.addOption('reweightPath', 'dev/AnalysisTop/TTbarNNLOReweighter', type=str)
        self.addOption('reweightSuffix', '_3iter_2023_LUX', type=str)
        self.addOption('nominalOnly', True, type=bool)

    def makeAlgs(self, config):
        # don't run on data!
        if config.dataType() is DataType.Data:
            return

        if config.geometry() == LHCPeriod.Run3:
            print("Run 3 NNLO ttbar reweighting is not supported yet! skipping the algorithm.")
            return

        # internal mapping for the tool
        map_sampleID = {
           "Nominal" : [410470],
           "PH7new": [],
           "aMCH7": [],
           "PH7": [],
           "aMCP8": [],
           "PP8hdamp": [],
           "PP8muRup": [],
           "PP8muRdown": [],
           "PP8muFup": [],
           "PP8muFdown": [],
           "PP8FSRup": [],
           "PP8FSRdown": [],
           "PP8pthard1": [],
           "MECoff": [],
        }
        if self.sampleID == 'AutoConfig':
            for key, value in map_sampleID.items():
                if config.dsid() in value:
                    self.sampleID = key
                    break
            else:
                # this sample is not supported for reweighting:
                # simply don't set up the algorithm
                print(f"Sample with DSID {config.dsid()} is not supported for ttbar NNLO reweighting, skipping the algorithm.")
                return

        alg = config.createAlgorithm('top::RunTtbarNNLORecursiveRewAlg', 'RunTtbarNNLORecursiveRewAlg'+self.reweightType)
        alg.sampleID = self.sampleID
        alg.reweightingType = self.reweightType
        alg.reweightingPath = self.reweightPath
        alg.reweightingSuffix = self.reweightSuffix
        if self.nominalOnly:
            alg.affectingSystematicsFilter = '.*'

        # give appropriate names for the handles to decorate
        alg.NNLO_weight = 'NNLO_'+self.reweightType+'_weight_%SYS%'
        config.addOutputVar('EventInfo', alg.NNLO_weight, 'weight_NNLO_'+self.reweightType)
