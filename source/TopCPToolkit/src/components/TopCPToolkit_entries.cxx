/*
  Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
*/

#include <TopCPToolkit/KLFitterAlg.h>
#include <TopCPToolkit/KLFitterFinalizeOutputAlg.h>
#include <TopCPToolkit/ParticleLevelAlg.h>
#include <TopCPToolkit/PartonToJetsMatchAlg.h>
#include <TopCPToolkit/RunPartonHistoryAlg.h>
#include <TopCPToolkit/CARAlg.h>
#include <TopCPToolkit/RunSpaNetAlg.h>
#include <TopCPToolkit/RunTtbarNNLORecursiveRewAlg.h>
#include <TopCPToolkit/JetMatchingAlg.h>
#include <TopCPToolkit/SoftMuonSelectorAlg.h>
#include <TopCPToolkit/PartonHistoryToSpinInputAlg.h>
#include <TopCPToolkit/TopSpinDensityMatrixAlg.h>
#include <TopCPToolkit/SinglelepHadronicChi2RecoAlg.h>

DECLARE_COMPONENT (top::RunTtbarNNLORecursiveRewAlg)
DECLARE_COMPONENT (top::RunSpaNetAlg)
DECLARE_COMPONENT (top::RunPartonHistoryAlg)
DECLARE_COMPONENT (top::CARAlg)
DECLARE_COMPONENT (top::PartonToJetsMatchAlg)
DECLARE_COMPONENT (top::ParticleLevelAlg)
DECLARE_COMPONENT (top::KLFitterFinalizeOutputAlg)
DECLARE_COMPONENT (top::KLFitterAlg)
DECLARE_COMPONENT (top::JetMatchingAlg)
DECLARE_COMPONENT (top::SoftMuonSelectorAlg)
DECLARE_COMPONENT (top::PartonHistoryToSpinInputAlg)
DECLARE_COMPONENT (top::TopSpinDensityMatrixAlg)
DECLARE_COMPONENT (top::SinglelepHadronicChi2RecoAlg)
