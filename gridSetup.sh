setupATLAS -q
cd build
asetup --restore
source */setup.sh
cd ../run
lsetup "rucio -w"
voms-proxy-init -voms atlas
lsetup panda pyami