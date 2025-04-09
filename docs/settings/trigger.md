## Config blocks

### [TriggerAnalysisBlock](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/TriggerAnalysisAlgorithms/python/TriggerAnalysisConfig.py) / [TriggerAnalysisSFBlock](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/TriggerAnalysisAlgorithms/python/TriggerAnalysisSFConfig.py)
This config block allows you to perform trigger decision for any trigger leg, and for electrons/muons/photons also trigger matching and the retrieval of the correct efficiencies and scale factors (especially for multi-lepton triggers).

Name in YAML: **Trigger**

!!! note
    The `ConfigFactory` name **Trigger** will actually schedule both the [TriggerAnalysisBlock](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/TriggerAnalysisAlgorithms/python/TriggerAnalysisConfig.py) and the [TriggerAnalysisSFBlock](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/TriggerAnalysisAlgorithms/python/TriggerAnalysisSFConfig.py) in the backend.
    This is what is needed for most users, and there are ways of turning off the SF part as described below.
    If you really need to access only one config block, you'll need to set them up yourself.

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

!!! success "Registers the following variables:"
    - `trigPassed`: whether a specific trigger is passed (no systematics)
    - `globalTriggerMatch`: whether the event passes trigger matching requirements
    - `globalTriggerEffSF`: the event-level global trigger efficiency SF

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
:   defines whether we decorate also the trigger scale efficiency. The default is `False` (save only SF).

`containerName`
:   the input electron container, with a possible selection, in the format `container` or `container.selection`.

!!! success "Registers the following variables:
    - `trigEffSF`: the per-electron trigger efficiency or efficiency SF
    - `trigEff`: the per-electron trigger efficiency (if `saveEff`)

### [MuonTriggerAnalysisSFBlock](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/MuonAnalysisAlgorithms/python/MuonAnalysisConfig.py)
This config block allows you to retrieve the per-muon trigger SFs.
(*since AnalysisBase 25.2.14*)

Name in YAML: **Muons.TriggerSF**

`triggerChainsPerYear`
:   a dictionary with key (string) the year and value (list of strings) the trigger chains. The default is `{}` (empty dictionary).

`muonID`
:   the muon ID WP (string) to use.

`electronIsol`
:   the electron isolation WP (string) to use.

`muonID`
:   the muon quality WP (string) to use.

`saveEff`
:   defines whether we decorate also the trigger scale efficiency. The default is `False` (save only SF).

`containerName`
:   the input electron container, with a possible selection, in the format `container` or `container.selection`.

!!! success "Registers the following variables:
    - `trigEffSF`: the per-muon trigger efficiency or efficiency SF
    - `trigEff`: the per-muon trigger efficiency (if `saveEff`)

### [TauTriggerAnalysisSFBlock](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/TauAnalysisAlgorithms/python/TauAnalysisConfig.py)
This config block allows you to retrieve the per-tau-jet trigger SFs.
(*since AnalysisBase 25.2.13*)

Name in YAML: **TauJets.TriggerSF**

`triggerChainsPerYear`
:   a dictionary with key (string) the year and value (list of strings) the trigger chains. The default is `{}` (empty dictionary).

`tauID`
:   the tau quality WP (string) to use.

`containerName`
:   the input tau-jet container, with a possible selection, in the format `container` or `container.selection`.

!!! success "Registers the following variables:"
    - `trigEffSF`: the per-tau-jet trigger SF
