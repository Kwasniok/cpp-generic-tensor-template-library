/**
 * @file
 * @brief multi-dimensional index
 */

#ifndef GTTL_MULTI_INDEX_HPP
#define GTTL_MULTI_INDEX_HPP

#include <iostream>

#include "cexpr/array.hpp"
#include "dimensions.hpp"
#include "index.hpp"

namespace gttl
{

// TODO: rename to generic type name and use as alias
/**
 * @brief Multi dimensional extension of gttl::Index.
 */
template <std::size_t RANK, Dimensions<RANK> DIMENSIONS>

requires(cexpr::array::all_strictly_positive(DIMENSIONS)) struct MultiIndex {
  public:
    constexpr static std::size_t rank{RANK};
    constexpr static Dimensions<RANK> dimensions{DIMENSIONS};

    using first_type = Index<DIMENSIONS[0]>;
    using rest_type = MultiIndex<RANK - 1, cexpr::array::rest(DIMENSIONS)>;

  private:
    first_type first_value{};
    rest_type rest_value{};

  public:
    constexpr MultiIndex() = default;

    constexpr MultiIndex(const first_type first, const rest_type& rest)
        : first_value{first}, rest_value{rest}
    {
    }

    constexpr MultiIndex(const Dimension first, const rest_type& rest)
        : first_value{first}, rest_value{rest}
    {
    }

    /**
     * @brief initializes some or all indices from first to last
     * @note The remaining indices are zero-initialized.
     */
    template <typename... Ts>
    constexpr MultiIndex(const Dimension first, const Ts... rest)
        : first_value(first), rest_value{rest...}
    {
    }

    constexpr MultiIndex(const MultiIndex&) = default;
    constexpr MultiIndex(MultiIndex&&) = default;
    constexpr MultiIndex&
    operator=(const MultiIndex&) = default;
    constexpr MultiIndex&
    operator=(MultiIndex&&) = default;
    constexpr ~MultiIndex() = default;

    // trivial conversions
    constexpr explicit MultiIndex(const Dimensions<RANK>& is)
        : first_value{cexpr::array::first(is)},
          rest_value{cexpr::array::rest(is)}
    {
    }

    [[nodiscard]] constexpr operator Dimensions<RANK>() const
    {
        return cexpr::array::concatenate(
            static_cast<Dimension>(first_value),
            static_cast<Dimensions<RANK - 1>>(rest_value)
        );
    }

    MultiIndex&
    operator=(const Dimensions<RANK>& is)
    {
        first_value = cexpr::array::first(is);
        rest_value = cexpr::array::rest(is);
        return *this;
    }

  public:
    // note:significant spped up for concatenate etc. with Trusted
    constexpr explicit MultiIndex(
        const Dimensions<RANK>& is, const internal::Trusted
    )
        : first_value{cexpr::array::first(is), internal::Trusted{}},
          rest_value{cexpr::array::rest(is), internal::Trusted{}}
    {
    }

    constexpr bool
    operator==(const MultiIndex&) const = default;
    constexpr bool
    operator!=(const MultiIndex&) const = default;

    /**
     * @note Strict equality of values required (not modulo).
     */
    constexpr bool
    operator==(const Dimensions<RANK>& is) const
    {
        return (static_cast<Dimension>(first_value) == cexpr::array::first(is)
               ) &&
               (rest_value == cexpr::array::rest(is));
    }

    /**
     * @note Scrict equality of values required (not modulo).
     */
    constexpr bool
    operator!=(const Dimensions<RANK>& is) const
    {
        return (static_cast<Dimension>(first_value) != cexpr::array::first(is)
               ) ||
               (rest_value != cexpr::array::rest(is));
    }

    constexpr first_type
    first() const
    {
        return first_value;
    }

    constexpr first_type&
    first()
    {
        return first_value;
    }

    constexpr const rest_type&
    rest() const
    {
        return rest_value;
    }

    constexpr rest_type&
    rest()
    {
        return rest_value;
    }

    template <std::size_t I>
    constexpr Index<DIMENSIONS[I]>
    get() const requires(I < RANK)
    {
        if constexpr (I == 0) {
            return first_value;
        } else {
            return rest_value.template get<I - 1>();
        }
    }

    template <std::size_t I>
    constexpr Index<DIMENSIONS[I]>&
    get() requires(I < RANK)
    {
        if constexpr (I == 0) {
            return first_value;
        } else {
            return rest_value.template get<I - 1>();
        }
    }

    // .second == overflow
    constexpr std::pair<MultiIndex&, bool>
    operator++()
    {
        auto [_, rest_overlfow] = ++rest_value;
        bool overflow = false;
        if (rest_overlfow) {
            ++first_value;
            overflow = first_value == 0;
        }
        return {*this, overflow};
    }

    // .second == underflow
    constexpr std::pair<MultiIndex&, bool>
    operator--()
    {
        auto [_, rest_underflow] = --rest_value;
        bool underflow = false;
        if (rest_underflow) {
            --first_value;
            underflow = first_value == DIMENSIONS[0] - 1;
        }
        return {*this, underflow};
    }

    /**
     * @brief concatenate with another index
     * @note Left hand will become prefix.
     */
    template <std::size_t RANK_RHS, Dimensions<RANK_RHS> DIMENSIONS_RHS>
    constexpr MultiIndex<
        RANK + RANK_RHS,
        cexpr::array::concatenate(DIMENSIONS, DIMENSIONS_RHS)>
    concatenate(const MultiIndex<RANK_RHS, DIMENSIONS_RHS>& rhs) const
    {
        using ResMultiIndex = MultiIndex<
            RANK + RANK_RHS,
            cexpr::array::concatenate(DIMENSIONS, DIMENSIONS_RHS)>;
        using ThisArray = Dimensions<RANK>;
        using RhsArray = Dimensions<RANK_RHS>;
        // convert to arrays and concatenate then convert back
        const ThisArray this_array = ThisArray(*this);
        const RhsArray rhs_array = RhsArray(rhs);
        return ResMultiIndex(
            cexpr::array::concatenate(this_array, rhs_array),
            internal::Trusted{}
        );
    }

    /**
     * @brief split another index and insert each index at various positions
     * @note Positions are relative to original left hand index.
     * @note Relative positions of inserted elements are preserved.
     */
    template <
        std::size_t N,
        std::array<std::size_t, N> POSITIONS,
        std::size_t INSERTINDEX_RANK,
        Dimensions<INSERTINDEX_RANK> INSERTINDEX_DIMENSIONS>
    constexpr MultiIndex<
        RANK + INSERTINDEX_RANK,
        cexpr::array::multi_insert_at<N, POSITIONS>(
            INSERTINDEX_DIMENSIONS, DIMENSIONS
        )>
    multi_insert_at(
        const MultiIndex<INSERTINDEX_RANK, INSERTINDEX_DIMENSIONS>& insert_index
    ) const
        requires(cexpr::array::all_less_than(
            static_cast<std::size_t>(RANK) + 1, POSITIONS
        ))
    {
        using InsertedMultiIndex = MultiIndex<
            RANK + INSERTINDEX_RANK,
            cexpr::array::multi_insert_at<N, POSITIONS>(
                INSERTINDEX_DIMENSIONS, DIMENSIONS
            )>;
        using ThisArray = Dimensions<RANK>;
        using InsertArray = Dimensions<INSERTINDEX_RANK>;
        // convert to arrays and insert then convert back
        const ThisArray this_as_array = ThisArray(*this);
        const InsertArray insert_as_array = InsertArray(insert_index);
        return InsertedMultiIndex(
            cexpr::array::multi_insert_at<N, POSITIONS>(
                insert_as_array, this_as_array
            ),
            internal::Trusted{}
        );
    }

    /**
     * @brief erase various dimensions
     */
    template <std::size_t N, std::array<std::size_t, N> POSITIONS>
    constexpr MultiIndex<
        RANK - N,
        cexpr::array::multi_erase_at<N, POSITIONS>(DIMENSIONS)>
    multi_erase_at() const
        requires((N <= RANK) && cexpr::array::all_less_than(RANK, POSITIONS))
    {
        using ResMultiIndex = MultiIndex<
            RANK - N,
            cexpr::array::multi_erase_at<N, POSITIONS>(DIMENSIONS)>;
        using ThisArray = Dimensions<RANK>;
        // convert to arrays and erase, then convert back
        const ThisArray this_as_array = ThisArray(*this);
        return ResMultiIndex(
            cexpr::array::multi_erase_at<N, POSITIONS>(this_as_array),
            internal::Trusted{}
        );
    }
};

/**
 * @brief Specialization for zero-dimensional gttl::MultiIndex.
 */
template <>
class MultiIndex<0, Dimensions<0>{}>
{
  public:
    constexpr MultiIndex() = default;
    constexpr MultiIndex(const MultiIndex&) = default;
    constexpr MultiIndex(MultiIndex&&) = default;
    constexpr MultiIndex&
    operator=(const MultiIndex&) = default;
    constexpr MultiIndex&
    operator=(MultiIndex&&) = default;
    constexpr ~MultiIndex() = default;

    constexpr explicit MultiIndex(const Dimensions<0>&) { }

    [[nodiscard]] constexpr operator Dimensions<0>() const { return {}; }

  public:
    constexpr explicit MultiIndex(const Dimensions<0>&, const internal::Trusted)
    {
    }

    MultiIndex&
    operator=(const Dimensions<0>&)
    {
        return *this;
    }

    constexpr bool
    operator==(const MultiIndex&) const = default;
    constexpr bool
    operator!=(const MultiIndex&) const = default;

    constexpr bool
    operator==(const Dimensions<0>&) const
    {
        return true;
    }

    constexpr bool
    operator!=(const Dimensions<0>&) const
    {
        return false;
    }

    constexpr std::pair<MultiIndex&, bool>
    operator++()
    {
        return {*this, true};
    }

    constexpr std::pair<MultiIndex&, bool>
    operator--()
    {
        return {*this, true};
    }

    template <std::size_t RANK_RHS, Dimensions<RANK_RHS> DIMENSIONS_RHS>
    constexpr MultiIndex<RANK_RHS, DIMENSIONS_RHS>
    concatenate(const MultiIndex<RANK_RHS, DIMENSIONS_RHS>& rhs) const
    {
        return rhs;
    }

    template <
        std::size_t N,
        std::array<std::size_t, N> POSITIONS,
        std::size_t INSERTINDEX_RANK,
        Dimensions<INSERTINDEX_RANK> INSERTINDEX_DIMENSIONS>
    constexpr MultiIndex<INSERTINDEX_RANK, INSERTINDEX_DIMENSIONS>
    multi_insert_at(
        const MultiIndex<INSERTINDEX_RANK, INSERTINDEX_DIMENSIONS>& insert_index
    ) const
        requires(
            cexpr::array::all_less_than(static_cast<std::size_t>(1), POSITIONS)
        )
    {
        return insert_index;
    }

    template <std::size_t N, std::array<std::size_t, N> POSITIONS>
    constexpr MultiIndex<0, Dimensions<0>{}>
    multi_erase_at() const requires(N == 0)
    {
        return *this;
    }
};

template <std::size_t RANK, Dimensions<RANK> DIMENSIONS>
std::ostream&
operator<<(std::ostream& os, const MultiIndex<RANK, DIMENSIONS>& index)
{
    os << "MI<" << RANK << ", {";
    for (const auto dim : DIMENSIONS) {
        os << dim << ',';
    }
    os << "}>{";
    auto values = Dimensions<RANK>(index);
    for (const auto val : values) {
        os << val << ',';
    }
    os << '}';
    return os;
}

} // namespace gttl

#endif
