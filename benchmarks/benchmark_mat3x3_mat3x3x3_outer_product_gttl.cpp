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
using TensorIn1 = gttl::Tensor<Scalar, 2, Dims<2>{3_D, 3_D}>;
using TensorIn2 = gttl::Tensor<Scalar, 3, Dims<3>{3_D, 3_D, 3_D}>;
using TensorOut = gttl::Tensor<Scalar, 5, Dims<5>{3_D, 3_D, 3_D, 3_D, 3_D}>;

template <typename Tensor>
constexpr auto
make()
{
    Tensor tensor;
    std::iota(std::begin(tensor), std::end(tensor), 0);
    return tensor;
}

TensorIn1 tensor_in1{make<TensorIn1>()};
TensorIn2 tensor_in2{make<TensorIn2>()};
TensorOut tensor_out{};

void
benchmark_body()
{
    tensor_out = gttl::outer_product(tensor_in1, tensor_in2);
}
