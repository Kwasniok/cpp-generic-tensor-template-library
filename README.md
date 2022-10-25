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

## Namespaces
- library: `gttl`
- literal suffixes: `gttl::literals`

## Dependencies
- C++20 (for using)
- C++23 (for testing)
- Boost 1.80+ (for testing)
