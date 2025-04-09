## Software management

### Starting a new analysis

#### If you do not need to modify TopCPToolkit

An example [skeleton package](https://gitlab.cern.ch/atlasphys-top/reco/HowToExtendTopCPToolkit) is provided to help set up your own package to work with the TopCPToolkit. After setting up TopCPToolkit, you can download the skeleton package under `TopCPToolkit/source/`. Steps to install and use the skeleton package can be found in its [README](https://gitlab.cern.ch/atlasphys-top/reco/HowToExtendTopCPToolkit#howtoextendtopcptoolkit).

#### If you plan to merge your code back to TopCPToolkit

You should create your own [fork](https://docs.gitlab.com/ee/user/project/repository/forking_workflow.html) of this repository.
When submitting a merge request to include a feature centrally, you can always change the target repository to this one.

Then make your own folder under [`share/configs`](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/share/configs/).
You may take some inspiration from the [example configs](../starting/analysis.md#example-yaml-configs).

### Keeping the code up-to-date

You can avoid manual updates by setting up [repository mirroring](https://docs.gitlab.com/ee/user/project/repository/mirror/).
In your forked repository, you should be able to enable a **pull mirroring** which will periodically update the `main` branch and the various tags.

!!! warning
    When you do so, you can't use the `main` branch anymore on your fork!
    You should always work on a separate branch anyway.

### Keeping yourself up-to-date

Subscribe to our [Mattermost channel](../index.md#get-in-touch) and check out the [Changelog](../changelog/index.md) once in a while!

### Submitting a merge request

You can submit a MR directly, but it's always nicer if you can explain things a little bit more first in an [issue ticket](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/issues).
See also the [Contributing page](../contributing/index.md).

## Running an ntuple production

### Optimise the Grid settings

With the large and sometimes wildly fluctuating size of `DAOD_PHYS` (and `DAOD_PHYSLITE` to a lesser extent) files, it can be a bit tricky to get productions on the Grid to go smoothly.

We recommend taking advantage of [all the options](../starting/running_grid.md#configuring-the-grid-submission-script) offered by our submission script based on [`prun`](https://panda-wms.readthedocs.io/en/latest/client/prun.html).
When running with full systematics and some form of event reconstruction, it may be necessary to set e.g. `--nFilesPerJob` or `--maxNFilesPerJob` in `otherOptions`.

It is also possible to **ask for the ntuple production to be centrally run!**
See [this presentation](https://indico.cern.ch/event/1328739/#1-centralized-n-tuple-producti) and [this guide](https://atlassoftwaredocs.web.cern.ch/guides/ntuples_production/) to using the centralised production services.

### Reduce the size of the ntuples

It is _**strongly recommended**_ that you run your analysis locally on a few test files before attempting to submit the Grid jobs in bulk.
This could be e.g. one signal file, one background file, and one data file.
Making sure you can run the various MC campaigns, or with Run 2 vs Run 3 settings, is also a good idea.

You should then very carefully inspect the structure of the output.
Here we recommend discussing with your fellow analysis team members to ensure everyone is happy with the result (and better two pairs of eyes than one!).
The main three questions to keep in mind are:

- **do I have all the information I need?**
    - don't forget about systematics, scale factors, selection flags...
    - MC weights and their CutBookkeepers
    - bootstraps for data (**analysis recommendation**), optionally for signal MC
- **do I have only the information I need?**
    - make aggressive use of the options for [systematic](../starting/running_local.md#filtering-systematics) and [branch filtering](../starting/running_local.md#filtering-output-branches)
    - remove any block you're not actually using, as most blocks come with variables being registered by default!
    - **per-object scale factors should be avoided**, as in most cases a [per-event scale factor](../settings/scalefactors.md) can be achieved (e.g. flavour tagging, leptons)
- **is my event selection appropriate?**
    - running the [event selection](../settings/eventselection.md) is very inexpensive and can save a lot of space!
    - while its main use is to perform a general preselection or first region definition, to be refined offline, you could also code your own analysis-specific final selection
    - if you are only interested in skimming the ntuples with the event selection, you could also filter out the `pass_*_%SYS%` branches as these tend to collect a lot of systematic variations

### Speed up the execution

Does every single one of your analysis-specific algorithms need to be run on every event or every object?
If not, you should give their config blocks configurable properties to pass object- or event-wise selections.
This is covered in [part 2 of the general tutorial](../tutorials/write_algorithm.md#coding-the-physics).

## Writing new algorithms

### Do one thing well

In following with (part of) the Unix philosophy, **write algorithms to perform a single main task**.
This helps keep the system modular and adapt to new changes much more easily.
It also makes it possible for other developers and experts to help you debug and provide feedback.

The alternative is of course to write your entire analysis as one huge algorithm.
Do not expect any help from anyone else if that is the path you choose.

### Expose properties liberally

You may have always worked with the same name for jet collections, or only ever used a single type of overlap removal, so it feels "normal" to hardcode these names in the various handles of your algorithms.

Instead, why not **keep other users in mind** and make it so that everything about your algorithm is customisable?
By relying on systematic handles and property declarations, this is almost there for free anyway!
All you need is to expose these options in your `ConfigBlock`.

!!! tip
    When adding properties to your `ConfigBlock`, you can use the `info=''` argument to write a short description of what should go into that property.

### Favour config blocks over make-methods

Apart from a few complicated examples related to jet calibration and triggering, everything else can be done exclusively using `ConfigBlock`s instead of "make-methods".
Try as much as possible to do the same in your analysis code.