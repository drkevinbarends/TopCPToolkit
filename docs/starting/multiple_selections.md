A key feature of the CP algorithm formalism is the ability to define **object selections**, which are boolean flags associated to each object in a given collection.
As shown in the [example YAML config](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/share/configs/exampleTtbarLjets/reco.yaml?ref_type=heads) and detailed in [the tutorial](../tutorials/write_config/#looking-inside-the-yaml-config), a common use-case is to check the impact of e.g. different choices of _identification_ and _isolation_ working points.
Other types of selections could include _kinematic cuts_, _overlap removal_ and _JVT_ decisions, or be completely customised by declaring them in your own algorithm.

!!! example
    In a final state with multiple leptons, we might be interested in reconstructing a $Z$ boson candidate with a custom analysis algorithm.
    Further algorithms or selection cuts might want to know which two leptons have been declared as associated to the $Z$ boson and which haven't.
    One way to do this is to decorate the relevant information onto the `EventInfo` object, but a more elegant and convenient approach could instead be to add a custom selection flag `is_Z_lepton_%SYS%` onto the leptons.

In the examples below, we will see how to properly handle _multiple_ object selections.

## An example with "loose" vs "tight" electrons/muons

Let's consider first a common case, where we define some signal objects (electrons, muons, jets) for our analysis.
These will be the objects we use in building the event selection and reconstructed quantities of interest.
We will further want to estimate our "fake lepton" background: for that, we'll need information about objects selected with looser cuts.
A straightforward example is the typical $t\bar{t}$ lepton+jets final state.

### Define the electron and muon collections

We set up two "levels" of electron and muon selection: `tight` and `loose`.
The `tight` objects have strict identification and isolation cuts, while the `loose` ones relax some or all of these cuts.
Note that there is no technical requirement for one to be a subset of the other - this is purely an analysis choice.

```yaml
Electrons:
  - containerName: 'AnaElectrons'
    crackVeto: True
    WorkingPoint:
      - selectionName: 'loose'
        identificationWP: 'LooseBLayerLH'
        isolationWP: 'NonIso'
      - selectionName: 'tight'
        identificationWP: 'TightLH'
        isolationWP: 'Tight_VarRad'
    PtEtaSelection:
        minPt: 27000.0
        maxEta: 2.47
        useClusterEta: True

Muons:
  - containerName: 'AnaMuons'
    WorkingPoint:
      - selectionName: 'loose'
        quality: 'Loose'
        isolation: 'NonIso'
      - selectionName: 'tight'
        quality: 'Medium'
        isolation: 'Tight_VarRad'
    PtEtaSelection:
      - selectionName: 'tight'
        postfix: 'tight'
        minPt: 27000.0
        maxEta: 2.5
      - selectionNAme: 'loose'
        postfix: 'loose'
        minPt: 15000
        maxEta: 2.5

```

!!! note
    In the above example, both the `tight` and `loose` electrons have the same `PtEtaSelection` block: they will all have $p_\mathrm{T}>27$ GeV and $\vert\eta\vert < 2.47$.
    For muons, on the other hand, we relax the $p_\mathrm{T}$ selection on `loose` objects.
    In that case, don't forget to specify the `selectionName` (matching the ones defined in `WorkingPoint:`) and `postfix` (could be named anything here) options!

!!! abstract "References for the blocks used:"
    - [Electrons](../settings/electrons.md#electroncalibrationconfig)
    - [Electrons.WorkingPoint](../settings/electrons.md#electronworkingpointconfig)
    - [Muons](../settings/muons.md#muoncalibrationconfig)
    - [Muons.WorkingPoint](../settings/muons.md#muonworkingpointconfig)
    - [PtEtaSelection](../settings/objectselection.md#ptetaselectionblock)

### Define a unique jet collection

For jets, on the other hand, we are working with a unique definition: calibrated jets that pass the JVT requirements.
We will also run on a b-tagger on them.

```yaml
Jets:
  - containerName: 'AnaJets'
    jetCollection: 'AntiKt4EMPFlowJets'
    runJvtSelection: True
    PtEtaSelection:
        minPt: 20000.0
        maxEta: 2.5
    FlavourTagging:
      - btagger: 'DL1dv01'
        btagWP: 'FixedCutBEff_77'
```

All the jets in `AnaJets` are now calibrated, and by setting `runJvtSelection: True` we have defined the analysis-ready selection `AnaJets.baselineJvt` (this name is hard-coded centrally!).

!!! note
    As in the electron case above, the `PtEtaSelection` went into `AnaJets.baselineJvt`.
    The b-tagging block created a new selection `ftag_select_DL1dv01_FixedCutBEff_77_%SYS%` for us - we may choose to use it or not in subsequent algorithms.

!!! abstract "References for the blocks used:"
    - [Jets](../settings/jets.md#makejetanalysisconfig)
    - [Jets.FlavourTagging](../settings/jets.md#ftagconfig)
    - [PtEtaSelection](../settings/objectselection.md#ptetaselectionblock)

### Applying overlap removal

If we're going to analyse separately our `tight` (signal) and `loose` (for fake estimates) leptons, we need to properly take into account the overlap removal between them and the jets.
This means that we in fact have to run _two separate instances_ of the `OverlapRemoval` block.

!!! warning
    Jets might be affected differently by the overlap removal with respect to `tight` or `loose` leptons.
    This means that our single jet selection `AnaJets.baselineJvt` must now be split in two!

```yaml
OverlapRemoval:
  - inputLabel: 'preselectORloose'
    outputLabel: 'passesORloose'
    jets: 'AnaJets.baselineJvt'
    electrons: 'AnaElectrons.loose'
    muons: 'AnaMuons.loose'
    electronsSelectionName: 'loose'
    muonsSelectionName: 'loose'
    jetsSelectionName: 'baselineJvtLoose'
    addPreselection: True
  - inputLabel: 'preselectORtight'
    outputLabel: 'passesORtight'
    jets: 'AnaJets.baselineJvt'
    electrons: 'AnaElectrons.tight'
    muons: 'AnaMuons.tight'
    electronsSelectionName: 'tight'
    muonsSelectionName: 'tight'
    jetsSelectionName: 'baselineJvtTight'
    addPreselection: True
```

!!! note
    The example above is straightforward for electrons and muons: we had `loose` and `tight` selections coming into the `OverlapRemoval`, we have `loose` and `tight` selections coming out.
    However, these are now enhanced with the decision of the respective instance of `OverlapRemoval` (no need to keep track of anything else yourself!).

    For jets however, we plugged in twice the `baselineJvt` selection, but we created the `baselineJvtLoose` and `baselineJvtTight` selections after overlap removal thanks to the `jetsSelectionName` option.
    The additional option `addPreselection` combines these two new selections with the original one, i.e. `baselineJvt`.

!!! abstract "References for the blocks used:"
    - [OverlapRemoval](../settings/overlap.md#overlapanalysisconfig)

### Thinning and preparing the outputs

We won't alter the object selections any further, so we are ready to declare the `Thinning` and `Output` block.
In the example below, we will simply save any object that passes our selections of interest.
Remember that for jets in particular, this means saving both the `loose`-OR and `tight`-OR selections!

```yaml
Thinning:
  - containerName: 'AnaJets'
    outputName: 'OutJets'
    selectionName: 'baselineJvtTight||baselineJvtLoose'
  - containerName: 'AnaElectrons'
    outputName: 'OutElectrons'
    selectionName: 'tight||loose'
  - containerName: 'AnaMuons'
    outputName: 'OutMuons'
    selectionName: 'tight||loose'

Output:
  treeName: 'reco'
  containers:
      mu_: 'OutMuons'
      el_: 'OutElectrons'
      jet_: 'OutJets'
      '': 'EventInfo'
```

!!! abstract "References for the blocks used:"
    - [Thinning](../settings/objectselection.md#outputthinningblock)
    - [Output](../settings/ntupling.md#outputanalysisconfig)

### Trigger selection and trigger matching

Does the _trigger decision_ for the event change, depending on how we define our leptons? **No**.

But does the _trigger matching_ change? **Possibly**.

Therefore we should make sure to run the trigger decision algorithm _once_, but the trigger matching algorithm _twice_.

!!! warning
    Due to the current implementation of the trigger-related config blocks, the following is a bit clunky and might evolve over time.
    Please report to us if the setup below no longer works for you!

```yaml
Trigger:
    triggerChainsPerYear:
        '2015':
            - 'HLT_e24_lhmedium_L1EM20VH || HLT_e60_lhmedium || HLT_e120_lhloose'
            - 'HLT_mu20_iloose_L1MU15 || HLT_mu50'
        '2016':
            - 'HLT_e26_lhtight_nod0_ivarloose || HLT_e60_lhmedium_nod0 || HLT_e140_lhloose_nod0'
            - 'HLT_mu26_ivarmedium || HLT_mu50'
        ...
    noFilter: True
    electrons: 'AnaElectrons.tight'
    muons: 'AnaMuons.tight'
    electronID: 'Tight'
    electronIsol: 'Tight_VarRad'
    muonID: 'Medium'
    postfix: 'tight'

TriggerMatching:
    triggerChainsPerYear:
        '2015':
            - 'HLT_e24_lhmedium_L1EM20VH || HLT_e60_lhmedium || HLT_e120_lhloose'
            - 'HLT_mu20_iloose_L1MU15 || HLT_mu50'
        '2016':
            - 'HLT_e26_lhtight_nod0_ivarloose || HLT_e60_lhmedium_nod0 || HLT_e140_lhloose_nod0'
            - 'HLT_mu26_ivarmedium || HLT_mu50'
        ...
    noFilter: True
    electrons: 'AnaElectrons.loose'
    muons: 'AnaMuons.loose'
    electronID: 'Tight'
    electronIsol: ''
    muonID: 'Medium'
    postfix: 'loose'
```

!!! note
    The first block is the one you're probably used to, as found in all our examples and tutorials: it actually sets up both the trigger decision and matching algorithms.
    The second instance avoids repeating the trigger decision and instead directly sets up the alternative trigger matching setup.
    **Notice the `postfix`** used at the bottom of each setup: this allows to distinguish the trigger matching flags and output variables, and we will see a practical use case immediately after this!

    Also notice that we set `noFilter: True`.
    This turns out the event filtering for both the trigger decision and matching - if we did not do that, we would be taking the logical-AND of both trigger matching setups, which is not what we want!
    **Importantly**, it means you should apply the trigger cuts yourself, either using the branches in the output ntuple or directly in the `EventSelection` block.

### Event selection

Let's look at a simple event selection for our $t\bar{t}$ topology in final states with a single leptons and multiple jets.
As is usually done in such analyses, we will split it by lepton flavour, but also by lepton selection - again with the idea to perform a fake lepton estimation at some point!

```yaml
EventSelection:
  - electrons: 'AnaElectrons.tight||loose'
    muons: 'AnaMuons.tight||loose'
    jets: 'AnaJets.baselineJvtTight||baselineJvtLoose'
    btagDecoration: 'ftag_select_DL1dv01_FixedCutBEff_77'
    noFilter: False
    selectionCutsDict:
      'SUBcommon_loose': |
        GLOBALTRIGMATCH loose
        JET_N baselineJvtLoose 20000 >= 4
        JET_N_BTAG baselineJvtLoose >= 2
        SAVE
      'SUBcommon_tight': |
        GLOBALTRIGMATCH tight
        JET_N baselineJvtTight 20000 >= 4
        JET_N_BTAG baselineJvtTight >= 2
        SAVE
      'ejets_tight': |
        IMPORT SUBcommon_tight
        EL_N tight 27000 == 1
        MU_N tight 27000 == 0
        SAVE
      'mujets_tight': |
        IMPORT SUBcommon_tight
        EL_N tight 27000 == 0
        MU_N tight 27000 == 1
        SAVE
      'ejets_loose': |
        IMPORT SUBcommon_loose
        EL_N loose 27000 == 1
        MU_N loose 27000 == 0
        SAVE
      'mujets_loose': |
        IMPORT SUBcommon_loose
        EL_N loose 27000 == 0
        MU_N loose 27000 == 1
        SAVE
```

!!! note
    For both the input lepton and jet collections, we are passing the logical-OR of our selections of interest.
    When defining e.g. the fake-like muon region, we cut on `MU_N loose 27000 == 1`, i.e. asking exactly one `loose` muon with $p_\mathrm{T}>27$ GeV.
    This works because the muon selection is effectively `(loose || tight) && loose`, which is just `loose`.

    For jets, notice that we have to specify the selection both when counting the total number of jets above a certain $p_\mathrm{T}$ threshold and when counting the number of b-tagged jets.

    Finally, having defined the `postfix` option previously in the `Trigger` config block allows us here to specify that selection in our calls to `GLOBALTRIGMATCH`: each region only looks at the appropriate level of trigger matching.

### Using custom analysis algorithms

Having defined your various object selections and properly handled them through `OverlapRemoval`, `Thinning`, `Trigger`, and `EventSelection`, you may want to also run your own analysis algorithms and config blocks on them.
Bear in mind that we have seen two possible way to do that so far:

1. like in the `X.WorkingPoint` examples, create multiple instances of your config block and pass the relevant selection to them, e.g.
    ```yaml
    MyAnalysis:
      - setup: 'loose_leptons'
        electrons: 'AnaElectrons.loose'
        muons: 'AnaMuons.loose'
        ...
      - setup: 'tight_leptons'
        electrons: 'AnaElectrons.tight'
        muons: 'AnaMuons.tight'
        ...
    ```
1. like in `Thinning` or `EventSelection`, run a single instance of the config block on the logical-OR of selections, e.g.
    ```yaml
    MyAnalysis:
      - setup: 'combined_leptons'
        electrons: 'AnaElectrons.loose||tight'
        muons: 'AnaMuons.loose||tight'
        ...
    ```

!!! warning
    In the second case, your algorithm will really look at all leptons, no matter whether `tight` or `loose`.
    This is rarely what you want to do!
    **We would recommend the first approach**.
