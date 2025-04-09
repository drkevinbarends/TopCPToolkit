#include "PartonHistory/PartonHistory.h"

#include "TopCPToolkit/KLFitterResult.h"
#include "TopCPToolkit/KLFitterResultContainer.h"
#include "TopCPToolkit/KLFitterResultAuxContainer.h"

#ifdef __CINT__
#pragma extra_include "PartonHistory/PartonHistory.h";

#pragma extra_include "TopCPToolkit/KLFitterResult.h";
#pragma extra_include "TopCPToolkit/KLFitterResultContainer.h";
#pragma extra_include "TopCPToolkit/KLFitterResultAuxContainer.h";

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;

#pragma link C++ class xAOD::PartonHistory+;
#pragma link C++ class xAOD::PartonHistoryAux+;
#pragma link C++ class xAOD::PartonHistoryContainer+;
#pragma link C++ class xAOD::PartonHistoryAuxContainer+;

#pragma link C++ class xAOD::KLFitterResult+;
#pragma link C++ class xAOD::KLFitterResultContainer+;
#pragma link C++ class xAOD::KLFitterResultAuxContainer+;

#endif
