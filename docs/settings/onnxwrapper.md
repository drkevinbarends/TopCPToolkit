## C++ classes

### [ONNXWrapper](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/source/TopCPToolkit/TopCPToolkit/ONNXWrapper.h)

`ONNXWrapper(const std::string& name, const std::vector<std::string>& filepaths_model_cv)`
:   Constructor of ONNXWrapper.

    - `name` name of the object

    - `filepaths_model_cv` a list of file paths to the trained models in ONNX format

`unsigned getSessionIndex(unsigned long long eventNumber)`
:   Map event number to model index. This is useful for choosing the right model for inference and for avoiding evaluating the same events used in the training. The model index is determined by the order of model file paths in the constructor.

    - `eventNumber` a number to identify event

`clearInputs()`
:   Delete all input tensors previously added to the wrapper's internal container.

`clearOutputs()`
:   Delete all output tensors stored in the wrapper's internal container.

`template<typename T> void addInputs(T* p_data, size_t p_data_element_count, const int64_t* shape, size_t shape_len)`
:   Convert a data array to tensor and add to the wrapper's internal container to be used as inputs for evaluating models.

    - `p_data` pointer to the data array

    - `p_data_element_count` length of the data array

    - `shape` pointer to the array of tensor dimensions

    - `shape_len` length of the dimension array

`template<typename T> void addInputs(const std::vector<T>& values, const std::vector<int64_t>& shape)`
:   Convert a data vector to tensor and add to the wrapper's internal container to be used as inputs for evaluating models.

    - `values` vector of data

    - `shape` vector of tensor dimension

`template<typename T> void setInputs(const std::string& node_name, T* p_data, size_t p_data_element_count, const int64_t* shape, size_t shape_len)`
:   Convert a data array to tensor and use the tensor as input to the node according to the specified node name for evaluating models.

    - `node_name` name of the input node in the trained model

    - `p_data` pointer to the data array

    - `p_data_element_count` length of the data array

    - `shape` pointer to the array of tensor dimensions

    - `shape_len` length of the dimension array

`template<typename T> void setInputs(const std::string& node_name, const std::vector<T>& values, const std::vector<int64_t>& shape)`
:   Convert a data vector to tensor and use the tensor as input to the node according to the specified node name for evaluating models.

    - `node_name` name of the input node in the trained model

    - `values` vector of data

    - `shape` vector of tensor dimension

`void evaluate(unsigned index_network=0)`
:   Run inference.

    - `index_network` index of the model to use

`template<typename T> T* getOutputs(unsigned node)`
:   Get a pointer to the array from the output node with index `node` after inference.

    - `node` output node index

`template<typename T> T* getOutputs(const std::string& node_name)`
:   Get a pointer to the array from the output node with name `node_name` after inference.

    - `node_name` name of the output node
