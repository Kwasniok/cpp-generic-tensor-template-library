#include <gttl.hpp>
#include <numeric>

#include "benchmark.hpp"

using namespace gttl::literals; // for _D

// using Scalar = float;
// using Scalar = double;
// using Scalar = long double;

using Scalar = double;

template <std::size_t N>
using Dims = gttl::Dimensions<N>;
using Tensor = gttl::Tensor<Scalar, 2, Dims<2>{3_D, 3_D}>;

constexpr auto
make()
{
    Tensor tensor;
    std::iota(std::begin(tensor), std::end(tensor), 0);
    return tensor;
}

Tensor tensor_in = make();
Tensor tensor_out{};

void
benchmark_body()
{
    tensor_out = gttl::contraction<1, 2>(tensor_in, tensor_in);
}
