// clang-format off
#include <iostream>
#include <numeric>
#include <gttl.hpp>

using namespace gttl::literals; // for _D

using Scalar = float;
// unusual rank
constexpr std::size_t RANK{6};
// (non-homogenious) dimensions
constexpr gttl::Dimensions<RANK> DIMENSIONS{2_D, 3_D, 4_D, 5_D, 6_D, 7_D};
using TensorR6 = gttl::Tensor<Scalar, RANK, DIMENSIONS>;

int main()
{
    using std::cout;

    TensorR6 tensor;

    std::iota(std::begin(tensor), std::end(tensor), 0);

    cout << '\n';
    cout << "tensor.at(0, 1, 2, 3, 4)" << '\n';
    cout << (tensor.at(0, 1, 2, 3, 4)) << '\n';

}