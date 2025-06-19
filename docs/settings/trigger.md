## Config blocks

### [TriggerAnalysisBlock](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/TriggerAnalysisAlgorithms/python/TriggerAnalysisConfig.py) / [TriggerAnalysisSFBlock](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/TriggerAnalysisAlgorithms/python/TriggerAnalysisSFConfig.py)
This config block allows you to perform trigger decision for any trigger leg, and for electrons/muons/photons also trigger matching and the retrieval of the correct efficiencies and scale factors (especially for multi-lepton triggers).

Name in YAML: **Trigger** / **TriggerMatching**

!!! note
    The `ConfigFactory` name **Trigger** will actually schedule both the [TriggerAnalysisBlock](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/TriggerAnalysisAlgorithms/python/TriggerAnalysisConfig.py) and the [TriggerAnalysisSFBlock](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/TriggerAnalysisAlgorithms/python/TriggerAnalysisSFConfig.py) in the backend.
    This is what is needed for most users, and there are ways of turning off the SF part as described below.
    To set up the trigger matching on its own, use the name **TriggerMatching**.

`triggerChainsPerYear`
:   a dictionary with key (string) the year and value (list of strings) the trigger chains. You can also use `||` within a string to enforce an OR of triggers without looking up the individual triggers. Used for both trigger selection and SFs. The default is `{}` (empty dictionary).

`multiTriggerChainsPerYear`
:   (*since AnalysisBase 25.2.11*)
    a dictionary with key (string) a trigger set name and value a triggerChainsPerYear dictionary, following the previous convention. Relevant for analyses using different triggers in different categories, where the trigger global scale factors shouldn't be combined. The default is {} (empty dictionary).

    ??? success "Example"
        If leading lepton pT>XX, check single lepton trigger / else check di-lepton trigger.
        ```
        multiTriggerChainsPerYear:
            'SLT':
                '2015':
                    - 'HLT_mu26_ivarmedium'
                '2016':
                    - 'HLT_mu26_ivarmedium'
                '2017':
                    - 'HLT_mu26_ivarmedium'
                '2018':
                    - 'HLT_mu26_ivarmedium'
            'DLT':
                '2015':
                    - 'HLT_2mu14'
                '2016':
                    - 'HLT_2mu14'
                '2017':
                    - 'HLT_2mu14'
                '2018':
                    - 'HLT_2mu14'
        ```

!!! warning
    The option `triggerChainsPerYear` and `multiTriggerChainsPerYear` are mutually exclusive.

`triggerChainsForSelection`
:   a list of trigger chains (list of strings) to be used for trigger selection. Only set it if you need a different setup than for trigger SFs.
The default is `[]` (empty list).

`prescaleLumiCalcFiles`
:   a list of lumical files (list of strings) to calculate trigger prescales. The default is `[]` (empty list).

`prescaleTriggersFormula`
:   a formula used in (un)prescaling, producing overall prescale factor instead of prescale per trigger. The default is `''` (empty string).

`prescaleMC`
:   whether to prescale MC instead of unprescaling of data. The default is `False`.

`noFilter`
:   do not apply an event filter. The default is `False`, i.e. remove events not passing trigger selection and matching.

`electronID`
:   the electron ID WP (string) to use.

`electronIsol`
:   the electron isolation WP (string) to use.

`muonID`
:   the muon quality WP (string) to use.

`photonIsol`
:   the photon isolation WP (string) to use.

`electrons`
:   the input electron container, with a possible selection, in the format `container` or `container.selection`.

`muons`
:   the input muon container, with a possible selection, in the format `container` or `container.selection`.

`photons`
:   the input photon container, with a possible selection, in the format `container` or `container.selection`.

`noEffSF`
:   disables the calculation of efficiencies and scale factors. Experimental! only useful to test a new WP for which scale factors are not available. Still performs the global trigger matching (same behaviour as on data). The default is `False`.

`noGlobalTriggerEff`
:   disables the global trigger efficiency tool (including matching), which is only suited for electron/muon/photon trigger legs. The default is `False`.

`noL1`
:   toggle off the L1 trigger decision. This flag is required e.g. for running the algorithm on TLA stream data. The default is `False`.

`triggerMatchingChainsPerYear`
:   a dictionary with key (string) the year and value (list of strings) the trigger chains. The default is `{}` (empty dictionary).

`includeAllYears`
:   whether to include all years for trigger matching in all jobs. The default is `False`.

`postfix`
:   a unique identifier for the trigger matching decorations. Only useful when defining multiple setups. The default is `''` (empty string).

!!! note
    The `postfix` option allows you to generate multiple types of global trigger matching flag, which you can then call as `GLOBALTRIGMATCH <postfix>` in the [`EventSelection:`](../settings/eventselection.md) config block.

!!! success "Registers the following variables:"
    - `trigPassed`: whether a specific trigger is passed (no systematics)
    - `globalTriggerMatch`: whether the event passes trigger matching requirements
    - `globalTriggerEffSF`: the event-level global trigger efficiency SF

!!! success "Additional variables toggled by `triggerMatchingChainsPerYear`:"
    - `trigMatched`: whether a specific trigger is matched (no systematics)

### [ElectronTriggerAnalysisSFBlock](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/EgammaAnalysisAlgorithms/python/ElectronAnalysisConfig.py)
This config block allows you to retrieve the per-electron trigger SFs.
(*since AnalysisBase 25.2.14*)

Name in YAML: **Electrons.TriggerSF**

`triggerChainsPerYear`
:   a dictionary with key (string) the year and value (list of strings) the trigger chains. The default is `{}` (empty dictionary).

`electronID`
:   the electron ID WP (string) to use.

`electronIsol`
:   the electron isolation WP (string) to use.

`saveEff`
:   whether we decorate the trigger MC efficiencies. The default is `False`.

`prefixSF`
:   the decoration prefix for trigger scale factors. The default is `'trigEffSF'`.

`prefixEff`
:   the decoration prefix for triger MC efficiencies. The default is `'trigEff'`.

`prefixEffData`
:   the decoration prefix for trigger data efficiencies. The default is `'trigEffData'`.

`includeAllYears`
:   whether to configure all years in all jobs. The default is `False`.

`removeHLTPrefix`
:   whether to remove the HLT prefix from trigger chain names. The default is `True`.

`useToolKeyAsOutput`
:   whether to use the tool trigger key as output. The default is `False`.

`containerName`
:   the input electron container, with a possible selection, in the format `container` or `container.selection`.

!!! success "Registers the following variables:"
    - `<prefixSF>`: the per-electron trigger efficiency SF
    - `<prefixEff>`: the per-electron trigger MC efficiency (if `saveEff`)

### [MuonTriggerAnalysisSFBlock](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/MuonAnalysisAlgorithms/python/MuonAnalysisConfig.py)
This config block allows you to retrieve the per-muon trigger SFs.
(*since AnalysisBase 25.2.14*)

Name in YAML: **Muons.TriggerSF**

`triggerChainsPerYear`
:   a dictionary with key (string) the year and value (list of strings) the trigger chains. The default is `{}` (empty dictionary).

`muonID`
:   the muon quality WP (string) to use.

`saveSF`
:   whether to decorate the trigger scale factor. The default is `True`.

`saveEff`
:   whether to decorate the trigger MC efficiencies. The default is `False`.

`saveEffData`
:   whether to decorate the trigger data efficiencies. The default is `False`.

`prefixSF`
:   the decoration prefix for trigger scale factors. The default is `'trigEffSF'`.

`prefixEff`
:   the decoration prefix for triger MC efficiencies. The default is `'trigEff'`.

`prefixEffData`
:   the decoration prefix for trigger data efficiencies. The default is `'trigEffData'`.

`includeAllYears`
:   whether to configure all years in all jobs. The default is `False`.

`removeHLTPrefix`
:   whether to remove the HLT prefix from trigger chain names. The default is `True`.

`containerName`
:   the input muon container, with a possible selection, in the format `container` or `container.selection`.

!!! success "Registers the following variables:"
    - `<prefixSF>`: the per-muon trigger efficiency SF (if `saveSF`)
    - `<prefixEff>`: the per-muon trigger MC efficiency (if `saveEff`)
    - `<prefixEffData>`: the per-muon trigger data efficiency (if `saveEffData`)

### [TauTriggerAnalysisSFBlock](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/TauAnalysisAlgorithms/python/TauAnalysisConfig.py)
This config block allows you to retrieve the per-tau-jet trigger SFs.
(*since AnalysisBase 25.2.13*)

Name in YAML: **TauJets.TriggerSF**

`triggerChainsPerYear`
:   a dictionary with key (string) the year and value (list of strings) the trigger chains. The default is `{}` (empty dictionary).

`prefixSF`
:   the decoration prefix for trigger scale factors. The default is `'trigEffSF'`.

`includeAllYears`
:   whether to configure all years in all jobs. The default is `False`.

`removeHLTPrefix`
:   whether to remove the HLT prefix from trigger chain names. The default is `True`.

`tauID`
:   the tau quality WP (string) to use.

`prefixSF`
:   the decoration prefix for trigger scale factors. The default is `'trigEffSF'`.

`includeAllYears`
:   if `True`, all configured years will be included in all jobs. The default is `False`.

`removeHLTPrefix`
:   remove the HLT prefix from trigger chain names. The default is `True`.


`containerName`
:   the input tau-jet container, with a possible selection, in the format `container` or `container.selection`.

!!! success "Registers the following variables:"
    - `<prefixSF>`: the per-tau-jet trigger efficiency SF
