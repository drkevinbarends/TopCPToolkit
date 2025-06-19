#!/bin/bash
thisDir=$PWD
echo "Export ATLAS_LOCAL_ROOT_BASE"
# Now run the enviroment setup
export ATLAS_LOCAL_ROOT_BASE='/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase'
env | grep ATLAS_LOCAL_ROOT_BASE
echo "Source setupATLAS"
source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh
echo "Sourced"
