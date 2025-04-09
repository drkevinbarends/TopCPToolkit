## Make-methods

!!! tip
    The recommendation is to use [`makeMultipleEventSelectionConfigs`](https://topcptoolkit.docs.cern.ch/settings/eventselection/#makemultipleeventselectionconfigs), which is available in the text-based config approach under the name `EventSelection`.

<!---
### [makeEventSelectionConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/EventSelectionAlgorithms/python/EventSelectionConfig.py)

Performs a single event selection. To define multiple selection regions, see [`makeMultipleEventSelectionConfigs`](eventselection.md#makemultipleeventselectionconfigs) below.

`seq`
:   the config sequence.

`electrons`
:   the input electron container, with a possible selection, in the format `container` or `container.selection`. The default is `''` (empty string).

`muons`
:   the input muon container, with a possible selection, in the format `container` or `container.selection`. The default is `''` (empty string).

`jets`
:   the input jet container, with a possible selection, in the format `container` or `container.selection`. The default is `''` (empty string).

`largeRjets`
:   the large-R jet container, with a possible selection, in the format `container` or `container.selection`. The default is `''` (empty string).

`photons`
:   the input photon container, with a possible selection, in the format `container` or `container.selection`. The default is `''` (empty string).

`taus`
:   the input tau-jet container, with a possible selection, in the format `container` or `container.selection`. The default is `''` (empty string).

`met`
:   the input MET container. The default is `''` (empty string).

`btagDecoration`
:   the b-tagging decoration to use when defining b-jets. The default is `''` (empty string).

`preselection`
:   the event-wise selection flag to start this event selection from. The default is `''` (empty string).

`selectionCuts`
:   a single string listing one selection cut per line. See [available keywords]().

`noFilter`
:   do not apply an event filter. The default is `False`, i.e. remove events not passing the full list of selection cuts.

`debugMode`
:   whether to create an output branch for every single line of the selection cuts. The default is `False` (only saves the final decision).

`cutFlowHistograms`
:   whether to generate cutflow histograms for the selection cuts (performs a call to [`makeEventCutFlowConfig`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AsgAnalysisAlgorithms/python/AsgAnalysisConfig.py)). The default is `False` (no histograms).
--->

### [makeMultipleEventSelectionConfigs](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/EventSelectionAlgorithms/python/EventSelectionConfig.py)
Name in YAML: **EventSelection**

Performs multiple event selections, split into separate regions and subregions. A final logical OR of all region selections is used as event filter.
<!---
The arguments below are the same as for [`makeEventSelectionConfig`](eventselection.md#makeeventselectionconfig), except for `selectionCutsDict`!
--->

`seq`
:   the config sequence.

`electrons`
:   the input electron container, with a possible selection, in the format `container` or `container.selection`. The default is `''` (empty string).

`muons`
:   the input muon container, with a possible selection, in the format `container` or `container.selection`. The default is `''` (empty string).

`jets`
:   the input jet container, with a possible selection, in the format `container` or `container.selection`. The default is `''` (empty string).

`photons`
:   the input photon container, with a possible selection, in the format `container` or `container.selection`. The default is `''` (empty string).

`taus`
:   the input tau-jet container, with a possible selection, in the format `container` or `container.selection`. The default is `''` (empty string).

`met`
:   the input MET container. The default is `''` (empty string).

`btagDecoration`
:   the b-tagging decoration to use when defining b-jets. The default is `''` (empty string).

`preselection`
:   the event-wise selection flag to start this event selection from. The default is `''` (empty string).

`selectionCutsDict`
:   a dictionary of (keys) region names and (values) strings listing one selection cut per line. See [available keywords](). A region name starting with `SUB` is treated as a subregion (not saved, can be imported from another selection).

`noFilter`
:   do not apply an event filter. The default is `False`, i.e. remove events not passing the full list of selection cuts.

`debugMode`
:   whether to create an output branch for every single line of the selection cuts. The default is `False` (only saves the final decision).

`cutFlowHistograms`
:   whether to generate cutflow histograms for the selection cuts (performs a call to [`makeEventCutFlowConfig`](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AsgAnalysisAlgorithms/python/AsgAnalysisConfig.py)). The default is `False` (no histograms).

## Config blocks

### [EventSelectionConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/EventSelectionAlgorithms/python/EventSelectionConfig.py)

`name`
:   the name of the event selection, used to uniquely identify the `EventSelectionConfig` block.

`electrons`
:   the input electron container, with a possible selection, in the format `container` or `container.selection`. The default is `''` (empty string).

`muons`
:   the input muon container, with a possible selection, in the format `container` or `container.selection`. The default is `''` (empty string).

`jets`
:   the input jet container, with a possible selection, in the format `container` or `container.selection`. The default is `''` (empty string).

`largeRjets`
:   the large-R jet container, with a possible selection, in the format `container` or `container.selection`. The default is `''` (empty string).

`photons`
:   the input photon container, with a possible selection, in the format `container` or `container.selection`. The default is `''` (empty string).

`taus`
:   the input tau-jet container, with a possible selection, in the format `container` or `container.selection`. The default is `''` (empty string).

`met`
:   the input MET container. The default is `''` (empty string).

`btagDecoration`
:   the b-tagging decoration to use when defining b-jets. The default is `''` (empty string).

`preselection`
:   the event-wise selection flag to start this event selection from. The default is `''` (empty string).

`selectionCuts`
:   a single string listing one selection cut per line. See [available keywords]().

`noFilter`
:   do not apply an event filter. The default is `False`, i.e. remove events not passing the full list of selection cuts.

`debugMode`
:   whether to create an output branch for every single line of the selection cuts. The default is `False` (only saves the final decision).

### [EventSelectionMergerConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/EventSelectionAlgorithms/python/EventSelectionConfig.py)

`selections`
:   the selection decisions (list of strings) to unify into a final decision (internally: `selection_1 || selection_2 || ...`). The default is `[]` (empty list).

`noFilter`
:   do not apply an event filter. The default is `False`, i.e. remove events not passing the full list of selection cuts.

!!! tip
    The `selections` field expects expressions like `"pass_{name}_%SYS%"` for some selection `name`.

### [EventCutFlowBlock](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AsgAnalysisAlgorithms/python/AsgAnalysisConfig.py)

`containerName`
:   the name of the input container, typically `EventInfo`.

`selectionName`
:   the name of an optional selection decoration to use.

`postfix`
:   a postfix to apply in the naming of cutflow histograms. Set it when defining multiple cutflows.

`customSelections`
:   the selections for which to generate cutflow histograms. If a single string, corresponding to a particular event selection, the event cutflow for that selection will be looked up. If a list of strings, will use explicitly those selections. If left blank, all selections attached to the container will be looked up.

## Available keywords

The keywords listed in the table below can be used together with their respective options to define a selection cut (one per line).
In most cases, it might be beneficial to define multiple selection regions.
We then distinguish between subregions (whose name starts with `SUB`), that are not filtered on but still provide a decision flag, from normal regions, which are used in the final filtering.
Regions can import the decision flag of these subregions (see `IMPORT`) to avoid duplicating selection cuts in multiple places.

The symbol `$` below is a placeholder for any of the following comparison operators: `>=`, `>`, `==`, `<`, `<=`.

| **Keyword** | **Options** | **Effect** |
| ----------- | ----------- | ---------- |
| `EL_N` | `ptmin $ ref`<br><br>`sel ptmin $ ref` | Compares (`$`) the number of electrons with <br>$p_\mathrm{T}>$`ptmin` (in MeV) to `ref`.<br>Use `sel` to specify a different electron selection. |
| `MU_N` | `ptmin $ ref`<br><br>`sel ptmin $ ref` | Compares (`$`) the number of muons with <br>$p_\mathrm{T}>$`ptmin` (in MeV) to `ref`.<br>Use `sel` to specify a different muon selection. |
| `SUM_EL_N_MU_N` | `ptmin $ ref`<br><br>`ptminEL ptminMU $ ref` | Compares (`$`) the number of electrons and muons with <br>$p_\mathrm{T}>$`ptmin` (in MeV) to `ref`.<br>Same, but with different cuts for electrons/muons. |
| `JET_N` | `ptmin $ ref`<br><br>`sel ptmin $ ref` | Compares (`$`) the number of jets with <br>$p_\mathrm{T}>$`ptmin` (in MeV) to `ref`.<br>Use `sel` to specify a different jet selection. |
| `JET_N_BTAG` | `$ ref`<br><br>`tagger:WP $ ref` | Compares (`$`) the number of b-tagged jets to `ref`.<br><br>Use `tagger:WP` to specify a different b-tagging configuration. |
| `LJET_N` | `ptmin $ ref` <br><br>`sel ptmin $ ref` | Compares (`$`) the number of large-R jets with <br>$p_\mathrm{T}>$`ptmin` (in MeV) to `ref`.<br>Use `sel` to specify a different large-R jet selection. |
| `LJETMASS_N` | `massmin $ ref` <br><br>`sel massmin $ ref` | Compares (`$`) the number of large-R jets with <br>mass >`massmin` (in MeV) to `ref`.<br>Use `sel` to specify a different large-R jet selection. |
| `LJETMASSWINDOW_N` | `low high $ ref`<br><br>`sel low high $ ref` | Compares (`$`) the number of large-R jets with <br>`low`< mass <`high` (in MeV) to `ref`.<br>Use `sel` to specify a different large-R jet selection.<br>(Append `veto` to veto the mass window instead.) |
| `PH_N` | `ptmin $ ref`<br><br>`sel ptmin $ ref` | Compares (`$`) the number of photons with <br>$p_\mathrm{T}>$`ptmin` (in MeV) to `ref`.<br>Use `sel` to specify a different photon selection. |
| `TAU_N` | `ptmin $ ref`<br><br>`sel ptmin $ ref` | Compares (`$`) the number of tau-jets with <br>$p_\mathrm{T}>$`ptmin` (in MeV) to `ref`.<br>Use `sel` to specify a different tau-jet selection. |
| `MET` | `$ ref` | Compares (`$`) the MET to `ref` (in MeV). |
| `MWT` | `$ ref` | Compares (`$`) the transverse mass of the W boson<br>(lepton+MET system) to `ref` (in MeV). |
| `MET+MWT` | `$ ref` | Compares (`$`) the sum of the MET and the transverse mass<br>of the W boson(lepton+MET system) to `ref` (in MeV). |
| `MLL` | `$ ref` | Compares (`$`) the dilepton invariant mass to `ref` (in MeV). |
| `MLLWINDOW` | `low high`<br>`low high veto` | Selects the event if `low < MLL < high` (in MeV).<br>Rejects the event if `low < MLL < high` (in MeV). |
| `MLL_OSSF` | `low high`<br>`low high veto` | Selects the event if `low < MLL_OSSF < high` (in MeV).<br>Rejects the event if `low < MLL_OSSF < high` (in MeV). |
| `OS` | None | Selects the event if it contains two opposite-sign leptons. |
| `SS` | None | Selects the event if it contains two same-sign leptons. |
| `EVENTFLAG` | `sel` | Checks that the event passes the selection `sel`.<br>Allows to fold `sel` into the output branch<br>`pass_<region>_%SYS%` defined below.<br>Useful to save storage space as you can now filter out the<br>`sel` branches themselves. |
| `GLOBALTRIGMATCH` | None | Selects the event if it passes global trigger<br>matching (electrons/muons/photons).|
| `SAVE` | None | Saves the current selection (can be retrieved as `pass_<region>_%SYS%`). |
| `IMPORT` | `subreg` | Applies the selection cuts defined in another region `subreg`. |

!!! tip "Applying $m(\ell,\ell)$ window cuts, or vetos"
    To select OSSF dilepton events in a 10 GeV window of the $Z$ boson mass, run `MLLWINDOW 81 101`. To veto them (e.g. for a purer $t\bar{t}$ selection), run `MLLWINDOW 101 81`.