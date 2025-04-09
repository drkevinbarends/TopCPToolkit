#ifndef TOPCPTOOLKIT_ONNXWRAPPER_H
#define TOPCPTOOLKIT_ONNXWRAPPER_H

// ONNX includes
#include <onnxruntime_cxx_api.h>

// useful
#include <PathResolver/PathResolver.h>
#include <vector>

// Framework includes
#include "AsgTools/AsgTool.h"

namespace top {
  class ONNXWrapper : public asg::AsgTool {
  public:

    ONNXWrapper(const std::string& name, const std::vector<std::string>& filepaths_model_cv, bool verbose=false);

    virtual ~ONNXWrapper() = default;

    inline std::vector<Ort::Value> evaluate(
      std::vector<Ort::Value>& input_tensors,
      unsigned index_network = 0
    ) {
      Ort::Session& session ATLAS_THREAD_SAFE = *m_sessions[index_network];

      std::vector<const char*> inputNames(m_input_node_names.size());
      std::vector<const char*> outputNames(m_output_node_names.size());
      for (size_t i = 0; i < m_input_node_names.size(); ++i)
        inputNames[i] = m_input_node_names[i].c_str();
      for (size_t i = 0; i < m_output_node_names.size(); ++i)
        outputNames[i] = m_output_node_names[i].c_str();
      auto output_tensors = session.Run(Ort::RunOptions{nullptr}, inputNames.data(), input_tensors.data(), input_tensors.size(), outputNames.data(), outputNames.size());
      return output_tensors;
    }

    inline void evaluate(unsigned index_network=0) {
      m_output_tensors = evaluate(m_input_tensors, index_network);
    }

    virtual unsigned getSessionIndex(unsigned long long eventNumber) {
      return eventNumber % m_sessions.size();
    }

    void clearInputs() {m_input_tensors.clear();}
    void clearOutputs() {m_output_tensors.clear();}

    template<typename T>
    void addInputs(
      T* p_data, size_t p_data_element_count, const int64_t* shape, size_t shape_len
    ) {
      m_input_tensors.push_back(
        Ort::Value::CreateTensor<T>(m_memory_info, p_data, p_data_element_count, shape, shape_len)
      );
    }

    template<typename T>
    void addInputs(
      std::vector<T>& values, const std::vector<int64_t>& shape
    ) {
      m_input_tensors.push_back(
        Ort::Value::CreateTensor<T>(m_memory_info, values.data(), values.size(), shape.data(), shape.size())
      );
    }

    template<typename T>
    inline void setInputs(const std::string& node_name, T* p_data, size_t p_data_element_count, const int64_t* shape, size_t shape_len) {
      // resize m_input_tensors in case it is not of the same size as m_input_node_names

      // m_input_tensors.resize(m_input_node_names.size());
      // Does not compile. Ort::Value is not DefaultInsertable.
      // m_input_tensors.resize(m_input_node_names.size(), Ort::Value(nullptr));
      // Does not compile either. Ort::Value is not CopyInsertable.
      for (size_t i = 0; i < m_input_node_names.size()-m_input_tensors.size(); i++) {
        m_input_tensors.emplace_back(nullptr);
      }

      // find the node index from the node name
      try {
        unsigned node = m_input_name_index.at(node_name);
        m_input_tensors[node] = Ort::Value::CreateTensor<T>(m_memory_info, p_data, p_data_element_count, shape, shape_len);
      } catch (std::out_of_range& ex) {
        ANA_MSG_ERROR("Fail to assign input values to node " << node_name << ": " << ex.what());
      }
    }

    // overload
    template<typename T>
    void setInputs(const std::string& node_name, const std::vector<T>& values, const std::vector<int64_t>& shape) {
      setInputs(node_name, values.data(), values.size(), shape.data(), shape.size());
    }

    template<typename T>
    T* getOutputs(unsigned node) {
      return m_output_tensors.at(node).GetTensorMutableData<T>();
    }

    template<typename T>
    T* getOutputs(const std::string& node_name) {
      try {
        // find the node index from the node name
        unsigned node = m_output_name_index.at(node_name);
        return getOutputs<T>(node);
      } catch (std::out_of_range& ex) {
        ANA_MSG_ERROR("Fail to retrieve output from " << node_name << ": " << ex.what());
        return nullptr;
      }
    }

    void makeVerbose(bool verbosity) { m_verbose = verbosity; }

    void printInputInfo();
    void printOutputInfo();

  protected:

    // ort
    std::shared_ptr<Ort::Env> m_env;
    std::shared_ptr<Ort::SessionOptions> m_session_options;
    std::vector<std::shared_ptr<Ort::Session>> m_sessions;
    std::vector<std::string> m_input_node_names;
    std::vector<std::string> m_output_node_names;
    std::vector<std::vector<int64_t>> m_input_shapes;
    std::vector<std::vector<int64_t>> m_output_shapes;

    Ort::MemoryInfo m_memory_info; // where to allocate the tensors
    std::vector<Ort::Value> m_input_tensors;
    std::vector<Ort::Value> m_output_tensors;
    std::vector<ONNXTensorElementDataType> m_input_types;
    std::vector<ONNXTensorElementDataType> m_output_types;

    // map from node name to node index
    std::unordered_map<std::string, unsigned> m_input_name_index;
    std::unordered_map<std::string, unsigned> m_output_name_index;

    bool m_verbose;
  };

} // namespace top

#endif
