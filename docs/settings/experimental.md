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
:   list of cuts on $\Delta R$ between lepton and photon.

`photon_pT_cuts`
:   list of cuts on photon $p_\mathrm{T}$.

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