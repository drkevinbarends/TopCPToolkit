#!/bin/bash
echo "-----------------------------------------------"
echo "    Setting up the nightly release for 25.2   "
echo "-----------------------------------------------"
echo "    Entering Development directory...          "
echo "-----------------------------------------------"
thisDir=$PWD
echo "Export ATLAS_LOCAL_ROOT_BASE"
# Now run the enviroment setup
export ATLAS_LOCAL_ROOT_BASE='/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase'
env | grep ATLAS_LOCAL_ROOT_BASE
echo "Source setupATLAS"
source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh
echo "Sourced"

# Print info to get the most recent nightly as sometimes it is not built
REL=`ls -t -d /cvmfs/atlas-nightlies.cern.ch/repo/sw/main_AthAnalysis_x86_64-el9-gcc13-opt/*-* | head -1 | xargs basename`
echo "-> Setup nightly... $REL"
echo "--------------------------------------------------------"
echo "-> Running command : asetup AthAnalysis,main,$REL,here"
echo "--------------------------------------------------------"
asetup AthAnalysis,main,$REL,here
echo " AthAnalysis,main,$REL </a> </br> " > version.txt
cp $AthAnalysis_DIR/ReleaseData ReleaseData
