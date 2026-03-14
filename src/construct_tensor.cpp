#include <torch/torch.h>
#include <iostream>

using namespace std;


void create_huge_tensor(int gpu_index) {
    torch::Device device(torch::kCUDA, gpu_index);

    // Allocate a large tensor (e.g., 10 GB)
    size_t num_elements = ();
}