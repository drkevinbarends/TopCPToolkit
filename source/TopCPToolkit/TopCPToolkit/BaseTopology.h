#ifndef BASETOPOLOGY_H
#define BASETOPOLOGY_H

#include <memory>
#include <vector>

#include "PartonHistory/PartonHistory.h"
#include "TopCPToolkit/MatchingObjects.h"
#include "VectorHelpers/AccessorHelpers.h"

namespace top {

class BaseTopology {
 public:
  BaseTopology();
  virtual ~BaseTopology() = default;

  BaseTopology(const BaseTopology& rhs) = delete;
  BaseTopology(BaseTopology&& rhs) = delete;
  BaseTopology& operator=(const BaseTopology& rhs) = delete;

  void handleHadronicDecay(const xAOD::PartonHistory* history,
                           PartonAccessor& partonAcc,
                           const std::string& up_source,
                           const std::string& down_source,
                           std::vector<TruthObject>& truth_jets,
                           std::vector<TruthObject>& truth_electrons,
                           std::vector<TruthObject>& truth_muons);
  void handleLeptonicDecay(const xAOD::PartonHistory* history,
                           PartonAccessor& partonAcc, int lepton_pdgId,
                           const std::string& lep_source,
                           std::vector<TruthObject>& truth_jets,
                           std::vector<TruthObject>& truth_electrons,
                           std::vector<TruthObject>& truth_muons);
  void setHistory(const xAOD::PartonHistory* history);
  void setNLeptons(int val) { m_nLeptons = val; };
  int getNLeptons() { return m_nLeptons; };

  const xAOD::PartonHistory* m_history;

  // Method to validate and retrieve the topology
  virtual bool checkTopology() = 0;
  virtual bool fillTopology(std::vector<TruthObject>& truth_jets,
                            std::vector<TruthObject>& truth_electrons,
                            std::vector<TruthObject>& truth_muons) = 0;

 private:
  int m_nLeptons = 0;
};

}  // namespace top

#endif  // BASETOPOLOGY_H
