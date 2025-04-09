import GridSubmission.grid

# Full Run 2 Data
GridSubmission.grid.Add('PHYS_tWZ_Data_Run2').datasets = [
    'data15_13TeV:data15_13TeV.periodAllYear.physics_Main.PhysCont.DAOD_PHYS.grp15_v01_p6479',
    'data16_13TeV:data16_13TeV.periodAllYear.physics_Main.PhysCont.DAOD_PHYS.grp16_v01_p6479',
    'data17_13TeV:data17_13TeV.periodAllYear.physics_Main.PhysCont.DAOD_PHYS.grp17_v01_p6479',
    'data18_13TeV:data18_13TeV.periodAllYear.physics_Main.PhysCont.DAOD_PHYS.grp18_v01_p6479'
]

# Nominal Signal Full Run 2 
GridSubmission.grid.Add('PHYS_tWZ_MC_Signal_Run2').datasets = [
    'mc20_13TeV:mc20_13TeV.525955.aMCPy8EG_tWZ_Ztoll_DR1.deriv.DAOD_PHYS.e8553_a907_r14859_p6490',
    'mc20_13TeV:mc20_13TeV.525955.aMCPy8EG_tWZ_Ztoll_DR1.deriv.DAOD_PHYS.e8553_a907_r14860_p6490',
    'mc20_13TeV:mc20_13TeV.525955.aMCPy8EG_tWZ_Ztoll_DR1.deriv.DAOD_PHYS.e8553_a907_r14861_p6490'
]

# Nominal Background (Main - ttZ) Full Run 2
GridSubmission.grid.Add('PHYS_tWZ_MC_Background_Main_ttZ_Run2').datasets = [
    'mc20_13TeV:mc20_13TeV.504330.aMCPy8EG_NNPDF30NLO_A14N23LO_ttee.deriv.DAOD_PHYS.e8255_s3797_r13167_p6266',
    'mc20_13TeV:mc20_13TeV.504330.aMCPy8EG_NNPDF30NLO_A14N23LO_ttee.deriv.DAOD_PHYS.e8255_s3797_r13144_p6266',
    'mc20_13TeV:mc20_13TeV.504330.aMCPy8EG_NNPDF30NLO_A14N23LO_ttee.deriv.DAOD_PHYS.e8255_s3797_r13145_p6266',
    'mc20_13TeV:mc20_13TeV.504334.aMCPy8EG_NNPDF30NLO_A14N23LO_ttmumu.deriv.DAOD_PHYS.e8255_s3797_r13167_p6266',
    'mc20_13TeV:mc20_13TeV.504334.aMCPy8EG_NNPDF30NLO_A14N23LO_ttmumu.deriv.DAOD_PHYS.e8255_s3797_r13144_p6266',
    'mc20_13TeV:mc20_13TeV.504334.aMCPy8EG_NNPDF30NLO_A14N23LO_ttmumu.deriv.DAOD_PHYS.e8255_s3797_r13145_p6266',
    'mc20_13TeV:mc20_13TeV.504342.aMCPy8EG_NNPDF30NLO_A14N23LO_tttautau.deriv.DAOD_PHYS.e8255_s3797_r13167_p6266',
    'mc20_13TeV:mc20_13TeV.504342.aMCPy8EG_NNPDF30NLO_A14N23LO_tttautau.deriv.DAOD_PHYS.e8255_s3797_r13144_p6266',
    'mc20_13TeV:mc20_13TeV.504342.aMCPy8EG_NNPDF30NLO_A14N23LO_tttautau.deriv.DAOD_PHYS.e8255_s3797_r13145_p6266'
]

# Nominal Background (Main - ZZ + Jets) Full Run 2
GridSubmission.grid.Add('PHYS_tWZ_MC_Background_Main_ZZ_Run2').datasets = [
    'mc20_13TeV:mc20_13TeV.364250.Sherpa_222_NNPDF30NNLO_llll.deriv.DAOD_PHYS.e5894_s3681_r13167_p6026',
    'mc20_13TeV:mc20_13TeV.364250.Sherpa_222_NNPDF30NNLO_llll.deriv.DAOD_PHYS.e5894_s3681_r13144_p6026',
    'mc20_13TeV:mc20_13TeV.364250.Sherpa_222_NNPDF30NNLO_llll.deriv.DAOD_PHYS.e5894_s3681_r13145_p6026',
    'mc20_13TeV:mc20_13TeV.364283.Sherpa_222_NNPDF30NNLO_lllljj_EW6.deriv.DAOD_PHYS.e6055_s3681_r13167_p6026',
    'mc20_13TeV:mc20_13TeV.364283.Sherpa_222_NNPDF30NNLO_lllljj_EW6.deriv.DAOD_PHYS.e6055_s3681_r13144_p6026',
    'mc20_13TeV:mc20_13TeV.364283.Sherpa_222_NNPDF30NNLO_lllljj_EW6.deriv.DAOD_PHYS.e6055_s3681_r13145_p6026',
    'mc20_13TeV:mc20_13TeV.364288.Sherpa_222_NNPDF30NNLO_llll_lowMllPtComplement.deriv.DAOD_PHYS.e6096_s3681_r13167_p6490',
    'mc20_13TeV:mc20_13TeV.364288.Sherpa_222_NNPDF30NNLO_llll_lowMllPtComplement.deriv.DAOD_PHYS.e6096_s3681_r13144_p6490',
    'mc20_13TeV:mc20_13TeV.364288.Sherpa_222_NNPDF30NNLO_llll_lowMllPtComplement.deriv.DAOD_PHYS.e6096_s3681_r13145_p6490',
    'mc20_13TeV:mc20_13TeV.345706.Sherpa_222_NNPDF30NNLO_ggllll_130M4l.deriv.DAOD_PHYS.e6213_s3681_r13167_p6490',
    'mc20_13TeV:mc20_13TeV.345706.Sherpa_222_NNPDF30NNLO_ggllll_130M4l.deriv.DAOD_PHYS.e6213_s3681_r13144_p6490',
    'mc20_13TeV:mc20_13TeV.345706.Sherpa_222_NNPDF30NNLO_ggllll_130M4l.deriv.DAOD_PHYS.e6213_s3681_r13145_p6490'
]

# Other Background (WZ + jets) Full Run 2
GridSubmission.grid.Add('PHYS_tWZ_MC_Background_WZ_Run2').datasets = [
    'mc20_13TeV:mc20_13TeV.364253.Sherpa_222_NNPDF30NNLO_lllv.deriv.DAOD_PHYS.e5916_s3681_r13167_p6026',
    'mc20_13TeV:mc20_13TeV.364253.Sherpa_222_NNPDF30NNLO_lllv.deriv.DAOD_PHYS.e5916_s3681_r13144_p6026',
    'mc20_13TeV:mc20_13TeV.364253.Sherpa_222_NNPDF30NNLO_lllv.deriv.DAOD_PHYS.e5916_s3681_r13145_p6026',
    'mc20_13TeV:mc20_13TeV.364284.Sherpa_222_NNPDF30NNLO_lllvjj_EW6.deriv.DAOD_PHYS.e6055_s3681_r13167_p6026',
    'mc20_13TeV:mc20_13TeV.364284.Sherpa_222_NNPDF30NNLO_lllvjj_EW6.deriv.DAOD_PHYS.e6055_s3681_r13144_p6026',
    'mc20_13TeV:mc20_13TeV.364284.Sherpa_222_NNPDF30NNLO_lllvjj_EW6.deriv.DAOD_PHYS.e6055_s3681_r13145_p6026',
    'mc20_13TeV:mc20_13TeV.364289.Sherpa_222_NNPDF30NNLO_lllv_lowMllPtComplement.deriv.DAOD_PHYS.e6133_s3681_r13167_p6490',
    'mc20_13TeV:mc20_13TeV.364289.Sherpa_222_NNPDF30NNLO_lllv_lowMllPtComplement.deriv.DAOD_PHYS.e6133_s3681_r13144_p6490',
    'mc20_13TeV:mc20_13TeV.364289.Sherpa_222_NNPDF30NNLO_lllv_lowMllPtComplement.deriv.DAOD_PHYS.e6133_s3681_r13145_p6490'
]

# Other Background (tZq) Full Run 2
GridSubmission.grid.Add('PHYS_tWZ_MC_Background_tZq_Run2').datasets = [
    'mc20_13TeV:mc20_13TeV.512059.aMCPy8EG_NNPDF30NLO_A14N23LO_tllq_4FS.deriv.DAOD_PHYS.e8400_a907_r14859_p6490',
    'mc20_13TeV:mc20_13TeV.512059.aMCPy8EG_NNPDF30NLO_A14N23LO_tllq_4FS.deriv.DAOD_PHYS.e8400_a907_r14860_p6490',
    'mc20_13TeV:mc20_13TeV.512059.aMCPy8EG_NNPDF30NLO_A14N23LO_tllq_4FS.deriv.DAOD_PHYS.e8400_a907_r14861_p6490'
]

# Other Background (tt) Full Run 2
GridSubmission.grid.Add('PHYS_tWZ_MC_Background_tt_Run2').datasets = [
    'mc20_13TeV:mc20_13TeV.410472.PhPy8EG_A14_ttbar_hdamp258p75_dil.deriv.DAOD_PHYS.e6348_s3681_r13167_p6490',
    'mc20_13TeV:mc20_13TeV.410472.PhPy8EG_A14_ttbar_hdamp258p75_dil.deriv.DAOD_PHYS.e6348_s3681_r13144_p6490',
    'mc20_13TeV:mc20_13TeV.410472.PhPy8EG_A14_ttbar_hdamp258p75_dil.deriv.DAOD_PHYS.e6348_s3681_r13145_p6490'
]

# Other Background (Z + jets) Full Run 2
# GridSubmission.grid.Add('PHYS_tWZ_MC_Background_Zjets_Run2').datasets = [
#     'mc20_13TeV:mc20_13TeV.700320.Sh_2211_Zee_maxHTpTV2_BFilter.deriv.DAOD_PHYS.e8351_s3681_r13167_p6490',
#     'mc20_13TeV:mc20_13TeV.700320.Sh_2211_Zee_maxHTpTV2_BFilter.deriv.DAOD_PHYS.e8351_s3681_r13144_p6490',
#     'mc20_13TeV:mc20_13TeV.700320.Sh_2211_Zee_maxHTpTV2_BFilter.deriv.DAOD_PHYS.e8351_s3681_r13145_p6490',
#     'mc20_13TeV:mc20_13TeV.700321.Sh_2211_Zee_maxHTpTV2_CFilterBVeto.deriv.DAOD_PHYS.e8351_s3681_r13167_p6490',
#     'mc20_13TeV:mc20_13TeV.700321.Sh_2211_Zee_maxHTpTV2_CFilterBVeto.deriv.DAOD_PHYS.e8351_s3681_r13144_p6490',
#     'mc20_13TeV:mc20_13TeV.700321.Sh_2211_Zee_maxHTpTV2_CFilterBVeto.deriv.DAOD_PHYS.e8351_s3681_r13145_p6490',
#     'mc20_13TeV:mc20_13TeV.700322.Sh_2211_Zee_maxHTpTV2_CVetoBVeto.deriv.DAOD_PHYS.e8351_s3681_r13167_p6490',
#     'mc20_13TeV:mc20_13TeV.700322.Sh_2211_Zee_maxHTpTV2_CVetoBVeto.deriv.DAOD_PHYS.e8351_s3681_r13144_p6490',
#     'mc20_13TeV:mc20_13TeV.700322.Sh_2211_Zee_maxHTpTV2_CVetoBVeto.deriv.DAOD_PHYS.e8351_s3681_r13145_p6490',
#     'mc20_13TeV:mc20_13TeV.700323.Sh_2211_Zmumu_maxHTpTV2_BFilter.deriv.DAOD_PHYS.e8351_s3681_r13167_p6490',
#     'mc20_13TeV:mc20_13TeV.700323.Sh_2211_Zmumu_maxHTpTV2_BFilter.deriv.DAOD_PHYS.e8351_s3681_r13144_p6490',
#     'mc20_13TeV:mc20_13TeV.700323.Sh_2211_Zmumu_maxHTpTV2_BFilter.deriv.DAOD_PHYS.e8351_s3681_r13145_p6490',
#     'mc20_13TeV:mc20_13TeV.700324.Sh_2211_Zmumu_maxHTpTV2_CFilterBVeto.deriv.DAOD_PHYS.e8351_s3681_r13167_p6490',
#     'mc20_13TeV:mc20_13TeV.700324.Sh_2211_Zmumu_maxHTpTV2_CFilterBVeto.deriv.DAOD_PHYS.e8351_s3681_r13144_p6490',
#     'mc20_13TeV:mc20_13TeV.700324.Sh_2211_Zmumu_maxHTpTV2_CFilterBVeto.deriv.DAOD_PHYS.e8351_s3681_r13145_p6490',
#     'mc20_13TeV:mc20_13TeV.700325.Sh_2211_Zmumu_maxHTpTV2_CVetoBVeto.deriv.DAOD_PHYS.e8351_s3681_r13167_p6490',
#     'mc20_13TeV:mc20_13TeV.700325.Sh_2211_Zmumu_maxHTpTV2_CVetoBVeto.deriv.DAOD_PHYS.e8351_s3681_r13144_p6490',
#     'mc20_13TeV:mc20_13TeV.700325.Sh_2211_Zmumu_maxHTpTV2_CVetoBVeto.deriv.DAOD_PHYS.e8351_s3681_r13145_p6490',
#     'mc20_13TeV:mc20_13TeV.700326.Sh_2211_Ztautau_LL_maxHTpTV2_BFilter.deriv.DAOD_PHYS.e8351_s3681_r13167_p6266',
#     'mc20_13TeV:mc20_13TeV.700326.Sh_2211_Ztautau_LL_maxHTpTV2_BFilter.deriv.DAOD_PHYS.e8351_s3681_r13144_p6266',
#     'mc20_13TeV:mc20_13TeV.700326.Sh_2211_Ztautau_LL_maxHTpTV2_BFilter.deriv.DAOD_PHYS.e8351_s3681_r13145_p6266',
#     'mc20_13TeV:mc20_13TeV.700327.Sh_2211_Ztautau_LL_maxHTpTV2_CFilterBVeto.deriv.DAOD_PHYS.e8351_s3681_r13167_p6266',
#     'mc20_13TeV:mc20_13TeV.700327.Sh_2211_Ztautau_LL_maxHTpTV2_CFilterBVeto.deriv.DAOD_PHYS.e8351_s3681_r13144_p6266',
#     'mc20_13TeV:mc20_13TeV.700327.Sh_2211_Ztautau_LL_maxHTpTV2_CFilterBVeto.deriv.DAOD_PHYS.e8351_s3681_r13145_p6266',
#     'mc20_13TeV:mc20_13TeV.700328.Sh_2211_Ztautau_LL_maxHTpTV2_CVetoBVeto.deriv.DAOD_PHYS.e8351_s3681_r13167_p6266',
#     'mc20_13TeV:mc20_13TeV.700328.Sh_2211_Ztautau_LL_maxHTpTV2_CVetoBVeto.deriv.DAOD_PHYS.e8351_s3681_r13144_p6266',
#     'mc20_13TeV:mc20_13TeV.700328.Sh_2211_Ztautau_LL_maxHTpTV2_CVetoBVeto.deriv.DAOD_PHYS.e8351_s3681_r13145_p6266',
#     'mc20_13TeV:mc20_13TeV.700329.Sh_2211_Ztautau_LH_maxHTpTV2_BFilter.deriv.DAOD_PHYS.e8351_s3681_r13167_p6266',
#     'mc20_13TeV:mc20_13TeV.700329.Sh_2211_Ztautau_LH_maxHTpTV2_BFilter.deriv.DAOD_PHYS.e8351_s3681_r13144_p6266',
#     'mc20_13TeV:mc20_13TeV.700329.Sh_2211_Ztautau_LH_maxHTpTV2_BFilter.deriv.DAOD_PHYS.e8351_s3681_r13145_p6266',
#     'mc20_13TeV:mc20_13TeV.700330.Sh_2211_Ztautau_LH_maxHTpTV2_CFilterBVeto.deriv.DAOD_PHYS.e8351_s3681_r13167_p6266',
#     'mc20_13TeV:mc20_13TeV.700330.Sh_2211_Ztautau_LH_maxHTpTV2_CFilterBVeto.deriv.DAOD_PHYS.e8351_s3681_r13144_p6266',
#     'mc20_13TeV:mc20_13TeV.700330.Sh_2211_Ztautau_LH_maxHTpTV2_CFilterBVeto.deriv.DAOD_PHYS.e8351_s3681_r13145_p6266',
#     'mc20_13TeV:mc20_13TeV.700331.Sh_2211_Ztautau_LH_maxHTpTV2_CVetoBVeto.deriv.DAOD_PHYS.e8351_s3681_r13167_p6266',
#     'mc20_13TeV:mc20_13TeV.700331.Sh_2211_Ztautau_LH_maxHTpTV2_CVetoBVeto.deriv.DAOD_PHYS.e8351_s3681_r13144_p6266',
#     'mc20_13TeV:mc20_13TeV.700331.Sh_2211_Ztautau_LH_maxHTpTV2_CVetoBVeto.deriv.DAOD_PHYS.e8351_s3681_r13145_p6266',
#     'mc20_13TeV:mc20_13TeV.700332.Sh_2211_Ztautau_HH_maxHTpTV2_BFilter.deriv.DAOD_PHYS.e8351_s3681_r13167_p6266',
#     'mc20_13TeV:mc20_13TeV.700332.Sh_2211_Ztautau_HH_maxHTpTV2_BFilter.deriv.DAOD_PHYS.e8351_s3681_r13144_p6266',
#     'mc20_13TeV:mc20_13TeV.700332.Sh_2211_Ztautau_HH_maxHTpTV2_BFilter.deriv.DAOD_PHYS.e8351_s3681_r13145_p6266',
#     'mc20_13TeV:mc20_13TeV.700333.Sh_2211_Ztautau_HH_maxHTpTV2_CFilterBVeto.deriv.DAOD_PHYS.e8351_s3681_r13167_p6266',
#     'mc20_13TeV:mc20_13TeV.700333.Sh_2211_Ztautau_HH_maxHTpTV2_CFilterBVeto.deriv.DAOD_PHYS.e8351_s3681_r13144_p6266',
#     'mc20_13TeV:mc20_13TeV.700333.Sh_2211_Ztautau_HH_maxHTpTV2_CFilterBVeto.deriv.DAOD_PHYS.e8351_s3681_r13145_p6266',
#     'mc20_13TeV:mc20_13TeV.700334.Sh_2211_Ztautau_HH_maxHTpTV2_CVetoBVeto.deriv.DAOD_PHYS.e8351_s3681_r13167_p6266',
#     'mc20_13TeV:mc20_13TeV.700334.Sh_2211_Ztautau_HH_maxHTpTV2_CVetoBVeto.deriv.DAOD_PHYS.e8351_s3681_r13144_p6266',
#     'mc20_13TeV:mc20_13TeV.700334.Sh_2211_Ztautau_HH_maxHTpTV2_CVetoBVeto.deriv.DAOD_PHYS.e8351_s3681_r13145_p6266'
# ]

# Other Background (Z + jets Low Mass) Full Run 2
# GridSubmission.grid.Add('PHYS_tWZ_MC_Background_Zjets_LM_Run2').datasets = [
#     'mc20_13TeV:mc20_13TeV.364198.Sherpa_221_NN30NNLO_Zmm_Mll10_40_MAXHTPTV0_70_BVeto.deriv.DAOD_PHYS.e5421_s3681_r13167_p6026',
#     'mc20_13TeV:mc20_13TeV.364198.Sherpa_221_NN30NNLO_Zmm_Mll10_40_MAXHTPTV0_70_BVeto.deriv.DAOD_PHYS.e5421_s3681_r13144_p6026',
#     'mc20_13TeV:mc20_13TeV.364198.Sherpa_221_NN30NNLO_Zmm_Mll10_40_MAXHTPTV0_70_BVeto.deriv.DAOD_PHYS.e5421_s3681_r13145_p6026',
#     'mc20_13TeV:mc20_13TeV.364199.Sherpa_221_NN30NNLO_Zmm_Mll10_40_MAXHTPTV0_70_BFilter.deriv.DAOD_PHYS.e5421_s3681_r13167_p6026',
#     'mc20_13TeV:mc20_13TeV.364199.Sherpa_221_NN30NNLO_Zmm_Mll10_40_MAXHTPTV0_70_BFilter.deriv.DAOD_PHYS.e5421_s3681_r13144_p6026',
#     'mc20_13TeV:mc20_13TeV.364199.Sherpa_221_NN30NNLO_Zmm_Mll10_40_MAXHTPTV0_70_BFilter.deriv.DAOD_PHYS.e5421_s3681_r13145_p6026',
#     'mc20_13TeV:mc20_13TeV.364120.Sherpa_221_NNPDF30NNLO_Zee_MAXHTPTV140_280_CVetoBVeto.deriv.DAOD_PHYS.e5299_s3681_r13167_p6026',
#     'mc20_13TeV:mc20_13TeV.364120.Sherpa_221_NNPDF30NNLO_Zee_MAXHTPTV140_280_CVetoBVeto.deriv.DAOD_PHYS.e5299_s3681_r13144_p6026',
#     'mc20_13TeV:mc20_13TeV.364120.Sherpa_221_NNPDF30NNLO_Zee_MAXHTPTV140_280_CVetoBVeto.deriv.DAOD_PHYS.e5299_s3681_r13145_p6026',
#     'mc20_13TeV:mc20_13TeV.364121.Sherpa_221_NNPDF30NNLO_Zee_MAXHTPTV140_280_CFilterBVeto.deriv.DAOD_PHYS.e5299_s3681_r13167_p6026',
#     'mc20_13TeV:mc20_13TeV.364121.Sherpa_221_NNPDF30NNLO_Zee_MAXHTPTV140_280_CFilterBVeto.deriv.DAOD_PHYS.e5299_s3681_r13144_p6026',
#     'mc20_13TeV:mc20_13TeV.364121.Sherpa_221_NNPDF30NNLO_Zee_MAXHTPTV140_280_CFilterBVeto.deriv.DAOD_PHYS.e5299_s3681_r13145_p6026',
#     'mc20_13TeV:mc20_13TeV.364122.Sherpa_221_NNPDF30NNLO_Zee_MAXHTPTV140_280_BFilter.deriv.DAOD_PHYS.e5299_s3681_r13167_p6026',
#     'mc20_13TeV:mc20_13TeV.364122.Sherpa_221_NNPDF30NNLO_Zee_MAXHTPTV140_280_BFilter.deriv.DAOD_PHYS.e5299_s3681_r13144_p6026',
#     'mc20_13TeV:mc20_13TeV.364122.Sherpa_221_NNPDF30NNLO_Zee_MAXHTPTV140_280_BFilter.deriv.DAOD_PHYS.e5299_s3681_r13145_p6026',
#     'mc20_13TeV:mc20_13TeV.364123.Sherpa_221_NNPDF30NNLO_Zee_MAXHTPTV280_500_CVetoBVeto.deriv.DAOD_PHYS.e5299_s3681_r13167_p6026',
#     'mc20_13TeV:mc20_13TeV.364123.Sherpa_221_NNPDF30NNLO_Zee_MAXHTPTV280_500_CVetoBVeto.deriv.DAOD_PHYS.e5299_s3681_r13144_p6026',
#     'mc20_13TeV:mc20_13TeV.364123.Sherpa_221_NNPDF30NNLO_Zee_MAXHTPTV280_500_CVetoBVeto.deriv.DAOD_PHYS.e5299_s3681_r13145_p6026',
#     'mc20_13TeV:mc20_13TeV.364124.Sherpa_221_NNPDF30NNLO_Zee_MAXHTPTV280_500_CFilterBVeto.deriv.DAOD_PHYS.e5299_s3681_r13167_p6026',
#     'mc20_13TeV:mc20_13TeV.364124.Sherpa_221_NNPDF30NNLO_Zee_MAXHTPTV280_500_CFilterBVeto.deriv.DAOD_PHYS.e5299_s3681_r13144_p6026',
#     'mc20_13TeV:mc20_13TeV.364124.Sherpa_221_NNPDF30NNLO_Zee_MAXHTPTV280_500_CFilterBVeto.deriv.DAOD_PHYS.e5299_s3681_r13145_p6026',
#     'mc20_13TeV:mc20_13TeV.364125.Sherpa_221_NNPDF30NNLO_Zee_MAXHTPTV280_500_BFilter.deriv.DAOD_PHYS.e5299_s3681_r13167_p6026',
#     'mc20_13TeV:mc20_13TeV.364125.Sherpa_221_NNPDF30NNLO_Zee_MAXHTPTV280_500_BFilter.deriv.DAOD_PHYS.e5299_s3681_r13144_p6026',
#     'mc20_13TeV:mc20_13TeV.364125.Sherpa_221_NNPDF30NNLO_Zee_MAXHTPTV280_500_BFilter.deriv.DAOD_PHYS.e5299_s3681_r13145_p6026',
#     'mc20_13TeV:mc20_13TeV.364126.Sherpa_221_NNPDF30NNLO_Zee_MAXHTPTV500_1000.deriv.DAOD_PHYS.e5299_s3681_r13167_p6026',
#     'mc20_13TeV:mc20_13TeV.364126.Sherpa_221_NNPDF30NNLO_Zee_MAXHTPTV500_1000.deriv.DAOD_PHYS.e5299_s3681_r13144_p6026',
#     'mc20_13TeV:mc20_13TeV.364126.Sherpa_221_NNPDF30NNLO_Zee_MAXHTPTV500_1000.deriv.DAOD_PHYS.e5299_s3681_r13145_p6026',
#     'mc20_13TeV:mc20_13TeV.364127.Sherpa_221_NNPDF30NNLO_Zee_MAXHTPTV1000_E_CMS.deriv.DAOD_PHYS.e5299_s3681_r13167_p6026',
#     'mc20_13TeV:mc20_13TeV.364127.Sherpa_221_NNPDF30NNLO_Zee_MAXHTPTV1000_E_CMS.deriv.DAOD_PHYS.e5299_s3681_r13144_p6026',
#     'mc20_13TeV:mc20_13TeV.364127.Sherpa_221_NNPDF30NNLO_Zee_MAXHTPTV1000_E_CMS.deriv.DAOD_PHYS.e5299_s3681_r13145_p6026',
#     'mc20_13TeV:mc20_13TeV.364128.Sherpa_221_NNPDF30NNLO_Ztautau_MAXHTPTV0_70_CVetoBVeto.deriv.DAOD_PHYS.e5307_s3681_r13167_p6026',
#     'mc20_13TeV:mc20_13TeV.364128.Sherpa_221_NNPDF30NNLO_Ztautau_MAXHTPTV0_70_CVetoBVeto.deriv.DAOD_PHYS.e5307_s3681_r13144_p6026',
#     'mc20_13TeV:mc20_13TeV.364128.Sherpa_221_NNPDF30NNLO_Ztautau_MAXHTPTV0_70_CVetoBVeto.deriv.DAOD_PHYS.e5307_s3681_r13145_p6026',
#     'mc20_13TeV:mc20_13TeV.364129.Sherpa_221_NNPDF30NNLO_Ztautau_MAXHTPTV0_70_CFilterBVeto.deriv.DAOD_PHYS.e5307_s3681_r13167_p6026',
#     'mc20_13TeV:mc20_13TeV.364129.Sherpa_221_NNPDF30NNLO_Ztautau_MAXHTPTV0_70_CFilterBVeto.deriv.DAOD_PHYS.e5307_s3681_r13144_p6026',
#     'mc20_13TeV:mc20_13TeV.364129.Sherpa_221_NNPDF30NNLO_Ztautau_MAXHTPTV0_70_CFilterBVeto.deriv.DAOD_PHYS.e5307_s3681_r13145_p6026',
#     'mc20_13TeV:mc20_13TeV.364130.Sherpa_221_NNPDF30NNLO_Ztautau_MAXHTPTV0_70_BFilter.deriv.DAOD_PHYS.e5307_s3681_r13167_p6026',
#     'mc20_13TeV:mc20_13TeV.364130.Sherpa_221_NNPDF30NNLO_Ztautau_MAXHTPTV0_70_BFilter.deriv.DAOD_PHYS.e5307_s3681_r13144_p6026',
#     'mc20_13TeV:mc20_13TeV.364130.Sherpa_221_NNPDF30NNLO_Ztautau_MAXHTPTV0_70_BFilter.deriv.DAOD_PHYS.e5307_s3681_r13145_p6026',
#     'mc20_13TeV:mc20_13TeV.364131.Sherpa_221_NNPDF30NNLO_Ztautau_MAXHTPTV70_140_CVetoBVeto.deriv.DAOD_PHYS.e5307_s3681_r13167_p6026',
#     'mc20_13TeV:mc20_13TeV.364131.Sherpa_221_NNPDF30NNLO_Ztautau_MAXHTPTV70_140_CVetoBVeto.deriv.DAOD_PHYS.e5307_s3681_r13144_p6026',
#     'mc20_13TeV:mc20_13TeV.364131.Sherpa_221_NNPDF30NNLO_Ztautau_MAXHTPTV70_140_CVetoBVeto.deriv.DAOD_PHYS.e5307_s3681_r13145_p6026'
# ]

# Other Background (ttW) Full Run 2
GridSubmission.grid.Add('PHYS_tWZ_MC_Background_ttW_Run2').datasets = [
    'mc20_13TeV:mc20_13TeV.700205.Sh_2210_ttW_EWK.deriv.DAOD_PHYS.e8307_s3681_r13167_p6490',
    'mc20_13TeV:mc20_13TeV.700205.Sh_2210_ttW_EWK.deriv.DAOD_PHYS.e8307_s3681_r13144_p6490',
    'mc20_13TeV:mc20_13TeV.700205.Sh_2210_ttW_EWK.deriv.DAOD_PHYS.e8307_s3681_r13145_p6490'
]

# # Other Background (ttGamma) Full Run 2
# GridSubmission.grid.Add('PHYS_tWZ_MC_Background_ttGamma_Run2').datasets = [
#     'mc20_13TeV:mc20_13TeV.410389.MadGraphPythia8EvtGen_A14NNPDF23_ttgamma_nonallhadronic.deriv.DAOD_PHYS.e6155_s3681_r13167_p6490',
#     'mc20_13TeV:mc20_13TeV.410389.MadGraphPythia8EvtGen_A14NNPDF23_ttgamma_nonallhadronic.deriv.DAOD_PHYS.e6155_s3681_r13144_p6490',
#     'mc20_13TeV:mc20_13TeV.410389.MadGraphPythia8EvtGen_A14NNPDF23_ttgamma_nonallhadronic.deriv.DAOD_PHYS.e6155_s3681_r13145_p6490'
# ]

# Other Background (ttH) Full Run 2
GridSubmission.grid.Add('PHYS_tWZ_MC_Background_ttH_Run2').datasets = [
    'mc20_13TeV:mc20_13TeV.346345.PhPy8EG_A14NNPDF23_NNPDF30ME_ttH125_dilep.deriv.DAOD_PHYS.e7148_s3681_r13167_p6490',
    'mc20_13TeV:mc20_13TeV.346345.PhPy8EG_A14NNPDF23_NNPDF30ME_ttH125_dilep.deriv.DAOD_PHYS.e7148_s3681_r13144_p6490',
    'mc20_13TeV:mc20_13TeV.346345.PhPy8EG_A14NNPDF23_NNPDF30ME_ttH125_dilep.deriv.DAOD_PHYS.e7148_s3681_r13145_p6490'
]

# Other Background (VVV) Full Run 2
# GridSubmission.grid.Add('PHYS_tWZ_MC_Background_VVV_Run2').datasets = [

# ]

# Other Background (ttWW) Full Run 2
GridSubmission.grid.Add('PHYS_tWZ_MC_Background_ttWW_Run2').datasets = [
    'mc20_13TeV:mc20_13TeV.410081.MadGraphPythia8EvtGen_A14NNPDF23_ttbarWW.deriv.DAOD_PHYS.e4111_s3681_r13167_p6490',
    'mc20_13TeV:mc20_13TeV.410081.MadGraphPythia8EvtGen_A14NNPDF23_ttbarWW.deriv.DAOD_PHYS.e4111_s3681_r13144_p6490',
    'mc20_13TeV:mc20_13TeV.410081.MadGraphPythia8EvtGen_A14NNPDF23_ttbarWW.deriv.DAOD_PHYS.e4111_s3681_r13145_p6490'
]

# Other Background (ZH) Full Run 2
GridSubmission.grid.Add('PHYS_tWZ_MC_Background_ZH_Run2').datasets = [
    'mc20_13TeV:mc20_13TeV.346310.PowhegPythia8EvtGen_NNPDF30_AZNLO_ZH125J_Zincl_H_incl_MINLO.deriv.DAOD_PHYS.e7151_s3681_r13167_p6490',
    'mc20_13TeV:mc20_13TeV.346310.PowhegPythia8EvtGen_NNPDF30_AZNLO_ZH125J_Zincl_H_incl_MINLO.deriv.DAOD_PHYS.e7151_s3681_r13144_p6490',
    'mc20_13TeV:mc20_13TeV.346310.PowhegPythia8EvtGen_NNPDF30_AZNLO_ZH125J_Zincl_H_incl_MINLO.deriv.DAOD_PHYS.e7151_s3681_r13145_p6490'
]

# Other Background (WH) Full Run 2
# GridSubmission.grid.Add('PHYS_tWZ_MC_Background_WH_Run2').datasets = [

# ]

# Other Background (ttt) Full Run 2
GridSubmission.grid.Add('PHYS_tWZ_MC_Background_ttt_Run2').datasets = [
    'mc20_13TeV:mc20_13TeV.700355.Sh_2211_tttt_muQHT2.deriv.DAOD_PHYS.e8351_s3797_r13167_p6490',
    'mc20_13TeV:mc20_13TeV.700355.Sh_2211_tttt_muQHT2.deriv.DAOD_PHYS.e8351_s3797_r13144_p6490',
    'mc20_13TeV:mc20_13TeV.700355.Sh_2211_tttt_muQHT2.deriv.DAOD_PHYS.e8351_s3797_r13145_p6490'
]

# Other Background (tttt) Full Run 2
GridSubmission.grid.Add('PHYS_tWZ_MC_Background_tttt_Run2').datasets = [

]

# Other Background (tW) Full Run 2
# GridSubmission.grid.Add('PHYS_tWZ_MC_Background_tW_Run2').datasets = [

# ]

# Other Background (ZGamma) Full Run 2
# GridSubmission.grid.Add('PHYS_tWZ_MC_Background_ZGamma_Run2').datasets = [

# ]

# Signal Systematics Full Run 2
GridSubmission.grid.Add('PHYS_tWZ_MC_Signal_Systematics_Run2').datasets = [
    'mc20_13TeV:mc20_13TeV.525956.aMCPy8EG_tWZ_Ztoll_DR2.deriv.DAOD_PHYS.e8553_a907_r14859_p6490',
    'mc20_13TeV:mc20_13TeV.525956.aMCPy8EG_tWZ_Ztoll_DR2.deriv.DAOD_PHYS.e8553_a907_r14860_p6490',
    'mc20_13TeV:mc20_13TeV.525956.aMCPy8EG_tWZ_Ztoll_DR2.deriv.DAOD_PHYS.e8553_a907_r14861_p6490'
]

# Background Systematics Full Run 2
GridSubmission.grid.Add('PHYS_tWZ_MC_Background_Systematics_Run2').datasets = [
    'mc20_13TeV:mc20_13TeV.700309.Sh_2211_ttll.deriv.DAOD_PHYS.e8312_s3797_r13167_p6266',
    'mc20_13TeV:mc20_13TeV.700309.Sh_2211_ttll.deriv.DAOD_PHYS.e8312_s3797_r13144_p6266',
    'mc20_13TeV:mc20_13TeV.700309.Sh_2211_ttll.deriv.DAOD_PHYS.e8312_s3797_r13145_p6266'
]
