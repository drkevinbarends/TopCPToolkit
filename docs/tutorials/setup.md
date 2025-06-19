All the tutorials listed here can be run within the same software environment.
Let's set it up now: **please make sure you are working in a clean shell**.

## Getting the code

We need to get a local copy of the [source code for TopCPToolkit](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit).
Download it with:

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

Note that we are now on the `main` branch, which is used for development.
For the purposes of this tutorial, we will use a [stable release](../changelog/index.md) instead:
```sh
git fetch -a
git checkout tags/v2.19.0
```

In most cases, an analysis team needs to implement and maintain its analysis-specific configurations and algorithms, which usually need not be merged back to the central TopCPToolkit repository. It is therefore a good practice to keep the analysis-specific code and the central TopCPToolkit separate. Doing so also makes it almost trivial to update TopCPToolkit to its latest version or to switch between different versions. For this reason, we provide a skeleton package [HowToExtendTopCPToolkit](https://gitlab.cern.ch/atlasphys-top/reco/HowToExtendTopCPToolkit), which we will be using for this tutorial.
To install, checkout the package under the `source` directory:

=== "Kerberos"
    ```sh
    git clone https://:@gitlab.cern.ch:8443/atlasphys-top/reco/HowToExtendTopCPToolkit.git source/HowToExtendTopCPToolkit
    ```
=== "SSH"
    ```sh
    git clone ssh://git@gitlab.cern.ch:7999/atlasphys-top/reco/HowToExtendTopCPToolkit.git source/HowToExtendTopCPToolkit
    ```
=== "HTML"
    ```sh
    git clone https://gitlab.cern.ch/atlasphys-top/reco/HowToExtendTopCPToolkit.git source/HowToExtendTopCPToolkit
    ```

We will switch to a tagged version for this tutorial as well:
```sh
cd source/HowToExtendTopCPToolkit
git fetch -a
git checkout tags/tutorial2025 -b mytutorial
cd ../..
```

## Setting up the environment

Assuming you are working on Lxplus or an equivalent setup with access to CVMFS, all you need is the following:
```sh
setupATLAS
cd build
asetup AnalysisBase,25.2.52
cmake ../source
make -j4
source */setup.sh
```

??? tip "Re-using an existing setup"
    You may want to come back to the tutorial at a later time, or start again from a clean shell. You don't have to recompile everything from scratch! Simply run
    ```sh
    setupATLAS -q
    cd build
    asetup --restore
    source */setup.sh
    cd ..
    ```
    and you will recover your working environment.

    **Tip**: put this code in a file `setup_env.sh` so you can easily call `source setup_env.sh`.

??? tip "Modifying files"
    Throughout the tutorials, you will be invited to make modifications to the package. If these modifications are made on files that need compiling (e.g. `.cxx` or `.h`), you will need run again
    ```sh
    cd build
    make -j4
    source */setup.sh
    cd ../
    ```

    **Tip**: put this code in a file `quick_compile.sh` so you can easily call `source quick_compile.sh`.

??? tip "Creating new files"
    As above, we need to recompile the package, but also tell CMake to discover these new files:
    ```sh
    cd build
    cmake ../source
    make -j4
    source */setup.sh
    cd ../
    ```
    When the changes are significant, it may sometimes be necessary to clear the `build` directory altogether and compile again from scratch.

    **Tip**: put this code in a file `full_compile.sh` so you can easily call `source full_compile.sh`.
