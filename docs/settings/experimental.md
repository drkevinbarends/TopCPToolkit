<!---
## Make-methods

!!! warning
    No such method exist for experimental algorithms!
--->

## Config blocks

### [VGammaORConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AsgAnalysisAlgorithms/python/VGammaORConfig.py)
Name in YAML: **VGammaOR**

Adds a new event-level variable `in_vgamma_overlap` using the [VGammaORTool](https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/VGammaORTool). The new variable can be used to remove to overlap between e.g. V+jets and Vy+jets samples.

`dR_lepton_photon_cuts`
:   list of cuts on $\Delta R$ between lepton and photon. The default is `[0.0, 0.05, 0.075, 0.1, 0.125, 0.15, 0.2]`.

`photon_pT_cuts`
:   list of $p_\mathrm{T}$ cuts (in MeV) on the photon. The default is `[10e3]`.

`noFilter`
:   do not apply an event filter. The default is `False`, i.e. remove events not passing the overlap removal. If set to `True`, all events are kept and the decision flag is written to the output ntuple instead.

`keepInOverlap`
:   list of DSIDs (integers) for which events are to be kept if found to be in the overlap region. For instance, $V\gamma$ samples in $V+$jets vs $V\gamma+$jets overlap removal. The default list was taken from the [PmgWeakBosonProcesses twiki](https://twiki.cern.ch/twiki/bin/view/AtlasProtected/PmgWeakBosonProcesses) but is not actively maintained! The default is `[700011, 700012, 700013, 700014, 700015, 700016, 700017]`.

`removeInOverlap`
:   list of DSIDs (integers) for which events are to be removed if found to be in the overlap region. For instance, $V$ samples in $V+$jets vs $V\gamma+$jets overlap removal. The default list was taken from the [PmgWeakBosonProcesses twiki](https://twiki.cern.ch/twiki/bin/view/AtlasProtected/PmgWeakBosonProcesses) but is not actively maintained! The default is `[700320, 700321, 700322, 700467, 700468, 700469, 700323, 700324, 700325, 700470, 700471, 700472, 700326, 700327, 700328, 700329, 700330, 700331, 700332, 700333, 700334, 700473, 700474, 700475, 700476, 700477, 700478, 700479, 700480, 700481, 700341, 700342, 700343, 700338, 700339, 700340, 700344, 700345, 700346, 700347, 700348, 700349, 700598, 700599, 700439, 700440, 700441]`.

!!! success "Registers the following variables:"
    - `in_vgamma_overlap`: can either be 0 or 1 for an event (no systematics). An event should be kept if it is 0 for V+jets samples or if it is 1 for V$\gamma$+jets samples.

### [IOStatsConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AsgAnalysisAlgorithms/python/AsgAnalysisConfig.py)
Name in YAML: **IOStats**

Print a list of branches that are read when processing input files using the [ReadStats](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/Event/xAOD/xAODCore/xAODCore/tools/ReadStats.h) tool. The implementation follows the twiki page [here](https://twiki.cern.ch/twiki/bin/view/AtlasProtected/DerivationFramework#Checking_What_Branches_an_Analys).

`printOption`
:   option to pass the standard ROOT printing function. Can be "Summary", "ByEntries" or "ByBytes". The default is "Summary".

### [ExtraParticleDecorationConfig](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/v2.9.2/source/TopCPToolkit/python/ExtraParticleDecorationConfig.py) :no_entry:

**Deprecated since v2.10.0** ([issue #145](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/issues/145))

!!! warning "User actions required for updating to version 2.10.0 or higher"

    Drop the following lines from YAML configs if applicable:
    ```
    ExtraParticleDecoration:
        - particles: 'AnaElectrons'
        - particles: 'AnaMuons'
        - particles: 'AnaJets
    ```
    The declaration of `ExtraParticleDecoration` under `AddConfigBlocks` also need to be removed.

Name in YAML: **ExtraParticleDecoration**

Allows to add generic particle decorations and save them as output variables.

`particles`
:   the input particle container.

!!! success "Registers the following variables:"
    - `e`: object $E$

### [ExtraPhotonDecorationConfig](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/v2.9.2/TopCPToolkit/python/ExtraPhotonDecorationConfig.py) :no_entry:
**Deprecated since v2.10.0** ([issue #146](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/issues/146))

!!! warning "User actions required for updating to version 2.10.0 or higher"

    Calls to a distinct `ExtraPhotonDecoration:` block can be replaced by a one-liner within the `Photons:` block:
    ```
    Photons:
      - containerName: 'AnaPhotons'
        recomputeIsEM: False
        ExtraVariables: {} # <-- no extra settings needed!
        WorkingPoint:
          - ...
    ```
    The declaration of `ExtraPhotonDecoration` under `AddConfigBlocks` should also be removed if applicable.

Name in YAML: **ExtraPhotonDecoration**

Decorates the output photons with the conversion type and calo eta.

`photons`
:   the input photon container.

!!! success "Registers the following variables:"
    - `conversionType`: photon conversion type.
    - `caloEta`: calorimeter eta.
