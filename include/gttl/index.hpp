/**
 * @file
 * @brief bounded natural number index
 */

#ifndef GTTL_INDEX_HPP
#define GTTL_INDEX_HPP

#include "dimensions.hpp"
#include "internal/trusted.hpp"

namespace gttl
{

/**
 * @brief Bounded natural number index.
 * @note  The index value is in `0..(DIMENSION - 1)` at all times.
 */
template <Dimension DIMENSION>

requires(DIMENSION > 0) class Index
{
  public:
    /* @brief Dimension of the index set (every index is less than this) */
    constexpr static Dimension dimension{DIMENSION};

  private:
    /*
     * @brief Value of index.
     * @note  Must be in `0..(DIMENSION - 1)`
     */
    Dimension value{0};

  public:
    constexpr Index() = default;
    constexpr Index(const Index&) = default;
    constexpr Index(Index&&) = default;
    constexpr Index&
    operator=(const Index&) = default;
    constexpr Index&
    operator=(Index&&) = default;
    constexpr ~Index() = default;

    /**
     * @brief Construct from value.
     * @note Index value will be `value % DIMENSION`.
     */
    constexpr explicit Index(const Dimension value)
        : value{static_cast<Dimension>(value % DIMENSION)}
    {
    }

    /**
     * @brief Assign a value.
     * @note Index value will be `value % DIMENSION`.
     */
    constexpr Index&
    operator=(const Dimension val)
    {
        value = static_cast<Dimension>(val % DIMENSION);
        return *this;
    }

  public:
    /**
     * @brief Construct from TRSUTED value.
     * @note Trusted value MUST be in `0..(DIMENSION-1)` or else the result is
     *       undefined.
     */
    constexpr Index(const Dimension value, const internal::Trusted)
        : value(value)
    {
    }

    constexpr operator Dimension() const { return value; }

    constexpr bool
    operator==(const Index&) const = default;
    constexpr bool
    operator!=(const Index&) const = default;
    constexpr bool
    operator<(const Index&) const = default;
    constexpr bool
    operator<=(const Index&) const = default;
    constexpr bool
    operator>(const Index&) const = default;
    constexpr bool
    operator>=(const Index&) const = default;

    constexpr Dimension
    operator++()
    {
        ++value;
        value %= DIMENSION;
        return value;
    }

    constexpr Dimension
    operator--()
    {
        // note: avoid negative numbers due to % behaviour
        value += DIMENSION - 1;
        value %= DIMENSION;
        return value;
    }
};

} // namespace gttl

#endif
