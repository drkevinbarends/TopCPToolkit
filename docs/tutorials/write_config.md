!!! abstract "In this session you will..."
    - learn how to run TopCPToolkit
    - become more familiar with the output ntuple structure
    - understand how algorithms can be set up from a YAML config
    - write a simple analysis selection
    - start exploring the documentation of algorithms

*Let's dive right in!*

## Running TopCPToolkit

After a successful compilation, you now have access to `runTop_el.py` on the command line: it's [this script](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/bin/runTop_el.py).
Go to your `run` directory to execute the next commands.

### Getting help

To check that your compilation works fine, try to get help about TopCPToolkit:
```sh
runTop_el.py -h
```

You can find more information about the available command line options [in the docs](../starting/running_local.md#command-line-options).

### Defining an input sample to process

In this tutorial, we will use the nominal $t\bar{t}$ $\ell+$jets sample for Run 3, with DSID 601229, in DAOD PHYS derivation format.
We just need one file for testing purposes, and there is one conveniently available on CVMFS so let's use it:
```sh
echo "/cvmfs/atlas.cern.ch/repo/sw/database/GroupData/dev/AnalysisTop/ContinuousIntegration/R22-Run3/MC/p5855/DAOD_PHYS.601229_r13829_p5855.pool.root.1" > input.txt
```

!!! tip
    There is a little "copy to clipboard" button on the right-hand side of such code blocks, to make your life easier!

### Running from a YAML config file

Config files are located in the [`source/TopCPToolkit/share/configs`](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/tree/main/source/TopCPToolkit/share/configs?ref_type=heads) folder of TopCPToolkit.
Each analysis is set up as one subfolder, containing up to three YAML files: `reco.yaml`, `particle.yaml` and `parton.yaml`.
These allow us to run separate analyses at detector-, particle- and parton-level.

You can browse the config files hosted there to get an idea of what a "real" analysis might look like.
For now, we'll use the `tutorial` folder, which contains a `reco.yaml`, a `particle.yaml` and a `solution.yaml`.

!!! warning
    Don't look at the solution yet! :pray: :upside_down:

Let's now have our first exercise. You can toggle the solution block below if needed.

!!! example "Exercise"
    Run TopCPToolkit on the tutorial config (`tutorial`) and input file (`input.txt`), but only for the first 100 events and without systematics. The output file should be called `output.root`

??? success "Solution"
    ```
    runTop_el.py -i input.txt -o output -t tutorial -e 100 --no-systematics
    ```

### Exploring the output file

At this point, you should find that a new file has been created: `output.root`.
If you open it with ROOT, you'll find the following content:

- a single TTree called `reco`
- a series of TH1 histograms called `cflow_*`
- two TH1 histograms called `listOfSystematics` and `CutBookkeeper_*`
- a few objects called `EventLoop_*`

In this tutorial, **we will focus only on the TTree**, which contains our processed ntuple.
You can find more information about the other outputs [in the docs](../starting/running_local.md).
Very briefly, there are object-wise and event-wise cutflows, a record of all the sytematics that were considered in the job, complete with book-keeping of systematic-wise sums of weights, and some EventLoop metadata.
These (except maybe the EventLoop stuff) are absolutely necessary when running a physics analysis over multiple files and samples.
The format is particularly adapted e.g. to the [`FastFrames` histogramming package](https://gitlab.cern.ch/atlas-amglab/fastframes).

You may now want to investigate the structure of the `reco` TTree.
You'll find quite a lot of branches, with scalar or vector content.
Notice for instance the branches associated to electrons, prefixed with `el_`: these are vectors (of length the number of electrons in the event) of e.g. $\eta$ (`el_eta`), $\phi$ (`el_phi`), charge (`el_charge`).
There is also a branch for the electron $p_\mathrm{T}$, with a different naming convention: `el_pt_NOSYS`.
This is a quantity that is affected by the EGamma calibration: if you run with systematis enabled, you'll see that we still get only a single version of e.g. `el_eta`, `el_phi` and `el_charge`, but different values of `el_pt_%SYS%`.

Here are a few more easy exercises to keep you engaged :wink:

!!! example "Exercise"
    Run again the same command, but enabling systematics. How does the output differ?

??? success "Solution"
    The command is
    ```
    runTop_el.py -i input.txt -o output -t tutorial -e 100
    ```
    and produces many more branches. In particular, we can now see which systematics affect which physical quantities in the `reco` tree.

!!! example "Exercise"
    Which object collection you might have reasonably expected from a $t\bar{t}$ $\ell+$jets analysis is missing from the output?

??? success "Solution"
    Electrons (`el_*`) and jets (`jet_*`) are there, but muons are missing! (no `mu_*` branches)

!!! example "Exercise"
    Run again the same command, but enabling the particle-level analysis. How does the output differ?

??? success "Solution"
    The command is
    ```
    runTop_el.py -i input.txt -o output -t tutorial -e 100 --particle
    ```
    and it leads to one more tree, `particleLevel`, being created in the output file.

## What is actually going on ?

As you were running TopCPToolkit, you saw the horrifyingly long wall of terminal printouts that it generated :scream:
Here we will give some **basic pointers of what to look for in this printed output**, and how that connects to the analysis we ran.

### The top part (takes a couple of seconds)

This is where we start loading all the libraries needed to run our EventLoop, and it may take a little bit of time.
It's also where some of the metadata reading might take place - this is a crucial step in ensuring the code can configure itself automatically depending on whether we're looking at data/MC, fullsim/fastsim, Run 2/Run 3, etc.

It looks like
```
xAOD::Init                INFO    Environment initialised for data access
Py:AutoConfigFlags    INFO Obtaining metadata of auto-configuration by peeking into '/cvmfs/atlas.cern.ch/repo/sw/database/GroupData/dev/AnalysisTop/ContinuousIntegration/R22-Run3/MC/p5855/DAOD_PHYS.601229_r13829_p5855.pool.root.1'
Py:MetaReader        INFO Current mode used: lite
Py:MetaReader        INFO Current filenames: ['/cvmfs/atlas.cern.ch/repo/sw/database/GroupData/dev/AnalysisTop/ContinuousIntegration/R22-Run3/MC/p5855/DAOD_PHYS.601229_r13829_p5855.pool.root.1']
```

and eventually, if everything is in order, we get a summary of the metadata that has been loaded (very useful when asking for help!)
```
=========================================================================
==================== TOPCPTOOLKIT FLAG CONFIGURATION ====================
=========================================================================
   DataType:        DataType.FullSim
   LHCPeriod:       LHCPeriod.Run3
   RunNumber:       [410000]
   MCChannelNumber: 601229
   RunNumberAsInt:  410000
   AMITag:          e8472_e8475_s3873_s3874_r13829_r13831_p5855
   isRun3:          True
   isPHYSLITE:      False
   isTRUTH:         False
   MCCampaign:      Campaign.MC21a
   GeneratorInfo:   {'Powheg': None, 'Pythia8': '307', 'EvtGen': '2.1.1'}
   eTag:            e8472
=========================================================================
```

### The second part (quick'n'dirty)

At that point, the various analysis algorithms are being loaded and configured.
This happens in several steps, separated by these statements:
```
>>> Setting up configuration based on YAML
>>> Configuring algorithms based on YAML file
>>> Configuration used:
```
We won't go into the detail of this configuration, but it's also valuable information for developing and debugging TopCPToolkit.
The next interesting bit is right after, and takes the form of a tree structure:
```
/*** AlgSequence/AlgSequence ***************************************************
| /*** PythonConfig AsgService/CP::SystematicsSvc/SystematicsSvc *****************
| \--- (End of PythonConfig AsgService/CP::SystematicsSvc/SystematicsSvc) --------
| /*** PythonConfig AsgService/CP::SelectionNameSvc/SelectionNameSvc *************
| \--- (End of PythonConfig AsgService/CP::SelectionNameSvc/SelectionNameSvc) ----
| /*** PythonConfig AnaAlgorithm/CP::SysListDumperAlg/SystematicsPrinter *********
| |- histogramName: 'listOfSystematics'
| \--- (End of PythonConfig AnaAlgorithm/CP::SysListDumperAlg/SystematicsPrinter)
| /*** PythonConfig AnaAlgorithm/CP::PileupReweightingAlg/PileupReweightingAlg ***
| |- pileupReweightingTool:
| | /*** Private Tool CP::PileupReweightingTool/pileupReweightingTool **************
| | |- ConfigFiles: ['GoodRunsLists/data22_13p6TeV/20230207/purw.actualMu.2022.root', 'PileupReweighting/mc21_common/mc21a.410000.physlite.prw.v1.root']
| | |- LumiCalcFiles: ['GoodRunsLists/data22_13p6TeV/20230207/ilumicalc_histograms_None_431810-440613_OflLumi-Run3-003.root']
| | \--- (End of Private Tool CP::PileupReweightingTool/pileupReweightingTool) -----
| \--- (End of PythonConfig AnaAlgorithm/CP::PileupReweightingAlg/PileupReweightingAlg)
(...)
```

Here you find the exact sequence of all the algorithms that will be run in the EventLoop, in order, and with the various relevant settings.
In the (short) excerpt above, we see that the very first item is the `SystematicsSvc` (service), which is responsible for handling systematics (whether to use them, which ones, and for each algorithm what it needs to be aware of).

The last item shown above is the `PileupReweightingAlg`, which wraps around the familiar `PileupReweightingTool`, here configured with the relevant Run 3 PRW and lumicalc files.

!!! example "Exercise"
    Look further down in the algorithm sequence, for the ones related to electron identification and selection (e.g. `ElectronPtCutAlg`, `ElectronEtaCutAlg`, `ElectronLikelihoodAlg`). What electron ID is requested? What $p_\mathrm{T}$ and $\eta$ cuts are applied?

??? success "Solution"
    The ID WP is "TightLH" (`DFCommonElectronsLHTight`). The electrons are requested to have $\lvert\eta\rvert <2.45$ with a veto of the LAr gap $1.37<\lvert\eta\rvert<1.52$, and to have $p_\mathrm{T}>4.5$ GeV.

Please also note what's happening at the very bottom of this algorithm sequence, in particular the block right after
```
| /*** PythonConfig AnaAlgorithm/CP::AsgxAODNTupleMakerAlg/NTupleMaker ***********
```
These are the algorithms responsible for writing the output ntuple.
You can already notice the `Branches` property, which is a list of strings of the form
```
'OutElectrons_%SYS%.e_%SYS% -> el_e_%SYS%'
```
We'll come back to those in due time, but this is another powerful feature of this framework: **we can create all sorts of output branches simply by writing strings!** No need to fiddle with the TTree creation and management ourselves. :heart_eyes:

### The third part (varied and entertaining)

At this point, the full analysis sequence has been set up, and each algorithm and its corresponding tools will undergo initialisation.
This is evidenced in the first few lines, e.g.
```
Package.EventLoop        INFO    Opening file file:///cvmfs/atlas.cern.ch/repo/sw/database/GroupData/dev/AnalysisTop/ContinuousIntegration/R22-Run3/MC/p5855/DAOD_PHYS.601229_r13829_p5855.pool.root.1
CP::TPileupReweighting... INFO    Using MC16 Period configuration
```
(note that the printouts of the PRW tool are not quite up to date, but as we've seen earlier it is correctly configured for Run 3, so this is actually harmless! :sweat_smile:)

If you're already familiar with CP tools and DAOD-processing frameworks such as AnalysisTop or SUSYTools, then this part of the terminal output is very similar.
The conceptual difference is that instead of manipulating the tools ourselves, we have relied on the algorithms to set them up in the appropriate ways (following the latest CP recommendations at every stage).

Eventually we reach the end of all the initialisations, and start looping over events:
```
Package.EventLoop        INFO    Processing events 0-100 in file file:///cvmfs/atlas.cern.ch/repo/sw/database/GroupData/dev/AnalysisTop/ContinuousIntegration/R22-Run3/MC/p5855/DAOD_PHYS.601229_r13829_p5855.pool.root.1
```

!!! note
    The event number is only printed out every 10'000th event.

### The last part (looks like it finally stopped)

Once the event loop has terminated, each algorithm in the sequence undergoes its finalisation stage.
Most of the time, this is a do-nothing operation.
In a few cases, we may get some useful printouts, such as:
```
SystematicsPrinter       INFO    Systematics regex '' matched:
SystematicsPrinter       INFO      ''
PrimaryVertexSelectorAlg INFO    accepted 100 out of 100 events for filter VertexSelection (vertex selection)
EventFlagSelectionAlg    INFO    accepted 100 out of 100 events for filter JetCleaning (selecting events passing: DFCommonJets_eventClean_LooseBad,as_char)
CutBookkeeperAlg         INFO    CutBookkeeper information will be stored in CutBookkeeper_601229_410000_NOSYS
```

The first two lines are the `SystematicsPrinter` informing that we indeed ran without systematics in this example.
The third and fourth lines are our first examples of **event selection reports**: here requiring that events have a primary vertex and that they pass basic jet cleaning cuts.
Several other algorithms come with such event selections, e.g. trigger selection and matching, or as we will see in a next section, analysis-specific cuts.
The last line is the `CutBookkeeper` histogram we briefly mentioned earlier, that will also show up in the output file and contains the sum of MC weights seen, for cross section normalisation purposes.

!!! example "Exercise"
    What do the `SystematicsPrinter` and `CutBookkeeperAlg` return in their finalisation stage, when systematics are enabled?

??? success "Solution"
    The `SystematicsPrinter` prints out the names of all systematics that affect the object collections we defined (electrons, jets, flavour-tagging, etc.). The `CutBookkeeperAlg` lists all the PMG weights (scales, PDFs, etc.).

At the very bottom, we find some printouts from the EventLoop manager (and usually some summaries from `CalibrationDataInterfaceROOT`, perfectly fine).
A successful job will end with
```
Package.EventLoop        INFO    current job status: 1 success, 0 failure, 0 running/unknown
Package.EventLoop        INFO    done
```

There is a very final processing step in TopCPToolkit:
```
hadd Target file: output.root
hadd compression setting for all output: 1
hadd Source file 1: only_reco_histograms.root
hadd Source file 2: output_reco.root
hadd Target path: output.root:/
```

The last warning is safe to ignore, here. What we have done behind the scenes is write the ntuple to a file called `output_reco.root`, and the metadata histograms to `only_reco_histograms.root`.
We then merge them manually (with `hadd`) into `output.root`.
As we will see in a later section of this tutorial, it is possible to run multiple levels of analysis (using detector-level and truth-level objects) separately, which will require their own intermediate ntuples, before being merged to `output.root` too.

## Looking inside the YAML config

Let's now take a look at the YAML config we've been calling when running TopCPToolkit.
As a reminder, config files live under [`share/configs`](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/tree/main/source/TopCPToolkit/share/configs?ref_type=heads) and we're working with the `tutorial` one.
You should find a single file there, `reco.yaml`: this is our detector-level analysis configuration.

### Reading and writing YAML

YAML is a human-readable format featuring minimalist syntax, allowing for comments and supporting a range of data types - perfect to easily and cleanly script your analysis requirements!
There are many tutorials and introductions available online (see e.g. [here](https://www.educative.io/blog/yaml-tutorial) and [there](https://www.cloudbees.com/blog/yaml-tutorial-everything-you-need-get-started)).
Let's just note a few points of relevance to us:

- indentation is used for nesting objects: you **must use spaces, not tabs!**
- you will only ever need to pass **booleans** (e.g. `True`), **strings** (e.g. `AnaJets`), **floats** (e.g. `2.5`), or **lists** and **dictionaries** thereof
    - a dictionary is expressed as
    ```yaml
      key1: value1
      key2: value2
    ```
    - a list is expressed as
    ```yaml
      - item1
      - item2
    ```
    - dictionaries and lists can be empty, denoted as `{}` and `[]` respectively
    - more complicated structures are possible, using mixtures of the above:
    ```yaml
    fancyStructure:
      - class1:
          property1: someValue
          property2:
            - item1
            - item2
            - item3
      - class2:
          property1: {}
          property2: 'someString'
    ```
- comments can be inserted following the `#` symbol

!!! note
    You will never have to write any code to interpret the YAML structure yourself, but it can be helpful to know what's going on in order to fix typical formatting mistakes!

### YAML blocks

Let's look at a reasonably short example, the configuration of electrons.
This is the corresponding YAML block you'll find the in `tutorial/reco.yaml`:
```yaml
Electrons:
  - containerName: 'AnaElectrons'
    crackVeto: True
    IFFClassification: {}
    WorkingPoint:
      - selectionName: 'loose'
        identificationWP: 'TightLH'
        isolationWP: 'NonIso'
        noEffSF: True
      - selectionName: 'tight'
        identificationWP: 'TightLH'
        isolationWP: 'Tight_VarRad'
        noEffSF: True
    PtEtaSelection:
        minPt: 25000.0
        maxEta: 2.47
        useClusterEta: True
```

Observe that we are mixing "Pascal case" (e.g. `WorkingPoint`) and "Camel case" (e.g. `selectionName`).
This is on purpose: **camel-case indicates a configurable property, while Pascal-case indicates an independent structure**.
So here we are setting up our electrons with a one-element list, which has the configurable properties `containerName` and `crackVeto`.
Then we also associate to it three sub-structures: `IFFClassification` (which is set with an empty dictionary), `WorkingPoint` (which is a two-element list with three properties for each element), and `PtEtaSelection` (which is a dictionary with two properties).

If this is clear to you, let's now connect this notation with our actual algorithm sequence.

### Common CP algorithms and their config blocks

If you know something about EGamma CP, you'll have recognised that the YAML block we've just discussed looks like it might be calibrating electrons and requesting specific ID and isolation working points.
But where does this notation come from?
To answer that, let's first take a look at [ConfigFactory.py](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AnalysisAlgorithmsConfig/python/ConfigFactory.py), specifically the `addDefaultAlgs` method therein.

You'll find a series of `import` statements followed by calls to `config.addAlgConfigBlock`.
This is where we connect the config blocks (Python wrapper classes) for specific CP algorithms to the names we use in the YAML config.
Following on our example with electrons:
```python
# electrons
from EgammaAnalysisAlgorithms.ElectronAnalysisConfig import ElectronCalibrationConfig
config.addAlgConfigBlock(algName="Electrons", alg=ElectronCalibrationConfig)
from EgammaAnalysisAlgorithms.ElectronAnalysisConfig import ElectronWorkingPointConfig
config.addAlgConfigBlock(algName="WorkingPoint", alg=ElectronWorkingPointConfig,
                         superBlocks="Electrons")
```

As we suspected, the YAML block headed by `Electrons:` is setting up the calibration of electrons, which is really done by the [`ElectronCalibrationConfig`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/EgammaAnalysisAlgorithms/python/ElectronAnalysisConfig.py) class, and the one headed by `WorkingPoint:` is creating not one but two instances of [`ElectronWorkingPointConfig`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/EgammaAnalysisAlgorithms/python/ElectronAnalysisConfig.py).
If you dig a little bit in the Python code for the config blocks, you'll find that the block responsible for the calibration of electrons looks up the "Electrons" collection in the input DAOD and writes the calibrated output to whatever is specified as `containerName` (here: "AnaElectrons").
The block responsible for applying ID and isolation cuts in turns needs to know what calibrated container to run on.
Were we to make longer chains of blocks that each require properties set by previous blocks, the bookkeeping might become quite tedious.
This is why we've written the `WorkingPoint:` indented within the "parent" `Electrons:` (declared above with `superBlocks`): it will now by default inherit all the properties of its parent block.
In this case, avoiding us the trouble of tracking the container names (and other subtleties) - convenient!

!!! example "Exercise"
    Inspect the [`ElectronCalibrationConfig`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/EgammaAnalysisAlgorithms/python/ElectronAnalysisConfig.py) block a little bit (hint: just the first few lines!). Using the DAOD collection "Electrons", as we've mentioned, would only work for DAOD PHYS. If we wanted to run on DAOD PHYSLITE, would we actually need to change anything in our YAML config?

??? success "Solution"
    No, we wouldn't! The config block is able to detect by itself whether we are running on DAOD PHYSLITE (the line `if config.isPhyslite()`), in which case it would automatically adjust to the appropriate name for the input electrons.

**Now for a little trick:**
!!! tip
    Start typing "ElectronWorkingPointConfig" in the search bar at the top right of this page. You should quickly be directed to the relevant section our ["Settings" documentation](../settings/electrons.md#electronworkingpointconfig). There, we document all the available config blocks for the common CP algorithms, as well as those registered by our users in TopCPToolkit.

    Use the left-hand side menu to navigate between the different physics objects and analysis features. On the right-hand side, you'll find we've divided the code into "make-methods" and "config blocks" - only the latter are relevant to us in this tutorial.

    For each block, we list all the configurable properties, what type of value they expect, and where relevant the allowed choices. Scrolling down, you'll also find a mention of "registered variables", i.e. output branches that are automatically created by the addition of this config block in your YAML config.

Using the documentation, see if you can answer the following questions:

!!! example "Exercise"
    What does the line with `IFFClassification: {}` set up, in our example electron config?

??? success "Solution"
    It runs an instance of the [`IFFLeptonDecorationBlock`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AsgAnalysisAlgorithms/python/AsgAnalysisConfig.py) on the calibrated electrons, using the default settings of the block. Specifically, these default settings mean that we end up with a separate IFF class for charge-flipped electrons, and that the IFF class is decorated as `IFFClass_%SYS%` onto each electron. Furthermore, this decoration is saved as a branch in the output ntuple.

!!! example "Exercise"
    What is meant by "loose" and "tight" in our electron block? Are we limited to these two choices?

??? success "Solution"
    In the `WorkingPoint` YAML sub-block, we generate two instances of the [`ElectronWorkingPointConfig`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/EgammaAnalysisAlgorithms/python/ElectronAnalysisConfig.py). One will bear the name "tight" and the other "loose", meaning that we have created two distinct labels for our electrons: though they both carry the same ID working point, one is non-isolated while the other has a strong isolation selection applied.

    This is very useful in analysis, where one might want to use these "tight" electrons for the main measurement, and rely on the "loose" electrons to better model the fake lepton contributions. Note that there is no limitation whatsoever to how many labels you can generate, nor to what you name them. We could very easily add a third "mycustomelectrons" label with yet another choice of ID and isolation working points.

### Text-based event selections

Look further down in the `tutorial/reco.yaml` config until you find the block headed by `EventSelection:`.
This is a very useful piece of the config, which allows us to play with a **set of keywords to define an event selection with which to filter our output ntuple**, without having to write a single line of C++ code ourselves!

While you explore the documentation related to this block, let us point out a few important features, drawing on the following excerpt:
```yaml
selectionCutsDict:
    'SUBcommon': |
      JET_N_BTAG >= 2
      JET_N 25000 >= 4
      MET >= 20000
      SAVE
```
The pipe symbol (`|`) here allows us to write a single string over several lines, which makes for a much more human-readable format.
We are creating a selection named "SUBcommon" and defining it as a series of cuts that rely on [specific keywords](../settings/eventselection.md#available-keywords).
The selection is terminated by the "SAVE" keyword, which defines "SUBcommon" as the logical AND over all the previous cuts.

Notice now that we have prefixed this particular selection by "SUB", and not done so for any other selection.
This is because "SUBcommon" here plays a special role: it is a common set of cuts we would like to avoid repeating in other selections, but that we do not want to filter events on.
For example, the "ejets" selection starts with the line `IMPORT SUBcommon`, which as the name suggests will load the decision from "SUBcommon" before applying the electron-specific requirements.
Similarly for our "mujets" selection.
Eventually, we only want to select events that pass either "ejets" or "mujets", and not just "SUBcommon" (which would select any event with jets!).

!!! example "Exercise"
    Currently "SUBcommon" requires that a selected event have at least 2 b-tagged jets at the 85% efficiency working point. How would you amend the selection to apply a second cut, this time requiring that at least 1 of these b-tagged jets be tagged at the tighter 70% efficiency working point?

??? success "Solution"
    As per the [documentation of keywords](../settings/eventselection.md#available-keywords), you can specify a custom b-tagger and efficiency working point as an additional argument to `JET_N_BTAG`. The selection would then read
    ```yaml
    'SUBcommon': |
      JET_N_BTAG >= 2
      JET_N_BTAG DL1dv01:FixedCutBEff_70 >= 1
      JET_N 25000 >= 4
      MET >= 20000
      SAVE
    ```

## The final exercise

As we remarked earlier, muons are suspiciously absent from our analysis config.
You may have seen it coming: you now have to implement them yourself! :smiling_imp:

Let's do it step by step... and of course you can use the documentation and search bar! :innocent:

!!! example "Exercise"
    **Step 1:** set up a muon collection and call it "AnaMuons"

      - add a "loose" selection with "Medium" quality and no isolation
      - add a "tight" selection with "Medium" quality and "Tight_VarRad" isolation
      - for tight muons, enforce a more detailed breakdown of systematics
      - make sure that all muons have $p_\mathrm{T}>25$ GeV and $\lvert\eta\rvert<2.5$
      - additionally decorate these muons with their IFF class

    **Step 2:** make sure that we actually save these muons to file!

      - make a cutflow for them
      - add them to the MET definition, removing the `setMuonJetEMScale: False` setting
      - add them to the overlap removal
      - thin the output in a similar way as for electrons
      - add them to the output with the prefix "mu_"

    **Step 3:** edit the trigger selection to include single-muon trigger legs (see [Muon Trigger Recommendations for 2022](https://twiki.cern.ch/twiki/bin/view/Atlas/MuonTriggerPhysicsRecommendationsRun32022))

    **Step 4:** add an instance of the `LeptonSF:` block, which should read in the tight electrons and muons, and produce a single event-wise lepton SF from all the different per-lepton SFs.

    **Step 5:** fix the event selection such that it treats the muons properly

      - add the "tight" muons as inputs to the event selection
      - the "ejets" selection should veto any muons in the event
      - the "mujets" selection should select exactly one muon

    **Step 6:** add to the output ntuple the information "truthOrigin" about the muons, which is present at DAOD-level and does not depend on systematics

??? success "Solution"
    **Step 1:** the entire block should read
    ```yaml
    Muons:
    - containerName: 'AnaMuons'
        IFFClassification: {}
        WorkingPoint:
        - selectionName: 'loose'
            quality: 'Medium'
            isolation: 'NonIso'
        - selectionName: 'tight'
            quality: 'Medium'
            isolation: 'Tight_VarRad'
            systematicBreakdown: True
        PtEtaSelection:
            minPt: 25000.0
            maxEta: 2.5
    ```

    **Step 2:** in the `Thinning:` block, add a list element
    ```yaml
    - containerName: 'AnaMuons'
      outputName: 'OutMuons'
      selectionName: 'tight||loose'
    ```
    Similarly, in the `ObjectCutFlow:` block, add a list element
    ```yaml
    - containerName: 'AnaMuons'
      selectionName: 'tight'
    ```
    In the `MissingET:` and `OverlapRemoval:` blocks, add the property
    ```yaml
    muons: 'AnaMuons.tight'
    ```
    Finally, in the `Output:` block, under `containers:`, add the line
    ```yaml
    mu_: 'OutMuons'
    ```

    **Step 3:** in the `Trigger:` block, pass the properties
    ```yaml
    muons: 'AnaMuons.tight'
    muonID: 'Medium'
    ```
    and add the following line to the 2022 entry of `triggerChainsPerYear`
    ```yaml
    - 'HLT_mu24_ivarmedium_L1MU14FCH || HLT_mu50_L1MU14FCH'
    ```
    **Step 4:** define the `LeptonSF:` block as follows
    ```yaml
    LeptonSF:
    - electrons: 'AnaElectrons.tight'
      muons: 'AnaMuons.tight'
      lepton_postfix: 'tight'
    ```

    **Step 5:** in the `EventSelection:` block, pass the property
    ```yaml
    muons: 'AnaMuons.tight'
    ```
    and add the cuts `MU_N 25000 == 0` to "ejets" and `MU_N 25000 == 1` to "mujets"

    **Step 6:** in the `Output:` block, add a list element to `vars`
    ```yaml
    - 'OutMuons_NOSYS.truthOrigin -> mu_truthOrigin'
    ```

**_Congratulations for making it to the end!_** :star2: :sparkles:

If you're still hungry for more, head over to the next chapter by clicking the button on the side  :point_right: