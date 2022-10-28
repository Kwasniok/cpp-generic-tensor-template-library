// clang-format off
#include <iostream>
#include <numeric>
#include <gttl.hpp>

using namespace gttl::literals; // for _D

// custom scalar type
using Scalar = float; 
// custom tensor rank
constexpr std::size_t RANK1{3};
constexpr std::size_t RANK2{2};
// custom (non-homogenious) dimensions
constexpr gttl::Dimensions<RANK1> DIMENSIONS1{3_D, 2_D, 3_D};
constexpr gttl::Dimensions<RANK2> DIMENSIONS2{3_D, 2_D};
using Ten3x2 = gttl::Tensor<Scalar, RANK1, DIMENSIONS1>;
using Ten3x2x4 = gttl::Tensor<Scalar, RANK2, DIMENSIONS2>;

int main()
{
    using std::cout;

    Ten3x2 tensor1{};
    std::iota(std::begin(tensor1), std::end(tensor1), 1);

    Ten3x2x4 tensor2{};
    std::iota(std::begin(tensor2), std::end(tensor2), Ten3x2::size + 1);
    
    cout << "tensors:" << '\n';
    cout << "tensor1" << '\n';
    cout << (tensor1) << '\n';
    cout << "tensor2" << '\n';
    cout << (tensor2) << '\n';

    cout << '\n' << '\n';
    cout << "contraction:" << '\n';
    cout << "contraction<0, 2>(tensor1)" << '\n';
    cout << (contraction<0, 2>(tensor1)) << '\n';
    cout << '\n';
    cout << "note: oder of indices is irrelevant" << '\n';
    cout << "contraction<2, 0>(tensor1)" << '\n';
    cout << (contraction<2, 0>(tensor1)) << '\n';
    // note: contraction<0, 1>, contraction<1, 0> etc. are illegal
    //       contractions and consequently refuse to compile

    cout << '\n' << '\n';
    cout << "contraction of outer product:" << '\n';
    cout << "contraction<0, 3>(tensor1, tensor2)" << '\n';
    cout << (contraction<0, 3>(tensor1, tensor2)) << '\n';
    cout << '\n';
    cout << "note: indices may belong to the same tensor" << '\n';
    cout << "contraction<0, 2>(tensor1, tensor2)" << '\n';
    cout << (contraction<0, 2>(tensor1, tensor2)) << '\n';
    // note: illegal contractions are refused again
}