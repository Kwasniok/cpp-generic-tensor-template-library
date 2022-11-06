
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

namespace functional

{
/*
 * @brief functional absulute value
 * @note Defaults to std::abs.
 */
template <class T>
struct abs {
    constexpr auto
    operator()(const T& x) const
    {
        return std::abs(x);
    }
};

} // namespace functional

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
    using neg = std::negate<T>;
    constexpr static T zero = static_cast<T>(0);
    constexpr static T one = static_cast<T>(1);
    using abs = gttl::functional::abs<T>;
};

} // namespace gttl

#endif
