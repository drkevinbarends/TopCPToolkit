#include "TopCPToolkit/ONNXWrapper.h"

#include <functional>
#include <numeric>

namespace top {

ONNXWrapper::ONNXWrapper(const std::string& name,
                         const std::vector<std::string>& filepaths_model_cv,
                         bool verbose)
    : asg::AsgTool(name),
      m_env(std::make_shared<Ort::Env>(ORT_LOGGING_LEVEL_WARNING, "")),
      m_session_options(std::make_shared<Ort::SessionOptions>()),
      m_memory_info(
          Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault)),
      m_verbose(verbose) {
  // TODO check at least one model path is provided

  // any session options are set via this object
  // use single thread (single CPU core) for the model evaluation
  m_session_options->SetIntraOpNumThreads(1);
  // ONNX can perform optimizations of the model graph to improve performance
  // (ORT_ENABLE_EXTENDED)
  m_session_options->SetGraphOptimizationLevel(
      GraphOptimizationLevel::ORT_ENABLE_EXTENDED);

  // print out the current onnxruntime version and supported opset
  if (m_verbose) {
    std::string version = Ort::GetVersionString();
    ANA_MSG_INFO("ONNXRuntime version: " << version);
  }

  // create the session and load model into memory
  for (auto& fpath_model : filepaths_model_cv) {
    auto fp = PathResolver::find_file(fpath_model, "CALIBPATH",
                                      PathResolver::RecursiveSearch);
    if (m_verbose) {
      ANA_MSG_INFO("Load model from " << fp);
    }
    m_sessions.emplace_back(
        new Ort::Session(*m_env, fp.c_str(), *m_session_options));
  }

  // retrieve the list of input and output tensor names
  auto session = m_sessions.front();

  // inputs
  // first vector -- the individual input nodes
  // second vector -- the shape of the input node, e.g. for 1xN shape, the
  // vector has two elements with values {1, N}
  size_t input_node_count = session->GetInputCount();
  m_input_shapes = std::vector<std::vector<int64_t>>(input_node_count);

  for (size_t inode = 0; inode < input_node_count; inode++) {
    // node name
    auto name =
        std::string(session->GetInputNameAllocated(inode, m_allocator).get());
    m_input_name_index[session->GetInputNameAllocated(inode, m_allocator)
                           .get()] = inode;
    m_input_node_names.push_back(
        session->GetInputNameAllocated(inode, m_allocator).get());

    // node shape and type
    m_input_shapes[inode] =
        session->GetInputTypeInfo(inode).GetTensorTypeAndShapeInfo().GetShape();
    m_input_types.push_back(session->GetInputTypeInfo(inode)
                                .GetTensorTypeAndShapeInfo()
                                .GetElementType());
  }

  // outputs
  size_t output_node_count = session->GetOutputCount();
  m_output_shapes = std::vector<std::vector<int64_t>>(output_node_count);
  for (size_t inode = 0; inode < output_node_count; inode++) {
    // node name
    auto name =
        std::string(session->GetOutputNameAllocated(inode, m_allocator).get());
    m_output_name_index[name] = inode;
    m_output_node_names.push_back(name);

    // node shape and type
    m_output_shapes[inode] = session->GetOutputTypeInfo(inode)
                                 .GetTensorTypeAndShapeInfo()
                                 .GetShape();
    m_output_types.push_back(session->GetOutputTypeInfo(inode)
                                 .GetTensorTypeAndShapeInfo()
                                 .GetElementType());
  }

  if (m_verbose) {
    printInputInfo();
    printOutputInfo();
  }

}  // ONNXWrapper::ONNXWrapper

void ONNXWrapper::printInputInfo(bool printTensorContent) {
  this->msg().setLevel(MSG::INFO);
  ANA_MSG_INFO("Input:");
  for (size_t i = 0; i < m_input_node_names.size(); i++) {
    // name
    ANA_MSG_INFO(" " << m_input_node_names[i]);
    // type
    ANA_MSG_INFO("  type = " << m_input_types.at(i));
    // shape
    auto& shape_i = m_input_shapes[i];
    ANA_MSG_INFO("  shape = [");
    for (size_t j = 0; j < shape_i.size(); j++) {
      ANA_MSG_INFO("   " << shape_i[j]);
    }
    ANA_MSG_INFO("  ]");

    // Print what is inside the tensors
    if (printTensorContent)
      this->printTensorContent(m_input_tensors[i]);
  }
}

void ONNXWrapper::printTensorContent(const Ort::Value& tensor) {
  // Print what is inside the tensors
  ANA_MSG_INFO("Contents of tensor:");
  auto tensor_info = tensor.GetTensorTypeAndShapeInfo();
  auto tensor_shape = tensor_info.GetShape();
  auto tensor_type = tensor_info.GetElementType();

  size_t total_elements = tensor_info.GetElementCount();
  if (tensor_type == ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT) {
    // For float tensors
    const float* data = tensor.GetTensorData<float>();
    for (size_t j = 0; j < total_elements; ++j) {
      ANA_MSG_INFO("Element " << j << " = " << data[j]);
    }
  } else if (tensor_type == ONNX_TENSOR_ELEMENT_DATA_TYPE_INT64) {
    // For int64 tensors
    const int64_t* data = tensor.GetTensorData<int64_t>();
    for (size_t j = 0; j < total_elements; ++j) {
      ANA_MSG_INFO("Element " << j << " = " << data[j]);
    }
  }
}

void ONNXWrapper::printOutputInfo(bool printTensorContent) {
  this->msg().setLevel(MSG::INFO);
  ANA_MSG_INFO("Output:");
  for (size_t i = 0; i < m_output_node_names.size(); i++) {
    // name
    ANA_MSG_INFO(" " << m_output_node_names[i]);
    // type
    ANA_MSG_INFO("  type = " << m_output_types.at(i));
    // shape
    auto& shape_i = m_output_shapes[i];
    ANA_MSG_INFO("  shape = [");
    for (size_t j = 0; j < shape_i.size(); j++) {
      ANA_MSG_INFO("   " << shape_i[j]);
    }
    ANA_MSG_INFO("  ]");

    // Print what is inside the tensors
    if (printTensorContent)
      this->printTensorContent(m_output_tensors[i]);
  }
}

}  // namespace top
