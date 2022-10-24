/**
 * @file
 * @brief
 */

#ifndef GTTL_INDEX_HPP
#define GTTL_INDEX_HPP

#include "dimensions.hpp"
#include "internal/trusted.hpp"

namespace gttl
{

template <Dimension DIMENSION>

requires(DIMENSION > 0) class Index
{
  public:
    using type = Index<DIMENSION>;
    constexpr static Dimension dimension{DIMENSION};

  private:
    Dimension value{0}; // invariance: be in 0..(DIMENSION - 1)

  public:
    constexpr Index() = default;
    constexpr Index(const Index&) = default;
    constexpr Index(Index&&) = default;
    constexpr Index&
    operator=(const Index&) = default;
    constexpr Index&
    operator=(Index&&) = default;
    constexpr ~Index() = default;

    constexpr explicit Index(const Dimension value)
        : value{static_cast<Dimension>(value % DIMENSION)}
    {
    }

    constexpr Index&
    operator=(const Dimension val)
    {
        value = static_cast<Dimension>(val % DIMENSION);
        return *this;
    }

  public:
    // it is trusted that 0 <= value < DIMENSION
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
