#ifndef TOPCPTOOLKIT_DICT_H
#define TOPCPTOOLKIT_DICT_H

#include "TopCPToolkit/BoostedJetTaggersAlg.h"
#include "TopCPToolkit/CARAlg.h"
#include "TopCPToolkit/ComponentFactoryPreloader.h"
#include "TopCPToolkit/EventShape.h"
#include "TopCPToolkit/HSTPFilterAlg.h"
#include "TopCPToolkit/JetMatchingAlg.h"
#include "TopCPToolkit/ParticleLevelSoftMuonAlg.h"
#include "TopCPToolkit/PartonHistoryToSpinInputAlg.h"
#include "TopCPToolkit/PartonMatchAlg.h"
#include "TopCPToolkit/RunHyPERAlg.h"
#include "TopCPToolkit/RunNuFlowsAlg.h"
#include "TopCPToolkit/RunPartonHistoryAlg.h"
#include "TopCPToolkit/RunSpaNetAlg.h"
#include "TopCPToolkit/RunTtbarNNLORecursiveRewAlg.h"
#include "TopCPToolkit/SinglelepHadronicChi2RecoAlg.h"
#include "TopCPToolkit/SoftMuonSelectorAlg.h"
#include "TopCPToolkit/TopSpinDensityMatrixAlg.h"
#include "TopCPToolkit/ToponiumDileptonReconstructionAlg.h"

// EDM include(s).
#include "xAODCore/tools/DictHelpers.h"
// Instantiate all necessary types for the dictionary.
namespace {
struct GCCXML_DUMMY_INSTANTIATION_TOPCPTOOLKIT {
  // Local type(s).
  XAOD_INSTANTIATE_NS_CONTAINER_TYPES(xAOD, PartonHistoryContainer);
  XAOD_INSTANTIATE_NS_OBJECT_TYPES(xAOD, PartonHistory);
};
}  // namespace

#endif
