#include "TopSpaNet/TopologyTtbarLjetsNu.h"
#include "AthContainers/ConstDataVector.h"

namespace top {

  TopSpaNetTtbarLjetsNu::TopSpaNetTtbarLjetsNu(const std::string& name, const std::string& model_even, const std::string& model_odd) :
    TopSpaNetTopology(name, model_even, model_odd),
    m_lep_b(-1),
    m_had_b(-1),
    m_down(-1),
    m_up(-1),
    m_hadtop_assignment(0.),
    m_leptop_assignment(0.),
    m_hadtop_detection(0.),
    m_leptop_detection(0.),
    m_regressed_nu_eta(0.),
    m_regressed_nu_px(0.),
    m_regressed_nu_py(0.),
    m_regressed_nu_pz(0.),
    m_regressed_ttbar_m(0.)
  {
    m_MAX_JETS = 20;
    m_NUM_JET_FEATURES = m_input_shapes[0][2];

    m_MAX_LEPTONS = 1;
    m_NUM_LEPTON_FEATURES = m_input_shapes[2][2];

    m_MAX_GLOBALS = 1;
    m_NUM_GLOBAL_FEATURES = m_input_shapes[4][2];
  }

  void TopSpaNetTtbarLjetsNu::Predict(ConstDataVector<xAOD::ElectronContainer>& electrons,
				      ConstDataVector<xAOD::MuonContainer>& muons,
				      ConstDataVector<xAOD::JetContainer>& jets,
				      float met_met, float met_phi,
				      unsigned long long eventNumber) {

    // Set container for leptons and their properties
    xAOD::IParticleContainer leptons(SG::VIEW_ELEMENTS);
    std::vector<int> lepton_charges;
    lepton_charges.clear();
    std::vector<int> lepton_etags;
    lepton_etags.clear();
    std::vector<int> lepton_mutags;
    lepton_mutags.clear();

    // Process electrons and store their properties
    for (const xAOD::Electron *t : electrons){
      leptons.push_back(const_cast<xAOD::Electron*>(t));
      lepton_charges.push_back(t->charge());
      lepton_etags.push_back(1);
      lepton_mutags.push_back(0);
    }

    // Process muons and store their properties
    for (const xAOD::Muon *t : muons){
      leptons.push_back(const_cast<xAOD::Muon*>(t));
      lepton_charges.push_back(t->charge());
      lepton_etags.push_back(0);
      lepton_mutags.push_back(1);
    }

    // Since this is Ttbar l+jets we expect exactly one lepton
    if (leptons.size() > 1) ANA_MSG_VERBOSE("WARNING: Multiple leptons found, using first one only");

    // currently theres a bug with spanet whereby we need to use a batchsize > 1; so for now, we will just add a dummy second event, hence the [2] below
    float jet_values[2][20][6]; // TODO: avoid hard coding these sizes?
    bool jet_masks[2][20];

    float lepton_values[2][1][7];
    bool lepton_masks[2][1];

    float global_values[2][1][3];
    bool global_masks[2][1];

    // Process jets and store their properties
    for (long unsigned int i=0; i < static_cast<long unsigned int>(m_MAX_JETS); ++i){
      std::vector<float> jet_kin;

      if (i < jets.size()){
        // TODO: Avoid hard coding these? maybe give the list of inputs in the config, or read the spanet config file, or something?
      	auto jet = jets[i];
        jet_kin.push_back(std::log(jet->p4().E() + 1));
        jet_kin.push_back(std::log(jet->p4().Pt() + 1));
        jet_kin.push_back(jet->p4().Eta());
        jet_kin.push_back(sin(jet->p4().Phi()));
        jet_kin.push_back(cos(jet->p4().Phi()));
        if (jet->isAvailable<int>("ftag_quantile_"+m_btagger+"_Continuous")){
          jet_kin.push_back(jet->auxdataConst<int>("ftag_quantile_"+m_btagger+"_Continuous"));
        }
        else {
          jet_kin.push_back(0);
        }
        jet_masks[0][i]=1;

        for (int j=0; j < m_NUM_JET_FEATURES; ++j) jet_values[0][i][j] = jet_kin[j];
        ANA_MSG_VERBOSE("PT = " << jet_values[0][i][1] << \
                        ", ETA = " << jet_values[0][i][2] << \
                        ", COSPHI = " << jet_values[0][i][4] <<  \
                        ", SINPHI = " << jet_values[0][i][3] << \
                        ", E = " << jet_values[0][i][0] << \
                        ", btag = " << jet_values[0][i][5]);
      }
      else {
        // now fill the dummy values for the rest
        for (int j=0; j < m_NUM_JET_FEATURES; ++j) jet_values[0][i][j] = 0.0;
        jet_masks[0][i]=0;
      }
      // add a dummy second event because einsum is dumb with batchsize=1
      // TODO: fix this :)
      jet_masks[1][i] = 0;
      for (int j=0; j < m_NUM_JET_FEATURES; ++j) jet_values[1][i][j] = 0.0;
    }

    for (long unsigned int i=0; i < static_cast<long unsigned int>(m_MAX_LEPTONS); ++i){
      std::vector<float> lepton_kin;

      if (i < leptons.size()){
        // TODO: Avoid hard coding these? maybe give the list of inputs in the config, or read the spanet config file, or something?
        auto lepton = leptons[i];
        lepton_kin.push_back(std::log(lepton->p4().E() + 1));
        lepton_kin.push_back(std::log(lepton->p4().Pt() + 1));
        lepton_kin.push_back(lepton->p4().Eta());
        lepton_kin.push_back(sin(lepton->p4().Phi()));
        lepton_kin.push_back(cos(lepton->p4().Phi()));
        lepton_kin.push_back(lepton_etags[i]);
        lepton_kin.push_back(lepton_mutags[i]);
        lepton_masks[0][i]=1;

        for (int j=0; j < m_NUM_LEPTON_FEATURES; ++j) lepton_values[0][i][j] = lepton_kin[j];
        ANA_MSG_VERBOSE("PT = " << lepton_values[0][i][1] << \
                        ", ETA = " << lepton_values[0][i][2] << \
                        ", COSPHI = " << lepton_values[0][i][4] << \
                        ", SINPHI = " << lepton_values[0][i][3] << \
                        ", E = " << lepton_values[0][i][0] << \
                        ", etag = " << lepton_values[0][i][5]  << \
                        ", mutag = " << lepton_values[0][i][6]);
      }
      else {
        // now fill the dummy values for the rest
        for (int j=0; j < m_NUM_LEPTON_FEATURES; ++j) lepton_values[0][i][j] = 0.0;
        lepton_masks[0][i]=0;
      }
      // add a dummy second event because einsum is dumb with batchsize=1
      // TODO: fix this :)
      lepton_masks[1][i] = 0;
      for (int j=0; j < m_NUM_LEPTON_FEATURES; ++j) lepton_values[1][i][j] = 0.0;
    }

    for (int i=0; i < m_MAX_GLOBALS; ++i){
      std::vector<float> global_kin;

      global_kin.push_back(std::log(met_met + 1));
      global_kin.push_back(sin(met_phi));
      global_kin.push_back(cos(met_phi));

      global_masks[0][i]=1;

      for (int j=0; j < m_NUM_GLOBAL_FEATURES; ++j) global_values[0][i][j] = global_kin[j];
      ANA_MSG_VERBOSE("MET = " << global_values[0][i][0] << \
                      ", COSPHI = " << global_values[0][i][2] << \
                      ", SINPHI = " << global_values[0][i][1]);
      // add a dummy second event because einsum is dumb with batchsize=1
      // TODO: fix this :)
      global_masks[1][i] = 0;
      for (int j=0; j < m_NUM_GLOBAL_FEATURES; ++j) global_values[1][i][j] = 0.0;
    }

    // create a vector of 64-bit integers with the input tensor dimension
    std::vector<int64_t> input_jet_tensor_dims = {2, m_MAX_JETS, m_NUM_JET_FEATURES}; // TODO: Fix this when the batching is sorted out
    std::vector<int64_t> input_lepton_tensor_dims = {2, m_MAX_LEPTONS, m_NUM_LEPTON_FEATURES}; // TODO: Fix this when the batching is sorted out
    std::vector<int64_t> input_global_tensor_dims = {2, m_MAX_GLOBALS, m_NUM_GLOBAL_FEATURES}; // TODO: Fix this when the batching is sorted out
    // and the masks
    std::vector<int64_t> input_jet_tensor_dims_mask = {2, m_MAX_JETS};
    std::vector<int64_t> input_lepton_tensor_dims_mask = {2, m_MAX_LEPTONS};
    std::vector<int64_t> input_global_tensor_dims_mask = {2, m_MAX_GLOBALS};

    // create the Ort::Value tensor storing floats.
    // The data is taken from the input_values vector.
    // The dimensions of the tensor are specified based on the input_tensor_dims.

    this->clearInputs();

    int jet_input_size = 1;
    for (unsigned long int i=0; i < input_jet_tensor_dims.size(); ++i){
        jet_input_size = jet_input_size*input_jet_tensor_dims[i];
    }
    this->addInputs(
      **jet_values, jet_input_size, input_jet_tensor_dims.data(), input_jet_tensor_dims.size()
    );
    int jet_mask_size = 1;
    for (unsigned long int i=0; i < input_jet_tensor_dims_mask.size(); ++i){
      jet_mask_size = jet_mask_size*input_jet_tensor_dims_mask[i];
    }
    this->addInputs(
      *jet_masks, jet_mask_size, input_jet_tensor_dims_mask.data(), input_jet_tensor_dims_mask.size()
    );

    int lepton_input_size = 1;
    for (unsigned long int i=0; i < input_lepton_tensor_dims.size(); ++i){
        lepton_input_size = lepton_input_size*input_lepton_tensor_dims[i];
    }
    this->addInputs(
      **lepton_values, lepton_input_size, input_lepton_tensor_dims.data(), input_lepton_tensor_dims.size()
    );
    int lepton_mask_size = 1;
    for (unsigned long int i=0; i < input_lepton_tensor_dims_mask.size(); ++i){
      lepton_mask_size = lepton_mask_size*input_lepton_tensor_dims_mask[i];
    }
    this->addInputs(
      *lepton_masks, lepton_mask_size, input_lepton_tensor_dims_mask.data(), input_lepton_tensor_dims_mask.size()
    );

    int global_input_size = 1;
    for (unsigned long int i=0; i < input_global_tensor_dims.size(); ++i){
        global_input_size = global_input_size*input_global_tensor_dims[i];
    }
    this->addInputs(
      **global_values, global_input_size, input_global_tensor_dims.data(), input_global_tensor_dims.size()
    );
    int global_mask_size = 1;
    for (unsigned long int i=0; i < input_global_tensor_dims_mask.size(); ++i){
      global_mask_size = global_mask_size*input_global_tensor_dims_mask[i];
    }
    this->addInputs(
      *global_masks, global_mask_size, input_global_tensor_dims_mask.data(), input_global_tensor_dims_mask.size()
    );

    // make sure we are using the right network!
    unsigned imodel = getSessionIndex(eventNumber);
    this->evaluate(imodel);

    // now read the output
    auto thpred = this->getOutputs<float>("th_assignment_probability"); // TODO: Might be nice to avoid hard coding this?
    float max = -999;
    const int NUM_JETS = jets.size();

    int bestrow =-1;
    int bestcol = -1;
    int bestz = -1;

    // the size of the output tensor
    int dim = m_MAX_JETS + m_MAX_LEPTONS; // Globals cannot be predicted so are not included here

    // hadtop prediction order in config is bhad, up, down
    for (int row=0; row < NUM_JETS; ++row){// third particle in event file from SPANET
        for (int col = 0; col < NUM_JETS; ++col) {// second particle
            for (int z = 0; z < NUM_JETS; ++z) {// first particle

                if (row == col || row == z || z == col){continue;} //ignore overlapping predictions

                if (thpred[row+col*dim+z*dim*dim] > max){
                    max = thpred[row+col*dim+z*dim*dim];
                    bestrow = row;
                    bestcol = col;
                    bestz = z;
                }
    }}}
    ANA_MSG_VERBOSE("EventNo: " << eventNumber <<  ", Max = " << max << ", indx = " << bestrow << "," << bestcol << "," << bestz);

    float* tlpred = this->getOutputs<float>("tl_assignment_probability");

    int bestlb = -1;
    float max_lb = -999;
    for (int i=0; i < NUM_JETS; ++i){ // loop only over the jets, we dont want to predict the lep entry
      // For our case, we want to prioritise the hadtop prediction over the leptop; so ignore jets in the hadtop prediction

      if (i == bestz || i == bestrow || i == bestcol ) continue;
      if (tlpred[i] > max_lb){
        max_lb = tlpred[i];
        bestlb = i;
      }
    }
    ANA_MSG_VERBOSE("EventNo: " << eventNumber <<  ", Max = " << max << ", indx = " << bestrow << "," << bestcol << "," << bestz);

    ANA_MSG_VERBOSE("SPANET Down jet = " << bestz << ", up jet = " << bestcol << ", bhad = " << bestrow << ", blep = " << bestlb << " (Njets = " << NUM_JETS << ")");

    m_lep_b = bestlb;
    m_had_b = bestz;
    m_up = bestcol;
    m_down = bestrow;

    m_hadtop_assignment = max;
    m_leptop_assignment = max_lb;


    // now gxrab the spanet prob that the particle is reconstructable
    m_hadtop_detection = *(this->getOutputs<float>("th_detection_probability"));
    m_leptop_detection = *(this->getOutputs<float>("tl_detection_probability"));

    // finally, grab the regressions
    m_regressed_nu_eta  = *(this->getOutputs<float>("EVENT/neutrino_eta"));
    m_regressed_nu_px   = *(this->getOutputs<float>("EVENT/neutrino_px"));
    m_regressed_nu_py   = *(this->getOutputs<float>("EVENT/neutrino_py"));
    m_regressed_nu_pz   = *(this->getOutputs<float>("EVENT/neutrino_pz"));
    m_regressed_ttbar_m = *(this->getOutputs<float>("EVENT/ttbar_invariant_mass"));
  }

  std::vector<int> TopSpaNetTtbarLjetsNu::GetOutputIndices() {

    // indices of the best assignment
    std::vector<int> indices = {m_lep_b, m_had_b, m_down, m_up};

    return indices;
  }

  std::vector<float> TopSpaNetTtbarLjetsNu::GetOutputScores() {

    // scores of the best assignment
    std::vector<float> scores = {m_hadtop_assignment, m_leptop_assignment, m_hadtop_detection, m_leptop_detection};

    return scores;
  }

  std::vector<float> TopSpaNetTtbarLjetsNu::GetRegressedValues() {

    // regressed quantities
    std::vector<float> regressions = {m_regressed_nu_eta, m_regressed_nu_px, m_regressed_nu_py, m_regressed_nu_pz, m_regressed_ttbar_m};

    return regressions;
  }

} // namespace top
