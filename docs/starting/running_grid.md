Once local runs on test samples provide a satisfactory output, it's time to take your analysis to the next level!
Typically, this means processing extensive collections of signal and background samples on the Grid.

## Obtaining Grid submission scripts

Examples are available; in your `run` directory, try
```sh
getExamples
```
This will create a sub-folder called `grid`, with several python files within.
You can also see them [here](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/tree/main/source/GridSubmission/scripts).

Most of them simply list MC and data containers, neatly organised into "samples", e.g. `PHYS_ttbar_PP8_mc20e`.
For your own analysis, you should define the necessary groups of samples you want to process.
The main script is `submitToGrid.py`, and we'll see shortly what it does and how to use it.

## Preparing the environment

You need the usual software setup (see [Installation](installation.md#setting-up-the-environment)), plus the following:
```sh
lsetup emi -q
voms-proxy-init -voms atlas
lsetup "rucio -w"
lsetup panda pyami
```

!!! note
    The command `lsetup "rucio -w` sets up [Rucio](https://rucio-ui.cern.ch/) as a wrapper. This is needed when AnalysisBase is set up.

## Configuring the Grid submission script

You'll need to edit the contents of `submitToGrid.py`.
This is a high-level driver that will eventually write out a [`prun` command](https://panda-wms.readthedocs.io/en/latest/client/prun.html).
Check the table below to see what you should edit and how.


You are now ready to run!
```sh
python submitToGrid.py
```

| **Setting**   | **Description** |
| ----------    | --------------- |
| `code`         | The `runTop_el.py` command to run on each sample (omitting the `-o` flag). |
| `outputName`   | The name of the output directory from EventLoop. Recommended to leave as `'output'`. |
| `gridUsername` | The username attached to your Grid certificate. Can also set group production roles. |
| `suffix`       | A tag to be added to the output container name. Recommended to make it informative. |
| `excludedSite` | Possible Grid sites to veto, can be left empty. |
| `noSubmit`     | Set it to True if you want to check your submission commands<br>before executing them (recommended). |
| `mergeType`    | Set to `'Default'` to reduce the number of files in the output containers, or `'None'` otherwise. |
| `destSE`       | Specify a destination Grid site to send the output containers, or leave empty (recommended). |
| `memory`       | Request more memory on the Grid sites, or leave empty for the default 2GB (recommended). |
| `otherOptions` | Any other options accepted by `prun`. |
| `names`        | A list of the samples to process. |
