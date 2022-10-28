// clang-format off
#include <iostream>
#include <numeric>
#include <gttl.hpp>

using namespace gttl::literals; // for _D

// custom scalar type
using Scalar = float; 
// custome tensor ranks
constexpr std::size_t RANK1{1};
constexpr std::size_t RANK2{2};
// custom (non-homogenious) dimensions
constexpr gttl::Dimensions<RANK1> DIMENSIONS1{3_D};
constexpr gttl::Dimensions<RANK2> DIMENSIONS2{4_D, 2_D};
using Ten3 = gttl::Tensor<Scalar, RANK1, DIMENSIONS1>;
using Ten4x2 = gttl::Tensor<Scalar, RANK2, DIMENSIONS2>;

int main()
{
    using std::cout;

    Ten3 tensor1{};
    std::iota(std::begin(tensor1), std::end(tensor1), 1);

    Ten4x2 tensor2{};
    std::iota(std::begin(tensor2), std::end(tensor2), Ten3::size + 1);
    
    cout << "tensor1" << '\n';
    cout << (tensor1) << '\n';
    cout << "tensor2" << '\n';
    cout << (tensor2) << '\n';

    cout << '\n';
    cout << "outer_product(tensor1, tensor2)" << '\n';
    cout << (outer_product(tensor1, tensor2)) << '\n';
    cout << "outer_product(tensor2, tensor1)" << '\n';
    cout << (outer_product(tensor2, tensor1)) << '\n';
}