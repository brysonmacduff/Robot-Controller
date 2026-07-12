#include <onnxruntime_cxx_api.h>
#include <iostream>

int main() {
    // Create ONNX Runtime environment
    Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "simple");

    // Create session options
    Ort::SessionOptions session_options;
    session_options.SetIntraOpNumThreads(1);

    // Load the ONNX model
    Ort::Session session(env, "model.onnx", session_options);

    // Input data (1x4 tensor)
    std::vector<float> input_data = {0.1f, 0.2f, 0.3f, 0.4f};
    std::vector<int64_t> input_shape = {1, 4};

    // Create CPU memory info (required on ARM)
    Ort::MemoryInfo memory_info = Ort::MemoryInfo::CreateCpu(
        OrtDeviceAllocator, OrtMemTypeCPU);

    // Create input tensor (note: byte size!)
    Ort::Value input_tensor = Ort::Value::CreateTensor(
        memory_info,
        input_data.data(),
        input_data.size() * sizeof(float),
        input_shape.data(),
        input_shape.size()
    );

    // Names must match your ONNX export
    const char* input_names[] = {"input"};
    const char* output_names[] = {"output"};

    // Run inference
    auto output_tensors = session.Run(
        Ort::RunOptions{nullptr},
        input_names,
        &input_tensor,
        1,
        output_names,
        1
    );

    // Extract output
    float* output = output_tensors.front().GetTensorMutableData<float>();
    std::cout << "Output: " << output[0] << std::endl;

    return 0;
}
