Various scale factors are computed by the respective CP algorithms. The default approach is that the relevant object blocks compute and decorate per-object SFs, and some of them also by default store these per-object SFs as vectors of floats in the output NTuple. Please refer to the documentation of the relevant objects under [Settings](index.md).

!!! tip
    To disable the per-object SFs, use the branch filtering feature of [`OutputAnalysisConfig`](ntupling.md/#outputanalysisconfig).

<!---
## Make-methods

!!! warning
    No such method exist yet for event-level scale factor algorithms!
--->

## Config blocks

### [PerEventSFBlock](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AsgAnalysisAlgorithms/python/AsgAnalysisConfig.py)
Name in YAML: **PerEventSF**

Calculates a per-event SF which is the product of per-object SFs, for objects passing an optional selection.

`algoName`
:   unique name given to the underlying algorithm computing the per-event scale factors

`particles`
:   the input object container, with a possible selection, in the format `container` or `container.selection`.

`objectSF`
:   the name of the per-object SF decoration to be used.

`eventSF`
:   the name of the per-event SF decoration.

!!! success "Registers the following variables:"
    - The name of the output variable is the same as the one specified in the `eventSF` option.

### [LeptonSFCalculatorConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/AsgAnalysisAlgorithms/python/LeptonSFCalculatorConfig.py)
Name in YAML: **LeptonSF**

Computes the per-event lepton SF, i.e. a product over the individual reconstruction, identification and isolation SFs for all electrons, muons or photons in the event.

`electrons`
:   the input electron container, with a possible selection, in the format `container` or `container.selection`.

`electronSFs`
:   list of decorated electron SFs to use in the computation. If not set, will use reconstruction x ID x isolation.

`muons`
:   the input muon container, with a possible selection, in the format `container` or `container.selection`.

`muonSFs`
:   list of decorated muon SFs to use in the computation. If not set, will use reconstruction x isolation x TTVA.

`photons`
:   the input photon container, with a possible selection, in the format `container` or `container.selection`.

`photonSFs`
:   list of decorated photon SFs to use in the computation. If not set, will use ID x isolation.

`lepton_postfix`
:   the name of the common lepton SF, e.g. `'tight'`.

!!! success "Registers the following variables:"
    - `weight_leptonSF`: the per-event lepton SF, the product of the electron reconstruction/ID/isolation SFs, the muon reconstruction/TTVA/isolation SFs, and the photon ID/isolation SFs
