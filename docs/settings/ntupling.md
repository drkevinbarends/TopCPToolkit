<!---
## Make-methods

!!! warning
    No such method exist for ntupling algorithms!
--->

## Config blocks

### [OutputAnalysisConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AsgAnalysisAlgorithms/python/OutputAnalysisConfig.py)
Name in YAML: **Output**

`postfix`
:   a postfix to apply to decorations and algorithm names. Typically not needed here.

`vars`
:   a list of mappings (list of strings) between containers and decorations to output branches. The default is `[]` (empty list).

`varsOnlyForMC`
:   same as `vars`, but for MC-only variables so as to avoid a crash when running on data. The default is `[]` (empty list).

`metVars`
:   a list of mappings (list of strings) between containers and decorations to output branches. Specficially for MET variables, where only the final MET term is retained. The default is `[]` (empty list).

`containers`
:   a dictionary mapping prefixes (key) to container names (values) to be used when saving to the output tree. Branches are then of the form `prefix_decoration`.

`containersOnlyForMC`
:   same as containers, but for MC-only containers so as to avoid a crash when running on data. The default is `{}` (empty dictionary).

`treeName`
:   name of the output TTree to save. The default is `analysis`.

`metTermName`
:   the name (string) of the MET term to save, turning the MET container into a single object. The default is `'Final'`.

    !!! tip
        For a particle-level truth analysis, you likely want to recover `'NonInt'` instead. See [MET_Truth](https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/Run2xAODMissingET#MET_Truth).

`storeSelectionFlags`
:   whether to store one branch for each object selection. The default is `True`.

`selectionFlagPrefix`
:   the prefix (string) used when naming selection branches. The default is `'select'`.

`alwaysAddNosys`
:   if set to `True`, all branches will be given a systematics suffix, even if they have no systematics (beyond the nominal). The default is `False`.

`commands`
:   a list of strings containing commands (regexp strings prefaced by the keywords `enable` or `disable`) to turn on/off the writing of branches to the output ntuple. The default is `None` (no modification to the scheduled output branches).

    !!! example
        Suppose you run b-tagging and JVT but don't actually need to store the individual per-jet SFs for either b-tagging or JVT. The list that is passed to the `commands` argument would look like:
        ```python
        commands = [
            'disable jet_.*.eff.*',
            'dlsable jet_jvtEfficiency.*'
        ]
        ```
