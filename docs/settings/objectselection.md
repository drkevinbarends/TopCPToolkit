<!---
## Make-methods

### [makeOutputThinningConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AsgAnalysisAlgorithms/python/AsgAnalysisConfig.py)

`seq`
:   the config sequence.

`containerName`
:   the name of the input container.

`postfix`
:   a postfix to apply to decorations and algorithm names. Typically not needed here.

`selection`
:   the name of an optional selection decoration to use.

`selectionName`
:   the name of the selection to append this to. The default is `''` (empty string), meaning that the cuts are applied to every object within the container. Specifying a name (e.g. `loose`) applies the cut only to those object who also pass that selection.

`outputName`
:   an optional name for the output container.

`configName`
:   name of the block config, typically not needed here. The default is `Thinning`.

--->

## Config blocks

### [PtEtaSelectionBlock](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AsgAnalysisAlgorithms/python/AsgAnalysisConfig.py)
Name in YAML: **X.PtEtaSelection**, with X amongst: Jets, Electrons, Photons, Muons, TauJets

`containerName`
:   the name of the input container.

`minPt`
:   minimum $p_\mathrm{T}$ value to cut on, in MeV. No default value.

`maxPt`
:   maximum $p_\mathrm{T}$ value to cut on, in MeV. No default value.

`minEta`
:   minimum $\vert\eta\vert$ value to cut on. No default value.

`maxEta`
:   maximum $\vert\eta\vert$ value to cut on. No default value.

`etaGapLow`
:   low end of the $\vert\eta\vert$  gap. No default value.

`etaGapHigh`
:   high end of the $\vert\eta\vert$ gap. No default value.

`useClusterEta`
:   whether to use the cluster $\eta$ (`etaBE(2)`) instead of the object $\eta$ (for electrons and photons). The default is `False`.

`useDressedProperties`
:   whether to use the dressed kinematic properties (for truth particles only). The default is False.

`selectionDecoration`
:   the name of the decoration to set. The default is `None`, which will set it to `'selectPtEta'` followed by the selection name.

`selectionName`
:   the name of the selection to append this to. The default is `''` (empty string), meaning that the cuts are applied to every object within the container. Specifying a name (e.g. `loose`) applies the cut only to those object who also pass that selection.

### [OutputThinningBlock](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AsgAnalysisAlgorithms/python/AsgAnalysisConfig.py)
Name in YAML: **Thinning**

`containerName`
:   the name of the input container.

`postfix`
:   a postfix to apply to decorations and algorithm names. Typically not needed here.

`selection`
:   the name of an optional selection decoration to use.

`selectionName`
:   the name of the selection to append this to. The default is `''` (empty string), meaning that the cuts are applied to every object within the container. Specifying a name (e.g. `loose`) applies the cut only to those object who also pass that selection.

`outputName`
:   an optional name for the output container.

`configName`
:   name of the block config, e.g. `Thinning`.

`deepCopy`
:   run a deep copy of the container. The default is `False`.

`sortPt`
:   whether to sort objects in $p_\mathrm{T}$. The default is `False`.

`noUniformSelection`
:   do not run the union over all selections. The default is `False`.

### [ObjectCutFlowBlock](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AsgAnalysisAlgorithms/python/AsgAnalysisConfig.py)
Name in YAML: **ObjectCutFlow**

`containerName`
:   the name of the input container.

`selectionName`
:   the name of the selection to perform the cutflow for. The default is `''` (empty string), meaning that the cutflow is performed for every object within the container. Specifying a name (e.g. `loose`) generates the cutflow only for those object that also pass that selection.

`forceCutSequence`
:   whether to force the cut sequence and not accept objects if previous cuts failed. The default is `False`.
