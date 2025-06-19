"""
    Copyright (C) 2002-2025 CERN for the benefit of the ATLAS collaboration
    Implemented by Zackary Alegria following instructions from the JETM
    twiki recommendation on Dijet sample normalization
    https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/JetEtMissMCSamples#Dijet_normalization_procedure_HS
"""

from AnalysisAlgorithmsConfig.ConfigBlock import ConfigBlock


class HSTPFilterConfig(ConfigBlock):
    """Config Block for the HSTPFilter algorithm"""

    def __init__(self):
        super(HSTPFilterConfig, self).__init__()
        self.addOption(
            "truthHSCollection",
            "AntiKt4TruthDressedWZJets",
            type=str,
            info="Name of the truth HS jet collection",
        )
        self.addOption(
            "truthPUCollection",
            "InTimeAntiKt4TruthJets",
            type=str,
            info="Name of the truth PU jet collection",
        )
        baselinePy8DijetDSIDs = [str(dijetDSID) for dijetDSID in range(364700, 364713)]
        self.setOptionValue("skipOnData", True)
        self.setOptionValue("onlyForDSIDs", baselinePy8DijetDSIDs)

    def makeAlgs(self, config):

        alg = config.createAlgorithm("CP::HSTPFilterAlg", "HSTPFilterAlg")

        alg.truthHSCollection = self.truthHSCollection
        alg.truthPUCollection = self.truthPUCollection
