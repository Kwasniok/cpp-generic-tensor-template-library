# Generic Tensor Template Library

## Target
- highly flexible and math-orientated tensor library
  - **selectable field/scalar type** with traits
  - **arbitrary rank** for tensors
  - **arbitrary dimension** per rank
- best possible efficiency given these constraints

## Structure
This is a header-only library. It is located inside the `include` folder.
To import everything use
```
#include <gttl.hpp>
```
To import parts of the library use
```
#include <gttl/<path-to-part>.hpp>
```

## Documentation
To generate the documentation in the folder `doc` run
```
make doc
```
(see [requirements])

## Tests
To generate and execute all tests run
```
make test
```
(see [requirements])

## Basic Example
```cpp
#include <iostream>
#include <gttl.hpp>

using namespace gttl::literals; // for _D

// custome scalar type
using Scalar = float; 
// custome tensor rank
constexpr std::size_t RANK{2};
// custome (non-homogenious) dimensions
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
```
output:
```
basic vector operations:
-mat1
{{-1,-2,-3},{-4,-5,-6}}
mat1 + mat2
{{7,7,7},{7,7,7}}
mat1 - mat2
{{-5,-3,-1},{1,3,5}}
mat1 * 3
{{3,6,9},{12,15,18}}

element access:
mat1.at(1)
{4,5,6}
mat1.at(1, 2)
6

elementwise operations:
mat1.elementwise(square)
{{1,4,9},{16,25,36}}
```

## 'Extreme' Example
```cpp
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
```
output:
```
tensor.at(0, 1, 2, 3, 4)
{1414,1415,1416,1417,1418,1419,1420}
```

## Outer Product / Tensor Product Example
```cpp
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
```
output:
```
tensor1
{1,2,3}
tensor2
{{4,5},{6,7},{8,9},{10,11}}

outer_product(tensor1, tensor2)
{{{4,5},{6,7},{8,9},{10,11}},{{8,10},{12,14},{16,18},{20,22}},{{12,15},{18,21},{24,27},{30,33}}}
outer_product(tensor2, tensor1)
{{{4,8,12},{5,10,15}},{{6,12,18},{7,14,21}},{{8,16,24},{9,18,27}},{{10,20,30},{11,22,33}}}
```

## Contraction Example
```cpp
#include <iostream>
#include <numeric>
#include <gttl.hpp>

using namespace gttl::literals; // for _D

// custome scalar type
using Scalar = float; 
// custome tensor rank
constexpr std::size_t RANK{3};
// custome (non-homogenious) dimensions
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
```
output:
```
contractions:
contraction<0, 2>(mat1)
{24,33}
contraction<2, 0>(mat1)
{24,33}
```

## Namespaces
- library: `gttl`
- literal suffixes: `gttl::literals`

## Requirements
- C++20 (for using)
- C++23 (for testing)
- make 4.3+ (for documentation and testing)
- doxygen 1.9+ (for documentation)
- Boost 1.80+ (for testing)

[requirements]: #requirements
