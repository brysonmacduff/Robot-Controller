#include <torch/script.h>
#include <iostream>

int main() {
    torch::jit::script::Module module;

    module = torch::jit::load("model.pt");

    // Create input tensor
    torch::Tensor input = torch::rand({1, 4});

    // Run inference
    std::vector<torch::jit::IValue> inputs;
    inputs.push_back(input);

    at::Tensor output = module.forward(inputs).toTensor();

    std::cout << output << std::endl;
}
