import GridSubmission.grid

# Full Run 2 Data
GridSubmission.grid.Add('PHYS_tWZ_Data_Run2').datasets = [
    'data15_13TeV:data15_13TeV.periodAllYear.physics_Main.PhysCont.DAOD_PHYS.grp15_v01_p6697',
    'data16_13TeV:data16_13TeV.periodAllYear.physics_Main.PhysCont.DAOD_PHYS.grp16_v01_p6697',
    'data17_13TeV:data17_13TeV.periodAllYear.physics_Main.PhysCont.DAOD_PHYS.grp17_v01_p6697',
    'data18_13TeV:data18_13TeV.periodAllYear.physics_Main.PhysCont.DAOD_PHYS.grp18_v01_p6697'
]

# Nominal Signal Full Run 2 
GridSubmission.grid.Add('PHYS_tWZ_MC_Signal_tWZ_Run2').datasets = [
    'mc20_13TeV:mc20_13TeV.525955.aMCPy8EG_tWZ_Ztoll_DR1.deriv.DAOD_PHYS.e8553_a907_r14859_p6697',
    'mc20_13TeV:mc20_13TeV.525955.aMCPy8EG_tWZ_Ztoll_DR1.deriv.DAOD_PHYS.e8553_a907_r14860_p6697',
    'mc20_13TeV:mc20_13TeV.525955.aMCPy8EG_tWZ_Ztoll_DR1.deriv.DAOD_PHYS.e8553_a907_r14861_p6697'
]

# Signal Systematics Full Run 2
GridSubmission.grid.Add('PHYS_tWZ_MC_SystematicSample_tWZ_Run2').datasets = [
    'mc20_13TeV:mc20_13TeV.525956.aMCPy8EG_tWZ_Ztoll_DR2.deriv.DAOD_PHYS.e8553_a907_r14859_p6697',
    'mc20_13TeV:mc20_13TeV.525956.aMCPy8EG_tWZ_Ztoll_DR2.deriv.DAOD_PHYS.e8553_a907_r14860_p6697',
    'mc20_13TeV:mc20_13TeV.525956.aMCPy8EG_tWZ_Ztoll_DR2.deriv.DAOD_PHYS.e8553_a907_r14861_p6697'
]

# Nominal Background (Main - ttZ) Full Run 2
GridSubmission.grid.Add('PHYS_tWZ_MC_Background_ttZ_Run2').datasets = [
    'mc20_13TeV:mc20_13TeV.700309.Sh_2211_ttll.deriv.DAOD_PHYS.e8312_s3797_r13167_p6490',
    'mc20_13TeV:mc20_13TeV.700309.Sh_2211_ttll.deriv.DAOD_PHYS.e8312_s3797_r13144_p6490',
    'mc20_13TeV:mc20_13TeV.700309.Sh_2211_ttll.deriv.DAOD_PHYS.e8312_s3797_r13145_p6490'
]

# Background Systematic (ttZ) Full Run 2
GridSubmission.grid.Add('PHYS_tWZ_MC_Background_Systematics_ttZ_Run2').datasets = [
    'mc20_13TeV:mc20_13TeV.504330.aMCPy8EG_NNPDF30NLO_A14N23LO_ttee.deriv.DAOD_PHYS.e8255_s3797_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.504330.aMCPy8EG_NNPDF30NLO_A14N23LO_ttee.deriv.DAOD_PHYS.e8255_s3797_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.504330.aMCPy8EG_NNPDF30NLO_A14N23LO_ttee.deriv.DAOD_PHYS.e8255_s3797_r13145_p6697',
    'mc20_13TeV:mc20_13TeV.504334.aMCPy8EG_NNPDF30NLO_A14N23LO_ttmumu.deriv.DAOD_PHYS.e8255_s3797_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.504334.aMCPy8EG_NNPDF30NLO_A14N23LO_ttmumu.deriv.DAOD_PHYS.e8255_s3797_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.504334.aMCPy8EG_NNPDF30NLO_A14N23LO_ttmumu.deriv.DAOD_PHYS.e8255_s3797_r13145_p6697',
    'mc20_13TeV:mc20_13TeV.504342.aMCPy8EG_NNPDF30NLO_A14N23LO_tttautau.deriv.DAOD_PHYS.e8255_s3797_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.504342.aMCPy8EG_NNPDF30NLO_A14N23LO_tttautau.deriv.DAOD_PHYS.e8255_s3797_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.504342.aMCPy8EG_NNPDF30NLO_A14N23LO_tttautau.deriv.DAOD_PHYS.e8255_s3797_r13145_p6697'
]

# Nominal Background (Main - VV) Full Run 2
GridSubmission.grid.Add('PHYS_tWZ_MC_Background_Main_VV_Run2').datasets = [
    # EWK
    'mc20_13TeV:mc20_13TeV.700587.Sh_2212_lllljj.deriv.DAOD_PHYS.e8433_s3681_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.700587.Sh_2212_lllljj.deriv.DAOD_PHYS.e8433_s3681_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.700587.Sh_2212_lllljj.deriv.DAOD_PHYS.e8433_s3681_r13145_p6697',
    'mc20_13TeV:mc20_13TeV.700588.Sh_2212_lllvjj.deriv.DAOD_PHYS.e8433_s3681_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.700588.Sh_2212_lllvjj.deriv.DAOD_PHYS.e8433_s3681_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.700588.Sh_2212_lllvjj.deriv.DAOD_PHYS.e8433_s3681_r13145_p6697',
    # EWK-QCD Interference
    'mc20_13TeV:mc20_13TeV.700591.Sh_2212_lllljj_Int.deriv.DAOD_PHYS.e8433_s3681_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.700591.Sh_2212_lllljj_Int.deriv.DAOD_PHYS.e8433_s3681_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.700591.Sh_2212_lllljj_Int.deriv.DAOD_PHYS.e8433_s3681_r13145_p6697',
    'mc20_13TeV:mc20_13TeV.700592.Sh_2212_lllvjj_Int.deriv.DAOD_PHYS.e8433_s3681_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.700592.Sh_2212_lllvjj_Int.deriv.DAOD_PHYS.e8433_s3681_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.700592.Sh_2212_lllvjj_Int.deriv.DAOD_PHYS.e8433_s3681_r13145_p6697',
    # QCD
    'mc20_13TeV:mc20_13TeV.700600.Sh_2212_llll.deriv.DAOD_PHYS.e8433_s3681_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.700600.Sh_2212_llll.deriv.DAOD_PHYS.e8433_s3681_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.700600.Sh_2212_llll.deriv.DAOD_PHYS.e8433_s3681_r13145_p6697',
    'mc20_13TeV:mc20_13TeV.700601.Sh_2212_lllv.deriv.DAOD_PHYS.e8433_s3681_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.700601.Sh_2212_lllv.deriv.DAOD_PHYS.e8433_s3681_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.700601.Sh_2212_lllv.deriv.DAOD_PHYS.e8433_s3681_r13145_p6697'
]

# Background Systematic (VV) Full Run 2
GridSubmission.grid.Add('PHYS_tWZ_MC_Background_Systemetic_VV_Run2').datasets = [
    'mc20_13TeV:mc20_13TeV.364250.Sherpa_222_NNPDF30NNLO_llll.deriv.DAOD_PHYS.e5894_s3681_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.364250.Sherpa_222_NNPDF30NNLO_llll.deriv.DAOD_PHYS.e5894_s3681_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.364250.Sherpa_222_NNPDF30NNLO_llll.deriv.DAOD_PHYS.e5894_s3681_r13145_p6697',
    'mc20_13TeV:mc20_13TeV.364283.Sherpa_222_NNPDF30NNLO_lllljj_EW6.deriv.DAOD_PHYS.e6055_s3681_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.364283.Sherpa_222_NNPDF30NNLO_lllljj_EW6.deriv.DAOD_PHYS.e6055_s3681_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.364283.Sherpa_222_NNPDF30NNLO_lllljj_EW6.deriv.DAOD_PHYS.e6055_s3681_r13145_p6697',
    'mc20_13TeV:mc20_13TeV.364288.Sherpa_222_NNPDF30NNLO_llll_lowMllPtComplement.deriv.DAOD_PHYS.e6096_s3681_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.364288.Sherpa_222_NNPDF30NNLO_llll_lowMllPtComplement.deriv.DAOD_PHYS.e6096_s3681_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.364288.Sherpa_222_NNPDF30NNLO_llll_lowMllPtComplement.deriv.DAOD_PHYS.e6096_s3681_r13145_p6697',
    'mc20_13TeV:mc20_13TeV.345706.Sherpa_222_NNPDF30NNLO_ggllll_130M4l.deriv.DAOD_PHYS.e6213_s3681_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.345706.Sherpa_222_NNPDF30NNLO_ggllll_130M4l.deriv.DAOD_PHYS.e6213_s3681_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.345706.Sherpa_222_NNPDF30NNLO_ggllll_130M4l.deriv.DAOD_PHYS.e6213_s3681_r13145_p6697',
    'mc20_13TeV:mc20_13TeV.364253.Sherpa_222_NNPDF30NNLO_lllv.deriv.DAOD_PHYS.e5916_s3681_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.364253.Sherpa_222_NNPDF30NNLO_lllv.deriv.DAOD_PHYS.e5916_s3681_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.364253.Sherpa_222_NNPDF30NNLO_lllv.deriv.DAOD_PHYS.e5916_s3681_r13145_p6697',
    'mc20_13TeV:mc20_13TeV.364284.Sherpa_222_NNPDF30NNLO_lllvjj_EW6.deriv.DAOD_PHYS.e6055_s3681_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.364284.Sherpa_222_NNPDF30NNLO_lllvjj_EW6.deriv.DAOD_PHYS.e6055_s3681_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.364284.Sherpa_222_NNPDF30NNLO_lllvjj_EW6.deriv.DAOD_PHYS.e6055_s3681_r13145_p6697',
    'mc20_13TeV:mc20_13TeV.364289.Sherpa_222_NNPDF30NNLO_lllv_lowMllPtComplement.deriv.DAOD_PHYS.e6133_s3681_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.364289.Sherpa_222_NNPDF30NNLO_lllv_lowMllPtComplement.deriv.DAOD_PHYS.e6133_s3681_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.364289.Sherpa_222_NNPDF30NNLO_lllv_lowMllPtComplement.deriv.DAOD_PHYS.e6133_s3681_r13145_p6697'
]

# Nominal Background (Main - VVV) Full Run 2
GridSubmission.grid.Add('PHYS_tWZ_MC_Background_Main_VVV_Run2').datasets = [
    'mc20_13TeV:mc20_13TeV.364242.Sherpa_222_NNPDF30NNLO_WWW_3l3v_EW6.deriv.DAOD_PHYS.e5887_s3681_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.364242.Sherpa_222_NNPDF30NNLO_WWW_3l3v_EW6.deriv.DAOD_PHYS.e5887_s3681_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.364242.Sherpa_222_NNPDF30NNLO_WWW_3l3v_EW6.deriv.DAOD_PHYS.e5887_s3681_r13145_p6697',
    'mc20_13TeV:mc20_13TeV.364243.Sherpa_222_NNPDF30NNLO_WWZ_4l2v_EW6.deriv.DAOD_PHYS.e5887_s3681_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.364243.Sherpa_222_NNPDF30NNLO_WWZ_4l2v_EW6.deriv.DAOD_PHYS.e5887_s3681_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.364243.Sherpa_222_NNPDF30NNLO_WWZ_4l2v_EW6.deriv.DAOD_PHYS.e5887_s3681_r13145_p6697',
    'mc20_13TeV:mc20_13TeV.364244.Sherpa_222_NNPDF30NNLO_WWZ_2l4v_EW6.deriv.DAOD_PHYS.e5887_s3681_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.364244.Sherpa_222_NNPDF30NNLO_WWZ_2l4v_EW6.deriv.DAOD_PHYS.e5887_s3681_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.364244.Sherpa_222_NNPDF30NNLO_WWZ_2l4v_EW6.deriv.DAOD_PHYS.e5887_s3681_r13145_p6697',
    'mc20_13TeV:mc20_13TeV.364245.Sherpa_222_NNPDF30NNLO_WZZ_5l1v_EW6.deriv.DAOD_PHYS.e5887_s3681_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.364245.Sherpa_222_NNPDF30NNLO_WZZ_5l1v_EW6.deriv.DAOD_PHYS.e5887_s3681_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.364245.Sherpa_222_NNPDF30NNLO_WZZ_5l1v_EW6.deriv.DAOD_PHYS.e5887_s3681_r13145_p6697',
    'mc20_13TeV:mc20_13TeV.364246.Sherpa_222_NNPDF30NNLO_WZZ_3l3v_EW6.deriv.DAOD_PHYS.e5887_s3681_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.364246.Sherpa_222_NNPDF30NNLO_WZZ_3l3v_EW6.deriv.DAOD_PHYS.e5887_s3681_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.364246.Sherpa_222_NNPDF30NNLO_WZZ_3l3v_EW6.deriv.DAOD_PHYS.e5887_s3681_r13145_p6697',
    'mc20_13TeV:mc20_13TeV.364247.Sherpa_222_NNPDF30NNLO_ZZZ_6l0v_EW6.deriv.DAOD_PHYS.e5887_s3681_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.364247.Sherpa_222_NNPDF30NNLO_ZZZ_6l0v_EW6.deriv.DAOD_PHYS.e5887_s3681_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.364247.Sherpa_222_NNPDF30NNLO_ZZZ_6l0v_EW6.deriv.DAOD_PHYS.e5887_s3681_r13145_p6697',
    'mc20_13TeV:mc20_13TeV.364248.Sherpa_222_NNPDF30NNLO_ZZZ_4l2v_EW6.deriv.DAOD_PHYS.e5887_s3681_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.364248.Sherpa_222_NNPDF30NNLO_ZZZ_4l2v_EW6.deriv.DAOD_PHYS.e5887_s3681_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.364248.Sherpa_222_NNPDF30NNLO_ZZZ_4l2v_EW6.deriv.DAOD_PHYS.e5887_s3681_r13145_p6697',
    'mc20_13TeV:mc20_13TeV.364249.Sherpa_222_NNPDF30NNLO_ZZZ_2l4v_EW6.deriv.DAOD_PHYS.e5887_s3681_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.364249.Sherpa_222_NNPDF30NNLO_ZZZ_2l4v_EW6.deriv.DAOD_PHYS.e5887_s3681_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.364249.Sherpa_222_NNPDF30NNLO_ZZZ_2l4v_EW6.deriv.DAOD_PHYS.e5887_s3681_r13145_p6697'
]

# Other Background (tZq) Full Run 2
GridSubmission.grid.Add('PHYS_tWZ_MC_Background_tZq_Run2').datasets = [
    'mc20_13TeV:mc20_13TeV.545027.aMCPy8EG_tllq_4FS_HT6.deriv.DAOD_PHYS.e8564_s3797_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.545027.aMCPy8EG_tllq_4FS_HT6.deriv.DAOD_PHYS.e8564_s3797_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.545027.aMCPy8EG_tllq_4FS_HT6.deriv.DAOD_PHYS.e8564_s3797_r13145_p6697',
    'mc20_13TeV:mc20_13TeV.545028.aMCPy8EG_tbarllq_4FS_HT6.deriv.DAOD_PHYS.e8564_s3797_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.545028.aMCPy8EG_tbarllq_4FS_HT6.deriv.DAOD_PHYS.e8564_s3797_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.545028.aMCPy8EG_tbarllq_4FS_HT6.deriv.DAOD_PHYS.e8564_s3797_r13145_p6697'
]

# Other Background (tt) Full Run 2
GridSubmission.grid.Add('PHYS_tWZ_MC_Background_tt_Run2').datasets = [
    'mc20_13TeV:mc20_13TeV.410472.PhPy8EG_A14_ttbar_hdamp258p75_dil.deriv.DAOD_PHYS.e6348_s3681_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.410472.PhPy8EG_A14_ttbar_hdamp258p75_dil.deriv.DAOD_PHYS.e6348_s3681_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.410472.PhPy8EG_A14_ttbar_hdamp258p75_dil.deriv.DAOD_PHYS.e6348_s3681_r13145_p6697'
]

# Other Background (ttW) Full Run 2
GridSubmission.grid.Add('PHYS_tWZ_MC_Background_ttW_Run2').datasets = [
    'mc20_13TeV:mc20_13TeV.700205.Sh_2210_ttW_EWK.deriv.DAOD_PHYS.e8307_s3797_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.700205.Sh_2210_ttW_EWK.deriv.DAOD_PHYS.e8307_s3797_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.700205.Sh_2210_ttW_EWK.deriv.DAOD_PHYS.e8307_s3797_r13145_p6697',
    'mc20_13TeV:mc20_13TeV.700168.Sh_2210_ttW.deriv.DAOD_PHYS.e8273_s3797_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.700168.Sh_2210_ttW.deriv.DAOD_PHYS.e8273_s3797_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.700168.Sh_2210_ttW.deriv.DAOD_PHYS.e8273_s3797_r13145_p6697'
]

# Other Background (ttGamma) Full Run 2
GridSubmission.grid.Add('PHYS_tWZ_MC_Background_ttGamma_Run2').datasets = [
    'mc20_13TeV:mc20_13TeV.504554.aMCPy8EG_tty_yprod.deriv.DAOD_PHYS.e8261_s3681_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.504554.aMCPy8EG_tty_yprod.deriv.DAOD_PHYS.e8261_s3681_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.504554.aMCPy8EG_tty_yprod.deriv.DAOD_PHYS.e8261_s3681_r13145_p6697'
]

# Other Background (ttH) Full Run 2
GridSubmission.grid.Add('PHYS_tWZ_MC_Background_ttH_Run2').datasets = [
    'mc20_13TeV:mc20_13TeV.346345.PhPy8EG_A14NNPDF23_NNPDF30ME_ttH125_dilep.deriv.DAOD_PHYS.e7148_s3681_r13167_p6490',
    'mc20_13TeV:mc20_13TeV.346345.PhPy8EG_A14NNPDF23_NNPDF30ME_ttH125_dilep.deriv.DAOD_PHYS.e7148_s3681_r13144_p6490',
    'mc20_13TeV:mc20_13TeV.346345.PhPy8EG_A14NNPDF23_NNPDF30ME_ttH125_dilep.deriv.DAOD_PHYS.e7148_s3681_r13145_p6490'
]

# Other Background (ttWW) Full Run 2
GridSubmission.grid.Add('PHYS_tWZ_MC_Background_ttWW_Run2').datasets = [
    'mc20_13TeV:mc20_13TeV.410081.MadGraphPythia8EvtGen_A14NNPDF23_ttbarWW.deriv.DAOD_PHYS.e4111_s3681_r13167_p6490',
    'mc20_13TeV:mc20_13TeV.410081.MadGraphPythia8EvtGen_A14NNPDF23_ttbarWW.deriv.DAOD_PHYS.e4111_s3681_r13144_p6490',
    'mc20_13TeV:mc20_13TeV.410081.MadGraphPythia8EvtGen_A14NNPDF23_ttbarWW.deriv.DAOD_PHYS.e4111_s3681_r13145_p6490'
]

# Other Background (ttt) Full Run 2
GridSubmission.grid.Add('PHYS_tWZ_MC_Background_ttt_Run2').datasets = [
    'mc20_13TeV:mc20_13TeV.304014.MadGraphPythia8EvtGen_A14NNPDF23_3top_SM.deriv.DAOD_PHYS.e4324_s3681_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.304014.MadGraphPythia8EvtGen_A14NNPDF23_3top_SM.deriv.DAOD_PHYS.e4324_s3681_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.304014.MadGraphPythia8EvtGen_A14NNPDF23_3top_SM.deriv.DAOD_PHYS.e4324_s3681_r13145_p6697'
]

# Other Background (tttt) Full Run 2
GridSubmission.grid.Add('PHYS_tWZ_MC_Background_tttt_Run2').datasets = [
    'mc20_13TeV:mc20_13TeV.412043.aMcAtNloPythia8EvtGen_A14NNPDF31_SM4topsNLO.deriv.DAOD_PHYS.e7101_a907_r14859_p6697',
    'mc20_13TeV:mc20_13TeV.412043.aMcAtNloPythia8EvtGen_A14NNPDF31_SM4topsNLO.deriv.DAOD_PHYS.e7101_a907_r14860_p6697',
    'mc20_13TeV:mc20_13TeV.412043.aMcAtNloPythia8EvtGen_A14NNPDF31_SM4topsNLO.deriv.DAOD_PHYS.e7101_a907_r14861_p6697'
]

# Other Background (VH) Full Run 2
GridSubmission.grid.Add('PHYS_tWZ_MC_Background_VH_Run2').datasets = [
    'mc20_13TeV:mc20_13TeV.346310.PowhegPythia8EvtGen_NNPDF30_AZNLO_ZH125J_Zincl_H_incl_MINLO.deriv.DAOD_PHYS.e7151_s3681_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.346310.PowhegPythia8EvtGen_NNPDF30_AZNLO_ZH125J_Zincl_H_incl_MINLO.deriv.DAOD_PHYS.e7151_s3681_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.346310.PowhegPythia8EvtGen_NNPDF30_AZNLO_ZH125J_Zincl_H_incl_MINLO.deriv.DAOD_PHYS.e7151_s3681_r13145_p6697',
    'mc20_13TeV:mc20_13TeV.346311.PowhegPythia8EvtGen_NNPDF30_AZNLO_WpH125J_Wincl_H_incl_MINLO.deriv.DAOD_PHYS.e7151_s3681_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.346311.PowhegPythia8EvtGen_NNPDF30_AZNLO_WpH125J_Wincl_H_incl_MINLO.deriv.DAOD_PHYS.e7151_s3681_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.346311.PowhegPythia8EvtGen_NNPDF30_AZNLO_WpH125J_Wincl_H_incl_MINLO.deriv.DAOD_PHYS.e7151_s3681_r13145_p6697',
    'mc20_13TeV:mc20_13TeV.346312.PowhegPythia8EvtGen_NNPDF30_AZNLO_WmH125J_Wincl_H_incl_MINLO.deriv.DAOD_PHYS.e7151_s3681_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.346312.PowhegPythia8EvtGen_NNPDF30_AZNLO_WmH125J_Wincl_H_incl_MINLO.deriv.DAOD_PHYS.e7151_s3681_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.346312.PowhegPythia8EvtGen_NNPDF30_AZNLO_WmH125J_Wincl_H_incl_MINLO.deriv.DAOD_PHYS.e7151_s3681_r13145_p6697'
]

# Other Background (tW) Full Run 2
GridSubmission.grid.Add('PHYS_tWZ_MC_Background_tW_Run2').datasets = [
    ' mc20_13TeV:mc20_13TeV.410648.PowhegPythia8EvtGen_A14_Wt_DR_dilepton_top.deriv.DAOD_PHYS.e6615_s3681_r13167_p6490',
    'mc20_13TeV:mc20_13TeV.410648.PowhegPythia8EvtGen_A14_Wt_DR_dilepton_top.deriv.DAOD_PHYS.e6615_s3681_r13144_p6490',
    'mc20_13TeV:mc20_13TeV.410648.PowhegPythia8EvtGen_A14_Wt_DR_dilepton_top.deriv.DAOD_PHYS.e6615_s3681_r13145_p6490',
    'mc20_13TeV:mc20_13TeV.410649.PowhegPythia8EvtGen_A14_Wt_DR_dilepton_antitop.deriv.DAOD_PHYS.e6615_s3681_r13167_p6490',
    'mc20_13TeV:mc20_13TeV.410649.PowhegPythia8EvtGen_A14_Wt_DR_dilepton_antitop.deriv.DAOD_PHYS.e6615_s3681_r13144_p6490',
    'mc20_13TeV:mc20_13TeV.410649.PowhegPythia8EvtGen_A14_Wt_DR_dilepton_antitop.deriv.DAOD_PHYS.e6615_s3681_r13145_p6490'
]

# Other Background (Zgamma) Full Run 2
GridSubmission.grid.Add('PHYS_tWZ_MC_Background_Zgamma_Run2').datasets = [
    'mc20_13TeV:mc20_13TeV.700398.Sh_2211_mumugamma.deriv.DAOD_PHYS.e8338_s3681_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.700398.Sh_2211_mumugamma.deriv.DAOD_PHYS.e8338_s3681_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.700398.Sh_2211_mumugamma.deriv.DAOD_PHYS.e8338_s3681_r13145_p6697',
    'mc20_13TeV:mc20_13TeV.700399.Sh_2211_eegamma.deriv.DAOD_PHYS.e8338_s3681_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.700399.Sh_2211_eegamma.deriv.DAOD_PHYS.e8338_s3681_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.700399.Sh_2211_eegamma.deriv.DAOD_PHYS.e8338_s3681_r13145_p6697',
    'mc20_13TeV:mc20_13TeV.700400.Sh_2211_tautaugamma.deriv.DAOD_PHYS.e8338_s3681_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.700400.Sh_2211_tautaugamma.deriv.DAOD_PHYS.e8338_s3681_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.700400.Sh_2211_tautaugamma.deriv.DAOD_PHYS.e8338_s3681_r13145_p6697'
]

# Nominal Background (Main - V) Full Run 2
GridSubmission.grid.Add('PHYS_tWZ_MC_Background_Main_V_Run2').datasets = [
    # QCD
    'mc20_13TeV:mc20_13TeV.700320.Sh_2211_Zee_maxHTpTV2_BFilter.deriv.DAOD_PHYS.e8351_s3681_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.700320.Sh_2211_Zee_maxHTpTV2_BFilter.deriv.DAOD_PHYS.e8351_s3681_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.700320.Sh_2211_Zee_maxHTpTV2_BFilter.deriv.DAOD_PHYS.e8351_s3681_r13145_p6697',
    'mc20_13TeV:mc20_13TeV.700321.Sh_2211_Zee_maxHTpTV2_CFilterBVeto.deriv.DAOD_PHYS.e8351_s3681_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.700321.Sh_2211_Zee_maxHTpTV2_CFilterBVeto.deriv.DAOD_PHYS.e8351_s3681_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.700321.Sh_2211_Zee_maxHTpTV2_CFilterBVeto.deriv.DAOD_PHYS.e8351_s3681_r13145_p6697',
    'mc20_13TeV:mc20_13TeV.700322.Sh_2211_Zee_maxHTpTV2_CVetoBVeto.deriv.DAOD_PHYS.e8351_s3681_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.700322.Sh_2211_Zee_maxHTpTV2_CVetoBVeto.deriv.DAOD_PHYS.e8351_s3681_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.700322.Sh_2211_Zee_maxHTpTV2_CVetoBVeto.deriv.DAOD_PHYS.e8351_s3681_r13145_p6697',
    'mc20_13TeV:mc20_13TeV.700323.Sh_2211_Zmumu_maxHTpTV2_BFilter.deriv.DAOD_PHYS.e8351_s3681_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.700323.Sh_2211_Zmumu_maxHTpTV2_BFilter.deriv.DAOD_PHYS.e8351_s3681_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.700323.Sh_2211_Zmumu_maxHTpTV2_BFilter.deriv.DAOD_PHYS.e8351_s3681_r13145_p6697',
    'mc20_13TeV:mc20_13TeV.700324.Sh_2211_Zmumu_maxHTpTV2_CFilterBVeto.deriv.DAOD_PHYS.e8351_s3681_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.700324.Sh_2211_Zmumu_maxHTpTV2_CFilterBVeto.deriv.DAOD_PHYS.e8351_s3681_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.700324.Sh_2211_Zmumu_maxHTpTV2_CFilterBVeto.deriv.DAOD_PHYS.e8351_s3681_r13145_p6697',
    'mc20_13TeV:mc20_13TeV.700325.Sh_2211_Zmumu_maxHTpTV2_CVetoBVeto.deriv.DAOD_PHYS.e8351_s3681_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.700325.Sh_2211_Zmumu_maxHTpTV2_CVetoBVeto.deriv.DAOD_PHYS.e8351_s3681_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.700325.Sh_2211_Zmumu_maxHTpTV2_CVetoBVeto.deriv.DAOD_PHYS.e8351_s3681_r13145_p6697',
    # EWK
    'mc20_13TeV:mc20_13TeV.700358.Sh_2211_Zee2jets_Min_N_TChannel.deriv.DAOD_PHYS.e8357_s3681_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.700358.Sh_2211_Zee2jets_Min_N_TChannel.deriv.DAOD_PHYS.e8357_s3681_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.700358.Sh_2211_Zee2jets_Min_N_TChannel.deriv.DAOD_PHYS.e8357_s3681_r13145_p6697',
    'mc20_13TeV:mc20_13TeV.700359.Sh_2211_Zmm2jets_Min_N_TChannel.deriv.DAOD_PHYS.e8357_s3681_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.700359.Sh_2211_Zmm2jets_Min_N_TChannel.deriv.DAOD_PHYS.e8357_s3681_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.700359.Sh_2211_Zmm2jets_Min_N_TChannel.deriv.DAOD_PHYS.e8357_s3681_r13145_p6697',
    # Low mass
    'mc20_13TeV:mc20_13TeV.700467.Sh_2211_Zee_maxHTpTV2_m10_40_pT5_BFilter.deriv.DAOD_PHYS.e8379_s3681_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.700467.Sh_2211_Zee_maxHTpTV2_m10_40_pT5_BFilter.deriv.DAOD_PHYS.e8379_s3681_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.700467.Sh_2211_Zee_maxHTpTV2_m10_40_pT5_BFilter.deriv.DAOD_PHYS.e8379_s3681_r13145_p6697',
    'mc20_13TeV:mc20_13TeV.700468.Sh_2211_Zee_maxHTpTV2_m10_40_pT5_CFilterBVeto.deriv.DAOD_PHYS.e8379_s3681_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.700468.Sh_2211_Zee_maxHTpTV2_m10_40_pT5_CFilterBVeto.deriv.DAOD_PHYS.e8379_s3681_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.700468.Sh_2211_Zee_maxHTpTV2_m10_40_pT5_CFilterBVeto.deriv.DAOD_PHYS.e8379_s3681_r13145_p6697',
    'mc20_13TeV:mc20_13TeV.700469.Sh_2211_Zee_maxHTpTV2_m10_40_pT5_CVetoBVeto.deriv.DAOD_PHYS.e8379_s3681_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.700469.Sh_2211_Zee_maxHTpTV2_m10_40_pT5_CVetoBVeto.deriv.DAOD_PHYS.e8379_s3681_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.700469.Sh_2211_Zee_maxHTpTV2_m10_40_pT5_CVetoBVeto.deriv.DAOD_PHYS.e8379_s3681_r13145_p6697',
    'mc20_13TeV:mc20_13TeV.700470.Sh_2211_Zmumu_maxHTpTV2_m10_40_pT5_BFilter.deriv.DAOD_PHYS.e8379_s3681_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.700470.Sh_2211_Zmumu_maxHTpTV2_m10_40_pT5_BFilter.deriv.DAOD_PHYS.e8379_s3681_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.700470.Sh_2211_Zmumu_maxHTpTV2_m10_40_pT5_BFilter.deriv.DAOD_PHYS.e8379_s3681_r13145_p6697',
    'mc20_13TeV:mc20_13TeV.700471.Sh_2211_Zmumu_maxHTpTV2_m10_40_pT5_CFilterBVeto.deriv.DAOD_PHYS.e8379_s3681_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.700471.Sh_2211_Zmumu_maxHTpTV2_m10_40_pT5_CFilterBVeto.deriv.DAOD_PHYS.e8379_s3681_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.700471.Sh_2211_Zmumu_maxHTpTV2_m10_40_pT5_CFilterBVeto.deriv.DAOD_PHYS.e8379_s3681_r13145_p6697',
    'mc20_13TeV:mc20_13TeV.700472.Sh_2211_Zmumu_maxHTpTV2_m10_40_pT5_CVetoBVeto.deriv.DAOD_PHYS.e8379_s3681_r13167_p6697',
    'mc20_13TeV:mc20_13TeV.700472.Sh_2211_Zmumu_maxHTpTV2_m10_40_pT5_CVetoBVeto.deriv.DAOD_PHYS.e8379_s3681_r13144_p6697',
    'mc20_13TeV:mc20_13TeV.700472.Sh_2211_Zmumu_maxHTpTV2_m10_40_pT5_CVetoBVeto.deriv.DAOD_PHYS.e8379_s3681_r13145_p6697',
]


