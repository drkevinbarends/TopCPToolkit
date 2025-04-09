After a successful compilation, you now have access to `runTop_el.py` on the command line: it's [this script in AthAnalysis](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/bin/aa/runTop_el.py) or [this script in AnalysisBase](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/bin/ab/runTop_el.py).
It's recommended to run the code in a dedicated directory, such as the top-level `run` folder we created earlier.

## Running locally

You can run the event loop on a `DAOD_PHYS` sample (one or multiple files).
Put the absolute or relative path to the file(s) in a text file, which we'll assume from now on you called `inputs.txt`.
Everything else is configured from arguments to the command line, and we'll go over the most important ones.

!!! note
    The code expects to run over **a single sample** at a time, i.e. you shouldn't list multiple DSIDs or MC campaigns in the same input text file.
    For that, you always have to write your own script.

The minimal working command you can issue, using all default settings, is:

```sh
runTop_el.py -i inputs.txt -o output
```


which will **process the entire sample** with the **default analysis** setup, at **detector-level only**, and **including all systematics and event filters**.
This may not be what you want to do, so read on below to see what you can change.

The output ntuple file is saved in the local directory as `output.root`.
The CutBookkeeper histograms are available in the same file.

### Cutflows

The CutBookkeeper histograms serve the dual purpose of keeping track of the MC sum of weights (for later cross section normalisation) and representing cutflows.
We distinguish between two types of cutflows: _object-wise_ and _event-wise_.

**_Object_-wise cutflows** are handled internally from the common CP algorithms associated to each object (electrons, muons, jets, etc.) and decorate systematic-dependent flags onto them at each step of the calibration, identification, isolation... processes.
In order to obtain these cutflow histograms in the output, a specific algorithm config is needed (see [`ObjectCutFlowBlock`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AsgAnalysisAlgorithms/python/AsgAnalysisConfig.py)).
You may then write e.g.
```python
# object-based cutflow
configSeq += makeConfig ('ObjectCutFlow', containerName='AnaElectrons', selectionName='tight')
```
to enable the cutflow histograms for `tight` electrons.
You will find them in `output/hist-output.root`, as `cflow_OBJECT_PRESELECTION_SYSTEMATIC`.
Each bin is filled with the number of objects passing the corresponding selection cut, which is stored as the bin label.

**_Event_-wise cutflows** can technically be generated independently using the [`EventCutFlowBlock`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AsgAnalysisAlgorithms/python/AsgAnalysisConfig.py), but the expected use case is in conjonction with the [event selection config](../settings/eventselection.md).
In that case, you just need to pass the `cutFlowHistograms=True` argument.
You will also find them in `output/hist-output.root`, as `cflow_EventInfo__SELECTION_SYSTEMATIC`.
Each selection region defines its own cutflow.

!!! warning "Issue with the representation of the `IMPORT` keyword"
    There is an inherent limitation to the current setup for event-wise cutflows: if your selection calls a preselection with the `IMPORT` keyword, all we can do is insert the cutflow histogram corresponding to that particular preselection into the one for the selection. This means that:

    - if your selection _starts with_ `IMPORT`, you will see the correct cutflow;
    - if it _doesn't_, you will your cutflow go down as the first few cuts are applied, then a jump back to the original number of events when you reach the `IMPORT`
    keyword.

    In the latter case, the number of events you see in the bins corresponding to the preselection do not actually represent the number of events after your initial cuts + the preselection cuts.
    The correct behaviour resumes afterwards, when the preselection cuts are done.
    In any case, please note that the event filtering itself (and the corresponding event flag in the output tree) is fine.
    This is just a representation issue with the cutflow histograms.

### Sum of weights

Another type of CutBookkeeper histograms is stored in the `output.root`, with name `CutBookkeeper_DSID_RUN_SYSTEMATIC`.
Such histograms contain 3 bins recording information about _all the events seen_ when processing the input files for a given job:

1. the total number of events
1. the sum of weights
1. the sum of squared weights

Since MC generator weight variations are not expected to conserve the nominal inclusive cross section associated with a given MC sample, it is necessary to provide such a histogram for each variation.

When projecting to a histogram representing the number of expected events, the following formula ensures the proper normalisation:

$$ \sigma_\text{eff} \cdot \mathcal{L} \cdot \dfrac{w_i}{\sum_i w_i} $$

with $\mathcal{L}$ the integrated luminosity, $\sigma_\text{eff}$ the effective cross section (generator cross section times filtering efficiency times k-factor, with these numbers provided in the [PMG](https://atlas-groupdata.web.cern.ch/atlas-groupdata/dev/PMGTools/PMGxsecDB_mc16.txt) or [TDP](https://atlas-groupdata.web.cern.ch/atlas-groupdata/dev/AnalysisTop/TopDataPreparation/) databases), $w_i$ the appropriate MC event weight (branch in the output tree), and $\sum_i w_i$ the sum of weights from the corresponding CutBookkeeper histogram.

### Sample metadata

An additional TH1 histogram is written to the output file, named `metadata`. It holds a few relevant aspect of the original DAOD metadata as its bin labels (i.e. read `metadata->GetXaxis()->GetBinLabel(x)`), namely:

- bin 1: the **type of data** (e.g. 'data', 'fullsim', 'fastsim'...)
- bin 2: the MC **campaign** (e.g. 'mc20a', 'mc23c'...) or **data-taking year**
- bin 3: the MC **DSID** (e.g. '410470') or '0' for data samples

This information is particularly important for post-processing with [`FastFrames`](https://gitlab.cern.ch/atlas-amglab/fastframes/).
The bin content is irrelevant, and simply represents the number of individual files that were merged together (usually on the Grid, via the `hadd` operation).

### List of systematics

There are multiple ways one could retrieve the list of systematics run by the various algorithms.
The most robust one is to rely on the [`CP::SysListDumperAlg`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AsgAnalysisAlgorithms/Root/SysListDumperAlg.cxx), an instance of which can be set up via the [`CommonServicesConfig`](../settings/others.md#commonservicesconfig) (see [commonAlgoConfig.py](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/python/commonAlgoConfig.py)).

In the same `output.root`, it generates an empty histogram `listOfSystematics` whose bin labels are the various systematics (including `NOSYS` to represent the nominal calibrations).

!!! tip
    The systematic names are _exactly_ those provided by the various CP tools.

### Filtering systematics

It is possible to not run on every single systematic that would otherwise be set up automatically by the systematics service.
For that, you need to edit the `filterSystematics` property of the [`CommonServicesConfig`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AsgAnalysisAlgorithms/python/AsgAnalysisConfig.py).
This is a [regexp](https://regex101.com/) string that is by default set to `(.*)`, i.e. matches all systematics.
Any valid regexp string can be passed: any systematic name that matches will be accepted and run.

For example:

- `"^(?=.*(?:JER|PRW)|$).*"` will **select only those systematics that contains** "JER" (jet energy resolution) or "PRW" (pileup reweighting) in their name, or the empty string (the `?` at the end) corresponding to nominal.
- `"(?:(?!JER|PRW).)*"` will **veto any systematic that contains** "JER" or "PRW" in their name, but still accept the empty string.

Since AnalysisBase 25.2.9, a user friendly systematics category option `onlySystematicsCategories` [has been introduced](https://gitlab.cern.ch/atlas/athena/-/merge_requests/70864) to [`CommonServicesConfig`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AsgAnalysisAlgorithms/python/AsgAnalysisConfig.py) that allows enabling groups of systematics using a list of keywords. Recognized keywords are: `jets`, `electrons`, `muons`, `photons`, `taus`, `met`, `ftag`, `event`. For example, the following config should set only JER, JES, JVT, FTag and MET, plus nominal/NOSYS:
```
CommonServices:
  runSystematics: True
  systematicsHistogram: 'listOfSystematics'
  onlySystematicsCategories:
    - jets
    - ftag
    - met
```

!!! warning
    The CP algorithms always require the nominal variation (empty string, turned into "NOSYS" internally) to be available within the list of systematics.

### Filtering output branches

It is also possible to remove branches from the output ntuple. This is often needed to save space, as many CP algorithms may declare output variables that you do not need. For that, we can pass a number of commands to the aptly-named `commands` property of the [`OutputAnalysisConfig`](https://gitlab.cern.ch/atlas/athena/-/blob/main/PhysicsAnalysis/Algorithms/AsgAnalysisAlgorithms/python/OutputAnalysisConfig.py).
These are [regexp](https://regex101.com/) strings prefaced by `enable` or `disable`.

For example:

- `"disable jet_.*_eff.*"` will **remove the per-jet efficiency SFs**, which may be desirable when you're already saving their product as a per-event variable (e.g. JVT, b-tagging)
- `"disable trigPassed_HLT.*"` followed by `"enable trigPassed_HLT_e.*"`  will **keep only the electron trigger decisions**.

## Using a text-based config file

Any sequence of algorithmic config blocks, that we have so far implicitly assumed to be written as a pythonic **analysis module** (we'll define those [later on](analysis.md#analysis-modules)) and loaded up with the `-a` flag of `runTop_el.py`, can be translated into a text-based format, specifically using the YAML language. One can then run


```sh
runTop_el.py -i inputs.txt -o output -t exampleTtbarLjets
```


Here the `-t` flag points the driver to the folder [`share/configs/exampleTtbarLjets`](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/tree/main/source/TopCPToolkit/share/configs/exampleTtbarLjets). Up to three config files can be present in that folder and will be automatically looked up: `reco.yaml`, `particle.yaml` and `parton.yaml`. These correspond to detector-level, particle-level and parton-level analysis configurations respectively. You can either pass an absolute or relative path to that folder, or, as in the example above, point to config folder registered in TopCPToolkit.

## Command line options

At minima, only the text file with the input samples and the name of the output file are required to run `runTop_el.py`.
All other flags are optional and listed in the table below. You can also run
```sh
runTop_el.py -h
```


| **Option** | **Default** | **Usage** |
| ---------- | ----------- | --------- |
| `--max-events`<br>`-e`  | -1                                             | Specify the number of events to run over. |
| `--analysis`<br>`-a`    | `TopCPToolkit.TtbarCPalgoConfigBlocksAnalysis` | Specify the analysis module to run as `package.moduleName`. |
| `--text-config`<br>`-t` | None                                           | Specify the text config to run as `folderName`. Alternative to loading an analysis module. |
| `--parton`              | False                                          | Toggles the parton-level analysis. |
| `--particle`            | False                                          | Toggles the particle-level analysis. |
| `--no-reco`             | False                                          | Toggles off the detector-level analysis. |
| `--no-systematics`      | False                                          | Toggles off the computation of systematics. |
| `--no-filter`           | False                                          | Save all events regardless of analysis filters (still saves the decision). |

In AthAnalysis you also get some additional useful options (you can use these in most athena jobs):

| **Option** | **Default** | **Usage**                                                                                                                                                                                                                                                                                                                                      |
| ---------- | ----------- |------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
|`--skipEvents` | 0 | Skip over the first N events                                                                                                                                                                                                                                                                                                                   |
| `--interactive` | None | Drops you into an interactive prompt that you can use to modify the job configuration in before the job launches. For example, you could type:<br>`>>> self.getEventAlgo("NTupleMaker").OutputLevel=2`<br> to turn on debugging output for the `NTupleMaker` algorithm.<br>When ready, type `exit()` (or hit Ctrl+D) to start the job running. |
|`--debug`<br>`-d` | None | Attach gdb debugger at initialization of job                                                                                                                                                                                                                                                                                                   |
|`--perfmon fullmonmt` | - | Enables performance monitoring statistics (memory usage, CPU usage etc) |


Here are a few use cases:
=== "Typical analysis"
    ```sh
    runTop_el.py -i inputs.txt -o output --parton --particle -a TopCPToolkit.MyCustomAnalysis
    ```
=== "Developer, no systs"
    ```sh
    runTop_el.py -i inputs.txt -o output --no-systematics -e 100
    ```