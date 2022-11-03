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
using Tensor3 = gttl::Tensor<Scalar, 1, Dims<1>{3_D}>;
using Tensor3x3x3 = gttl::Tensor<Scalar, 3, Dims<3>{3_D, 3_D, 3_D}>;

template <typename Tensor>
constexpr auto
make()
{
    Tensor tensor;
    std::iota(std::begin(tensor), std::end(tensor), 0);
    return tensor;
}

Tensor3x3x3 tensor_in1{make<Tensor3x3x3>()};
Tensor3 tensor_in2{make<Tensor3>()};
Tensor3 tensor_in3{make<Tensor3>()};
Tensor3 tensor_out{};

void
benchmark_body()
{
    // inspiration: Christoffel-velocity-velocity contraction

    // contraction scheme:
    // gamma^i_j_k u^j u^k
    //       0 1 2   3   4   (contract over i <-> 1 and 3)
    //       0   1       2   (contract over j <-> 1 and 2)

    tensor_out = gttl::contraction<1, 2>(
        gttl::contraction<1, 3>(tensor_in1, tensor_in2), tensor_in3
    );
}
