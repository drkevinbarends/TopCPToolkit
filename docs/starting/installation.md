Whether you intend to run locally or on the Grid, you will need to get the [source code for TopCPToolkit](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit) and compile it against a suitable software environment.
Here we guide you through the necessary first steps.

## Getting the code

Get a local copy of the git repository with:

=== "Kerberos"
    ```sh
    git clone https://:@gitlab.cern.ch:8443/atlasphys-top/reco/TopCPToolkit.git
    ```
=== "SSH"
    ```sh
    git clone ssh://git@gitlab.cern.ch:7999/atlasphys-top/reco/TopCPToolkit.git
    ```
=== "HTML"
    ```sh
    git clone https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit.git
    ```

and move into the `TopCPToolkit` directory with:
```
cd TopCPToolkit
```

The actual code lives in the top-level `source` folder; everything else is needed for e.g. CI tests, GitLab features, powering this website, etc.
Create the additional folders where we will compile the code and run some examples:
```sh
mkdir -p build run
```

Note that you are now on the `main` branch, which is used for development. To access a [stable release](../changelog/index.md), you need to switch to the corresponding tag:
```sh
git fetch -a
git checkout tags/vX.Y.Z -b mybranch
```

## Setting up the environment

Assuming you are working on Lxplus or an equivalent setup with access to CVMFS, all you need is the following:
=== "Simple"
    ```sh
    setupATLAS
    cd build
    ```
=== "Development"
    ```sh
    setupATLAS
    lsetup git -q
    cd build
    ```

followed by a suitable choice of Analysis release:
=== "New AnalysisBase setup"
    ```sh
    asetup AnalysisBase,25.2.X
    ```
=== "New AthAnalysis setup (with acmSetup)"
    ```sh
    acmSetup AthAnalysis,25.2.X
    ```
=== "Re-use existing setup"
    ```sh
    asetup --restore
    ```
=== "Re-use existing acmSetup"
    ```sh
    acmSetup
    ```

!!! tip
    Analysis releases in the 25.2.x series are [listed here](https://twiki.cern.ch/twiki/bin/view/AtlasProtected/AnalysisBaseReleaseNotes25pt2), together with the changes that went into them. Those that affect CP algorithms are tagged with `CPAlgorithms`.

For a fresh compilation, or to include new files from the `source/` folder:
=== "Simple (not necessary if you used acmSetup above)"
    ```sh
    cmake ../source
    ```
=== "Debugging"
    ```sh
    cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS='-g -O0' ../source
    ```

then compile and set up the new environment:
=== "Compile"
    ```sh
    make -j4
    source */setup.sh
    ```
=== "Compile with acmSetup"
    ```sh
    acm compile
    ```
=== "Re-use existing setup (not necessary with acmSetup)"
    ```sh
    source */setup.sh
    ```

And you're ready to go!

### Developing in VS Code [experimental]

TopCPToolkit is shipped with a VS Code configuration which is built alongside the rest of the package. To use it, setup the same Athena environment used to build the package, make sure you ran the `cmake` step in the build folder and then open the `TopCPToolkit` directory as a VS Code workspace. Do not configure the project via `CMake` in VS Code as this is not currently supported. If using a different workspace, then the following lines in `.vscode/settings.json` need to be updated accordingly:

```json
"python.defaultInterpreterPath": "<path to build/ide_python>",
"python.envFile": "<path to build/env.txt>",

"python.formatting.autopep8Path": "<path to build/ide_autopep8>",

"python.linting.flake8Path": "<path to build/ide_flake8>",
```

The default python interpreter will be linked to the corresponding python interpreter in Athena. This should enable code completion from Athena and from `TopCPToolkit`. If it does not, please [open an issue](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/issues).

To **disable** the IDE configuration in the build step, change the line in `source/CMakeLists.txt` from

```cmake
set( ATLAS_ENABLE_IDE_HELPERS ON CACHE BOOL "Enable IDE helpers" )
```

to

```cmake
set( ATLAS_ENABLE_IDE_HELPERS OFF CACHE BOOL "Enable IDE helpers" )
```

<!-- !!! tip
    Using VS Code? You may want to grab the config files we provide at `/afs/cern.ch/user/o/omajersk/public/TopCPToolKitStuff/.vscode/` (experimental!) :thinking_face: -->

## Expert mode: compiling against Analysis Release

!!! warning
    Only do this if you really have to... :slight_smile:

    Most common use cases: you need to run with a modified version of a tool/algorithm in [Athena](https://gitlab.cern.ch/atlas/athena/).

!!! tip
    This [cheatsheet](https://atlassoftwaredocs.web.cern.ch/gittutorial/GitCheatSheet.pdf) is very useful!

Get a local copy of the git repository with:

=== "Kerberos"
    ```sh
    git clone https://:@gitlab.cern.ch:8443/atlasphys-top/reco/TopCPToolkit.git TopCPToolkit_wAthena
    ```
=== "SSH"
    ```sh
    git clone ssh://git@gitlab.cern.ch:7999/atlasphys-top/reco/TopCPToolkit.git TopCPToolkit_wAthena
    ```
=== "HTTP"
    ```sh
    git clone https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit.git TopCPToolkit_wAthena
    ```

Set up a sparse-checkout of Athena under the directory `TopCPToolkit_wAthena/source/`:

```sh
cd TopCPToolkit_wAthena/source/
setupATLAS
lsetup git
```

!!! tip
    See the [ATLAS software git tutorial](https://atlassoftwaredocs.web.cern.ch/gittutorial/env-setup/) for more details on how to set up and fork Athena on gitlab, sparse checkout packages, and make a merge request.

```sh
git atlas init-workdir https://:@gitlab.cern.ch:8443/[YOUR_USER_NAME]/athena.git
cd athena/
git atlas addpkg [PACKAGE_NAMES]
```
The packages you might want to add probably live under [PhysicsAnalysis/Algorithms/](https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Algorithms/).

Remove the Athena project directory:
```sh
rm -r Projects/
```
Prepare the build area:
```sh
cd ../..
mkdir -p build && cd build
```
Switch to the relevant AnalysisBase branch or release/25.2.x tag, for example:
```sh
asetup AnalysisBase,main,latest
```

Compile and set up environment:
=== "Simple"
    ```sh
    cmake ../source
    make -j4
    source */setup.sh
    ```
=== "Debugging"
    ```sh
    cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS='-g -O0' ../source
    make -j4
    source */setup.sh
    ```