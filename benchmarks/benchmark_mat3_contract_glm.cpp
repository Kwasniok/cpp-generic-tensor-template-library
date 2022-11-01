#include <glm/glm.hpp>

#include "benchmark.hpp"

using Tensor = glm::mat3x3; // is matrix of floats

Tensor
make()
{
    return {1, 2, 3, 4, 5, 6, 7, 8, 9};
}

Tensor tensor_in = make();
Tensor tensor_out{};

void
benchmark_body()
{
    tensor_out = tensor_in * tensor_in;
}
