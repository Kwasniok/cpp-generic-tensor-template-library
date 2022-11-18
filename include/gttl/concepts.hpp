
/**
 * @file
 * @brief concepts
 */

#ifndef GTTL_CONCEPTS
#define GTTL_CONCEPTS

#include <concepts>

namespace gttl
{

namespace internal
{

/** @brief can be converted to U but is not the same as V*/
template <typename T, typename U, typename V>
concept convertible_to_but_not_same_as =
    std::convertible_to<T, U> && !std::same_as<T, V>;

} // namespace internal

} // namespace gttl

#endif
