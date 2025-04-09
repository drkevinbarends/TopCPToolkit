To understand how to write a custom analysis configuration, it's instructive to first take a look at [`runTop_el.py`](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/bin/runTop_el.py).
There, we find calls such as
```python
from TopCPToolkit.commonAlgoConfig import makeRecoSequence
algSeq = makeRecoSequence(args.analysis, flags,
                          noSystematics=args.no_systematics,
                          noFilter=args.no_filter)
```
and similarly to `makeTruthSequence` and `makeParticleLevelSequence`.

These methods are defined in [commonAlgoConfig.py](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/python/commonAlgoConfig.py), and you can see that they perform the following actions:

- create `AlgSequence` and `ConfigSequence` objects, a (blank) chain of algorithms to be executed, and the `ConfigFactory` to help populate them;
- initialise the [`CommonServicesConfig`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AsgAnalysisAlgorithms/python/AsgAnalysisConfig.py), which
    - checks whether we are running on data: if we are, we do not need to consider systematics or truth information!
    - creates an instance of the `CP::SystematicsSvc`, which will record and handle all the systematics that we need, based on the objects that we deal with;
    - creates an instance of the `CP::SelectionNameSvc`, to handle cutflows;
- for truth-level analyses, set up the very first algorithm, the `CP::VertexSelectionAlg`, to retain only events with a reconstructed primary vertex;
- **load and configure the requested analysis algorithms**;
- add a final algorithm, the `CP::SysListDumperAlg`, helpful to monitor which systematics were collected.

It's the statement in bold that is of interest to us. There are **two possible approaches**: we can either define an *analysis module*, and equip it with the necessary `makeRecoConfiguration` (equivalently, `makeTruthConfiguration` or `makeParticleLevelConfiguration`) method, or we can go for a *text-based config*.

## Analysis modules

Analysis modules are python files whose role is to define a `makeRecoConfiguration` method that can be overloaded by [commonAlgoConfig.py](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/python/commonAlgoConfig.py).
A good place to start, is the [example analysis module](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/python/TtbarCPalgoConfigBlocksAnalysis.py).
Schematically, it looks like this:
```python
from AnalysisAlgorithmsConfig.ConfigSequence import ConfigSequence
from AnalysisAlgorithmsConfig.ConfigAccumulator import ConfigAccumulator

def makeRecoConfiguration(flags, algSeq, configSeq, noFilter=False):

    # ... everything you need goes here ...

    configAccumulator = ConfigAccumulator(dataType, algSeq)
    configSeq.fullConfigure(configAccumulator)

    return
```

### Understanding the skeleton

The first import allow us to define a `ConfigSequence` object, which we will populate with subsequent calls to the `ConfigFactory` (via `makeConfig`).
For instance, when we we request to calibrate a new electron collection "AnaElectrons":
```python
configSeq += makeConfig ('Electrons', containerName='AnaElectrons')
```
This particular config block is defined in [ElectronAnalyisConfig.py](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/EgammaAnalysisAlgorithms/python/ElectronAnalysisConfig.py), and sets up the various algorithms necessary to reconstructing and calibrating electrons: a $p_\mathrm{T}$/$\eta$ selection, a track selection, an Egamma quality cut, the `ElectronCalibrationAndSmearingAlg` itself, etc.

The second import creates an instance of `ConfigAccumulator`, which (as the name suggests) will read the chain of config blocks and accumulate it into a sequence of algorithms.
It communicates all the relevant metadata between blocks during the configuration, and keeps track of all the input and output containers needed at each stage.

### Creating the output ntuple

A very special config block is set up at the very end of our sequence: the [OutputAnalysisConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AsgAnalysisAlgorithms/python/OutputAnalysisConfig.py).
It is responsible for creating the tree we will save, with all the branches we've declared in our successive config blocks, and eventually for filling said tree.
It is set up as follows:
```python
configSeq += makeConfig ('Output')
configSeq.setOptionValue ('.treeName', 'reco')
configSeq.setOptionValue ('.vars', reco_branches)
configSeq.setOptionValue ('.metVars', met_branches)
configSeq.setOptionValue ('.containers', outputContainers)
```

While `treeName` is straightforwardly the desired name of the output tree, some explanations are needed for the three other options.

- `'vars'`: while output branches should be declared within each config block, we may want to store extra variables, by passing a list of mappings of the form `'Object.inputname -> outputname'`.

    - Any variable that is an aux decoration of an xAOD object or container can be stored. If the object is a container, e.g. a jet container, the variable stored is a vector of elements corresponding to the objects in the container. For example,
    `'OutJets_%SYS%.EMFrac -> jet_EMFrac_%SYS%'` will create a vector in the output, where each element is the fraction of energy in the EM calorimeter of the jet in the OutJets container. The `'_%SYS%'` suffix ensures that systematic variation are stored for this variable.
    - Not all decorations are associated to a systematic variation. In this case, the `'_NOSYS'` suffix should be used. For example, truth decorations about the muons can be saved as `'OutMuons_NOSYS.truthOrigin -> mu_truthOrigin'`.
    - For single objects, such as EventInfo, the variable itself is stored directly. Example: `'EventInfo.beamPosX -> beamPosX'` will directly store a flat variable containing the beamspot position X coordinate.

- `'metVars'`: same as above, but specifically for the MET container, which contains different levels of MET. By default, the ["Final" MET term](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AsgAnalysisAlgorithms/AsgAnalysisAlgorithms/AsgxAODMetNTupleMakerAlg.h) is selected.
- `'containers'`: a dictionary that maps the object containers we want to write out to some prefix used in naming branches. For instance, `{'jet_':'OutJets'}` will result in all the variables we save from the `OutJets` container being saved as `jet_something`.

!!! tip
    Event-level variables (e.g. reconstructed resonances, object multiplicities, MVA discriminants, etc.) should be decorated onto the `'EventInfo'` container.

### Running ML inference

In case one wishes to run ML inference on trained models and store the model predictions to the output ntuple, a config block can be added to the algorithm sequence of your analysis module. One example can be founded in [TopSpaNetConfig.py](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/python/TopSpaNetConfig.py). This config block loads an algorithm `RunSpaNetAlg` defined in [RunSpaNetAlg.h](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/TopCPToolkit/RunSpaNetAlg.h). To run ML inference on custom models, a similar algorithm e.g. `RunCustomNNAlg` need to be added. This algorithm is responsible for:

* applying event and object selections
* computing input tensors for the models from selected objects
* loading models and running inference
* retrieving the model outputs and storing them into ntuple branches.

The model inference is done using [ONNX runtime](https://onnxruntime.ai/) C++ interface. Models trained in any framework are expected to be already converted to the ONNX format. A [`top::ONNXWrapper`](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/TopCPToolkit/ONNXWrapper.h) class is provided that hides most of the ONNX runtime API calls behind more convenient interfaces. The user algorithm `RunCustomNNAlg` needs to contain an instance of `top::ONNXWrapper` (or an instance of a class that inherits from `top::ONNXWrapper`) for the purpose of loading models, setting inputs, running inference, and retrieving outputs. Below is a code skeleton that shows an example of using the `top::ONNXWrapper` interface for running ML inference.

??? "Show me the code"
    ```cpp
    namespace top {

        class RunCustomNNAlg : public EL::AnaAlgorithm {

            // An ONNXWrapper object
            std::unique_ptr<ONNXWrapper> m_NN;

            // Input handlers
            CP::SysReadHandle<xAOD::JetContainer> m_jetsHandle {
                this, "jets", "", "the jet container to use"
            };
            CP::SysReadSelectionHandle m_jetSelection {
                this, "jetSelection", "", "the selection on the input jets"
            };

            ...

            // Output handlers
            CP::SysWriteDecorHandle<float> m_model_pred0 {
                this, "my_model_pred1", "my_model_pred1_%SYS%", "One of my model predictions"
            };

            ...

            // Systematics
            CP::SysListHandle m_systematicsList {this};

            // Execute
            StatusCode execute_syst(const CP::SystematicSet &sys) {

                ...

                // Retrieve objects from containers and apply selections
                const xAOD::JetContainer *jets = nullptr;
                ANA_CHECK(m_jetsHandle.retrieve(jets, sys));

                ConstDataVector<xAOD::JetContainer> selected_jets(SG::VIEW_ELEMENTS);
                for (const xAOD::Jet *jet : *jets) {
                    if (m_jetSelection.getBool(*jet, sys))
                        selected_jets.push_back(jet);
                }

                ...

                // Compute inputs from the selected objects
                std::vector<float> input_vec = compute_inputs_from(selected_jets, ...);

                // Define input shapes
                std::vector<int64_t> input_vec = {LENGTH, WIDTH, ...};

                // Add inputs and convert to tensors via the ONNXWrapper interface
                m_NN -> clearInputs();
                m_NN -> addInputs(input_vec, input_dims);

                // Pick the right model based on eventNumber
                // This is for avoiding evaluating the same events that are used for training.
                // For example, the model trained using events with odd event numbers should
                // be used for inference only on events with even event numbers.
                unsigned imodel = getSessionIndex(eventNumber);

                // run inference
                m_NN -> evaluate(imodel);

                // get model predictions
                float* out_p = m_NN -> getOutputs<float>("my_model_output_name");

                // write output to ntuple
                m_model_pred0 = out_p[0];

            }

        public:

            RunCustomNNAlg(const std::string &name, ISvcLocator *pSvcLocator) {
                // do constructor stuff
            }

            virtual StatusCode initialize() override {

                // initialize inputs
                ANA_CHECK(m_jetsHandle.initialize(m_systematicsList));
                ANA_CHECK(m_jetSelection.initialize(m_systematicsList, m_jetsHandle, SG::AllowEmpty));

                ...

                // initialize outputs
                ANA_CHECK(m_model_pred0.initialize(m_systematicsList, m_eventInfoHandle));

                ...

                // initialize and load models
                m_NN = std::make_unique<ONNXWrapper>("MyFavouriteNN", <filepahts_to_trained_models.onnx>, ...);

                // Other initialization and configuration
                ...

                return StatusCode::SUCCESS;
            }

            virtual StatusCode execute() override {
                for (const auto &sys : m_systematicsList.systematicsVector()) {
                    ANA_CHECK(execute_syst(sys));
                }
                return StatusCode::SUCCESS;
            }

            virtual StatusCode finalize() override {
                return StatusCode::SUCCESS;
            }

        };
    } // namespace top
    ```

## Text-based (YAML) configs

While the YAML config to schedule algorithms is a seemingly completely different approach, it's just another level of abstraction: CP tools are set up by algorithms, which are configured in blocks, and registering these blocks with the [`TextConfig`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AnalysisAlgorithmsConfig/python/ConfigText.py) object allows to manipulate them in the YAML file. Therefore, everything we've discussed up until this point for config blocks and analysis modules, still applies.

### Registering new blocks directly in the YAML config (since AnalysisBase 24.2.40)

Custom config blocks created by the user can be registered directly in the YAML config, such that they are recognised by the `ConfigAccumulator`.
This does not require modifying the TopCPToolkit code, unlike the approach using the python interface described below.

Suppose we have a package named `MyPackage` (this could be e.g. TopCPToolkit itself) with the `MyAnalysisConfigModule.py` file declaring a new block class `MyAnalysisConfig`.
The registration of the config block is done using the `AddConfigBlocks` block in YAML:

```yaml
AddConfigBlocks:
  - modulePath: 'MyPackage.MyAnalysisConfigModule'
    functionName: 'MyAnalysisConfig'
    algName: 'ThisAnalysis'
    pos: 'Output'
```

The argument of `AddConfigBlocks` is a list, so that any number of custom blocks can be registered.
The blocks can be subsequently used in the YAML config, as specified by the `algName`.
In our concrete example, we would configure the block in YAML using the `ThisAnalysis` keyword.
The `pos` argument specifies where in the chain to place this block: here, before the `Output` block.
This is the appropriate location for most analysis algorithms, which should be run after anything related to object calibration, working points, overlap removal, etc.

### Registering new blocks in the python interface

In [commonAlgoConfig.py](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/blob/main/source/TopCPToolkit/python/commonAlgoConfig.py), we define the `makeTextBasedSequence` function, which will ultimately set up an instance of `ConfigAccumulator` as before. Crucially, we also declare all possible config blocks: those defined by default in the `addDefaultAlgs` method of the [`ConfigFactory`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AnalysisAlgorithmsConfig/python/ConfigFactory.py), and adding manually those blocks defined only in TopCPToolkit. Below is an example for the NNLO $t\bar{t}$ reweighting algorithm:

```python
from TopCPToolkit.TtbarNNLORecursiveRewConfig import TtbarNNLORecursiveRewConfig
config.addAlgConfigBlock(algName='TtbarNNLO',
                         alg=TtbarNNLORecursiveRewConfig, pos='Output')
```

In this example, we match the `TtbarNNLORecursiveRewConfig` config block with the name `TtbarNNLO`, which is how we'll refer to that block in the YAML config. The `pos='Output'` argument is a technical requirement, to make sure that this custom block is inserted in the list of all possible blocks *before* the `Output` block, as that one needs to know about all other blocks in order to correctly write the output branches to file.

A similar operation allows us to define e.g. the common services and PMG weights (in [`addDefaultAlgs`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AnalysisAlgorithmsConfig/python/ConfigFactory.py)), as well as the truth parton histories. We can then write our example parton-level YAML config as:
```yaml
CommonServices: {}

GeneratorLevelAnalysis: {}

PartonHistory:
  - histories: 'Ttbar.TtbarLight'

TtbarNNLO: {}

# After configuring each container, many variables will be saved automatically.
Output:
  treeName: 'truth'
  vars:
    - 'EventInfo.mcChannelNumber -> mcChannelNumber'
    - 'EventInfo.runNumber -> runNumber'
    - 'EventInfo.eventNumber -> eventNumber'
  containers:
      # Format should follow: '<suffix>:<output container>'
      '': 'EventInfo'
      'Ttbar_': 'TopPartonHistoryTtbar'
      'TtbarLight_': 'TopPartonHistoryTtbarLight'
```

### A few pointers for writing a YAML config

First off, and most obviously, make yourself familiar with the [YAML language](https://www.cloudbees.com/blog/yaml-tutorial-everything-you-need-get-started). The text config we are writing is essentially a big nested directory, with strings and lists of strings.

The minimal setup for your config must include the blocks `CommonServices` and `Output`.
In some cases, you may be able to rely entirely on the default settings for these config blocks.
This is done by passing an empty dictionary:
```yaml
GeneratorLevelAnalysis: {}
```

- Before a block can be called, it has to be declared (see previous section).
- A block that is not explicitly called in the config will not be set up.
- Blocks are set up with their default arguments, unless otherwise specified: this is what we write in our YAML config, an overload of the default arguments.

With these simple rules in mind, you are ready to write your own config!

!!! tip "Expert mode"
    A subtlety in the algorithm sequence that will be generated, is that it depends not on the order in which you call the blocks in the config, but rather on the **order in which their defaults are set up**. This is the purpose of the `pos` argument described earlier. We suggest to use `pos='Output'`, i.e. always inserting your new blocks before the `Output` one, as a generic solution. Of course, if you introduce two new blocks, the output of one is needed to configure the other, you must ensure that the ordering is correct!

### Example YAML configs

Example configs are available at [`share/configs/`](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/tree/main/source/TopCPToolkit/share/configs/), and real analyses can be archived there as well.
They should be arranged as up to three config files (`reco.yaml`, `particle.yaml` and `parton.yaml`) within a single subdirectory, with an explicit enough name.

We currently have:

- [`exampleTtbarLjets`](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/tree/main/source/TopCPToolkit/share/configs/exampleTtbarLjets): an example $t\bar{t}\to\ell$+jets analysis, only for demonstration purposes.
- [`tutorial`](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/tree/main/source/TopCPToolkit/share/configs/tutorial): used for the TopCPToolkit tutorial, only for demonstration purposes.
- [`bTaggerCalibration`](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/tree/main/source/TopCPToolkit/share/configs/bTaggerCalibration): used by the FTAG group to run simple ntuple productions for calibration.