# The name of the package:
atlas_subdir(TopCPToolkit)

# Find KLFitter package in atlasexternals,
# source from https://www.github.com/KLFitter/KLFitter
find_package(KLFitter)
find_package(onnxruntime)

# FastJet - needed for jet reclustering
find_package( FastJet COMPONENTS fastjetplugins fastjettools )

find_package( ROOT COMPONENTS Core Physics )

# Add the shared library:
if( XAOD_STANDALONE )
  # Generate a CINT dictionary source file:
  atlas_add_root_dictionary( TopCPToolkitLib _cintDictSource
    ROOT_HEADERS Root/LinkDef.h
    EXTERNAL_PACKAGES ROOT )

  atlas_add_library(
    TopCPToolkitLib
    TopCPToolkit/*.h PartonHistory/*.h TopParticleLevel/*.h TopSpaNet/*.h TopNNLOReweighter/*.h VectorHelpers/*.h
    Root/*.cxx Root/preloader/*.cxx ${_cintDictSource}
    INCLUDE_DIRS ${ROOT_INCLUDE_DIRS} ${KLFITTER_INCLUDE_DIRS} ${ONNXRUNTIME_INCLUDE_DIRS}
    PUBLIC_HEADERS TopCPToolkit PartonHistory TopParticleLevel TopSpaNet TopNNLOReweighter VectorHelpers
    LINK_LIBRARIES ${ROOT_LIBRARIES} ${KLFITTER_LIBRARIES} ${ONNXRUNTIME_LIBRARIES}
    AnaAlgorithmLib AsgAnalysisAlgorithmsLib xAODMissingET xAODJet xAODBTagging xAODEgamma xAODMuon xAODTau xAODTruth xAODCore PathResolver FourMomUtils FTagAnalysisInterfacesLib SystematicsHandlesLib SelectionHelpersLib MCTruthClassifierLib GammaORToolsLib AthContainers
    # THE FOLLOWING ARE NEEDED FOR THE COMPONENT PRELOADER:
    EgammaAnalysisAlgorithmsLib ElectronPhotonFourMomentumCorrectionLib MuonEfficiencyCorrectionsLib ElectronPhotonSelectorToolsLib TrackingAnalysisAlgorithmsLib TruthClassificationLib PMGToolsLib EventSelectionAlgorithmsLib xAODBTaggingEfficiencyLib JetAnalysisAlgorithmsLib JetJvtEfficiencyLib ElectronEfficiencyCorrectionLib JetUncertaintiesLib MuonAnalysisAlgorithmsLib FTagAnalysisAlgorithmsLib TrigGlobalEfficiencyCorrectionLib MuonSelectorToolsLib MuonMomentumCorrectionsLib METUtilitiesLib ParticleJetToolsLib PileupReweightingLib egammaMVACalibLib MetAnalysisAlgorithmsLib GoodRunsListsLib TauAnalysisAlgorithmsLib TriggerAnalysisAlgorithmsLib TrigConfxAODLib AssociationUtilsLib
    # END OF COMPONENT PRELOADER
  )
else ()
  atlas_add_library(
    TopCPToolkitLib
    TopCPToolkit/*.h HiggsPartons/*.h TopParticleLevel/*.h TopSpaNet/*.h TopNNLOReweighter/*.h VectorHelpers/*.h
    Root/*.cxx
    INCLUDE_DIRS ${KLFITTER_INCLUDE_DIRS} ${ONNXRUNTIME_INCLUDE_DIRS} ${FASTJET_INCLUDE_DIRS}
    PUBLIC_HEADERS TopCPToolkit PartonHistory TopParticleLevel TopSpaNet TopNNLOReweighter VectorHelpers
    LINK_LIBRARIES ${KLFITTER_LIBRARIES} ${ONNXRUNTIME_LIBRARIES} ${FASTJET_LIBRARIES}
    AnaAlgorithmLib AsgAnalysisAlgorithmsLib xAODMissingET xAODJet xAODBTagging xAODEgamma xAODMuon xAODTau xAODTruth xAODCore PathResolver FourMomUtils FTagAnalysisInterfacesLib SystematicsHandlesLib SelectionHelpersLib MCTruthClassifierLib GammaORToolsLib
    )
endif ()

if ( XAOD_STANDALONE ) #Add the dictionary (for AnalysisBase only):
  atlas_add_dictionary(
    TopCPToolkitDict TopCPToolkit/TopCPToolkitDict.h
    TopCPToolkit/selection.xml LINK_LIBRARIES TopCPToolkitLib
    EXTRA_FILES Root/dict/*.cxx
    )
else () # Add a component library for AthAnalysis only:
  atlas_add_component (
    TopCPToolkit src/components/*.cxx LINK_LIBRARIES
    TopCPToolkitLib
    )
  # still need a minimal dictionary for objects written to storegate
  atlas_add_dictionary(
          TopCPToolkitDict TopCPToolkit/TopCPToolkitDict.h
          TopCPToolkit/selection_ath.xml LINK_LIBRARIES TopCPToolkitLib
          EXTRA_FILES Root/dict/*.cxx
  )
endif ()

# Install files from the package:
atlas_install_python_modules(python/*.py)

if (XAOD_STANDALONE)
  atlas_install_joboptions(bin/ab/*.py)
  atlas_install_scripts(bin/ab/*.py)
else()
  atlas_install_joboptions(bin/aa/*.py)
  atlas_install_scripts(bin/aa/*.py)
endif()

atlas_install_data(share/*)
