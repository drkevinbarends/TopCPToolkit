from AnalysisAlgorithmsConfig.ConfigBlock import ConfigBlock


class BoostedJetTaggersConfig(ConfigBlock):
    """Skeleton of a Config Block, for the TopCPToolkit tutorial"""

    def __init__(self):
        super(BoostedJetTaggersConfig, self).__init__()
        self.addOption("ljets", None, type=str)
        self.addOption("taggerType", "JSSWTopTaggerDNN", type=str)
        self.addOption(
            "taggerCalibArea", "Winter2024_R22_PreRecs/JSSWTopTaggerDNN/", type=str
        )
        self.addOption(
            "taggerConfigFile",
            "DNNTagger_AntiKt10UFOSD_TopContained80_Oct30.dat",
            type=str,
        )
        self.addOption("taggerDecoration", "DNNTaggerTopQuarkContained80", type=str)

    def makeAlgs(self, config):

        from AnalysisAlgorithmsConfig.ConfigAccumulator import DataType

        isMC = not (config.dataType() == DataType.Data)

        ljets = config.readName(self.ljets)

        alg = config.createAlgorithm(
            "top::BoostedJetTaggersAlg", "BoostedJetTaggers_" + self.taggerDecoration
        )
        alg.ljets = ljets
        alg.isMC = isMC
        alg.taggerType = self.taggerType
        alg.taggerCalibArea = self.taggerCalibArea
        alg.taggerConfigFile = self.taggerConfigFile
        alg.taggerDecoration = self.taggerDecoration

        config.addOutputVar(
            self.ljets,
            self.taggerDecoration + "_tagged_%SYS%",
            self.taggerDecoration + "_tagged",
        )
        config.addOutputVar(
            self.ljets,
            self.taggerDecoration + "_score_%SYS%",
            self.taggerDecoration + "_score",
        )
        config.addOutputVar(
            self.ljets,
            self.taggerDecoration + "_passMass_%SYS%",
            self.taggerDecoration + "_passMass",
        )
        config.addOutputVar(
            self.ljets,
            self.taggerDecoration + "_validKinRange_%SYS%",
            self.taggerDecoration + "_validKinRange",
        )
        config.addOutputVar(
            self.ljets,
            self.taggerDecoration + "_SF_%SYS%",
            self.taggerDecoration + "_SF",
        )
        config.addOutputVar(
            self.ljets,
            self.taggerDecoration + "_efficiency_%SYS%",
            self.taggerDecoration + "_efficiency",
        )
        config.addOutputVar(
            self.ljets,
            self.taggerDecoration + "_effSF_%SYS%",
            self.taggerDecoration + "_effSF",
        )
