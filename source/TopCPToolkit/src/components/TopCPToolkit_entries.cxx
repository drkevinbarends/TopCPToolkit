/*
  Copyright (C) 2002-2025 CERN for the benefit of the ATLAS collaboration
*/

#include <TopCPToolkit/BoostedJetTaggersAlg.h>
#include <TopCPToolkit/CARAlg.h>
#include <TopCPToolkit/EventShape.h>
#include <TopCPToolkit/HSTPFilterAlg.h>
#include <TopCPToolkit/JetMatchingAlg.h>
#include <TopCPToolkit/ParticleLevelSoftMuonAlg.h>
#include <TopCPToolkit/PartonHistoryToSpinInputAlg.h>
#include <TopCPToolkit/PartonMatchAlg.h>
#include <TopCPToolkit/RunHyPERAlg.h>
#include <TopCPToolkit/RunNuFlowsAlg.h>
#include <TopCPToolkit/RunPartonHistoryAlg.h>
#include <TopCPToolkit/RunSpaNetAlg.h>
#include <TopCPToolkit/RunTtbarNNLORecursiveRewAlg.h>
#include <TopCPToolkit/SinglelepHadronicChi2RecoAlg.h>
#include <TopCPToolkit/SoftMuonSelectorAlg.h>
#include <TopCPToolkit/TopSpinDensityMatrixAlg.h>

DECLARE_COMPONENT(top::RunTtbarNNLORecursiveRewAlg)
DECLARE_COMPONENT(top::RunSpaNetAlg)
DECLARE_COMPONENT(top::RunNuFlowsAlg)
DECLARE_COMPONENT(top::RunHyPERAlg)
DECLARE_COMPONENT(top::RunPartonHistoryAlg)
DECLARE_COMPONENT(top::CARAlg)
DECLARE_COMPONENT(top::PartonMatchAlg)
DECLARE_COMPONENT(top::JetMatchingAlg)
DECLARE_COMPONENT(top::SoftMuonSelectorAlg)
DECLARE_COMPONENT(top::PartonHistoryToSpinInputAlg)
DECLARE_COMPONENT(top::TopSpinDensityMatrixAlg)
DECLARE_COMPONENT(top::SinglelepHadronicChi2RecoAlg)
DECLARE_COMPONENT(top::EventShape)
DECLARE_COMPONENT(top::BoostedJetTaggersAlg)
DECLARE_COMPONENT(CP::ParticleLevelSoftMuonAlg)
DECLARE_COMPONENT(CP::HSTPFilterAlg)
