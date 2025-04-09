#!/bin/bash

# the directory of the script
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

echo "By-passing the tutorial by copying all the solution files to the right places..."

cp "$SCRIPT_DIR/.tutorial/TutorialAlg.cxx"   "$SCRIPT_DIR/source/TopCPToolkit/Root/"
cp "$SCRIPT_DIR/.tutorial/TutorialAlg.h"     "$SCRIPT_DIR/source/TopCPToolkit/TopCPToolkit/"
cp "$SCRIPT_DIR/.tutorial/TutorialConfig.py" "$SCRIPT_DIR/source/TopCPToolkit/python/"

echo "Adding the relevant lines for compilation..."

sed -i -e '/#endif/ i\#include "TopCPToolkit/TutorialAlg.h"' "$SCRIPT_DIR/source/TopCPToolkit/TopCPToolkit/TopCPToolkitDict.h"
sed -i -e '$s|</lcgdict>|  <class name="TutorialAlg" />\n&|' "$SCRIPT_DIR/source/TopCPToolkit/TopCPToolkit/selection.xml"
sed -i -e '/DECLARE_COMPONENT (TtbarCPalgoAnalysis)/ i\#include <TopCPToolkit/TutorialAlg.h>' "$SCRIPT_DIR/source/TopCPToolkit/TopCPToolkit/selection.xml"

echo "Writing the correct YAML config under share/configs/tutorial/ ..."

cp "$SCRIPT_DIR/source/TopCPToolkit/share/configs/tutorial/solution.yaml" "$SCRIPT_DIR/source/TopCPToolkit/share/configs/tutorial/reco.yaml"
sed -i -e '/Output:/i\
\
Tutorial:\
  - electrons: '\''AnaElectrons'\''\
    muons: '\''AnaMuons'\''\
    met: '\''AnaMET'\''\
    jets: '\''AnaJets'\''\
    selection: '\''pass_ejets_%SYS% || pass_mujets_%SYS%'\''' "$SCRIPT_DIR/source/TopCPToolkit/share/configs/tutorial/reco.yaml"

sed -i '$a\
    - modulePath: '\''TopCPToolkit.TutorialConfig'\''\
      functionName: '\''TutorialConfig'\''\
      algName: '\''Tutorial'\''\
      pos: '\''Output'\''' "$SCRIPT_DIR/source/TopCPToolkit/share/configs/tutorial/reco.yaml"

echo "Done!"
