!!! abstract "In this session you will..."
    - understand the differences between a CP tool and a CP algorithm
    - write a simple algorithm to perform a piece of analysis
    - create new output variables (both event-wise and object-wise)
    - make your algorithm configurable in YAML and useable by others

_For this part, we assume you're already familiar with the contents of ["Your first config"](write_config.md) and have followed the ["Initial setup"](setup.md) instructions._

## The necessary basics

We have alluded to CP tools and algorithms in many places so far, let's try to clarify these concepts a little bit: what exactly are we talking about?

!!! tip
    The [ATLAS Software tutorials](https://atlassoftwaredocs.web.cern.ch/ABtutorial/alg_basic_intro/) are also very relevant to this session!

    If you already feel confident with these topics, you may skip this section :student:

### CP tools

First and foremost: "CP" stands for "Combined Performance", and refers to the centralised and essential aspects of any ATLAS analysis such as object reconstruction and calibration.
In ATLAS, there are CP groups for electrons/photons, muons, taus, jets/missing energy, etc.
The relevant tools they provide are the official "CP tools".

!!! note
    Technically, "CP tools" all derive in a more or less complicated way from the [`AsgTool`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/Control/AthToolSupport/AsgTools/AsgTools/AsgTool.h) class.

Let's take a look at one example of a CP Tool: the [`EgammaCalibrationAndSmearingTool`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/ElectronPhotonID/ElectronPhotonFourMomentumCorrection/Root/EgammaCalibrationAndSmearingTool.cxx#0265), which (unsurprisingly) is used to calibrate and smear "Egamma" objects (i.e. electrons and photons).

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
In the [previous part](../tutorials/write_config.md) of the tutorial, we had set up a YAML config to run on a $t\bar{t}$ sample (under Run 3 conditions) and to apply a simple $\ell+$jets event selection.
Our task will now be to **write a custom algorithm to compute two quantities** of interest for our imaginary analysis.
These will be:

- the [transverse $W$ mass](https://en.wikipedia.org/wiki/Transverse_mass), $M_\mathrm{T}(W)$, that we will define here from the lepton kinematics and missing transverse energy as
$$ M_\mathrm{T}(W) = \sqrt{2E_\mathrm{T}^\ell E_\mathrm{T}^\text{miss}\left(1-\cos\Delta\phi\right)} $$
- the azimuthal separation, $\Delta\phi$, between each jet and the missing energy vector

!!! note
    The transverse $W$ mass can be used to measure the mass of the $W$ boson by noting its kinematical endpoint $M_T(W)\leq M(W)$.
    It is an interesting example of an _event-wise variable_: one that we will attach to the `EventInfo` object.
    On the other hand, the quantity $\Delta\phi(j_i, \vec{p_\mathrm{T}^\text{miss}})$ is defined for each jet $j_i$ in the event, the number of which varies from event to event.
    It's an example of an _object-wise variable_.

### How to make a new algorithm compile

We will start from the skeleton structure provided in these two files: [Root/TutorialAlg.cxx](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/Root/TutorialAlg.cxx) and its header [TopCPToolkit/TutorialAlg.h](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/TopCPToolkit/TutorialAlg.h).

!!! example "Exercise"
    What class do these files define? What does it currently do?

??? success "Solution"
    They define the class `top::TutorialAlg` (notice the scope, to differentiate from CP algorithms).
    Currently, it only initialises the list of systematics and prints an info message at the start.
    There is no `finalize()` method implemented (not needed).

We already compiled TopCPToolkit successfully, but we also need to register this `TutorialAlg` as an algorithm in a few special places.
These are:

- [source/TopCPToolkit/TopCPToolkit/TopCPToolkitDict.h](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/TopCPToolkit/TopCPToolkitDict.h)
- [source/TopCPToolkit/TopCPToolkit/selection.xml](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/TopCPToolkit/selection.xml)
- [source/TopCPToolkit/src/components/TopCPToolkit_entries.cxx](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/src/components/TopCPToolkit_entries.cxx)

!!! example "Exercise"
    Inspect these 3 files and insert the necessary lines.

??? success "Solution"
    In `TopCPToolkitDict.h`, you need to add:
    ```cpp
    #include "TopCPToolkit/TutorialAlg.h"
    ```
    In `selection.xml`:
    ```xml
    <class name="TutorialAlg" />
    ```
    In `TopCPToolkit_entries.cxx`:
    ```cpp
    #include <TopCPToolkit/TutorialAlg.h>
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
These should be entirely configurable by the users, and we should take into account systematics.
We further want to decorate two new variables, for which we will also have systematics.

!!! example "Exercise"
    What kind of object do we need to invoke here? Make a precise list.

??? success "Solution"
    We need the following [systematic handles](#a-list-of-useful-handles):

    - a `SysReadHandle` for each type of object: `xAOD::ElectronContainer`, `xAOD::MuonContainer`, `xAOD::JetContainer`, `xAOD::METContainer`, `xAOD::EventInfo`.
    - a `SysWriteDecorHandle` for both the variables we want to compute and decorate.

!!! example "Exercise"
    Now add them to the header file for our algorithm.
    Make sure to use the following names for properties: "electrons", "muons", "jets", "met", "eventInfo", "mtw", "dphimet".
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
      CP::SysReadHandle<xAOD::ElectronContainer> m_electronsHandle {
        this, "electrons", "", "the input electron container"
      };
      CP::SysReadHandle<xAOD::MuonContainer> m_muonsHandle {
        this, "muons", "", "the input muon container"
      };
      CP::SysReadHandle<xAOD::MissingETContainer> m_metHandle {
        this, "met", "", "the input MET container"
      };
      CP::SysReadHandle<xAOD::JetContainer> m_jetsHandle {
        this, "jets", "", "the input jet container"
      };
      CP::SysReadHandle<xAOD::EventInfo> m_eventInfoHandle {
        this, "eventInfo", "EventInfo", "the input EventInfo container"
      };
      CP::SysWriteDecorHandle<float> m_dphimetHandle {
        this, "dphimet", "dphimet_%SYS%", "decoration name for DeltaPhi between each jet and the MET"
      };
      CP::SysWriteDecorHandle<float> m_mtwHandle {
        this, "mtw", "mtw_%SYS%", "decoration name for the transverse W mass"
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

??? success "Solution"
    In the `TutorialAlg::initialize()` method, and before the line with `m_systematicsList.initialize()`, add:
      ```cpp
      ANA_CHECK(m_electronsHandle.initialize(m_systematicsList));
      ANA_CHECK(m_muonsHandle.initialize(m_systematicsList));
      ANA_CHECK(m_jetsHandle.initialize(m_systematicsList));
      ANA_CHECK(m_metHandle.initialize(m_systematicsList));
      ANA_CHECK(m_eventInfoHandle.initialize(m_systematicsList));

      ANA_CHECK(m_dphimetHandle.initialize(m_systematicsList, m_jetsHandle));
      ANA_CHECK(m_mtwHandle.initialize(m_systematicsList, m_eventInfoHandle));
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

To compute the $M_\mathrm{T}(W)$ observable, we need access to the one lepton in the event - but we don't know if it's an electron or a muon!
There are multiple ways of dealing with such a situation, but we'll do something very simple: we'll set up an empty `PtEtaPhiMVector` and check the sizes of both the electron and muon containers to decide which one to read from, then we'll fill the `PtEtaPhiMVector` with the first element of that container.

We also need to collect the missing transverse momentum magnitude and its azimuthal angle from the MET container.
Note that the MET container in fact contains multiple objects, the various MET terms, and we need to access by name the one called "Final".

!!! example "Exercise"
    Build the lepton `PtEtaPhiMVector` from the electron and muon containers, and call it `lepton`.
    Obtain the MET components `et_miss` and `phi_miss` from the appropriate MET object.

??? success "Solution"
    Still in the loop over systematics, and after the code we've just added, write:
    ```cpp
    PtEtaPhiMVector lepton;
    if ( electrons->size() > 0 ) {
      lepton = electrons->at(0)->p4();
    }
    else if ( muons->size() > 0) {
      lepton = muons->at(0)->p4();
    }

    float et_miss  = (*met)["Final"]->met();
    float phi_miss = (*met)["Final"]->phi();
    ```

!!! note
    For simplicity, we are here completely disregarding what should be done in case there are more than one lepton in the event, or if there are indeed none at all.
    This is where analysis complexity builds up and you need to be careful about manipulating your object collections.
    In a moment, we'll however try to add some protection to our algorithm, so that it can at least run safely on every event.

At this point, we are ready to compute $M_\mathrm{T}(W)$ and save it as an event-wise variable.
Look up the definition we gave above in case you forgot!

!!! example "Exercise"
    Implement $M_\mathrm{T}(W)$ and decorate it onto the `EventInfo` object.

??? success "Solution"
    After the code we just wrote, add:
    ```cpp
    float dphi_lep_met = TVector2::Phi_mpi_pi( lepton.Phi() - phi_miss );
    float mtw = std::sqrt( 2 * lepton.Pt() * et_miss * (1-std::cos(dphi_lep_met)) );
    m_mtwHandle.set(*evtInfo, mtw, sys);
    ```

Now onto the per-jet observable: you already know how to compute the $\Delta\phi$ quantity, so this should be easy! :wink:

!!! example "Exercise"
    For every jet $j_i$ in the event, compute $\Delta\phi(j_i, \vec{p_\mathrm{T}^\text{miss}})$ and decorate the jet with it.

??? success "Solution"
    After the code we just wrote, add:
    ```cpp
    for (const xAOD::Jet *jet : *jets) {
      float dphi_jet_met = TVector2::Phi_mpi_pi( jet->phi() - phi_miss );
      m_dphimetHandle.set(*jet, dphi_jet_met, sys);
    }
    ```

As our final task here, let's add a small layer of protection to our algorithm: we will allow to pass an event-level selection.
This is not a perfect protection, but since we've already defined in our YAML config a lepton+jets event selection, if we manage to pass it here, we at least guard ourselves from events with no leptons.

!!! note
    The case of multiple leptons is not _completely_ solved, as we are ignoring (again, for simplicity) the various levels of level definition (ID, isolation, $p_\mathrm{T}$ cuts, etc.).
    It is possible to request per-object selections (examples are available in most CP and TopCPToolkit algorithms), and the behaviour is very similar as for event-level selections.

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
    Finally, in the `execute()` method, after calling `m_eventInfoHandle.retrieve(...)`, add the following:
    ```cpp
    if ( m_preselection && !m_preselection.getBool(*evtInfo, sys)) continue;
    ```

We are now faced with another problem, that would only reveal itself at runtime... :sweat:
When a specific event/systematic combination does not pass the event selection we've just implemented, our decorations are not set, and this is ill-defined for the ntuple writer!

We need to add default decorations for all our new variables.

!!! example "Exercise"
    Add a default decoration for $M_\mathrm{T}(W)$.
    You may do the same for $\Delta\phi(j_i, \vec{p_\mathrm{T}^\text{miss}})$, or move the loop over jets before the preselection is called (but don't forget about moving the MET variables up too!).

??? success "Solution"
    Before the call to `m_preselection.getBool(...)`, add the following lines:
    ```cpp
    m_mtwHandle.set(*evtInfo, -1, sys);
    for (const xAOD::Jet *jet : *jets) {
      m_dphimetHandle.set(*jet, -99, sys);
    }
    ```
    (The second part is the alternative to moving the loop over jets)

### Creating the user interface (config block)

We now have a fully functional algorithm, which can be safely run over every event and which takes into account all relevant systematics automatically.
However, we haven't specified any of its properties, like what the input lepton and jet containers should be, or what event selection to use.
This is on purpose: we don't want to hardcode any of that information!
Instead, we will **expose these properties to the user via a config block**.

You can find a skeleton of that config block in [source/TopCPToolkit/python/TutorialConfig.py](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/python/TutorialConfig.py).

!!! example "Exercise"
    What options does the config block currently have? Are they all one-to-one with the properties of the algorithm?

??? success "Solution"
    The config block is a class `TutorialConfig`, which inherits from `ConfigBlock`.
    It is used to set up an instance of the `top::TutorialAlg` (here called `tutorialStuff`).

    The config block itself currently has two options: a string `electrons`, which is not set by default, and will be used for the algorithm; and a completely unrelated boolean `silliness`, which prints out a message if turned on.

We are dealing here with a high-level python interface, which allows us a lot of flexibility in setting up our algorithm!

!!! note
    We make use of the `config.readName` method to get hold of the low-level name of the containers the user will set.
    A frequent alternative is `config.readNameAndSelection` to extract both the name of the container and an optional per-object selection cut.

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
    In the `makeAlg` method, first add:
    ```python
    muons = config.readName(self.muons)
    jets  = config.readName(self.jets)
    met   = config.readName(self.met)
    ```
    and then, after the algorithm is created:
    ```python
    alg.muons = muons
    alg.jets  = jets
    alg.met   = met
    alg.eventPreselection = self.selection
    ```

Finally, we wish to ensure that our calculations are written to file.
This could be left open to the user, who could map the decorations onto branches in the `Output:` block of the YAML config - but we know better and want to enforce it at this stage.

!!! example "Exercise"
    Make use of [`config.addOutputVar`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AnalysisAlgorithmsConfig/python/ConfigAccumulator.py) to create the two systematic-dependent branches we're after.

??? success "Solution"
    At the end of the `makeAlg` method, add:
    ```python
    config.addOutputVar('EventInfo', 'mtw_%SYS%', 'transverseWmass' )
    config.addOutputVar(self.jets, 'dphimet_%SYS%', 'deltaPhi_with_met')
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
    - modulePath: 'TopCPToolkit.TutorialConfig'
      functionName: 'TutorialConfig'
      algName: 'Tutorial'
      pos: 'Output'
    ```

We can now set up our config block in the `reco.yaml` file we've been using so far.

!!! example "Exercise"
    Set the block to run on `AnaElectrons`, `AnaMuon`, `AnaMET`, and only for events that pass the combined selection `pass_ejets_%SYS% || pass_mujets_%SYS%`.

??? success "Solution"
    You can technically put this block anywhere in the YAML config, but it's best to keep things logical and put it after the `EventSelection:` block:
    ```yaml
    Tutorial:
    - electrons: 'AnaElectrons'
      muons: 'AnaMuons'
      met: 'AnaMET'
      jets: 'AnaJets'
      selection: 'pass_ejets_%SYS% || pass_mujets_%SYS%'
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
    runTop_el.py -i input.txt -o output -t tutorial -e 100
    ```
    We find the branches we created: `transverseWmass` and `jet_deltaPhi_with_met`, together with all their systematic variations.
    They are set to default values only for events that do not pass our $\ell+$jets selection.
