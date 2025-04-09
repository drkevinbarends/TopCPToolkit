# File for configuration of common stuff
# includes event cleaning, GRL, PRW and MC weight
from AnaAlgorithm.AlgSequence import AlgSequence
from AnalysisAlgorithmsConfig.ConfigSequence import ConfigSequence
from AnalysisAlgorithmsConfig.ConfigAccumulator import ConfigAccumulator
from AnalysisAlgorithmsConfig.ConfigText import TextConfig
from AnalysisAlgorithmsConfig.ConfigFactory import ConfigFactory
from AnaAlgorithm.DualUseConfig import createAlgorithm, createService
from AnalysisAlgorithmsConfig.ConfigAccumulator import DataType
from TopCPToolkit import metaConfig
from ROOT import PathResolver


def makeRecoSequence(analysisName, flags, noSystematics=False, noFilter=False):
    algSeq = AlgSequence()

    configSeq = ConfigSequence()
    factory = ConfigFactory()

    configSeq += factory.makeConfig('CommonServices')
    configSeq.setOptionValue('.runSystematics', not noSystematics)
    configSeq.setOptionValue('.systematicsHistogram', 'listOfSystematics')

    import importlib
    try:
        analysisModule = importlib.import_module(analysisName)
    except ModuleNotFoundError:
        raise Exception(f'The package and module for your --analysis could not be found: {analysisName}')
    try:
        from AnaAlgorithm.DualUseConfig import isAthena, useComponentAccumulator
        if isAthena and useComponentAccumulator:
            return analysisModule.makeRecoConfiguration(flags, algSeq, configSeq, factory, noSystematics, noFilter)
        analysisModule.makeRecoConfiguration(flags, algSeq, configSeq, factory, noSystematics, noFilter)
    except AttributeError:
        raise Exception('The analysis you specified via --analysis does not have makeRecoConfiguration method implemented.'
                        'This is needed to configure the CP algorithms')

    return algSeq


def makeTruthSequence(analysisName, flags, noSystematics=False):
    algSeq = AlgSequence()

    configSeq = ConfigSequence()
    factory = ConfigFactory()

    configSeq += factory.makeConfig('CommonServices')
    # we always want systematics
    configSeq.setOptionValue('.runSystematics', True)
    configSeq.setOptionValue('.systematicsHistogram', 'listOfSystematicsPartonLevel')

    import importlib
    try:
        analysisModule = importlib.import_module(analysisName)
    except ModuleNotFoundError:
        raise Exception(f'The package and module for your --analysis could not be found: {analysisName}')
    try:
        from AnaAlgorithm.DualUseConfig import isAthena, useComponentAccumulator
        if isAthena and useComponentAccumulator:
            return analysisModule.makeTruthConfiguration(flags, algSeq, configSeq, factory, noSystematics)
        analysisModule.makeTruthConfiguration(flags, algSeq, configSeq, factory, noSystematics)
    except AttributeError:
        raise Exception('The analysis you specified via --analysis does not have makeTruthConfiguration method implemented.'
                        'This is needed to configure the CP algorithms')

    return algSeq

def makeParticleLevelSequence(analysisName, flags, noSystematics=False, noFilter=False):
    algSeq = AlgSequence()

    configSeq = ConfigSequence()
    factory = ConfigFactory()

    configSeq += factory.makeConfig('CommonServices')
    # we always want systematics!
    configSeq.setOptionValue('.runSystematics', True)
    configSeq.setOptionValue('.systematicsHistogram', 'listOfSystematicsParticleLevel')

    import importlib
    try:
        analysisModule = importlib.import_module(analysisName)
    except ModuleNotFoundError:
        raise Exception(f'The package and module for your --analysis could not be found: {analysisName}')
    try:
        from AnaAlgorithm.DualUseConfig import isAthena, useComponentAccumulator
        if isAthena and useComponentAccumulator:
            return analysisModule.makeParticleLevelConfiguration(flags, algSeq, configSeq, factory, noSystematics, noFilter)
        analysisModule.makeParticleLevelConfiguration(flags, algSeq, configSeq, factory, noSystematics, noFilter)
    except AttributeError:
        raise Exception('The analysis you specified via --analysis does not have makeParticleLevelConfiguration method implemented.'
                        'This is needed to configure the CP algorithms')

    return algSeq

def makeTextBasedSequence(analysisName, filename, flags, noSystematics=False):
    algSeq = AlgSequence()

    yamlconfig = PathResolver.find_file(f'{analysisName}/{filename}.yaml', "CALIBPATH", PathResolver.RecursiveSearch)
    if not yamlconfig:
        raise FileNotFoundError(f'PathResolver failed to locate \"{analysisName}/{filename}.yaml\" config file!'
                                'Check if you have a typo in -t/--text-config argument or missing file in the analysis configuration sub-directory.')
    print(">>> Setting up configuration based on YAML config:")
    print(yamlconfig + "\n")
    config = TextConfig(yamlconfig)

    # ==============================
    # INSERT CUSTOM BLOCKS BELOW
    # it's a good idea to keep "pos='Output'" to make sure the custom block
    # is configured *before* the Output one, so custom containers are
    # treated properly.

    # from TopCPToolkit.<module> import <config block>
    # config.addAlgConfigBlock(algName='<short name>',
    #                          alg=<config block>, pos='Output')
    # or use the "AddConfigBlocks:" block directly in YAML.

    # END OF CUSTOM BLOCKS
    # ===============================

    print(">>> Configuring algorithms based on YAML file")
    configSeq = config.configure()

    print(">>> Configuration used:")
    config.printConfig()

    configAccumulator = ConfigAccumulator(autoconfigFromFlags=flags,
                                          algSeq=algSeq,
                                          noSystematics=noSystematics)
    configSeq.fullConfigure(configAccumulator)

    from AnaAlgorithm.DualUseConfig import isAthena, useComponentAccumulator
    if isAthena and useComponentAccumulator:
        return configAccumulator.CA

    return algSeq
