from AnalysisAlgorithmsConfig.ConfigBlock import ConfigBlock
from AnalysisAlgorithmsConfig.ConfigAccumulator import DataType


def replace_in_list(string_list, old, new):
    """
    Replace occurrences of a substring in each item of a list of strings.

    Parameters:
    string_list (list): List of strings to be processed.
    old (str): The substring to be replaced.
    new (str): The substring to replace with.

    Returns:
    list: New list with replaced strings.
    """
    return [item.replace(old, new) for item in string_list]


branches = {
    "t": [
        "MC_t_beforeFSR_pt",
        "MC_t_beforeFSR_eta",
        "MC_t_beforeFSR_phi",
        "MC_t_beforeFSR_m",
        "MC_t_beforeFSR_pdgId",
        "MC_t_afterFSR_pt",
        "MC_t_afterFSR_eta",
        "MC_t_afterFSR_phi",
        "MC_t_afterFSR_m",
        "MC_t_afterFSR_pdgId",
        "MC_W_beforeFSR_from_t_pt",
        "MC_W_beforeFSR_from_t_eta",
        "MC_W_beforeFSR_from_t_phi",
        "MC_W_beforeFSR_from_t_m",
        "MC_W_beforeFSR_from_t_pdgId",
        "MC_W_afterFSR_from_t_pt",
        "MC_W_afterFSR_from_t_eta",
        "MC_W_afterFSR_from_t_phi",
        "MC_W_afterFSR_from_t_m",
        "MC_W_afterFSR_from_t_pdgId",
        "MC_b_beforeFSR_from_t_pt",
        "MC_b_beforeFSR_from_t_eta",
        "MC_b_beforeFSR_from_t_phi",
        "MC_b_beforeFSR_from_t_m",
        "MC_b_beforeFSR_from_t_pdgId",
        "MC_b_afterFSR_from_t_pt",
        "MC_b_afterFSR_from_t_eta",
        "MC_b_afterFSR_from_t_phi",
        "MC_b_afterFSR_from_t_m",
        "MC_b_afterFSR_from_t_pdgId",
        "MC_Wdecay1_beforeFSR_from_t_pt",
        "MC_Wdecay1_beforeFSR_from_t_eta",
        "MC_Wdecay1_beforeFSR_from_t_phi",
        "MC_Wdecay1_beforeFSR_from_t_m",
        "MC_Wdecay1_beforeFSR_from_t_pdgId",
        "MC_Wdecay2_beforeFSR_from_t_pt",
        "MC_Wdecay2_beforeFSR_from_t_eta",
        "MC_Wdecay2_beforeFSR_from_t_phi",
        "MC_Wdecay2_beforeFSR_from_t_m",
        "MC_Wdecay2_beforeFSR_from_t_pdgId",
        "MC_Wdecay1_afterFSR_from_t_pt",
        "MC_Wdecay1_afterFSR_from_t_eta",
        "MC_Wdecay1_afterFSR_from_t_phi",
        "MC_Wdecay1_afterFSR_from_t_m",
        "MC_Wdecay1_afterFSR_from_t_pdgId",
        "MC_Wdecay2_afterFSR_from_t_pt",
        "MC_Wdecay2_afterFSR_from_t_eta",
        "MC_Wdecay2_afterFSR_from_t_phi",
        "MC_Wdecay2_afterFSR_from_t_m",
        "MC_Wdecay2_afterFSR_from_t_pdgId",
    ],
    "tbar": [
        "MC_tbar_beforeFSR_pt",
        "MC_tbar_beforeFSR_eta",
        "MC_tbar_beforeFSR_phi",
        "MC_tbar_beforeFSR_m",
        "MC_tbar_beforeFSR_pdgId",
        "MC_tbar_afterFSR_pt",
        "MC_tbar_afterFSR_eta",
        "MC_tbar_afterFSR_phi",
        "MC_tbar_afterFSR_m",
        "MC_tbar_afterFSR_pdgId",
        "MC_W_beforeFSR_from_tbar_pt",
        "MC_W_beforeFSR_from_tbar_eta",
        "MC_W_beforeFSR_from_tbar_phi",
        "MC_W_beforeFSR_from_tbar_m",
        "MC_W_beforeFSR_from_tbar_pdgId",
        "MC_W_afterFSR_from_tbar_pt",
        "MC_W_afterFSR_from_tbar_eta",
        "MC_W_afterFSR_from_tbar_phi",
        "MC_W_afterFSR_from_tbar_m",
        "MC_W_afterFSR_from_tbar_pdgId",
        "MC_bbar_beforeFSR_from_tbar_pt",
        "MC_bbar_beforeFSR_from_tbar_eta",
        "MC_bbar_beforeFSR_from_tbar_phi",
        "MC_bbar_beforeFSR_from_tbar_m",
        "MC_bbar_beforeFSR_from_tbar_pdgId",
        "MC_bbar_afterFSR_from_tbar_pt",
        "MC_bbar_afterFSR_from_tbar_eta",
        "MC_bbar_afterFSR_from_tbar_phi",
        "MC_bbar_afterFSR_from_tbar_m",
        "MC_bbar_afterFSR_from_tbar_pdgId",
        "MC_Wdecay1_beforeFSR_from_tbar_pt",
        "MC_Wdecay1_beforeFSR_from_tbar_eta",
        "MC_Wdecay1_beforeFSR_from_tbar_phi",
        "MC_Wdecay1_beforeFSR_from_tbar_m",
        "MC_Wdecay1_beforeFSR_from_tbar_pdgId",
        "MC_Wdecay2_beforeFSR_from_tbar_pt",
        "MC_Wdecay2_beforeFSR_from_tbar_eta",
        "MC_Wdecay2_beforeFSR_from_tbar_phi",
        "MC_Wdecay2_beforeFSR_from_tbar_m",
        "MC_Wdecay2_beforeFSR_from_tbar_pdgId",
        "MC_Wdecay1_afterFSR_from_tbar_pt",
        "MC_Wdecay1_afterFSR_from_tbar_eta",
        "MC_Wdecay1_afterFSR_from_tbar_phi",
        "MC_Wdecay1_afterFSR_from_tbar_m",
        "MC_Wdecay1_afterFSR_from_tbar_pdgId",
        "MC_Wdecay2_afterFSR_from_tbar_pt",
        "MC_Wdecay2_afterFSR_from_tbar_eta",
        "MC_Wdecay2_afterFSR_from_tbar_phi",
        "MC_Wdecay2_afterFSR_from_tbar_m",
        "MC_Wdecay2_afterFSR_from_tbar_pdgId",
    ],
    "ttbar": [
        "MC_ttbar_beforeFSR_pt",
        "MC_ttbar_beforeFSR_eta",
        "MC_ttbar_beforeFSR_phi",
        "MC_ttbar_beforeFSR_m",
        "MC_ttbar_fromDecay_beforeFSR_pt",
        "MC_ttbar_fromDecay_beforeFSR_eta",
        "MC_ttbar_fromDecay_beforeFSR_phi",
        "MC_ttbar_fromDecay_beforeFSR_m",
        "MC_ttbar_fromDecay_afterFSR_pt",
        "MC_ttbar_fromDecay_afterFSR_eta",
        "MC_ttbar_fromDecay_afterFSR_phi",
        "MC_ttbar_fromDecay_afterFSR_m",
        "MC_ttbar_afterFSR_pt",
        "MC_ttbar_afterFSR_eta",
        "MC_ttbar_afterFSR_phi",
        "MC_ttbar_afterFSR_m",
    ],
    "b": [
        "MC_b_beforeFSR_pt",
        "MC_b_beforeFSR_eta",
        "MC_b_beforeFSR_phi",
        "MC_b_beforeFSR_m",
        "MC_b_beforeFSR_pdgId",
        "MC_b_afterFSR_pt",
        "MC_b_afterFSR_eta",
        "MC_b_afterFSR_phi",
        "MC_b_afterFSR_m",
        "MC_b_afterFSR_pdgId",
    ],
    "bbar": [
        "MC_bbar_beforeFSR_pt",
        "MC_bbar_beforeFSR_eta",
        "MC_bbar_beforeFSR_phi",
        "MC_bbar_beforeFSR_m",
        "MC_bbar_beforeFSR_pdgId",
        "MC_bbar_afterFSR_pt",
        "MC_bbar_afterFSR_eta",
        "MC_bbar_afterFSR_phi",
        "MC_bbar_afterFSR_m",
        "MC_bbar_afterFSR_pdgId",
    ],
    "Z": [
        "MC_Z_beforeFSR_pt",
        "MC_Z_beforeFSR_eta",
        "MC_Z_beforeFSR_phi",
        "MC_Z_beforeFSR_m",
        "MC_Z_beforeFSR_pdgId",
        "MC_Z_afterFSR_pt",
        "MC_Z_afterFSR_eta",
        "MC_Z_afterFSR_phi",
        "MC_Z_afterFSR_m",
        "MC_Z_afterFSR_pdgId",
        "MC_Zdecay1_beforeFSR_pt",
        "MC_Zdecay1_beforeFSR_eta",
        "MC_Zdecay1_beforeFSR_phi",
        "MC_Zdecay1_beforeFSR_m",
        "MC_Zdecay1_beforeFSR_pdgId",
        "MC_Zdecay1_afterFSR_pt",
        "MC_Zdecay1_afterFSR_eta",
        "MC_Zdecay1_afterFSR_phi",
        "MC_Zdecay1_afterFSR_m",
        "MC_Zdecay1_afterFSR_pdgId",
        "MC_Zdecay2_beforeFSR_pt",
        "MC_Zdecay2_beforeFSR_eta",
        "MC_Zdecay2_beforeFSR_phi",
        "MC_Zdecay2_beforeFSR_m",
        "MC_Zdecay2_beforeFSR_pdgId",
        "MC_Zdecay2_afterFSR_pt",
        "MC_Zdecay2_afterFSR_eta",
        "MC_Zdecay2_afterFSR_phi",
        "MC_Zdecay2_afterFSR_m",
        "MC_Zdecay2_afterFSR_pdgId",
        "MC_Z_IsOnShell",
    ],
    "W": [
        "MC_W_beforeFSR_pt",
        "MC_W_beforeFSR_eta",
        "MC_W_beforeFSR_phi",
        "MC_W_beforeFSR_m",
        "MC_W_beforeFSR_pdgId",
        "MC_W_afterFSR_pt",
        "MC_W_afterFSR_eta",
        "MC_W_afterFSR_phi",
        "MC_W_afterFSR_m",
        "MC_W_afterFSR_pdgId",
        "MC_Wdecay1_beforeFSR_pt",
        "MC_Wdecay1_beforeFSR_eta",
        "MC_Wdecay1_beforeFSR_phi",
        "MC_Wdecay1_beforeFSR_m",
        "MC_Wdecay1_beforeFSR_pdgId",
        "MC_Wdecay1_afterFSR_pt",
        "MC_Wdecay1_afterFSR_eta",
        "MC_Wdecay1_afterFSR_phi",
        "MC_Wdecay1_afterFSR_m",
        "MC_Wdecay1_afterFSR_pdgId",
        "MC_Wdecay2_beforeFSR_pt",
        "MC_Wdecay2_beforeFSR_eta",
        "MC_Wdecay2_beforeFSR_phi",
        "MC_Wdecay2_beforeFSR_m",
        "MC_Wdecay2_beforeFSR_pdgId",
        "MC_Wdecay2_afterFSR_pt",
        "MC_Wdecay2_afterFSR_eta",
        "MC_Wdecay2_afterFSR_phi",
        "MC_Wdecay2_afterFSR_m",
        "MC_Wdecay2_afterFSR_pdgId",
        "MC_W_IsOnShell",
    ],
    "Photon": [
        "MC_gamma_m",
        "MC_gamma_pt",
        "MC_gamma_eta",
        "MC_gamma_phi",
        "MC_gamma_origin",
    ],
    "Higgs": [
        "MC_H_beforeFSR_pt",
        "MC_H_beforeFSR_eta",
        "MC_H_beforeFSR_phi",
        "MC_H_beforeFSR_m",
        "MC_H_beforeFSR_pdgId",
        "MC_H_afterFSR_pt",
        "MC_H_afterFSR_eta",
        "MC_H_afterFSR_phi",
        "MC_H_afterFSR_m",
        "MC_H_afterFSR_pdgId",
        "MC_Hdecay1_beforeFSR_pt",
        "MC_Hdecay1_beforeFSR_eta",
        "MC_Hdecay1_beforeFSR_phi",
        "MC_Hdecay1_beforeFSR_m",
        "MC_Hdecay1_beforeFSR_pdgId",
        "MC_Hdecay1_afterFSR_pt",
        "MC_Hdecay1_afterFSR_eta",
        "MC_Hdecay1_afterFSR_phi",
        "MC_Hdecay1_afterFSR_m",
        "MC_Hdecay1_afterFSR_pdgId",
        "MC_Hdecay2_beforeFSR_pt",
        "MC_Hdecay2_beforeFSR_eta",
        "MC_Hdecay2_beforeFSR_phi",
        "MC_Hdecay2_beforeFSR_m",
        "MC_Hdecay2_beforeFSR_pdgId",
        "MC_Hdecay2_afterFSR_pt",
        "MC_Hdecay2_afterFSR_eta",
        "MC_Hdecay2_afterFSR_phi",
        "MC_Hdecay2_afterFSR_m",
        "MC_Hdecay2_afterFSR_pdgId",
        "MC_Hdecay1_decay1_beforeFSR_pt",
        "MC_Hdecay1_decay1_beforeFSR_eta",
        "MC_Hdecay1_decay1_beforeFSR_phi",
        "MC_Hdecay1_decay1_beforeFSR_m",
        "MC_Hdecay1_decay1_beforeFSR_pdgId",
        "MC_Hdecay1_decay1_afterFSR_pt",
        "MC_Hdecay1_decay1_afterFSR_eta",
        "MC_Hdecay1_decay1_afterFSR_phi",
        "MC_Hdecay1_decay1_afterFSR_m",
        "MC_Hdecay1_decay1_afterFSR_pdgId",
        "MC_Hdecay2_decay1_beforeFSR_pt",
        "MC_Hdecay2_decay1_beforeFSR_eta",
        "MC_Hdecay2_decay1_beforeFSR_phi",
        "MC_Hdecay2_decay1_beforeFSR_m",
        "MC_Hdecay2_decay1_beforeFSR_pdgId",
        "MC_Hdecay2_decay1_afterFSR_pt",
        "MC_Hdecay2_decay1_afterFSR_eta",
        "MC_Hdecay2_decay1_afterFSR_phi",
        "MC_Hdecay2_decay1_afterFSR_m",
        "MC_Hdecay2_decay1_afterFSR_pdgId",
        "MC_Hdecay1_decay2_beforeFSR_pt",
        "MC_Hdecay1_decay2_beforeFSR_eta",
        "MC_Hdecay1_decay2_beforeFSR_phi",
        "MC_Hdecay1_decay2_beforeFSR_m",
        "MC_Hdecay1_decay2_beforeFSR_pdgId",
        "MC_Hdecay1_decay2_afterFSR_pt",
        "MC_Hdecay1_decay2_afterFSR_eta",
        "MC_Hdecay1_decay2_afterFSR_phi",
        "MC_Hdecay1_decay2_afterFSR_m",
        "MC_Hdecay1_decay2_afterFSR_pdgId",
        "MC_Hdecay2_decay2_beforeFSR_pt",
        "MC_Hdecay2_decay2_beforeFSR_eta",
        "MC_Hdecay2_decay2_beforeFSR_phi",
        "MC_Hdecay2_decay2_beforeFSR_m",
        "MC_Hdecay2_decay2_beforeFSR_pdgId",
        "MC_Hdecay2_decay2_afterFSR_pt",
        "MC_Hdecay2_decay2_afterFSR_eta",
        "MC_Hdecay2_decay2_afterFSR_phi",
        "MC_Hdecay2_decay2_afterFSR_m",
        "MC_Hdecay2_decay2_afterFSR_pdgId",
    ],
}

truth_branches = {
    "Ttbar": branches["t"] + branches["tbar"] + branches["ttbar"],
    "Ttbarbbbar": branches["t"]
    + branches["tbar"]
    + branches["ttbar"]
    + branches["b"]
    + branches["bbar"],
    "Ttz": branches["t"] + branches["tbar"] + branches["ttbar"] + branches["Z"],
    "Ttw": branches["t"] + branches["tbar"] + branches["ttbar"] + branches["W"],
    "Tth": branches["t"] + branches["tbar"] + branches["ttbar"] + branches["Higgs"],
    "Ttgamma": branches["t"]
    + branches["tbar"]
    + branches["ttbar"]
    + branches["Photon"],
    "Tzq": branches["t"] + branches["Z"] + branches["b"],
    "Thq": branches["t"] + branches["Higgs"] + branches["b"],
    "Tqgamma": branches["t"] + branches["Photon"] + branches["b"],
    "FourTop": replace_in_list(branches["t"], "t_", "t1_")
    + replace_in_list(branches["t"], "t_", "t2_")
    + replace_in_list(branches["tbar"], "tbar_", "tbar1_")
    + replace_in_list(branches["tbar"], "tbar_", "tbar2_"),
    "HWW": branches["Higgs"],
    "HWW_nonresonant": branches["Higgs"],
    "HZZ": branches["Higgs"],
    "Zb": branches["Z"] + branches["b"] + branches["bbar"],
}


class truthConfig(ConfigBlock):
    """ConfigBlock for truth/parton-level objects"""

    def __init__(self):
        super(truthConfig, self).__init__()
        self.addOption("histories", None, type=str)

    def makeAlgs(self, config):

        if config.dataType() is DataType.Data:
            return

        # loop over all histories
        for history in self.getPartonHistories():
            alg = config.createAlgorithm(
                "top::RunPartonHistoryAlg", "PartonHistory" + history
            )
            alg.partonScheme = history
            container = "PartonHistory" + history
            _ = config.writeName(container)

            for branch in truth_branches[history]:
                config.addOutputVar(container, branch, branch, noSys=True)

    def getPartonHistories(self):
        # parton histories can be provided as History1.History2.etc
        return self.histories.split(".")

    def getOutputContainers(self):
        # return a dictionary of containers that can be added to the OutputAnalysisConfig
        containerDict = {}
        for history in self.getPartonHistories():
            containerDict[history + "_"] = "PartonHistory" + history
        return containerDict
