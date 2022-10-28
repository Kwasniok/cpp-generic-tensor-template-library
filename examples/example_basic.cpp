// clang-format off
#include <iostream>
#include <gttl.hpp>

using namespace gttl::literals; // for _D

// custom scalar type
using Scalar = float; 
// custom tensor rank
constexpr std::size_t RANK{2};
// custom (non-homogenious) dimensions
constexpr gttl::Dimensions<RANK> DIMENSIONS{2_D, 3_D};
using Mat2x3 = gttl::Tensor<Scalar, RANK, DIMENSIONS>;

int main()
{
    using std::cout;

    const Mat2x3 mat1{1, 2, 3, 4, 5, 6};
    const Mat2x3 mat2{6, 5, 4, 3, 2, 1};

    cout << "basic vector operations:" << '\n';
    cout << "-mat1" << '\n';
    cout << (-mat1) << '\n';
    cout << "mat1 + mat2" << '\n';
    cout << (mat1 + mat2) << '\n';
    cout << "mat1 - mat2" << '\n';
    cout << (mat1 - mat2) << '\n';
    cout << "mat1 * 3" << '\n';
    cout << (mat1 * 3) << '\n';

    cout << '\n';
    cout << "element access:" << '\n';
    cout << "mat1.at(1)" << '\n';
    cout << (mat1.at(1)) << '\n';
    cout << "mat1.at(1, 2)" << '\n';
    cout << (mat1.at(1, 2)) << '\n';

    constexpr auto square = [](const Scalar& x) { return x * x; };
    cout << '\n';
    cout << "elementwise operations:" << '\n';
    cout << "mat1.elementwise(square)" << '\n';
    cout << (mat1.elementwise(square)) << '\n';
}