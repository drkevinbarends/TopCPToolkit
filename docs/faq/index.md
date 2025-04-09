# Frequently Asked Questions

*The FAQ will be filled progressively as user questions come in!*

## General

??? question "Where can I ask questions about the code?"
    Check out the relevant [contact points](index.md#get-in-touch).

??? question "Who came up with the name 'TopCPToolkit'?"
    Our good friend ChatGPT :upside_down:. And DALL-E is responsible for the avatar of the project on GitLab.

??? question "Do you have any recommendations about best practices for analyses?"
    We do! Head over to [the dedicated page](../starting/best_practices.md).

## Warning messages

??? question "How do I fix 'The trigger you are looking for, year and mc are not consistent'?"
    If you see the following warning message repeatedly printed
    ```
    ToolSvc.MuonTrigEff      WARNING Could not find what you are looking for in the efficiency map. The trigger you are looking for, year and mc are not consistent, or the trigger is unavailable in this data period. Returning efficiency = 0.
    ```
    then it is likely that your muon trigger settings are wrong.
    One possible reason is a mismatch between Run 2 and Run 3 recommendations, i.e. you could be using an un-supported muon ID WP.
    See [MuonCP docs](https://atlas-mcp.docs.cern.ch/guidelines/release22/index.html#wps-for-run3).
    Incomplete recommendations could also be at fault, as previously reported in [ANALYSISTO-1368](https://its.cern.ch/jira/browse/ANALYSISTO-1368).

??? question "How do I enable debugging printouts?"
    All algorithms and tools inherit the `OutputLevel` property, which you can set to 0 (info), 1 (verbose) or 2 (debug). See also [`MsgLevel`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/Control/AthToolSupport/AsgMessaging/Root/MsgLevel.cxx).

    For an algorithm config in python, you can do something like
    ```python
    myAlgo = config.createAlgorithm( 'CP::SomethingAlg', 'someName' )
    myAlgo.OutputLevel = 2 # sets to DEBUG
    ```
    and `myAlgo` will printout messages wrapped with `ANA_MSG_DEBUG`. Any tool declared by `CP::SomethingAlg` can inherit the current message level (and therefore be driven entirely in python), by setting the corresponding property (in the C++ code):
    ```cpp
        ANA_CHECK( tool->setProperty("OutputLevel", msg().level()) );
    ```
    (or equivalently for our purposes, `ANA_CHECK`).


## Error messages

??? question "I get a crash on 'Exactly two leptons are required to check whether the event is OS or SS!'"
    This comes from your [event selection config](../settings/eventselection.md#available-keywords).
    As the error message suggests, you are trying to use either the `OS` or `SS` keywords in an event selection that is not enforcing the presence of exactly two electrons or muons.
    Enforce it, and the error will go away.

??? question "I get a crash on 'Failed to retrieve NSW hits!'"
    It looks something like
    ```
    MuonSelectionAlg_loose...FATAL   /build1/atnight/localbuilds/nightlies/AnalysisBase/main/athena/PhysicsAnalysis/MuonID/MuonSelectorTools/Root/MuonSelectionTool.cxx:1390 (void CP::MuonSelectionTool::fillSummary(const Muon&, CP::MuonSelectionTool::hitSummary&) const): /build1/atnight/localbuilds/nightlies/AnalysisBase/main/athena/PhysicsAnalysis/MuonID/MuonSelectorTools/Root/MuonSelectionTool.cxx:1390 Failed to retrieve NSW hits!! If you're using DxAODs (with smart slimming for muons), you should use p-tags >= p5834. OR set ExcludeNSWFromPrecisionLayers to True before crashing if you want to technically be able to run on old DAODs, noting that this is allowed only for testing purposes

    ```
    As suggested, please update your Run 3 sample by choosing a more recent p-tag, or edit the `MuonSelectionTool` config.

??? question "I get a crash on 'Unrecognised FTAG MC-to-MC generator setup None, aborting.'"
    Please check the `GeneratorInfo` entry of the printout of TopCPToolkit flags, which should appear at the top of your job log. If it is indeed `None`, this is an issue related to the metadata of your sample. Otherwise, please report it!

??? question "I get a crash on 'ValueError: invalid generator type'"
    This error will also tell you what the detected generator version is: the problem is that it's not supported by the current FTAG [MC-to-MC recommendations](https://ftag.docs.cern.ch/activities/mcmc/#mcmc-efficiency-map-software). To by-pass this restriction, you will have to set a supported generator version by hand, with the [`generator` property](../settings/jets.md#ftagconfig). Please consult your FTAG group liaison for advice on this. If you believe this generator should indeed be supported, please report it.

??? question "I get a crash on 'Unknown trigger 'my_jet_trigger' found while parsing trigger combination' / Why are jet triggers not supported?"
    Jet triggers are supported for trigger selection, but not trigger matching and the retrieval of trigger scale factors. These last two items should be performed on analysis-by-analysis basis for jet triggers (but they are supported centrally for lepton triggers).

    This is the reason for the crash: you are attempting to run trigger matching for jet triggers that are not supported by the [`TrigGlobalEfficiencyCorrectionTool`](https://twiki.cern.ch/twiki/bin/viewauth/Atlas/TrigGlobalEfficiencyCorrectionTool). When you pass the dictionary `triggerChainsPerYear`, the code will attempt to set up both the selection and matching tools. To set up only the selection tool, use the list `triggerChainsForSelection` instead. Or alternatively, set the option `noGlobalTriggerEff` to `True`. More details in [Trigger](../settings/trigger.md).

??? question "I get a crash on 'Sample is FastSim but no AF3 calibration is available yet with es2022_R22_PRE recommendations.'"
    As stated, you are running on an AF3 (fast simulation) sample, for which Egamma recommendations are not available in this AnalysisBase release. For a quick fix, you can set the `forceFullSimConfig` property to `True`, but please note that this is only acceptable in the absence of AF3 recommendations and will not be correct in the long-term for physics analysis! For more details, please get in touch with your local Egamma expert.

## Derivation formats

??? question "Which derivation formats are currently supported?"
    We support PHYS and PHYSLITE. If you would like to use this framework for CP-specific derivation formats, or have observed a breakage in a supported format, please [open an issue](../contributing/index.md).

??? question "Which p-tags are supported?"
    We always recommend using the [latest available p-tags](https://twiki.cern.ch/twiki/bin/view/AtlasProtected/FullListOfPtagsAndPatternsR22).

??? question "How can I check the metadata present in my sample?"
    After setting up an AnalysisBase release, you can run
    ```
    meta-reader -m peeker --hideContentList --hideTrigger <file>
    ```
    on a given DxAOD file to printout the metadata that is available to TopCPToolkit.

## Unexpected output

??? question "No events pass the GRL selection in this MC sample!"
    This is because your MC sample is being treated as data. It has been observed on a few MC samples that some of the metadata is wrongly written, e.g. the `SimFlavour` field is missing in p5631 and the code is misled into defaulting to data.


## Analysis use-cases

??? question "I've been asked to run bootstraps on data/MC. What are those? Can I use TopCPToolkit to do it?"
    "Bootstrapping" means reweighting your dataset (whether actual data or MC simulations) so as to provide _replicas_ that, as an _ensemble_, describe the expected statistical fluctuations. Essentially, for each event a series of $k$ random numbers are generated following a Poisson distribution with central value 1. The standard deviation of these $k$ replicas then represents the statistical uncertainty. Moreover, by making sure that each event leads to a unique series of random numbers, multiple analyses with overlapping event selections can determine the overlap, as well as the statistical correlations needed for a combination.

    This is the recommendation of the ATLAS Statistical Forum for combinations and analysis preservation (you are requested to store the bootstrapped data to HEPdata), as detailed in [ATL-PHYS-PUB-2021-011](https://cds.cern.ch/record/2759945/files/ATL-PHYS-PUB-2021-011.pdf). A dedicated algorithm was developed with TopCPToolkit, see [`makeBootstrapGeneratorConfig`](../settings/others.md#makebootstrapgeneratorconfig).

## Migrating from AnalysisTop

??? question "If I set up AnalysisBase, I can run vanilla AnalysisTop (`top-xaod`) directly with a config and a sample list. Can I do the same in TopCPToolkit?"
    The reason you could run `top-xaod` directly is because AnalysisTop is a package (`TopPhys`) within Athena, and is compiled as part of AnalysisBase releases. For the time being, TopCPToolkit is not. This means that you need to clone the repository and compile it before you can use `runTop_el.py`.

??? question "AnalysisTop works for both release 21 and release 24. Does TopCPToolkit as well?"
    No, TopCPToolkit is focused on release 24. If you're starting a new analysis, you should use release 24.

??? question "Is the output of AnalysisTop and TopCPToolkit any different?"
    Yes, very much so! See [Migration AnalysisTop to CP algorithms](https://indico.cern.ch/event/1257761/contributions/5359759/attachments/2644629/4577607/Migrating%20AT%20to%20CP%20algo.pdf) and [Making ntuples from PHYSLITE with CP algorithms](https://indico.cern.ch/event/1335237/contributions/5702431/attachments/2771372/4830140/Making%20ntuples%20from%20PHYSLITE%20with%20CP%20algorithms.pdf).

    It really boils down to this: a single TTree is produced for all your detector-level objects and their systematic variations. This means that for each object, we need to keep track of two things: what quantities associated to this object are affected by systematics? and is this object still valid (in the sense of JVT cuts, overlap removal, etc.) for a given systematic variation? To that end, you'll find branches named `object_select_XYZ_%SYS%` that contains booleans. These booleans tell you whether the object at that index passes the selection XYZ for a given systematic. Only when this is true should you consider the corresponding entry of the object kinematic vector.

??? question "What is the equivalent of a CustomEventSaver from AnalysisTop, in TopCPToolkit?"
    To perform any custom operation, you'll need to [write your own algorithm](https://atlassoftwaredocs.web.cern.ch/ABtutorial/alg_basic_algorithm/). While it is possible to write a single, huge algorithm to reproduce all the steps of your previous CustomEventSaver, this is not recommended. Instead, you should make life easier for yourself (and for others!) by writing algorithms that serve a single purpose: this will help with managing the code, updating and reusing it, and you may find that algorithms already exist that can help you streamline your analysis!

??? question "With AnalysisTop I could define my own objects with a CustomObjectLoader. Can this be done in TopCPToolkit?"
    You already have a lot of freedom in defining your physics objects (reconstruction, ID, isolation, kinematic cuts, etc.) and you can define as many such object collections as you wish (i.e. not limited to "loose" vs "tight"). If you need to define a radically different type of object, e.g. by disabling some aspects of reconstruction or plugging in your own code, this is also possible but should be discussed publicly within your PA group. All the common CP algorithms are available [here](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/).

??? question "I need to run on both reconstructed and truth objects, but with different selections (e.g. more inclusive fiducial phase-space for unfolding). Is this possible with TopCPToolkit?"
    Yes, and we provide examples of such configs (see e.g. [exampleTtbarLjets](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/tree/main/source/TopCPToolkit/share/configs/exampleTtbarLjets)). Once you have defined the `particle.yaml` and/or `parton.yaml` configs, you can toggle them by passing the `--particle` and/or `--parton` flags on the command line.

??? question "I need to run on both reconstructed and truth objects, and save everything in the same tree. Is this possible with TopCPToolkit?"
    Yes. There is no conceptual difference here: truth objects can also be accessed from some container in the input DAOD file and you can run any appropriate tool or algorithm on them. The systematics service will simply treat them as nominal-only.

    To achieve this in practice, just include the relevant truth-level blocks in your `reco.yaml` config.

??? question "Can I build histograms on-the-fly and save them in the output ROOT file?"
    It is possible to build histograms while running the EventLoop (e.g. cutflow histograms), but it's not a convenient feature nor one we are serioulsy considering for analysis purposes. We strongly recommend to leave the histogramming to a second, more specialised and efficient step.

??? question "How do I create new variables, that are not directly related to an existing object collection (e.g. jets, leptons)?"
    Any variable you wish to save to the output must be decorated onto objects belonging to some container. For event-level variables (e.g. object multiplicities, reconstructed resonance kinematics, etc.), you should decorate the `EventInfo` container (which has a single object). A simple example of algorithm decorating `EventInfo` is [`LeptonSFCalculatorAlg`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AsgAnalysisAlgorithms/Root/LeptonSFCalculatorAlg.cxx).

??? question "My favourite post-processing/histogramming framework doesn't work with the output of TopCPToolkit! How do I fix it?"
    This is expected, due to the change in format of the output (see earlier FAQ item). Dataframe-based workflows are more adapted than `ROOT::TTree::Draw`-style approaches. We strongly recommend [FastFrames](https://atlas-project-topreconstruction.web.cern.ch/fastframesdocumentation/).

??? question "Can I validate my AnalysisTop cutflows against the one from TopCPToolkit?"
    This is a bit tricky and depends on the event selection in question... There is a limitation in TopCPToolkit with how we represent event selections that inherit cuts from other selections (`SUB`+`IMPORT` keywords), as explained [here](running_local.md/#cutflows). Compared to AnalysisTop, the event selection associated to e.g. primary vertexing, event cleaning, or triggering, happens right when the relevant algorithms are run. This means that events that do reach your custom event selection have already passed these cuts. Therefore it may not be possible to fully validate the cutflows between AnalysisTop and TopCPToolkit, but at least you should observe the same number of events surviving the entire selection process.

??? question "Why do some systematically-varied vector branches sometimes feature default values?"
    This is a consequence of the single-TTree formalism. For each object, we need to keep track of two things: what quantities associated to this object are affected by systematics? and is this object still valid (in the sense of JVT cuts, overlap removal, etc.) for a given systematic variation? To that end, you'll find branches named `object_select_XYZ_%SYS%` that contains booleans. These booleans tell you whether the object at that index passes the selection XYZ for a given systematic. Only when this is true should you consider the corresponding entry of the object kinematic vector.

??? question "Are objects (electrons, muons, jets, ...) sorted in pT for each event in the output ntuples?"
    No. In CP algorithms (and hence TopCPToolkit) objects are **NOT** guaranteed to be sorted in pT because of the single-TTree format. With systematics, pT ordering cannot be preserved. Sorting in the NTuple is not attempted, not even in a nominal-only run.