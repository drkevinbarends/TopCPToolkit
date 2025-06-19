!!! abstract "In this session you will..."
    - understand the differences between a CP tool and a CP algorithm
    - write a simple algorithm to perform a piece of analysis
    - run inference using trained machine learning models
    - create new output variables (both event-wise and object-wise)
    - make your algorithm configurable in YAML and useable by others

_For this part, we assume you're already familiar with the contents of ["Your first config"](write_config.md) and have followed the ["Initial setup"](setup.md) instructions._

## The necessary basics

We have alluded to CP tools and algorithms in many places so far, let's try to clarify these concepts a little bit: what exactly are we talking about?

!!! tip
    The [ATLAS Software tutorials](https://atlassoftwaredocs.web.cern.ch/analysis-software/AnalysisSWTutorial/cpalg_intro/) are also very relevant to this session!

    If you already feel confident with these topics, you may skip this section :student:

### CP tools

First and foremost: "CP" stands for "Combined Performance", and refers to the centralised and essential aspects of any ATLAS analysis such as object reconstruction and calibration.
In ATLAS, there are CP groups for electrons/photons, muons, taus, jets/missing energy, etc.
The relevant tools they provide are the official "CP tools".

!!! note
    Technically, "CP tools" all derive in a more or less complicated way from the [`AsgTool`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/Control/AthToolSupport/AsgTools/AsgTools/AsgTool.h) class.

Let's take a look at one example of a CP Tool: the [`EgammaCalibrationAndSmearingTool`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/ElectronPhotonID/ElectronPhotonFourMomentumCorrection/Root/EgammaCalibrationAndSmearingTool.cxx), which (unsurprisingly) is used to calibrate and smear "Egamma" objects (i.e. electrons and photons).

We want to highlight the following features:

- the tool exposes a number of **properties** (here with `declareProperty`)
- the tool has an `initialize()` method and a `finalize()` method, to be run before and after the event loop
- the tool can have as many other methods as necessary, and in this particular case we find two interesting ones:
    - `setupSystematics()`: **the tool generates its own relevant list of systematics!**
    - `applyCorrection(xAOD::Egamma & input)`: this is how the tool will act on each individual electron (to calibrate it)

!!! note
    Depending on the purpose of the tool, it may act on individual objects or entire collections. It can even run solely on `EventInfo`. The tool doesn't know anything about the "event".

### CP algorithms

Since CP tools need to be configured in specific ways and applied onto relevant objects, we need something to take care of that: it's exactly what algorithms are for.

Let's take the example of electron calibration again: we want to use the latest recommendations from the Egamma CP group and calibrate our electron momenta.
Not only that, we know that there are uncertainties related to this calibration that we need to take into account in our analysis, so we want to obtain not just one calibrated collection of electrons for each event, but a series of them, corresponding to these systematically-varied calibration options.
This is what the [`EgammaCalibrationAndSmearingAlg`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/EgammaAnalysisAlgorithms/Root/EgammaCalibrationAndSmearingAlg.cxx) does.

Let's inspect the (much shorter!) code here:

- the algorithm can also **expose configurable properties** (here again with `declareProperty`)
- the algorithm also has `initialize()` and `finalize()` methods, and a new `execute()` one
    - in the `initialize()` step, we recognise that an instance of the calibration tool is being initialised itself. We also see a `m_systematicsList` object be initialised.
    - the `finalize()` method here doesn't need to do anything, so it's actually omitted (this is often, but not always, the case!)
    - in `execute()`, we loop over all relevant systematics, retrieve the electron container, apply the systematic variation in our tool and use it to calibrate the electrons.

You will find very similar flows in all the CP algorithms.
There is still a bit of a mystery remaining... how does the algorithm know which input/output containers to get, and what is this `m_systematicsList`?
The answer to both lies in a third concept we need to discuss.

### How to connect everything together: handles

Much like for real tools, it's a useful idea to give our CP tools and object containers various types of "handles" such that they know what to run on.
Handles in this context are "proxy objects" - we can use them to define tools and containers without having to specify exactly which tool or container to read in or write out.
This is left for later, when we have a full configuration of our analysis sequence in place.

Let's look at the [header file](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/EgammaAnalysisAlgorithms/EgammaAnalysisAlgorithms/EgammaCalibrationAndSmearingAlg.h) for our electron calibration algorithm.
We find the following declarations:

```cpp
SysCopyHandle<xAOD::EgammaContainer> m_egammaHandle {
    this, "egammas", "Electrons", "the egamma collection to run on"};
```

This particular handle is used to generate systematicall-varied copies of `xAOD::EgammaContainer` - exactly what we need to define our newly calibrated electron collections!
It can be configured via a property named `egammas`, which is by default set to "Electrons" (which is indeed the name we use in DAOD PHYS).
The last part is obviously an info-string.

```cpp
ToolHandle<CP::IEgammaCalibrationAndSmearingTool> m_calibrationAndSmearingTool;
```

This is a much more succinct type of handle, used to pass a tool (here the calibration tool itself).

```cpp
SysListHandle m_systematicsList {this};
```

This final and very particular handle registers a list of systematics.

Let's now go back to the [main code of the algorithm](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/EgammaAnalysisAlgorithms/Root/EgammaCalibrationAndSmearingAlg.cxx) and explain the "mystery behaviours" we anticipated earlier.

```cpp
ANA_CHECK (m_egammaHandle.initialize (m_systematicsList));
```

Here we take the electron handle and initialise it to be connected to the "Electrons" container in the DAOD.
In case that container came with systematics of its own (here it doesn't), these systematics are added to the handle.

```cpp
ANA_CHECK (m_systematicsList.addSystematics (*m_calibrationAndSmearingTool));
```

Now we fill the systematics handle with the systematics that are being generated by the calibration tool.
At this point, we have all the systematics that we need, so we lock them in the handle:

```cpp
ANA_CHECK (m_systematicsList.initialize());
```

The loop over systematics that takes place in `execute()` is then **exclusively over the relevant systematics** (that we have just registered!).
We use our electron handle to retrieve the proper container for each systematic:

```cpp
xAOD::EgammaContainer *egammas = nullptr;
ANA_CHECK (m_egammaHandle.getCopy (egammas, sys));
```

### CP algorithms: the flow

We will soon attempt to write our own CP algorithm, so let's summarise the key points:

- we can set configurable properties to drive the behaviour of the algorithm
- we can pass any tool we need
- we use handles to read in and write out any container
- this allows us to automatically deal with systematics without ever having to know what they are

### A list of useful handles

The following handles are commonly used in CP and analysis algorithms.
We'll use some of them ourselves in this tutorial!
For container handles, any `xAOD::` container can be used; for decoration handles, all the standard types are supported (even `PtEtaPhiMVector` and `std::vector`!).
It is also possible to make arrays of handles, to be configured by the user.

- [`SysListHandle`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/SystematicsHandles/SystematicsHandles/SysListHandle.h): used exclusively for the systematics list
- [`SysCopyHandle`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/SystematicsHandles/SystematicsHandles/SysCopyHandle.h): make systematically-varied copies of an input collection (used in CP cases for calibration, rarely used in analysis)
- [`SysReadHandle`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/SystematicsHandles/SystematicsHandles/SysReadHandle.h): retrieve systematically-varied collections (that's the one you want for analysis!)
- [`SysWriteHandle`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/SystematicsHandles/SystematicsHandles/SysWriteHandle.h): register a new systematically-varied collection (rarely used in analysis)
- [`SysReadSelectionHandle`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/SelectionHelpers/SelectionHelpers/SysReadSelectionHandle.h): read an input systematically-varied selection cut
- [`SysWriteSelectionHandle`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/SelectionHelpers/SelectionHelpers/SysWriteSelectionHandle.h): register your new systematically-varied selection cut
- [`SysReadDecorHandle`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/SystematicsHandles/SystematicsHandles/SysReadDecorHandle.h): read an input systematically-varied decoration
- [`SysWriteDecorHandle`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/SystematicsHandles/SystematicsHandles/SysWriteDecorHandle.h): register your new systematically-varied decoration

!!! note
    We use **_selections_** to be able to **filter out events and objects**. We use **_decorations_** to store additional information that can either be retrieved in another algorithm, or **saved as a branch** in the output ntuple.

## Writing an algorithm

We are now ready to turn to a realistic analysis case.
In the [previous part](../tutorials/write_config.md) of the tutorial, we had set up a YAML config to run on a $t\bar{t}Z$ sample (under Run 3 conditions) and to apply a simple $4\ell$ event selection.
Our task will now be to **write a custom algorithm to compute two quantities** of interest for our imaginary analysis.
These will be:

- the azimuthal separation, $\Delta\phi$, between each jet and the missing energy vector

!!! note
    The quantity $\Delta\phi(j_i, \vec{p_\mathrm{T}^\text{miss}})$ is defined for each jet $j_i$ in the event, the number of which varies from event to event.
    It's an example of an _object-wise variable_.

- the machine learning discriminator $\mathcal{D}$ that aims to distinguish the signal process $t\bar{t}Z$ and the major background $ZZ$.

!!! note
    The dummy machine learning models were trained with three input features: transverse momentum of the leading jet $p_T^{j_1}$, transverse momentum of the leading electron $p_T^{e_1}$, and transverse momentum of the leading muon $p_T^{\mu_1}$. The models are converted to the ONNX format and can be found in the tutorial config directory `share/configs/tutorial2025/` of the HowToExtendTopCPToolkit package: `model_even.onnx` (trained using events with even eventNumber) and `model_odd.onnx` (trained using events with odd eventNumber).
    We will compute the necessary inputs, run inference with the help of [`top::ONNXWrapper`](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/TopCPToolkit/ONNXWrapper.h?ref_type=heads), and store the model prediction to our output. This is an example of an _event-wise variable_: one that we will attach to the `EventInfo` object.

### How to make a new algorithm compile

We will start from the skeleton structure provided in these two files in HowToExtendTopCPToolkit: [Root/TutorialAlg.cxx](https://gitlab.cern.ch/atlasphys-top/reco/HowToExtendTopCPToolkit/-/blob/tutorial2025/Root/TutorialAlg.cxx) and its header [HowToExtendTopCPToolkit/TutorialAlg.h](https://gitlab.cern.ch/atlasphys-top/reco/HowToExtendTopCPToolkit/-/blob/tutorial2025/HowToExtendTopCPToolkit/TutorialAlg.h).

!!! example "Exercise"
    What class do these files define? What does it currently do?

??? success "Solution"
    They define the class `toptutorial::TutorialAlg` (notice the scope, to differentiate from CP algorithms).
    Currently, it only initialises the list of systematics and prints an info message at the start.
    There is a trivial `finalize()` method implemented, which does nothing at the moment but can be extended for future use cases if needed.

We already compiled TopCPToolkit successfully, but we also need to register this `TutorialAlg` as an algorithm in a few special places.
These are:

- [source/HowToExtendTopCPToolkit/HowToExtendTopCPToolkit/HowToExtendTopCPToolkitDict.h](https://gitlab.cern.ch/atlasphys-top/reco/HowToExtendTopCPToolkit/-/blob/tutorial2025/HowToExtendTopCPToolkit/HowToExtendTopCPToolkitDict.h)
- [source/HowToExtendTopCPToolkit/HowToExtendTopCPToolkit/selection.xml](https://gitlab.cern.ch/atlasphys-top/reco/HowToExtendTopCPToolkit/-/blob/tutorial2025/HowToExtendTopCPToolkit/selection.xml)
- [source/HowToExtendTopCPToolkit/src/components/HowToExtendTopCPToolkit_entries.cxx](https://gitlab.cern.ch/atlasphys-top/reco/HowToExtendTopCPToolkit/-/blob/tutorial2025/src/components/HowToExtendTopCPToolkit_entries.cxx)

!!! example "Exercise"
    Inspect these 3 files and insert the necessary lines.

??? success "Solution"
    In `HowToExtendTopCPToolkitDict.h`, you need to add:
    ```cpp
    #include "HowToExtendTopCPToolkit/TutorialAlg.h"
    ```
    In `selection.xml`:
    ```xml
    <class name="toptutorial::TutorialAlg" />
    ```
    In `HowToExtendTopCPToolkit_entries.cxx`:
    ```cpp
    #include <HowToExtendTopCPToolkit/TutorialAlg.h>

    DECLARE_COMPONENT(toptutorial::TutorialAlg)
    ```

Let's now run a quick recompilation:
```sh
cd build
cmake ../source
make -j8
source */setup.sh
```

### Preparing the inputs

Recall that we will require the following ingredients for our calculation: leptons, missing energy, jets.
These should be entirely configurable by the users, including their selections. And we should take into account systematics.
We further want to decorate two new variables, for which we will also have systematics.

!!! example "Exercise"
    What kind of object do we need to invoke here? Make a precise list.

??? success "Solution"
    We need the following [systematic handles](#a-list-of-useful-handles):

    - a `SysReadHandle` for each type of object: `xAOD::ElectronContainer`, `xAOD::MuonContainer`, `xAOD::JetContainer`, `xAOD::METContainer`, `xAOD::EventInfo`.
    - a `SysReadSelectionHandle` for each type of object `xAOD::ElectronContainer`, `xAOD::MuonContainer`, `xAOD::JetContainer`.
    - a `SysWriteDecorHandle` for all the variables we want to compute and decorate.

!!! example "Exercise"
    Now add them to the header file for our algorithm.
    Make sure to use the following names for properties: "electrons", "muons", "jets", "met", "eventInfo", "electronSelection", "muonSelection", "muonSelection", "dphimet", "myNNOutput".
    Don't set any defaults for the container names, except "EventInfo".
    For the variable decoration names, don't forget to make the systematics-dependence explicit.

??? tip "Hint"
    The format for declaring handles is:
    ```cpp
    CP::SomeHandle<type> m_handle {
      this, "propertyName", "defaultValue", "brief description of the handle"
    };
    ```

??? success "Solution"
    Add the following to `TutorialAlg.h`:
    ```cpp
    private:

      // EventInfo
      CP::SysReadHandle<xAOD::EventInfo> m_eventInfoHandle {
        this, "eventInfo", "EventInfo", "the input EventInfo container"
      };
      // Electrons
      CP::SysReadHandle<xAOD::ElectronContainer> m_electronsHandle {
        this, "electrons", "", "the input electron container"
      };
      CP::SysReadSelectionHandle m_electronSelection{
        this, "electronSelection", "", "the selection on the input electrons."
      };
      // Muons
      CP::SysReadHandle<xAOD::MuonContainer> m_muonsHandle {
        this, "muons", "", "the input muon container"
      };
      CP::SysReadSelectionHandle m_muonSelection{
        this, "muonSelection", "", "the selection on the input muons."
      };
      // Jets
      CP::SysReadHandle<xAOD::JetContainer> m_jetsHandle {
        this, "jets", "", "the input jet container"
      };
      CP::SysReadSelectionHandle m_jetSelection{
        this, "jetSelection", "", "the selection on the input jets."
      };
      // MET
      CP::SysReadHandle<xAOD::MissingETContainer> m_metHandle {
        this, "met", "", "the input MET container"
      };

      // Handles that register new systematically-varied decorations
      CP::SysWriteDecorHandle<float> m_dphimetHandle{
        this, "dphimet", "dphimet_%SYS%",
        "decoration name for DeltaPhi between each jet and the MET"
      };
      CP::SysWriteDecorHandle<float> m_nnOutputHandle {
        this, "myNNOutput", "myNNOutput_%SYS%", "decoration name for the neural network output"
      };
    ```

### Initialising the algorithm

We now turn to the source code, in `TutorialAlg.cxx`.
Having declared all our input and output handles, we need to make sure they are properly initialised.

!!! example "Exercise"
    Add calls to initialise each handle within in the `TutorialAlg::initialize()` method.
    Make sure that the list of systematics is treated last!

??? tip "Hint"
    For handles that only depend on systematics:
    ```cpp
    m_handle.initialize(m_systematicsList);
    ```
    For handles that also depend on some other handle:
    ```cpp
    m_handle.initialize(m_systematicsList, m_otherHandle);
    ```
    For handles that are optional (e.g. object selections):
    ```cpp
    m_handle.initialize(m_systematicsList, m_otherHandle, SG::AllowEmpty);
    ```

??? success "Solution"
    In the `TutorialAlg::initialize()` method, and before the line with `m_systematicsList.initialize()`, add:
      ```cpp
      ANA_CHECK(m_electronsHandle.initialize(m_systematicsList));
      ANA_CHECK(m_electronSelection.initialize(m_systematicsList, m_electronsHandle, SG::AllowEmpty));

      ANA_CHECK(m_muonsHandle.initialize(m_systematicsList));
      ANA_CHECK(m_muonSelection.initialize(m_systematicsList, m_muonsHandle, SG::AllowEmpty));

      ANA_CHECK(m_jetsHandle.initialize(m_systematicsList));
      ANA_CHECK(m_jetSelection.initialize(m_systematicsList, m_jetsHandle, SG::AllowEmpty));

      ANA_CHECK(m_metHandle.initialize(m_systematicsList));

      ANA_CHECK(m_eventInfoHandle.initialize(m_systematicsList));

      ANA_CHECK(m_dphimetHandle.initialize(m_systematicsList, m_jetsHandle));
      ANA_CHECK(m_nnOutputHandle.initialize(m_systematicsList, m_eventInfoHandle));
      ```

### Coding the physics

Let's now focus on the `TutorialAlg::execute()` method, which will run on every event.
The loop over all relevant systematics is already set up for us, so let's comment it out and fill it with some actual physics calculations!

!!! example "Exercise"
    We need to start by retrieving the containers corresponding to each systematic.
    For that we can use the `m_handle.retrieve(container, sys)` syntax.
    Let's call the containers `electrons`, `muons`, `jets`, `met` and `evtInfo` - pretty standard notation.

??? success "Solution"
    Uncomment the loop over `m_systematicsList.systematicsVector()`, and fill it with
    ```cpp
    const xAOD::EventInfo *evtInfo = nullptr;
    const xAOD::ElectronContainer *electrons = nullptr;
    const xAOD::MuonContainer *muons = nullptr;
    const xAOD::JetContainer *jets = nullptr;
    const xAOD::MissingETContainer *met = nullptr;

    ANA_CHECK(m_eventInfoHandle.retrieve(evtInfo, sys));
    ANA_CHECK(m_electronsHandle.retrieve(electrons, sys));
    ANA_CHECK(m_muonsHandle.retrieve(muons, sys));
    ANA_CHECK(m_jetsHandle.retrieve(jets, sys));
    ANA_CHECK(m_metHandle.retrieve(met, sys));
    ```

!!! example "Exercise"
    We also need to select only the objects that pass object selections. We can retrieve the information on whether an object passes the selection via the relevant selection handle: `m_handleSelection.getBool(*object, sys)`. Let's loop over each of the electron, muon, and jet containers and pick out only the ones that pass the selections.

??? tip "Hint"
    We can collect the selected objects temporarily in a container of type [`ConstDataVector<T>`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/Control/AthContainers/AthContainers/ConstDataVector.h). In our case, `T` can be `xAOD::ElectronContainer`, `xAOD::MuonContainer`, or `xAOD::JetContainer`. More information about this transient only container type can be found in the [ATLAS software tutorial](https://atlassoftwaredocs.web.cern.ch/analysis-software/AnalysisSWTutorial/basic_xaod_modify/#constdatavector-advancedoptional).

??? success "Solution"
    Add the lines below after where we retrieved the containers
    ```cpp
    // Apply object selections
    ConstDataVector<xAOD::ElectronContainer> selected_electrons(SG::VIEW_ELEMENTS);
    for (const xAOD::Electron *el : *electrons) {
      if (m_electronSelection.getBool(*el, sys))
        selected_electrons.push_back(el);
    }

    ConstDataVector<xAOD::MuonContainer> selected_muons(SG::VIEW_ELEMENTS);
    for (const xAOD::Muon *mu : *muons) {
      if (m_muonSelection.getBool(*mu, sys))
        selected_muons.push_back(mu);
    }

    ConstDataVector<xAOD::JetContainer> selected_jets(SG::VIEW_ELEMENTS);
    for (const xAOD::Jet *jet : *jets) {
      if (m_jetSelection.getBool(*jet, sys))
        selected_jets.push_back(jet);
    }
    ```

To compute the model input variables $p_T^{j_1}$, $p_T^{e_1}$, and $p_T^{\mu_1}$, we can either sort the selected electron, muon, and jet containers by $p_T$ and take the first element, or do something very simple: loop over the container and pick the largest $p_T$.

We also need to collect the missing transverse momentum magnitude and its azimuthal angle from the MET container.
Note that the MET container in fact contains multiple objects, the various MET terms, and we need to access by name the one called "Final".

!!! example "Exercise"
    Loop over the selected electron and muon containers to determine the highest $p_T$ values `leading_electron_pt` and `leading_muon_pt`, respectively.
    Obtain the MET components `et_miss` and `phi_miss` from the appropriate MET object.

??? success "Solution"
    Still in the loop over systematics, and after the code we've just added, write:
    ```cpp
    float leading_electron_pt = -1;
    for (const xAOD::Electron *el : selected_electrons) {
      if (el->pt() > leading_electron_pt) {
        leading_electron_pt = el->pt();
      }
    }

    float leading_muon_pt = -1;
    for (const xAOD::Muon *mu : selected_muons) {
      if (mu->pt() > leading_muon_pt) {
        leading_muon_pt = mu->pt();
      }
    }

    float et_miss = (*met)["Final"]->met();
    float phi_miss = (*met)["Final"]->phi();
    ```

!!! note
    For simplicity, we are here ignoring the case where there are no electrons or muons in the event.
    This is where analysis complexity builds up and you need to be careful about manipulating your object collections and may consider splitting the events into multiple regions based on e.g. lepton flavour and charge.
    In a moment, we'll however try to add some protection to our algorithm by simply setting the default value of leading $p_T$ to -1, so that it can at least run safely on every event.

Now onto the jet container, we still need to determine the highest jet $p_T$ and also compute the per-jet observable $\Delta\phi$ between jet and MET.

!!! example "Exercise"
    Loop over the jet container to determine the largest jet $p_T$ `leading_jet_pt`.
    For every jet $j_i$ in the event, compute $\Delta\phi(j_i, \vec{p_\mathrm{T}^\text{miss}})$ and decorate the jet with it.

??? success "Solution"
    After the code we just wrote, add:
    ```cpp
    float leading_jet_pt = -1;
    for (const xAOD::Jet *jet : *jets) {
      float dphi_jet_met = TVector2::Phi_mpi_pi(jet->phi() - phi_miss);
      m_dphimetHandle.set(*jet, dphi_jet_met, sys);

      if (jet->pt() > leading_jet_pt) {
        leading_jet_pt = jet->pt();
      }
    }
    ```

At this point, we have all the input variables for the machine learning models and are ready to run inference.
We will do that with the help of [`top::ONNXWrapper`](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/TopCPToolkit/ONNXWrapper.h). It is a wrapper around the [onnxruntime](https://onnxruntime.ai/) APIs, providing a more convenient user interface to run ML inference.
We will add an instance of `top::ONNXWrapper` to our algorithm.

!!! example "Exercise"
    Declare an `top::ONNXWrapper` instance as a member of our `toptutorial::TutorialAlg` class, together with a vector of strings for storing filepaths to the trainned ONNX models (we will set the filepaths via the config block in the next section).
    Then construct the `top::ONNXWrapper` object using name "myNN" and the filepaths in the algorithm's `initialize()` method.

??? tip "Hint"
    See [here](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/7b614cd0e5b361c88c8d3b05216ab1fcaf706562/source/TopCPToolkit/TopCPToolkit/ONNXWrapper.h#L90) for the definition of `top::ONNXWrapper` constructor.
    We will need to include its header to our algorithm.

??? success "Solution"
    In the header file `TutorialAlg.h`, first include the header of `ONNXWrapper`
    ```
    #include "TopCPToolkit/ONNXWrapper.h"
    ```
    then add the declaration
    ```cpp
    private:
      std::unique_ptr<top::ONNXWrapper> m_NN;
      std::vector<std::string> m_model_paths;
    ```

    In the main code `TutorialAlg.cxx`, add to the `initalize()` method at the very end before `return`:
    ```cpp
    m_NN = std::make_unique<top::ONNXWrapper>("myNN", m_model_paths);
    ```

Next we need to pass the input variables to `ONNXWraper`. This is done via the method `ONNXWrapper::linkInputs`. This function can be called with different types of arguments (i.e. it is overloaded). Here we invoke the arguably the most convenient one that takes two vectors: one is a `std::vector<float>` containing the feature variables, and the other is a `std::vector<int64_t>` indicating the input tensor shape of the model.

!!! example "Exercise"
    Convert the features $p_T^{j_1}$, $p_T^{e_1}$, $p_T^{\mu_1}$ to the unit of GeV and add them to the model inputs.
    Call `ONNXWrapper::clearInputs()` to reset the internal tensors before adding new inputs.
    The trained model input tensor shape is (1, 3).

??? tip "Hint"
    Two member functions `ONNXWrapper::printInputInfo` and `ONNXWrapper::printOutputInfo` can be used to print information on the input and output nodes, respectively, including their shapes, types, and contents.

??? success "Solution"
    In `TutorialAlg.cxx`, add in the `execute()` function after the loop over the selected jets
    ```cpp
    std::vector<int64_t> input_shapes {1, 3}; // a tensor of shape (1, 3)
    std::vector<float> input_values = {leading_jet_pt/1000, leading_electron_pt/1000, leading_muon_pt/1000}; // Convert to GeV

    m_NN->clearInputs();
    m_NN->linkInputs(input_values, input_shapes);
    ```

Now we are finally ready to run inference and add the model prediction to the output. To avoid overtraining, we will apply the model trained using events with even eventNumber to events with odd eventNumber in inference, and vice versa.

!!! example "Exercise"
    Pick the model index based on the event number using `ONNXWrapper::getSessionIndex`.
    Run inference by calling `ONNXWrapper::evaluate` with the model index.
    Retrieve the output and decorate it to `evtInfo`.

??? tip "Hint"
    For retrieving outputs, the function `ONNXWrapper::getOutputs<T>(unsigned node)` can be called. Note that it returns a pointer of type `T*`. This is because the model output per node is an array representing a tensor. In our case, the model output has only one node with a tensor of shape (1).

??? success "Solution"
    In `TutorialAlg.cxx`, following the code we just added
    ```cpp
    // Pick the proper model based on the event number
    unsigned imodel = m_NN->getSessionIndex(evtInfo->eventNumber());

    // Run
    m_NN->evaluate(imodel);

    // Get the output
    float* pred = m_NN->getOutputs<float>(0);

    // Decorate the output handle
    m_nnOutputHandle.set(*evtInfo, *pred, sys);
    ```
As our final task here, let's add a small layer of protection to our algorithm: we will allow to pass an event-level selection.
This is not a perfect protection, but since we've already defined in our YAML config a $4\ell$ event selection, if we manage to pass it here, we at least guard ourselves from events with no leptons.

!!! example "Exercise"
    Add a new handle `m_preselection` with property name "eventPreselection" and no default value.
    It should be allowed to be empty also upon initialisation.
    Check that it has non-zero value (with the [`getBool`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/SelectionHelpers/SelectionHelpers/SysReadSelectionHandle.h#0073) method) after we've retrieved the `EventInfo` container, but before we attempt to run any computation.

??? success "Solution"
    In the header file `TutorialAlg.h`, add the declaration
    ```cpp
    private:
      CP::SysReadSelectionHandle m_preselection {
        this, "eventPreselection", "", "event preselection to check before running this algorithm"
      };
    ```
    In the main code `TutorialAlg.cxx`, add to the `initalize()` method, after `m_eventInfoHandle` is initialised, the line
    ```cpp
    ANA_CHECK(m_preselection.initialize(m_systematicsList, m_eventInfoHandle, SG::AllowEmpty));
    ```
    Finally, in the `execute()` method, after calling `m_eventInfoHandle.retrieve(...)` and before applying object selections, add the following:
    ```cpp
    if ( m_preselection && !m_preselection.getBool(*evtInfo, sys)) continue;
    ```

We are now faced with another problem, that would only reveal itself at runtime... :sweat:
When a specific event/systematic combination does not pass the event selection we've just implemented, our decorations are not set, and this is ill-defined for the ntuple writer!

We need to add default decorations for all our new variables.

!!! example "Exercise"
    Add a default decoration for machine learning discriminator $\mathcal{D}$.
    You may do the same for $\Delta\phi(j_i, \vec{p_\mathrm{T}^\text{miss}})$, or move the loop over jets before the preselection is called (but don't forget about moving the MET variables up too!).

??? success "Solution"
    Before the call to `m_preselection.getBool(...)`, add the following lines:
    ```cpp
    m_nnOutputHandle.set(*evtInfo, -1, sys);
    for (const xAOD::Jet *jet : *jets) {
      m_dphimetHandle.set(*jet, -99, sys);
    }
    ```
    (The second part is the alternative to moving the loop over jets)

### Creating the user interface (config block)

We now have a fully functional algorithm, which can be safely run over every event and which takes into account all relevant systematics automatically.
However, we haven't specified any of its properties, like what the input lepton and jet containers should be, what event selection to use, or what ONNX models to use for inference.
This is on purpose: we don't want to hardcode any of that information!
Instead, we will **expose these properties to the user via a config block**.

You can find a skeleton of that config block in [source/HowToExtendTopCPToolkit/python/TutorialConfig.py](https://gitlab.cern.ch/atlasphys-top/reco/HowToExtendTopCPToolkit/-/blob/tutorial2025/python/TutorialConfig.py?ref_type=heads).

!!! example "Exercise"
    What options does the config block currently have? Are they all one-to-one with the properties of the algorithm?

??? success "Solution"
    The config block is a class `TutorialConfig`, which inherits from `ConfigBlock`.
    It is used to set up an instance of the `toptutorial::TutorialAlg` (here called `MyTutorial`).

    The config block itself currently has two options: a string `electrons`, which is not set by default, and will be used for the algorithm; and a completely unrelated boolean `silliness`, which prints out a message if turned on.

We are dealing here with a high-level python interface, which allows us a lot of flexibility in setting up our algorithm!

!!! note
    Here we make use of the `config.readNameAndSelection` to extract both the name of the container and an optional per-object selection cut.
    If there is no object selection needed, a simpler `config.readName` method can be used instead to retrieve only the container name.

!!! example "Exercise"
    Add configurable options for the muon, jet and MET containers, and pass them to the algorithm.
    These all need to be set for our algorithm to run properly, so use `None` as default - this will make the code crash if our user forgets to set them!
    Recall that we preset the `EventInfo` handle to have the value "EventInfo", and this is pretty much universal, so no need to expose that property.

    Add also an option for the event-selection: that one can be empty, so a `""` string as default will suffice.

??? success "Solution"
    In the `__init__` method of the class, add the lines:
    ```python
    self.addOption('muons', None, type=str)
    self.addOption('jets', None, type=str)
    self.addOption('met', None, type=str)
    self.addOption('selection', "", type=str)
    ```
    In the `makeAlg` method, after the algorithm is created:
    ```python
    alg.muons, alg.muonSelection = config.readNameAndSelection(self.muons)
    alg.jets, alg.jetSelection = config.readNameAndSelection(self.jets)
    alg.met = config.readName(self.met)
    alg.eventPreselection = self.selection
    ```

!!! example "Exercise"
    Add the configurable option for the ONNX model paths and pass it to the algorithm. The model paths should be a list. We can make the default empty and set it later in the YAML config file.
    To pass the value from the Python config block to the variable defined in the C++ class, we can call [`declareProperty(<name_in_config>, <variable_name_in_the_class>, <A string to provide some descriptions >)`]() in our algorithm's constructor.

??? success "Solution"
    In `TutorialAlg.cxx` in the body of the `TutorialAlg` constructor, add
    ```cpp
    declareProperty("model_paths", m_model_paths, "A list of paths to the trained model");
    ```

    In `TutorialConfig.py` and in the `__init__` method of the class, add
    ```python
    self.addOption('model_paths', [], type=list)
    ```
    and in the `makeAlgs` method, pass the config to the algorithm after it is created:
    ```python
    alg.model_paths = self.model_paths
    ```

Finally, we wish to ensure that our calculations are written to file.
This could be left open to the user, who could map the decorations onto branches in the `Output:` block of the YAML config - but we know better and want to enforce it at this stage.

!!! example "Exercise"
    Make use of [`config.addOutputVar`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AnalysisAlgorithmsConfig/python/ConfigAccumulator.py) to create the two systematic-dependent branches we're after.

??? success "Solution"
    At the end of the `makeAlg` method, add:
    ```python
    config.addOutputVar(self.jets.split(".")[0], "dphimet_%SYS%", "deltaPhi_with_met")
    config.addOutputVar('EventInfo', 'myNNOutput_%SYS%', 'myNNOutput')
    ```

!!! note
    We could also have exposed properties to configure the names of the decorations we wish to set, and made the branch names dependent on them.
    How much flexibility you want to give the user is entirely up to you :smirk:

### Connecting to YAML

Look at the very last part of `reco.yaml`.
You'll find the special `AddConfigBlocks:` block, which is also documented [here](../starting/analysis.md#registering-new-blocks-directly-in-the-yaml-config-since-analysisbase-24240).
Here we can connect our python config block with a custom name to use in our YAML config file, via the property `algName`.

!!! example "Exercise"
    Add the `TutorialConfig` block to the YAML, under the custom name "Tutorial".

??? success "Solution"
    Add the following list element to `AddConfigBlocks:`:
    ```yaml
    - modulePath: 'HowToExtendTopCPToolkit.TutorialConfig'
      functionName: 'TutorialConfig'
      algName: 'Tutorial'
      pos: 'Output'
    ```

We can now set up our config block in the `reco.yaml` file we've been using so far.

!!! example "Exercise"
    Set the block to run on `AnaElectrons.tight`, `AnaMuon.tight`, `AnaJets.baselineJvt`, `AnaMET`, and only for events that pass the selection `pass_fourlep_%SYS%`.
    For the ONNX models paths, we can use the ones provided in the tutorial config directory: `tutorial2025/model_odd.onnx` and `tutorial2025/model_even.onnx`.

??? success "Solution"
    You can technically put this block anywhere in the YAML config, but it's best to keep things logical and put it after the `EventSelection:` block:
    ```yaml
    Tutorial:
    - electrons: 'AnaElectrons.tight'
        muons: 'AnaMuons.tight'
        jets: 'AnaJets.baselineJvt'
        met: 'AnaMET'
        selection: 'pass_fourlep_%SYS%,as_char'
        model_paths:
        - 'tutorial2025/model_odd.onnx'
        - 'tutorial2025/model_even.onnx'
    ```

### Taking it for a spin!

We are now ready to run our brand new analysis algorithm for the first time :sunglasses:

!!! example "Exercise"
    Run TopCPToolkit as before (with systematics).
    Inspect the output file and look for the branches we added.
    Is everything there as expected?

??? success "Solution"
    As before, we run e.g.
    ```sh
    runTop_el.py -i input.txt -o output -t tutorial -e 10000
    ```
    We find the branches we created: `jet_deltaPhi_with_met` and `myNNOutput`, together with all their systematic variations.
    They are set to default values only for events that do not pass our $4\ell$ selection.

### What's next?

Often one needs to further process the output of TopCPToolkit to make histograms of interest. The [FastFrames](https://gitlab.cern.ch/atlas-amglab/fastframes) package is designed to do exactly that! In fact, you can use the output ntuple you just produced as the input to run the FastFrames tutorial [here :link:](https://atlas-project-topreconstruction.web.cern.ch/fastframesdocumentation/latest/eventtutorials/topws2025/Tutorial/).
