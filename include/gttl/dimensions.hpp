
/**
 * @file
 * @brief
 */

#ifndef GTTL_DIMENSIONS_HPP
#define GTTL_DIMENSIONS_HPP

#include <array>
#include <cstdint>

namespace gttl
{

// MAINTENANCE: Change this to various kinds of unsigned integral built-in types
//              to ensure code quality.
// MAINTENANCE: Make this part of a global configuration.
using Dimension = std::size_t;

template <std::size_t RANK>
using Dimensions = std::array<Dimension, RANK>;

namespace literals
{
constexpr Dimension operator"" _D(unsigned long long value)
{
    return static_cast<Dimension>(value);
}
} // namespace literals

} // namespace gttl

#endif
