setupATLAS
mkdir -p build run
cd build
asetup AnalysisBase,25.2.25
cmake ../source
make -j4
source */setup.sh