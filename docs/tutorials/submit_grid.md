!!! abstract "In this session you will..."
    - Submit jobs on the grid
    - Search and add samples for the grid jobs
    - Monitor and diagnose grid job issues
    - Download output files from the grid

_For this part, we assume you have successfully built the project following the ["Initial setup"](setup.md) instructions._

Pretty much any analysis will need to run TopCPToolkit on the grid simply due to the huge disk size of the derivations and the processing time required.

## Preparing for grid submission

TopCPToolkit contains a bunch of helper python scripts to ease the submission of grid jobs, which are located in the [GridSubmission](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/tree/main/source/GridSubmission) directory. To obtain copies of these scripts for yourself:
```bash
cd run # go to the run sub-directory in the directory where you set things up
getExamples
```
This script will create a `grid` directory and copy a grid job submission script as well as two sample files into `grid`.

Additionally, we will need to setup:
```bash
lsetup "rucio -w"
lsetup panda pyami
voms-proxy-init -voms atlas # set up grid proxy, enter password for your grid certificate, if asked
```

### Grid submission scripts configuration

Open the copied `submitToGrid.py` and examine its contents. Firstly, the `config.code` option controls what command will be used for execution on grid, i.e.:
```python
config.code = 'runTop_el.py --no-systematics --parton --particle'  # run nominal only with parton and particle level
```
Whenever preparing a config file for grid jobs, there are some **important considerations** to keep in mind:

- Are you accidentally including e.g. `-e 100` in the command to run only for a small number of events? Perhaps you ran a local check with a few hundred events, but on grid you in general want to run over all events available in a dataset.
- Do you need to run systematic uncertainties or not ? Running with systematics takes order of magnitude longer, and the output is also typically order of magnitude larger, so consider if you need to run systematics or only some studies with nominal TTree.

Next, you should set the `config.gridUsername` to your CERN account (what you use for login to lxplus) name and `config.suffix` to something unique and understandable (e.g. put date or release into the suffix). Here the task at grid will have name composed of the dataset name and the suffix, and note that you cannot submit jobs with the same name multiple times.

One of the greatest difficulties with efficient running on grid is determining number of files a single job should process, since grid sites have limits on maximum job time, as well as memory and storage constraints. Panda tries to determine this automatically, but you do have the option to set an upper limit per job, for example:
```python
config.maxNFilesPerJob = '4'
```
!!! tip
    You may want to run with a very small number of `maxNFilesPerJob`, given that the PHYS derivations have a lot of events per file (and also quite a lot of variance).

Internally, the grid submission scripts call the `prun` command for job submission to grid, so any other options relevant to the prun (e.g. see this [page](https://panda-wms.readthedocs.io/en/latest/client/prun.html)) can be supplied via:
```python
config.otherOptions = '' # anything that prun accepts
```

Next up, the submission script contains a list sample groups which will be submitted:
```python
names = ['PHYS_ttbar_PP8_mc20e']
```
Each entry represents one group of samples defined in the `MC20_PHYS.py` and `MC23_PHYS.py` files, which we copied as well. You can create your own list as well based on these examples and import it in the grid submission script. For example, the `MC20_PHYS.py` list contains entries such as:
```python
GridSubmission.grid.Add('PHYS_ttbar_PP8_mc20e').datasets = [
    'mc20_13TeV.410470.PhPy8EG_A14_ttbar_hdamp258p75_nonallhad.deriv.DAOD_PHYS.e6337_s3681_r13145_p5855'
]
```
where the `PHYS_ttbar_PP8_mc20e` defines a group of datasets, and those are specified in the python list assigned.

Obviously, the files we copied are only how-to examples, you have to make sure to use up-to-date samples. Let's do a simple check, whether the sample above is a reasonable choice.

Type the following:
```bash
rucio list-dids mc20_13TeV.410470.PhPy8EG_A14_ttbar_hdamp258p75_nonallhad.deriv.DAOD_PHYS.e6337_s3681_r13145_p5855
```
The command above lists samples matching the search pattern:
```bash
+---------------------------------------------------------------------------------------------------------------+-------------------+
| SCOPE:NAME                                                                                                    | [DID TYPE]        |
|---------------------------------------------------------------------------------------------------------------+-------------------|
| mc20_13TeV:mc20_13TeV.410470.PhPy8EG_A14_ttbar_hdamp258p75_nonallhad.deriv.DAOD_PHYS.e6337_s3681_r13145_p5855 | DIDType.CONTAINER |
+---------------------------------------------------------------------------------------------------------------+-------------------+
```
Note that the fact that a dataset container is listed by rucio does not mean the sample has not been deleted. To check that, do:
```bash
rucio list-files mc20_13TeV.410470.PhPy8EG_A14_ttbar_hdamp258p75_nonallhad.deriv.DAOD_PHYS.e6337_s3681_r13145_p4856
```
and you should get
```
+---------------------------------------------------+--------------------------------------+-------------+------------+----------+
| SCOPE:NAME                                        | GUID                                 | ADLER32     | FILESIZE   | EVENTS   |
|---------------------------------------------------+--------------------------------------+-------------+------------+----------|
| mc20_13TeV:DAOD_PHYS.34865851._000001.pool.root.1 | A9B6EAAC-51A4-614E-8FFD-ADE98D610F36 | ad:12d4865c | 1.169 GB   | 30000    |

...

| mc20_13TeV:DAOD_PHYS.34865833._000917.pool.root.1 | 9741DB52-4D25-B74F-9E73-C808E066C149 | ad:c7bc3d89 | 1.558 GB   | 40000    |
+---------------------------------------------------+--------------------------------------+-------------+------------+----------+
Total files : 2251
Total size : 7.678 TB
Total events : 197768000
```
This derivation is indeed available! Should we have checked an older version (by using a smaller p-tag), for example:
```
rucio list-files mc20_13TeV.410470.PhPy8EG_A14_ttbar_hdamp258p75_nonallhad.deriv.DAOD_PHYS.e6337_s3681_r13145_p4856
```
we would get an empty list
```
+--------------+--------+-----------+------------+----------+
| SCOPE:NAME   | GUID   | ADLER32   | FILESIZE   | EVENTS   |
|--------------+--------+-----------+------------+----------|
+--------------+--------+-----------+------------+----------+
Total files : 0
Total size : 0.000 B
```
which means this derivation has been deleted.

!!! example "Exercise"
    What is the latest p-tag for this sample?

??? success "Solution"
    Rucio accepts wildcards. Try
    ```
    rucio list-dids mc20_13TeV.410470.PhPy8EG_A14_ttbar_hdamp258p75_nonallhad.deriv.DAOD_PHYS.e6337_s3681_r13145_p*
    ```
    and look for the one with the largest p-tag.

Different MC campaigns exist which correspond to different years of data-taking and different releases. In particular, MC campaign MC20 corresponds to Run-2 data and release 22 (R22), and MC campaign MC23 corresponds to Run-3 data and release 25 (R25). See [here](https://twiki.cern.ch/twiki/bin/view/AtlasProtected/DataMCForAnalysis#Corresponding_MC_campaigns_for_d) for a table of summary.

For both release 22 and release 25, two derivation formats are available: `DAOD_PHYS` and `DAOD_PHYSLITE`. TopCPToolkit can work with either format. More information on `DAOD_PHYS` and `DAOD_PHYSLITE` can be found [here](https://twiki.cern.ch/twiki/bin/view/AtlasProtected/DAODPhys).

The [Physics Modelling Group (PMG)](https://twiki.cern.ch/twiki/bin/view/AtlasProtected/PhysicsModellingGroup) group maintains information about recommended MC samples:
- [Central page for MC20](https://twiki.cern.ch/twiki/bin/view/AtlasProtected/CentralMC20ProductionListNew)
- [Central page for MC23](https://twiki.cern.ch/twiki/bin/view/AtlasProtected/CentralMC23ProductionListNew)

If we don't know the full dataset name, it is usually possible to search it out via rucio following some assumptions. Let's assume that the derivation format we want is `DAOD_PHYSLITE`, and the DSID is 601229 (everyone's favorite PowhegPythia semi-leptonic ttbar sample in MC23). To obtain simulation for currently available Run-3 datasets, we will need two samples corresponding to the 2022 (mc23a) and 2023 (mc23d) data-taking period. This can be distinguished based on the r-tag, which is r14622 and r15224, respectively.

!!! example "Exercise"
    Let's search for the 601229 mc23a samples.

??? tip "Hint"
    The scope for MC23 samples is `mc23_13p6TeV`. The part indicating the derivation format in the sample name we are looking for is `deriv.DAOD_PHYSLITE`. Also it is useful to add the option `--filter 'type=CONTAINER'` to only list the dataset container.

??? success "Solution"
    ```bash
    rucio list-dids --filter 'type=CONTAINER' 'mc23_13p6TeV.601229*deriv.DAOD_PHYSLITE*r14622*'
    ```
    The output of the command above may yield a list of samples:
    ```
    +------------------------------------------------------------------------------------------------------------------------------------------+-------------------+
    | SCOPE:NAME                                                                                                                               | [DID TYPE]        |
    |------------------------------------------------------------------------------------------------------------------------------------------+-------------------|
    | mc23_13p6TeV:mc23_13p6TeV.601229.PhPy8EG_A14_ttbar_hdamp258p75_SingleLep.deriv.DAOD_PHYSLITE.e8514_e8528_s4162_s4114_r14622_r14663_p5855 | DIDType.CONTAINER |
    | mc23_13p6TeV:mc23_13p6TeV.601229.PhPy8EG_A14_ttbar_hdamp258p75_SingleLep.deriv.DAOD_PHYSLITE.e8514_e8528_s4162_s4114_r14622_r14663_p6026 | DIDType.CONTAINER |
    | mc23_13p6TeV:mc23_13p6TeV.601229.PhPy8EG_A14_ttbar_hdamp258p75_SingleLep.deriv.DAOD_PHYSLITE.e8514_s4162_r14622_p5855                    | DIDType.CONTAINER |
    | mc23_13p6TeV:mc23_13p6TeV.601229.PhPy8EG_A14_ttbar_hdamp258p75_SingleLep.deriv.DAOD_PHYSLITE.e8514_s4162_r14622_p6026                    | DIDType.CONTAINER |
    +------------------------------------------------------------------------------------------------------------------------------------------+-------------------+
    ```

Information on how a sample was processed is encoded in its tags. In the following a short description of of the individual tags is given and standard tags of each MC (sub-)campaign are listed.

#### Simulation

Simulation tags start with `s` are for full simulation (FS) with Geant4, and tags start with `a` are for fast simulation samples produced with the fast simulation AtlFast3 (AF3) with R22 or R25.

- [MC20](https://twiki.cern.ch/twiki/bin/view/AtlasProtected/AtlasProductionGroupMC20#Simulation):
    - FS: `s3681`
    - AF3: `a907`
- [MC23a](https://twiki.cern.ch/twiki/bin/view/AtlasProtected/AtlasProductionGroupMC23a#Hard_scatter_simulation):
    - FS: `s4162` (AMI tag), `s4114` (merging tag)
    - AF3: `a910` (AMI tag), `s4114` (merging tag)
- [MC23d](https://twiki.cern.ch/twiki/bin/view/AtlasProtected/AtlasProductionGroupMC23d#Hard_scatter_simulation):
    - FS: `s4159` (AMI tag), `s4114` (merging tag)
    - AF3: `a911` (AMI tag), `s4114` (merging tag)

Physics considerations need to be made here, which choice is feasible. Full-sim is generally preferred for physics description (e.g. for example for description of substructure of jets, since fast simulation performs approximations of calorimeter shower evolution), but is much more CPU-expensive to simulate, therefore for some processes there may not be any full-sim samples generated

#### Reconstruction:

- [MC20](https://twiki.cern.ch/twiki/bin/view/AtlasProtected/AtlasProductionGroupMC20#Reconstruction):
    - Nominal:
        - MC20a: `r13167`
        - MC20d: `r13144`
        - MC20e: `r13145`
        - MC20a (AF3 only): `r14859`
        - MC20d (AF3 only): `r14860`
        - MC20e (AF3 only): `r14861`
    - Without pile-up:
        - MC20a: `r13297`
        - MC20d: `r13298`
        - MC20e: `r13299`
        - MC20a (AF3 only): `r14862`
        - MC20d (AF3 only): `r14863`
        - MC20e (AF3 only): `r14864`
- [MC23a](https://twiki.cern.ch/twiki/bin/view/AtlasProtected/AtlasProductionGroupMC23a#Reconstruction):
    - Nominal: `r14622` (AMI tag), `r14663` (merging tag)
    - Without pile-up: 	`r14716` (AMI tag), `r14663` (merging tag)
    - Nominal (AF3 only): `r14932` (AMI tag), `r14663` (merging tag)
    - Without pile-up (AF3 only): `r14933` (AMI tag), `r14663` (merging tag)

- [MC23d](https://twiki.cern.ch/twiki/bin/view/AtlasProtected/AtlasProductionGroupMC23d#Reconstruction):
    - Nominal: `r15224` (AMI tag), `r15225` (merging tag)
    - Without pile-up: 	`r15439` (AMI tag), `r15225` (merging tag)

!!! note
    More information on the `a,s,r` tags including the non-standard ones can be found on the [ATLAS Production Group twiki](https://twiki.cern.ch/twiki/bin/view/AtlasProtected/AtlasProductionGroup#Specific_Information_on_MC_campa).

As for the various p-tags, typically the newer p-tag the better, and in general samples with older p-tags are obsoleted and deleted unless exceptions are requested. To find out what AthDerivation release was used to produce a given p-tag sample, and what are the changes in the release, refer to [DerivationProduction twiki](https://twiki.cern.ch/twiki/bin/view/AtlasProtected/DerivationProductionTeam).

!!! tip
    The following resources may prove invaluable when dealing with datasets:

    - [This page](https://twiki.cern.ch/twiki/bin/view/AtlasProtected/TopDerivationsRun3) provides information on the derivation production of the ATLAS top group.
    - For data derivation lists, see [TopGroupDataDerivationList page](https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/TopGroupDataDerivationList).
    - [AMI website](https://ami.in2p3.fr/) for metadata relevant for various tags and samples.
    - AMI information can be obtained also via command line tool ami (which we set up via `lsetup pyami`), see some documentation [here](https://ami.in2p3.fr/pyAMI/).

Finally, let's test actual dataset submission. We will not be actually running the sample on grid, only to check whether the submission command looks ok. For this purpose, in the submission script, set:
```python
config.gridUsername = <username>
```
where `<username>` is your `lxplus` username and
```python
config.noSubmit = True
```

Let's try to execute in shell:
```bash
python grid/submitToGrid.py
```
You should see the following output:
```bash
PHYS_ttbar_PP8_mc20e (1 dataset)
 - mc20_13TeV.410470.PhPy8EG_A14_ttbar_hdamp258p75_nonallhad.deriv.DAOD_PHYS.e6337_s3681_r13145_p5855 (found, latest p-tag: p6026)
Analysis Settings:
 -Code:           runTop_el.py --no-systematics --parton --particle
Grid Settings:
 -GridUsername:   username
 -Suffix:         230320-v0
 -ExcludedSites:
 -ForceSite:
 -NoSubmit:       True
 -MergeType:      Default out of (None, Default, xAOD)
 -memory:         2000 in MB
 -maxNFilesPerJob
 -OtherOptions:
 -nameShortener:  <bound method basicInDSNameShortener of <GridSubmission.grid.Config object at 0x7fddac9813a0>>
 -reuseTarBall:   False
 -DestSE          <not set>

For these samples

Starting submission of 1 sample

Submitting 1 of 1
prun \
--inDS=mc20_13TeV.410470.PhPy8EG_A14_ttbar_hdamp258p75_nonallhad.deriv.DAOD_PHYS.e6337_s3681_r13145_p5855 \
--outDS=user.username.410470.PhPy8EG.DAOD_PHYS.e6337_s3681_r13145_p5855.230320-v0 \
--useAthenaPackages --cmtConfig=x86_64-el9-gcc13-opt \
--writeInputToTxt=IN:in.txt \
--outputs=output:output.root \
--exec="runTop_el.py --no-systematics --parton --particle -i in.txt -o output" \
--outTarBall=top-el.tar.gz \
--noSubmit \
--mergeOutput \
--memory=2000 \
```
If you haven't specified your username you will see a corresponding error.

As you can see, what the script does is to list your settings and then put everything together in a `prun` command.

From `outDS` parameter, you can see the name that the output dataset will have (the dataset for the root files will be e.g. in this example above called `user.username.410470.PhPy8EG.DAOD_PHYS.e6337_s3681_r13145_p5855.230320-v0`, the username and the suffix depending on what you specify in the submission script.

To make sure the jobs do not fail on the grid, the submission script will automatically check that the datasets that you want to run over are available.

### Handling external files during grid submission

In certain cases, external files are required for running jobs on the grid. In this case, the files usually must be properly specified in the grid submission scripts and correct path must be used in the configuration file.

Firstly, **panda often likes to skip external files**, especially `.root` files and files above certain size limit (the logic isn't entirely trivial). The files to be included must be specified via `--extFile argument`. In your grid submission python script, you need:
```python
config.otherOptions = '--extFile=file1,file2,...' # just the names of the files should be sufficient, but relative paths should also (probably) work
```

Secondly, **never use absolute paths**, e.g. `/afs.cern.ch/user/...` in config files, as the directory structure on a grid site is different, the contents of the files sent to grid are extracted to some path on grid that you in general have no control over.

### Other useful features of the grid submission scripts

There are three extra settings supported in the grid submission scripts, that may prove usefull:

- To exclude sites which you don't want the jobs to run on (e.g. they may be problematic due to whatever reason):
```python
config.excludedSites = 'SITE1,SITE2' #comma-separeted list of site names
```
You can find site names on [bigpanda.cern.ch](https://bigpanda.cern.ch)

- To store grid job output at a specific destination (storage element) (see [https://rucio-ui.cern.ch/](https://rucio-ui.cern.ch/)):
```python
config.destSE = 'SOMESTORAGEELEMENT' (for example AGLT2_LOCALGROUPDISK)
```
!!! note
    Where you can store files (and how much in terms of size) is subject to permissions and grid quotas.

- To speed-up submission, you can re-use existing compressed `top-el.tar.gz` archive. **Beware** it is easy to forget about this if you change some files and forget to remove the `top-el.tar.gz`, hence your jobs will not be re-submitted with new input!
```python
config.reuseTarBall = True
```

## Monitoring and diagnosing issues with grid jobs

Usually within a few minutes since grid job submission, the jobs should start appearing on [bigpanda web page](https://bigpanda.cern.ch/user/). This web page allows one to check progress of jobs, retrieving logfiles (necessary for investigating failing jobs) and lot of other related stuff. Logs of jobs can be accesed by opening a panda task and then clicking the number of finished or failed jobs, and then scrolling down to individual PanDA IDs of the jobs. Clicking the ID yields the job page where logs and memory and IO plots (usefull for e.g. detecting memory leaks) can be accessed.

In addition, a command-line tool for managing running jobs is available, [pbook](https://twiki.cern.ch/twiki/bin/view/AtlasComputing/PandaBook), which allows for basic operations, such as retry and kill jobs specified by task ID number (reminder, it is setup via `lsetup panda`):
```python
retry(taskID)
kill(taskID)
kill([taskID1,taskID2]) # basically this is a python prompt so anything that yields a python array is valid
```

### Frequently-encountered issues

 The information above is a baseline that should be a starting point for investigating issues with jobs. We will try to expand on some typical issues when running on grid below. Note that this section is more like a short reference rather than an interactive tutorial (and yes, we are not kidding about the briefness of this section when considering how much stuff can go wrong on grid...)

- **I changed my code/scripts, submitted another task, but my changes are not reflected at grid!** Check if you have `config.reuseTarBall=True` option in the submission script. If yes, this means that if a compressed tarball that is sent to grid exists in the directory (`top-el.tar.gz`) it has been re-uploaded without actually updating with your changes! This option is very nice when submitting many jobs at once, but if you use it, make sure to always delete the `top-el.tar.gz` tarball before you submit new tasks with changes.

- **Jobs take forever to start.** This can be seen by large time to start of the job. This could be related to your priority (check the priority of the jobs running on the site). If this is not the case, the site may be busy (too many jobs in the site queues) or some other issue (in which case it may be advisable to ask the DAST experts -- see [DAST twiki](https://twiki.cern.ch/twiki/bin/viewauth/AtlasComputing/AtlasDAST) for more info, and for mailing lists for support).

- **Jobs crash.** In this case the error message displayed in the job list may or may not be helpfull, in any case it is useful to examine the log files. Log files can be accessed by opening the specific crashing job and examining log files (log files on panda), in particular the `setup.stdout` or `payload.stdout` log files, that include output from TopCPToolkit run (or any other task you run on grid in general).

- **Jobs take forever to run or crash due to exceeding site walltime limit.** Check if the job is running long for the first time, or if it has been retried. If the latter, this may indicate some issue with the code (a crash perhaps, see point above), or some issue related to the grid site potentially. Note that all grid sites have a limit on maximum time of a job (walltime). Thus you may have an issue due to your job being simply too long -- either the time to process an event is on average very high and/or the number of events to process in a single job is very high. This can become an issue in particular, if you are processing many systematics and/or using CPU-intensive algorithms, such as complex kinematic reconstruction algorithms (e.g. KLFitter, NeutrinoWeighting, etc). In this case, lowering the number of files per job may be desired. When submitting a sample to grid, this can be achieved by the `config.maxNFilesPerJob` setting, limiting the maximum amount of files per job. Alternatively, if you are retrying a job via pbook, it is possible to use the `nFilesPerJob` option specifying exact number of files per job (note, that you cannot use maxNFilesPerJob in pbook):
    ```python
    retry(taskID, newOpts={'nFilesPerJob':2})
    ```
    Ideally, it is useful to download a single file from the offending sample and try to run it locally to get an idea how long does it take to process N events and guesstimate the number of files accordingly to fit into the site walltime. For full-systematics NTuple production with KLFitter for instance, it is not unexpected having to run with single file per job for signal samples (and any samples that have high event selection efficiency in general)!

- **Broken tasks.** Each task runs several (typically 10?) scout jobs, which are used to check if the task does not crash, if the site meets requirements such as sufficient memory, etc. If all of the scout jobs fail, the task is sent to broken state, and no further jobs of the task are executed. This calls for investigating why the jobs crashed and fixing the issue. Either your jobs have crashed due to your fault, or the crashes are site-related, in both cases you encounter an issue, where the job to be resubmitted needs a new unique name. It is possible to resubmit a job with identical name, by running the submitToGrid.py script with the specific sample and additional parameter (**DRAGONS AHEAD**):
    ```python
    config.newOpts = ' --allowTaskDuplication'
    ```
!!! note
    When downloading output from grid from duplicated task, output of all duplicates will be downloaded!** In case of a broken task, where all jobs crashed, there is no output, so in such a case it is safe to use this option. Otherwise, prepare for duplicate events. Additionally, in case of site-related issues, it may be necessary to exclude it using the `config.excludedSites` option in the submission script.

- **Retrying failed task in pbook on a different site.** Sometimes, the only way to overcome a problem is to retry a job on a different site. This can be done using pbook:
    ```python
    #to exclude sites XXX and YYY, where XXX and YYY must match the bigpanda name
    retry(taskID, newOpts={'excludedSite':'XXX,YYY'})
    ```

- **Debugging memory-related problems on bigpanda.** Majority of the grid sites have a limit on memory for single-core jobs of 2GB. Unfortunately, for analyses using complex computations and many objects, and precision recommendations (looots of systematics), this becoming more and more common of a problem. First, in case of memory leaks, typically panda will show warnings on the task details page about jobs consuming excessive amount of memory, or kill your jobs right away. It is possible to view the memory consumption and input/output (I/O) of job as a function of time in graphs in the detailed view of a specific job and opening the Memory and IO plots. There may be legitimate reasons for high memory consumption, as outlined below.


- **Large RAM consumption due to branch buffer optimization.** Another frequent issue in case of large NTuples (many systematics and or plenty of branches, and/or heavy branches, such as vectors) is the RAM consumption of the branch buffer optimization. This problem manifests itself by a sharp increase in RAM consumption after a certain amount of time, when output events were processed which are about to be written to file. By default TTree cache in memory and the output is written out every 1000 events. The easiest recommendation is to reduce the size of NTuples as much as possible and consider if some of the stored variables could be computed offline instead. Another option, if running with systematics, is to split systematics into multiple jobs.

## Downloading grid output

To download output from grid, we use `rucio` tool, which can be set up via
```bash
lsetup "rucio -w"
```
!!! note
    It is advisable to setup rucio as a wrapper or in a new shell. This is in particular true if you already have AnalysisBase set up in a shell, rucio has known compatibility issues with AB.

Downloading of a dataset:
```bash
rucio download output-dataset-name
```
where the output dataset name is typically of the format of user.USERNAME.DSID....tags.SUFFIX. Rucio accepts `*` wildcards, so you can batch download many samples at once, for example:
```bash
rucio download 'user.username.*230320-v0'
```
where in the submission script, the `config.suffix` was set to `230320-v0`. **NOTE** Always encompass the download argument in *single quotes*, because otherwise your shell may attempt to interpret the `*` characters.

!!! tip
    You may be interested in the how-to on replicating of datasets (including output) to various grid sites, described in [this tutorial](https://topreco-tutorials.docs.cern.ch/TopWorkshop2023/Storage/#replicating-datasets-on-the-top-grid-disks).

Sometimes, if downloading file fails repeatedly, one potential trick is to make a temporary replica (see the link above) to a different grid site scratch disk and repeat the download. If this fails, contacting DAST mailing list (`hn-atlas-dist-analysis-help@cern.ch`) may be appropriate.
