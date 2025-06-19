mkdir -p build run
setupATLAS -q
asetup AnalysisBase,25.2.52
cmake ../source
make -j4
source */setup.sh
cd ../run