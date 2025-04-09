<!---
## Make-methods

### [makeMetAnalysisConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/MetAnalysisAlgorithms/python/MetAnalysisConfig.py)

`seq`
:   the config sequence.

`containerName`
:   the name of the output container after calibration.

`postfix`
:   a postfix to apply to decorations and algorithm names. Typically not needed here.

`useFJVT`
:   use the forward JVT decision in the calculation. The default is `False`.

`treatPUJets`
:   whether to treat pile-up jets in the MET significance calculation. The default is `False`.

`setMuonJetEMScale`
:   enables the handling of muons in jets for the MET calculation. Should be turned off for analyses where muons are not reconstructed at all. The default is `True`.

`jets`
:   the input jet container.

`electrons`
:   the input electron container, with a possible selection, in the format `container` or `container.selection`.

`muons`
:   the input muon container, with a possible selection, in the format `container` or `container.selection`.

`photons`
:   the input photon container, with a possible selection, in the format `container` or `container.selection`.

`taus`
:   the input tau-jet container, with a possible selection, in the format `container` or `container.selection`.

!!! note
    Defining a jet container is mandatory, but all other input containers are optional.

!!! success "Registers the following variables:"
    - `met`: the transverse missing energy $E_\mathrm{T}$
    - `phi`: the transverse missing energy $\phi$
    - `sumet`: the sum of all visible transverse missing momentum

!!! abstract "MET definitions"
    - `mpx`: $p_{x,\mathrm{miss}} = - \sum p_x$
    - `mpy`: $p_{y,\mathrm{miss}} = - \sum p_y$
    - `met`: $p_{\mathrm{T},\mathrm{miss}} = \sqrt{p_{x,\mathrm{miss}}^2+p_{y,\mathrm{miss}}^2}$
    - `sumet`: $\Sigma E_{\mathrm{T}} = \sum p_{\mathrm{T}}$
--->

## Config blocks

### [MetAnalysisConfig](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/MetAnalysisAlgorithms/python/MetAnalysisConfig.py)
Name in YAML: **MissingET**

`containerName`
:   the name of the output container after calibration.

`postfix`
:   a postfix to apply to decorations and algorithm names. Typically not needed here.

`useFJVT`
:   use the forward JVT decision in the calculation. The default is `False`.

`treatPUJets`
:   whether to treat pile-up jets in the MET significance calculation. The default is `False`.

`setMuonJetEMScale`
:   enables the handling of muons in jets for the MET calculation. Should be turned off for analyses where muons are not reconstructed at all. The default is `True`.

`jets`
:   the input jet container.

`electrons`
:   the input electron container, with a possible selection, in the format `container` or `container.selection`.

`muons`
:   the input muon container, with a possible selection, in the format `container` or `container.selection`.

`photons`
:   the input photon container, with a possible selection, in the format `container` or `container.selection`.

`taus`
:   the input tau-jet container, with a possible selection, in the format `container` or `container.selection`.

`invisible`
:   any input container to be treated as invisible particles, in the format `container` (no selection).

`metWP`
:   the MET working point to use: `Loose`, `Tight` (default), `Tighter`, `Tenacious`.

`skipSystematicJetSelection`
:   EXPERIMENTAL: whether to use simplified OR based on nominal jets and for jet-related systematics only. WARNING: this option is strictly for doing physics studies of the feasibility of this OR scheme, it should not be used in a regular analysis. The default is `False`.

`saveSignificance`
:   whether to save the MET significance. The default is `True`.

!!! note
    Defining a jet container is mandatory, but all other input containers are optional.

!!! success "Registers the following variables:"
    - `met`: the transverse missing energy $E_\mathrm{T}$
    - `phi`: the transverse missing energy $\phi$
    - `sumet`: the sum of all visible transverse missing momentum

!!! success "Additional variables toggled by `saveSignificance`:"
    - `significance`: the transverse missing energy significance

!!! abstract "MET definitions"
    - `mpx`: $p_{x,\mathrm{miss}} = - \sum p_x$
    - `mpy`: $p_{y,\mathrm{miss}} = - \sum p_y$
    - `met`: $p_{\mathrm{T},\mathrm{miss}} = \sqrt{p_{x,\mathrm{miss}}^2+p_{y,\mathrm{miss}}^2}$
    - `sumet`: $\Sigma E_{\mathrm{T}} = \sum p_{\mathrm{T}}$