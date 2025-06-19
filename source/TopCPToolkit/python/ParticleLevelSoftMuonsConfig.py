# Copyright (C) 2002-2025 CERN for the benefit of the ATLAS collaboration

from AnalysisAlgorithmsConfig.ConfigBlock import ConfigBlock


class ParticleLevelSoftMuonsBlock(ConfigBlock):
    """ConfigBlock for particle-level truth soft muons"""

    def __init__(self):
        super(ParticleLevelSoftMuonsBlock, self).__init__()
        self.addOption(
            "muons",
            "",
            type=str,
            info="the name of the input truth soft muons container",
        )
        self.addOption(
            "jets",
            "",
            type=str,
            info="the name of the input truth jets container",
        )
        self.addOption(
            "useRapidityForDeltaR",
            True,
            type=bool,
            info="use rapidity for muon-jet matching (default: True; if False use pseudorapidity)",
        )
        self.addOption(
            "useDressedProperties",
            False,
            type=bool,
            info="use dressed (True, default) or bare (False) momentum for soft muon",
        )
        # Always skip on data
        self.setOptionValue("skipOnData", True)

    def makeAlgs(self, config):

        alg = config.createAlgorithm(
            "CP::ParticleLevelSoftMuonAlg",
            "ParticleLevelSoftMuonAlg" + self.muons + self.jets,
        )
        alg.muons, alg.muonSelection = config.readNameAndSelection(self.muons)
        alg.jets, alg.jetSelection = config.readNameAndSelection(self.jets)
        alg.useRapidityForDeltaR = self.useRapidityForDeltaR
        alg.useDressedProperties = self.useDressedProperties
        alg.jetAssociated = "jetAssociated"

        config.addSelection(
            alg.muons, alg.jetAssociated, alg.jetAssociated + ",as_char"
        )

        # output branches to be scheduled only once
        if ParticleLevelSoftMuonsBlock.get_instance_count() == 1:
            outputVars = [
                ["barcode", "barcode"],
                ["truthBAncestor_pdgId", "truthBAncestor_pdgId"],
                ["truthBAncestor_pt", "truthBAncestor_pt"],
                ["truthBAncestor_eta", "truthBAncestor_eta"],
                ["truthBAncestor_phi", "truthBAncestor_phi"],
                ["truthBAncestor_e", "truthBAncestor_e"],
                ["truthCAncestor_pdgId", "truthCAncestor_pdgId"],
                ["truthCAncestor_pt", "truthCAncestor_pt"],
                ["truthCAncestor_eta", "truthCAncestor_eta"],
                ["truthCAncestor_phi", "truthCAncestor_phi"],
                ["truthCAncestor_e", "truthCAncestor_e"],
                ["truthTauAncestor_pdgId", "truthTauAncestor_pdgId"],
                ["truthFirstBAncestor_pdgId", "truthFirstBAncestor_pdgId"],
                ["truthFirstCAncestor_pdgId", "truthFirstCAncestor_pdgId"],
            ]
            for decoration, branch in outputVars:
                config.addOutputVar(alg.muons, decoration, branch, noSys=True)
