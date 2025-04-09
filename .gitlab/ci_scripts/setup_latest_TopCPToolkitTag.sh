#!/bin/bash
echo "-----------------------------------------------"
echo "     Changing the TopCPToolkit source code     "
echo "-----------------------------------------------"
echo "        to use the latest release tag...       "
echo "-----------------------------------------------"
thisDir=$PWD
echo "Fetching tag info"
git config --global --add safe.directory /builds/atlasphys-top/reco/TopCPToolkit
git fetch --tags
echo "Getting latest release tag"
TAG=`git tag | sort --version-sort | tail -n 1`
# Print info
echo "-> Setup tag... $TAG"
git checkout tags/$TAG
echo "$TAG" > tag.txt
echo "Done!"
