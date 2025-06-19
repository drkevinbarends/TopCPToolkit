#include "TMath.h"
#include "TopHyPER/HyPERUtils.h"

namespace top {

std::vector<std::vector<int64_t>> transpose(
    const std::vector<std::vector<int64_t>>& vecs) {
  if (vecs.empty() || vecs[0].empty())
    return {};

  std::size_t rows = vecs.size();
  std::size_t cols = vecs[0].size();
  std::vector<std::vector<int64_t>> transposed(cols,
                                               std::vector<int64_t>(rows));
  for (std::size_t i = 0; i < rows; ++i) {
    for (std::size_t j = 0; j < cols; ++j) {
      transposed[j][i] = vecs[i][j];
    }
  }
  return transposed;
}

std::vector<int64_t> range(int64_t n) {
  std::vector<int64_t> result;
  for (int64_t i{0}; i < n; ++i)
    result.push_back(i);
  return result;
}

// @enum HyPERTopology
HyPERTopology strToHyPERTopology(const std::string& str) {
  if (str == "TtbarLJets")
    return HyPERTopology::TtbarLJets;
  if (str == "TtbarLJetsNoBTag")
    return HyPERTopology::TtbarLJets;
  if (str == "TtbarAllHadronic")
    return HyPERTopology::TtbarAllHadronic;
  return HyPERTopology::NotSelected;
}

// Calculate delta phi between two angles in radians from -pi/2 to pi/2
float deltaPhi(float phi1, float phi2) {
  float PI = static_cast<float>(TMath::Pi());

  float dphi = phi1 - phi2;
  while (dphi > PI)
    dphi -= 2 * PI;
  while (dphi <= -PI)
    dphi += 2 * PI;

  return dphi;
}

}  // namespace top
