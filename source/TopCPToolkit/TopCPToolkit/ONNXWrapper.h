#ifndef TOPCPTOOLKIT_ONNXWRAPPER_H
#define TOPCPTOOLKIT_ONNXWRAPPER_H

// ONNX includes
#include <onnxruntime_cxx_api.h>

// useful
#include <PathResolver/PathResolver.h>

#include <type_traits>
#include <vector>

// Framework includes
#include "AsgTools/AsgTool.h"

namespace top {

// ONNX only accepts flattened arrays!
// So we inlcude helper functions for flattening vectors and getting their
// shapes This is helpful for passing multi-dimensional vectors to the onnx
// wrapper without having to know their shape in advance. All this is in service
// for the linkInputs method which can now accept multi-dimensional vectors

// First we define a way to get the type of each element in a vector
// Base class, if we have a scalar (element), just return the type
template <typename T>
struct scalar_type {
  using type = T;
};

// Used in recursion, if we have a vector, get the scalar type of that element
template <typename T>
struct scalar_type<std::vector<T>> {
  using type = typename scalar_type<T>::type;
};

// Now we define the function to flatten a vector
// This is also used recursively
// Base case: If we have a scalar, just add it to the output vector
template <typename T>
void flatten(const T& value,
             std::vector<typename scalar_type<T>::type>& output) {
  output.push_back(value);
}

// Recursive case for vectors: iterate and flatten each element
template <typename T>
void flatten(const std::vector<T>& vec,
             std::vector<typename scalar_type<T>::type>& output) {
  for (const auto& item : vec) {
    flatten(item, output);
  }
}

// Actual function one would typically use
// Creates the output using the correct type and
// Coppies into it a flattened version of the vector
template <typename T>
std::vector<typename scalar_type<T>::type> flatten(const std::vector<T>& vec) {
  std::vector<typename scalar_type<T>::type> output;
  flatten(vec, output);
  return output;
}

// Helper functions for getting the shape of a multi dimensional vector
// Base case for scalar values: return an empty vector
template <typename T>
std::vector<int64_t> calculateDimensions(const T& value) {
  static_cast<void>(value);  // Prevent unused parameter warning
  return std::vector<long>{};
}

// Recursive case for vectors: get the dimensions of the first element
template <typename T>
std::vector<int64_t> calculateDimensions(const std::vector<T>& vec) {
  if (vec.empty()) {
    // Return a vector with a single element (0) for empty vectors
    return std::vector<int64_t>{0};
  } else {
    // Calculate dimensions of the first element in the vector
    std::vector<int64_t> dims = calculateDimensions(vec[0]);
    // Prepend the size of the current vector to the dimensions
    dims.insert(dims.begin(), vec.size());
    return dims;
  }
}

class ONNXWrapper : public asg::AsgTool {
 public:
  ONNXWrapper(const std::string& name,
              const std::vector<std::string>& filepaths_model_cv,
              bool verbose = false);

  virtual ~ONNXWrapper() = default;

  inline std::vector<Ort::Value> evaluate(
      std::vector<Ort::Value>& input_tensors, unsigned index_network = 0) {
    Ort::Session& session ATLAS_THREAD_SAFE = *m_sessions[index_network];

    std::vector<const char*> inputNames(m_input_node_names.size());
    std::vector<const char*> outputNames(m_output_node_names.size());
    for (size_t i = 0; i < m_input_node_names.size(); ++i)
      inputNames[i] = m_input_node_names[i].c_str();
    for (size_t i = 0; i < m_output_node_names.size(); ++i)
      outputNames[i] = m_output_node_names[i].c_str();
    auto output_tensors = session.Run(
        Ort::RunOptions{nullptr}, inputNames.data(), input_tensors.data(),
        input_tensors.size(), outputNames.data(), outputNames.size());
    return output_tensors;
  }

  inline void evaluate(unsigned index_network = 0) {
    m_output_tensors = evaluate(m_input_tensors, index_network);
  }

  virtual unsigned getSessionIndex(unsigned long long eventNumber) {
    return eventNumber % m_sessions.size();
  }

  void clearInputs() { m_input_tensors.clear(); }
  void clearOutputs() { m_output_tensors.clear(); }

  // This form of initialization means that m_input_tensors own the buffers
  template <typename T>
  void addInputs(T* p_data, size_t p_data_element_count, const int64_t* shape,
                 size_t shape_len) {
    m_input_tensors.emplace_back(
        Ort::Value::CreateTensor<T>(m_allocator, shape, shape_len));
    std::memcpy(m_input_tensors.back().GetTensorMutableData<T>(), p_data,
                p_data_element_count * sizeof(T));
  }

  // This does not copy the buffer! So the caller must ensure the buffer
  // persists
  template <typename T>
  void linkInputs(T* p_data, size_t p_data_element_count, const int64_t* shape,
                  size_t shape_len) {
    m_input_tensors.push_back(Ort::Value::CreateTensor<T>(
        m_memory_info, p_data, p_data_element_count, shape, shape_len));
  }

  template <typename T>
  void addInputs(std::vector<T>& values, const std::vector<int64_t>& shape) {
    addInputs(values.data(), values.size(), shape.data(), shape.size());
  }

  template <typename T>
  void linkInputs(std::vector<T>& values, const std::vector<int64_t>& shape) {
    addInputs(values.data(), values.size(), shape.data(), shape.size());
  }

  // overload
  template <typename T>
  void setInputs(const std::string& node_name, std::vector<T>& values,
                 const std::vector<int64_t>& shape) {
    setInputLink(node_name, values.data(), values.size(), shape.data(),
                 shape.size());
  }

  // For multidimensional vectors with auto flatten and shape calculation
  template <typename T>
  void addInputs(std::vector<T>& values) {
    auto flat = flatten(values);
    auto shape = calculateDimensions(values);
    addInputs(flat, shape);
  }

  template <typename T>
  inline void setInputLink(const std::string& node_name, T* p_data,
                           size_t p_data_element_count, const int64_t* shape,
                           size_t shape_len) {
    // make sure the input tensors vector is large enough
    for (size_t i = 0; i < m_input_node_names.size() - m_input_tensors.size();
         i++) {
      m_input_tensors.emplace_back(nullptr);
    }

    // find the node index from the node name
    try {
      unsigned node = m_input_name_index.at(node_name);
      m_input_tensors[node] = Ort::Value::CreateTensor<T>(
          m_memory_info, p_data, p_data_element_count, shape, shape_len);
    } catch (std::out_of_range& ex) {
      ANA_MSG_ERROR("Fail to assign input values to node " << node_name << ": "
                                                           << ex.what());
    }
  }

  template <typename T>
  void setInputLink(const std::string& node_name, const std::vector<T>& values,
                    const std::vector<int64_t>& shape) {
    setInputLink(node_name, values.data(), values.size(), shape.data(),
                 shape.size());
  }

  template <typename T>
  T* getOutputs(unsigned node) {
    return m_output_tensors.at(node).GetTensorMutableData<T>();
  }

  template <typename T>
  T* getInputs(unsigned node) {
    return m_input_tensors.at(node).GetTensorMutableData<T>();
  }

  template <typename T>
  T* getOutputs(const std::string& node_name) {
    try {
      // find the node index from the node name
      unsigned node = m_output_name_index.at(node_name);
      return getOutputs<T>(node);
    } catch (std::out_of_range& ex) {
      ANA_MSG_ERROR("Fail to retrieve output from " << node_name << ": "
                                                    << ex.what());
      return nullptr;
    }
  }

  void makeVerbose(bool verbosity) { m_verbose = verbosity; }
  void printInputInfo(bool printTensorContent = false);
  void printOutputInfo(bool printTensorContent = false);
  void printTensorContent(const Ort::Value& tensors);

 protected:
  // ort
  std::shared_ptr<Ort::Env> m_env;
  std::shared_ptr<Ort::SessionOptions> m_session_options;
  std::vector<std::shared_ptr<Ort::Session>> m_sessions;
  std::vector<std::string> m_input_node_names;
  std::vector<std::string> m_output_node_names;
  std::vector<std::vector<int64_t>> m_input_shapes;
  std::vector<std::vector<int64_t>> m_output_shapes;

  Ort::AllocatorWithDefaultOptions m_allocator;  // default allocator
  Ort::MemoryInfo m_memory_info;  // where to allocate the tensors
  std::vector<Ort::Value> m_input_tensors;
  std::vector<Ort::Value> m_output_tensors;
  std::vector<ONNXTensorElementDataType> m_input_types;
  std::vector<ONNXTensorElementDataType> m_output_types;

  // map from node name to node index
  std::unordered_map<std::string, unsigned> m_input_name_index;
  std::unordered_map<std::string, unsigned> m_output_name_index;

  bool m_verbose;
};

}  // namespace top

#endif
