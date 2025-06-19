#ifndef MATCHINGOBJECTS_H
#define MATCHINGOBJECTS_H

#include <vector>

#include "Math/Vector4D.h"

namespace top {
using ROOT::Math::PtEtaPhiEVector;
using ROOT::Math::PtEtaPhiMVector;

class TruthObject {
 public:
  TruthObject(int idx, const PtEtaPhiMVector& p4, bool is_matched, int matches);
  int getIdx() const;
  void setIdx(int idx);
  PtEtaPhiMVector getP4() const;
  bool isMatched() const;
  int getMatches() const;
  void setMatched();
  void setMatches(int matches);

 private:
  int m_idx;
  PtEtaPhiMVector m_p4;
  bool m_isMatched;
  int m_matches;
};

class RecoObject {
 public:
  RecoObject(int idx, const PtEtaPhiEVector& p4, bool is_matched);
  int getIdx() const;
  void setIdx(int idx);
  PtEtaPhiEVector getP4() const;
  bool isMatched() const;
  void setMatched();

 private:
  int m_idx;
  PtEtaPhiEVector m_p4;
  bool m_isMatched;
};

}  // namespace top

#endif  // MATCHINGOBJECTS_H
