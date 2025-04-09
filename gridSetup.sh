setupATLAS -q
cd /afs/cern.ch/user/k/kebarend/public/TopCPToolKit_Package/TopCPToolkit/build
asetup --restore
source */setup.sh
cd /afs/cern.ch/user/k/kebarend/public/TopCPToolKit_Package/TopCPToolkit/run
lsetup "rucio -w"
voms-proxy-init -voms atlas
lsetup panda pyami