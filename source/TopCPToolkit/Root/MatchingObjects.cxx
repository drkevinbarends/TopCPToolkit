#include "TopCPToolkit/MatchingObjects.h"

namespace top {

// Implementation for TruthObject
TruthObject::TruthObject(int idx, const PtEtaPhiMVector& p4, bool is_matched,
                         int matches)
    : m_idx(idx), m_p4(p4), m_isMatched(is_matched), m_matches(matches) {}

/**
 * @brief Gets the index of the TruthObject.
 * @return Index as an integer.
 */
int TruthObject::getIdx() const {
  return m_idx;
}

/**
 * @brief Sets the index of the TruthObject.
 * @param idx New index value.
 */
void TruthObject::setIdx(int idx) {
  m_idx = idx;
}

/**
 * @brief Gets the four-momentum vector (p4) of the TruthObject.
 * @return Four-momentum vector as a PtEtaPhiMVector.
 */
PtEtaPhiMVector TruthObject::getP4() const {
  return m_p4;
}

/**
 * @brief Checks if the TruthObject is matched.
 * @return True if matched, otherwise false.
 */
bool TruthObject::isMatched() const {
  return m_isMatched;
}

/**
 * @brief Gets the number of matches for the TruthObject.
 * @return Number of matches as an integer.
 */
int TruthObject::getMatches() const {
  return m_matches;
}

/**
 * @brief Sets the TruthObject as matched.
 */
void TruthObject::setMatched() {
  m_isMatched = true;
}

/**
 * @brief Sets the number of matches for the TruthObject.
 * @param matches Number of matches.
 */
void TruthObject::setMatches(int matches) {
  m_matches = matches;
}

// Implementation for RecoObject
RecoObject::RecoObject(int idx, const PtEtaPhiEVector& p4, bool is_matched)
    : m_idx(idx), m_p4(p4), m_isMatched(is_matched) {}

/**
 * @brief Gets the index of the RecoObject.
 * @return Index as an integer.
 */
int RecoObject::getIdx() const {
  return m_idx;
}

/**
 * @brief Sets the index of the RecoObject.
 * @param idx New index value.
 */
void RecoObject::setIdx(int idx) {
  m_idx = idx;
}

/**
 * @brief Gets the four-momentum vector (p4) of the RecoObject.
 * @return Four-momentum vector as a PtEtaPhiEVector.
 */
PtEtaPhiEVector RecoObject::getP4() const {
  return m_p4;
}

/**
 * @brief Checks if the RecoObject is matched.
 * @return True if matched, otherwise false.
 */
bool RecoObject::isMatched() const {
  return m_isMatched;
}

/**
 * @brief Sets the RecoObject as matched.
 */
void RecoObject::setMatched() {
  m_isMatched = true;
}

}  // namespace top
