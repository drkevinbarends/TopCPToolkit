## Documentation

A comprehensive documentation about the TopCPToolkit package and how to run it can be found [here](https://topcptoolkit.docs.cern.ch/).

## Setting up the environment

Assuming you are working on Lxplus or an equivalent setup with access to CVMFS and if you are setting up for the first time, run:

```bash
source initialSetup.sh
```

If you restart your session or need to access your environment again, run:

```bash
source setup.sh
```

If you want to submit jobs on the grid (and if you are an ATLAS VOMS member), run:

```bash
source gridSetup.sh
```

## Running the package

First move into the "run" directory:

```bash
cd ../run
```

For the tWZ analysis, we have created a config file to run only over reco objects under source/TopCPToolkit/share/configs/tWZ. In order to run using this config, please do the following. Note: Please ensure you have an input dataset to run over (for local jobs) and the file locations stored in some text file, e.g., input.txt.

```bash
runTop_el.py --no-systematics -t tWZ -i input.txt -o output.root
```
