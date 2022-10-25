
/**
 * @file
 * @brief scalar (field in mathematics)
 */

#ifndef GTTL_FIELD_HPP
#define GTTL_FIELD_HPP

#include <concepts>
#include <functional>

namespace gttl
{

/**
 * @brief traits for field type
 * @note The axioms and semantics are NOT checked!
 */
template <typename T>
struct field_traits {
    using add = std::plus<T>;
    using sub = std::minus<T>;
    using mul = std::multiplies<T>;
    using div = std::divides<T>;
    constexpr static T zero = static_cast<T>(0);
    constexpr static T one = static_cast<T>(1);
};

} // namespace gttl

#endif
