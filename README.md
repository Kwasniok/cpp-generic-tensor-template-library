# Generic Tensor Template Library

## Target
- highly flexible and math-orientated tensor (representation) library allowing:
  - **arbitrary scalar types**
  - **arbitrary scalar operations** (type traits)
  - **arbitrary ranks** for tensors
  - **arbitrary dimensions** per rank
- with the goal to be as efficiency as possible under these constraints

## Features
- scalar type traits
- customizable tensor shapes
- scalar operations `+`, `-`, `*`, `/`
- tensor operations `+`, `-`, `+=`, `-=`, `*`, `*=` (scalar multiplication), `outer_product`, `contract`
- element and subtensor access
- (in-place) elementwise operations of arbitrary arity
- **static memory by default**: tensors are represented as (fixed-size) arrays of scalars
- strongly typed: **compile-time checks for contractions** etc.

## Non-Features
These things are not implemented in this library:
- reshaping tensors
- change of index order (e.g. transposition)
- dot, cross, length, trace or any other additional structure beyond the bare minimum
- solving linear equations

note: In the furture some of these might be provided as an extension. Forcing any specific interpretation of the tensors may contradict some of the use cases and is not desired.

## Extenions
The following items are supported via extensions of this library:
- solving differential equations via 'boost::numeric::odeint`

## Usage
This is a header-only library. It is located inside the `include` folder.
To import everything use
```cpp
#include <gttl.hpp>
```
To import parts of the library use
```cpp
#include <gttl/path-to-part.hpp>
```

For support of `boost::numeric::odeint` import
```cpp
#include <gttl/extensions/boost/numeric/odeint.hpp>
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

note: Some tests have additional requirements like `boost::numeric::odeint` and may need to be disabled manually.

## Basic Example
```cpp
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
// custom tensor ranks
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
```
output:
```
tensors:
tensor1
{{{1,2,3},{4,5,6}},{{7,8,9},{10,11,12}},{{13,14,15},{16,17,18}}}
tensor2
{{19,20},{21,22},{23,24}}


contraction:
contraction<0, 2>(tensor1)
{24,33}

note: oder of indices is irrelevant
contraction<2, 0>(tensor1)
{24,33}


contraction of outer product:
contraction<0, 3>(tensor1, tensor2)
{{{465,486},{528,552},{591,618}},{{654,684},{717,750},{780,816}}}

note: indices may belong to the same tensor
contraction<0, 2>(tensor1, tensor2)
{{{456,480},{504,528},{552,576}},{{627,660},{693,726},{759,792}}}
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
