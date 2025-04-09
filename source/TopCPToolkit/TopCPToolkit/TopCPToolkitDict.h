#ifndef TOPCPTOOLKIT_DICT_H
#define TOPCPTOOLKIT_DICT_H

#include "TopCPToolkit/ComponentFactoryPreloader.h"
#include "TopCPToolkit/KLFitterAlg.h"
#include "TopCPToolkit/KLFitterResult.h"
#include "TopCPToolkit/KLFitterResultContainer.h"
#include "TopCPToolkit/KLFitterResultAuxContainer.h"
#include "TopCPToolkit/KLFitterFinalizeOutputAlg.h"
#include "TopCPToolkit/RunPartonHistoryAlg.h"
#include "TopCPToolkit/CARAlg.h"
#include "TopCPToolkit/ParticleLevelAlg.h"
#include "TopCPToolkit/PartonToJetsMatchAlg.h"
#include "TopCPToolkit/RunSpaNetAlg.h"
#include "TopCPToolkit/RunTtbarNNLORecursiveRewAlg.h"
#include "TopCPToolkit/JetMatchingAlg.h"
#include "TopCPToolkit/SoftMuonSelectorAlg.h"
#include "TopCPToolkit/PartonHistoryToSpinInputAlg.h"
#include "TopCPToolkit/TopSpinDensityMatrixAlg.h"
#include "TopCPToolkit/SinglelepHadronicChi2RecoAlg.h"


// EDM include(s).
#include "xAODCore/tools/DictHelpers.h"
// Instantiate all necessary types for the dictionary.
namespace {
    struct GCCXML_DUMMY_INSTANTIATION_TOPCPTOOLKIT {
        // Local type(s).
        XAOD_INSTANTIATE_NS_CONTAINER_TYPES( xAOD, KLFitterResultContainer );
        XAOD_INSTANTIATE_NS_OBJECT_TYPES( xAOD, KLFitterResult );
        XAOD_INSTANTIATE_NS_CONTAINER_TYPES( xAOD, PartonHistoryContainer );
        XAOD_INSTANTIATE_NS_OBJECT_TYPES( xAOD, PartonHistory );
    };
}

#endif
