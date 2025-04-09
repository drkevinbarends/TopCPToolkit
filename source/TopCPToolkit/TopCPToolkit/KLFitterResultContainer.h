#ifndef TOPCPTOOLKIT_KLFITTERRESULTCONTAINER_H
#define TOPCPTOOLKIT_KLFITTERRESULTCONTAINER_H

// EDM include(s).
#include "AthContainers/DataVector.h"
#include "xAODCore/CLASS_DEF.h"

// Local include(s).
#include "TopCPToolkit/KLFitterResult.h"

namespace xAOD {

   /// Definition of the @c KLFitterResultContainer type
   typedef DataVector< xAOD::KLFitterResult > KLFitterResultContainer;

} // namespace xAOD

// Define a ClassID for the type.
CLASS_DEF( xAOD::KLFitterResultContainer, 1116647492, 1 )

#endif
