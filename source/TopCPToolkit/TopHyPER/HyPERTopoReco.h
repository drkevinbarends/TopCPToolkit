#ifndef HYPER_TOPORECO_H
#define HYPER_TOPORECO_H

/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

/// @author Zihan Zhang, Diego Baron

#include <iostream>
#include <memory>
#include <vector>

#include "TopHyPER/HyPERGraph.h"

using indices = std::vector<std::vector<int64_t>>;
using scores = std::vector<std::vector<float>>;

namespace top {
void RecoTtbarAllHadronic(const HyPERGraph& hyperGraph,
                          const scores& edge_scores,
                          const scores& hyperedge_scores, indices& reco_indices,
                          std::vector<float>& reco_scores,
                          std::vector<std::string>& reco_labels);
void RecoTtbarLJets(const HyPERGraph& hyperGraph, const scores& edge_scores,
                    const scores& hyperedge_scores, indices& reco_indices,
                    std::vector<float>& reco_scores,
                    std::vector<std::string>& reco_labels,
                    std::vector<std::vector<int64_t>>& reco_ids);
}  // namespace top

#endif
