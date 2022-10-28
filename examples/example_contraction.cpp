// clang-format off
#include <iostream>
#include <numeric>
#include <gttl.hpp>

using namespace gttl::literals; // for _D

// custom scalar type
using Scalar = float; 
// custom tensor rank
constexpr std::size_t RANK{3};
// custom (non-homogenious) dimensions
constexpr gttl::Dimensions<RANK> DIMENSIONS{3_D, 2_D, 3_D};
using Mat3x2x3 = gttl::Tensor<Scalar, RANK, DIMENSIONS>;

int main()
{
    using std::cout;

    Mat3x2x3 mat1{};
    std::iota(std::begin(mat1), std::end(mat1), 1);
    
    cout << "contractions:" << '\n';
    cout << "contraction<0, 2>(mat1)" << '\n';
    cout << (contraction<0, 2>(mat1)) << '\n';
    cout << "contraction<2, 0>(mat1)" << '\n';
    cout << (contraction<2, 0>(mat1)) << '\n';
    // note: contraction<0, 1> and contraction<1, 0> are illegal
    //       contractions and consequently refuse to compile
}