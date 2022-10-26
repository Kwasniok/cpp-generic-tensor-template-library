/**
 * @file
 * @brief multi-dimensional index range
 */

#ifndef GTTL_MULTI_INDEX_RANGE_HPP
#define GTTL_MULTI_INDEX_RANGE_HPP

#include <iterator>

#include "multi_index.hpp"

namespace gttl
{

/**
 * @brief Helper class for looping through all possible multi-index values.
 * @note To be used in range-based for loops only.
 */
template <std::size_t RANK, Dimensions<RANK> DIMENSIONS>
class MultiIndexRange
{
  public:
    constexpr static std::size_t rank{RANK};
    constexpr static Dimensions<RANK> dimensions{DIMENSIONS};

    // iterator traits
    using iterator_concept = std::forward_iterator_tag;
    using iterator_category = std::forward_iterator_tag;
    using value_type = MultiIndex<RANK, DIMENSIONS>;
    using difference_type = signed long int;
    using pointer = const value_type*;
    using reference = const value_type&;

  private:
    value_type value{};
    bool overflow{false};

  public:
    constexpr MultiIndexRange() = default;
    constexpr MultiIndexRange(const MultiIndexRange&) = default;
    constexpr MultiIndexRange(MultiIndexRange&&) = default;
    constexpr MultiIndexRange&
    operator=(const MultiIndexRange&) = default;
    constexpr MultiIndexRange&
    operator=(MultiIndexRange&&) = default;
    constexpr ~MultiIndexRange() = default;

  private:
    constexpr MultiIndexRange(const value_type& value, const bool overflow)
        : value{value}, overflow{overflow}
    {
    }

  public:
    constexpr MultiIndexRange&
    begin()
    {
        return *this;
    }

    constexpr MultiIndexRange
    end()
    {
        return MultiIndexRange{{}, true};
    }

    constexpr const value_type&
    operator*() const
    {
        return value;
    }

    constexpr MultiIndexRange&
    operator++()
    {
        auto [_, oflow] = ++value;
        overflow = oflow;
        return *this;
    }

    constexpr MultiIndexRange
    operator++(int)
    {
        auto tmp = *this;
        ++(*this);
        return tmp;
    }

    constexpr bool
    operator==(const MultiIndexRange& rhs) const
    {
        return overflow == rhs.overflow && value == rhs.value;
    }

    constexpr bool
    operator!=(const MultiIndexRange& rhs) const
    {
        return overflow != rhs.overflow || value != rhs.value;
    }
};

static_assert(
    std::forward_iterator<MultiIndexRange<0, std::array<std::size_t, 0>{}>>,
    "Must be STL compatible forward iterator type."
);
static_assert(
    std::forward_iterator<MultiIndexRange<1, std::array{Dimension{3}}>>,
    "Must be STL compatible forward iterator type."
);
static_assert(
    std::forward_iterator<
        MultiIndexRange<2, std::array{Dimension{3}, Dimension{3}}>>,
    "Must be STL compatible forward iterator type."
);

} // namespace gttl

#endif
