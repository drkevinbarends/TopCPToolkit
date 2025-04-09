#ifndef PARTICLE_LEVEL_H
#define PARTICLE_LEVEL_H

#include <vector>

// EDM include(s):
#include "AthContainers/AuxElement.h"
#include "AthContainers/DataVector.h"
#include "xAODCore/AuxContainerBase.h"
#include "xAODCore/CLASS_DEF.h"
#include "xAODEventInfo/EventInfo.h"
#include "xAODJet/JetContainer.h"
#include "xAODMissingET/MissingETContainer.h"
#include "xAODTruth/TruthEventContainer.h"
#include "xAODTruth/TruthMetaDataContainer.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "xAODTruth/TruthVertexContainer.h"

namespace top {

namespace ParticleLevel {

bool isLeptonFromTau(const xAOD::TruthParticle* truthParticle);
bool isFrom(const xAOD::TruthParticle* truthParticle,
            const std::vector<int>& parentPDGIds, bool bOnlyDirect = false);

}  // namespace ParticleLevel

}  // namespace top

#endif
