
/**
 * @file
 * @brief tensor
 */

#ifndef GTTL_TENSOR_HPP
#define GTTL_TENSOR_HPP

#include "dimensions.hpp"
#include "multi_index.hpp"
#include "multi_index_range.hpp"

namespace gttl
{

/**
 * @note Is a standard layout struct.
 * @note Is an aggregate type.
 */
template <typename Scalar, std::size_t RANK, Dimensions<RANK> DIMENSIONS>

requires(
    (RANK >= 0) && cexpr::array::all_strictly_positive(DIMENSIONS)
) struct Tensor {

    using scalar_type = Scalar;
    constexpr static std::size_t rank{RANK};
    constexpr static Dimensions<RANK> dimensions{DIMENSIONS};

    using multi_index_type = MultiIndex<RANK, DIMENSIONS>;
    using multi_index_range_type = MultiIndexRange<RANK, DIMENSIONS>;

    constexpr static Dimension size{cexpr::array::prod(DIMENSIONS)};
    // coefficient memory layout:
    //     let mi = {i_1, i_2, ..., i_n} be a multi-index
    //     A) Interpreting mi as a sequence of digits,
    //        each mi is associated a natural number n which is its location.
    //     OR
    //     B) Recursively cycle through all indices i_j where
    //        the left most index cycles the slowest.
    //
    //     EXAMPLE 2x3 matrix
    //     size = 2 * 3 = 6
    //     multi-index = (i, j)
    //       +---+---+---+---+---+---+
    //       |   |   |   |   |   |   |
    //       +---+---+---+---+---+---+
    //     i   0   0   0   1   1   1
    //     j   0   1   2   0   1   2
    using array_type = std::array<Scalar, size>;
    array_type coefficients;

    // note: No further data members are allowed!
    // note: Must be standard layout class!
    //  i.e. std::is_standard_layout_v<Tensor>
    //       && sizeof(Tensor) == sizeof(coefficients)
    //       must be true
    // note: Must be aggregate type!
    //  i.e. std::is_aggregate_v<Tensor>
    //       && sizeof(Tensor) == sizeof(coefficients)
    //       must be true

    template <std::size_t REMOVED_RANKS>
    requires(
        RANK - REMOVED_RANKS >= 0
    ) constexpr static std::size_t subtensor_size =
        Tensor<Scalar, RANK - REMOVED_RANKS, drop<REMOVED_RANKS>(DIMENSIONS)>::
            size;

    // trivial conversion to array
    operator const array_type&() const { return coefficients; }

    // rank zero tensor is trivialy convertible to scalar
    operator Scalar() const requires(RANK == 0) { return coefficients[0]; }

    operator Scalar&() requires(RANK == 0) { return coefficients[0]; }

    Tensor&
    operator=(const Scalar& value) requires(RANK == 0)
    {
        coefficients[0] = value;
        return *this;
    }

    constexpr auto
    begin() noexcept
    {
        return std::begin(coefficients);
    }

    constexpr auto
    begin() const noexcept
    {
        return std::cbegin(coefficients);
    }

    constexpr auto
    cbegin() const noexcept
    {
        return std::cbegin(coefficients);
    }

    constexpr auto
    end() noexcept
    {
        return std::end(coefficients);
    }

    constexpr auto
    end() const noexcept
    {
        return std::cend(coefficients);
    }

    constexpr auto
    cend() const noexcept
    {
        return std::cend(coefficients);
    }

    constexpr auto
    rbegin() noexcept
    {
        return std::rbegin(coefficients);
    }

    constexpr auto
    rbegin() const noexcept
    {
        return std::crbegin(coefficients);
    }

    constexpr auto
    crbegin() const noexcept
    {
        return std::crbegin(coefficients);
    }

    constexpr auto
    rend() noexcept
    {
        return std::rend(coefficients);
    }

    constexpr auto
    rend() const noexcept
    {
        return std::crend(coefficients);
    }

    constexpr auto
    crend() const noexcept
    {
        return std::crend(coefficients);
    }

    constexpr static multi_index_range_type
    make_index_range()
    {
        return {};
    }

    constexpr void
    swap(Tensor& other) noexcept(std::is_nothrow_swappable_v<Scalar>)
    {
        coefficients.swap(other.coefficients);
    }

    // return by reference in case of large subtensors
    // might throw: std::out_of_range
    template <typename... Ts>
    [[nodiscard]] constexpr const auto&
    at(const std::size_t i_first, const Ts... i_rest) const requires(RANK > 0)
    {
        using SubTensor =
            Tensor<Scalar, RANK - 1, cexpr::array::rest(DIMENSIONS)>;
        // note: Requires that coefficients are the only non-static data member.
        static_assert(
            std::is_standard_layout_v<Tensor>,
            "Tensor must be of standard layout."
        );
        static_assert(
            sizeof(Tensor) == sizeof(Tensor::coefficients),
            "`coefficients`must be only non-static data member of Tensor"
        );
        static_assert(
            std::is_standard_layout_v<SubTensor>,
            "SubTensor must be of standard layout."
        );
        static_assert(
            sizeof(SubTensor) == sizeof(SubTensor::coefficients),
            "`coefficients`must be only non-static data member of SubTensor"
        );
        const std::array<SubTensor, DIMENSIONS[0]>& subtensors =
            reinterpret_cast<const std::array<SubTensor, DIMENSIONS[0]>&>(*this
            );
        if constexpr (sizeof...(Ts) == 0) {
            return subtensors.at(i_first);
        } else {
            return subtensors.at(i_first).at(i_rest...);
        }
    }

    // might throw: std::out_of_range
    template <typename... Ts>
    [[nodiscard]] constexpr auto&
    at(const std::size_t i_first, const Ts... i_rest) requires(RANK > 0)
    {
        using SubTensor =
            Tensor<Scalar, RANK - 1, cexpr::array::rest(DIMENSIONS)>;
        // note: Requires that coefficients are the only non-static data member.
        static_assert(
            std::is_standard_layout_v<Tensor>,
            "Tensor must be of standard layout."
        );
        static_assert(
            sizeof(Tensor) == sizeof(Tensor::coefficients),
            "`coefficients`must be only non-static data member of Tensor"
        );
        static_assert(
            std::is_standard_layout_v<SubTensor>,
            "SubTensor must be of standard layout."
        );
        static_assert(
            sizeof(SubTensor) == sizeof(SubTensor::coefficients),
            "`coefficients`must be only non-static data member of SubTensor"
        );
        std::array<SubTensor, DIMENSIONS[0]>& subtensors =
            reinterpret_cast<std::array<SubTensor, DIMENSIONS[0]>&>(*this);
        if constexpr (sizeof...(Ts) == 0) {
            return subtensors.at(i_first);
        } else {
            return subtensors.at(i_first).at(i_rest...);
        }
    }

    [[nodiscard]] constexpr const auto&
    operator[](const decltype(DIMENSIONS[0]) i_first) const requires(RANK > 0)
    {
        using SubTensor =
            Tensor<Scalar, RANK - 1, cexpr::array::rest(DIMENSIONS)>;
        // note: Requires that coefficients are the only non-static data member.
        static_assert(
            std::is_standard_layout_v<Tensor>,
            "Tensor must be of standard layout."
        );
        static_assert(
            sizeof(Tensor) == sizeof(Tensor::coefficients),
            "`coefficients`must be only non-static data member of Tensor"
        );
        static_assert(
            std::is_standard_layout_v<SubTensor>,
            "SubTensor must be of standard layout."
        );
        static_assert(
            sizeof(SubTensor) == sizeof(SubTensor::coefficients),
            "`coefficients`must be only non-static data member of SubTensor"
        );
        const std::array<SubTensor, DIMENSIONS[0]>& subtensors =
            reinterpret_cast<const std::array<SubTensor, DIMENSIONS[0]>&>(*this
            );
        return subtensors[i_first];
    }

    [[nodiscard]] constexpr auto&
    operator[](const decltype(DIMENSIONS[0]) i_first) requires(RANK > 0)
    {
        using SubTensor =
            Tensor<Scalar, RANK - 1, cexpr::array::rest(DIMENSIONS)>;
        // note: Requires that coefficients are the only non-static data member.
        static_assert(
            std::is_standard_layout_v<Tensor>,
            "Tensor must be of standard layout."
        );
        static_assert(
            sizeof(Tensor) == sizeof(Tensor::coefficients),
            "`coefficients`must be only non-static data member of Tensor"
        );
        static_assert(
            std::is_standard_layout_v<SubTensor>,
            "SubTensor must be of standard layout."
        );
        static_assert(
            sizeof(SubTensor) == sizeof(SubTensor::coefficients),
            "`coefficients`must be only non-static data member of SubTensor"
        );
        std::array<SubTensor, DIMENSIONS[0]>& subtensors =
            reinterpret_cast<std::array<SubTensor, DIMENSIONS[0]>&>(*this);
        return subtensors[i_first];
    }

    // return by reference in case of large subtensors
    // might throw: std::out_of_range
    template <std::size_t index_rank, Dimensions<index_rank> index_dimensions>
    [[nodiscard]] constexpr const auto&
    at(const MultiIndex<index_rank, index_dimensions>& mi) const requires(
        (RANK > 0) && (index_rank > 0) && (index_rank <= RANK) &&
        cexpr::array::is_prefix_of(index_dimensions, DIMENSIONS)
    )
    {
        if constexpr (index_rank == 1) {
            // return at(mi.first()); // using operator[] is safe here
            return (*this)[mi.first()];
        } else {
            // return at(mi.first()).at(mi.rest()); // using operator[] is safe
            // here
            return (*this)[mi.first()].at(mi.rest()
            ); // using operator[] is safe here
        }
    }

    // specialization for scalar
    [[nodiscard]] constexpr auto
    at(const MultiIndex<0, Dimensions<0>{}>&) const requires(RANK == 0)
    {
        return coefficients[0];
    }

    // might throw: std::out_of_range
    template <std::size_t index_rank, Dimensions<index_rank> index_dimensions>
    [[nodiscard]] constexpr auto&
    at(const MultiIndex<index_rank, index_dimensions>& mi) requires(
        (RANK > 0) && (index_rank > 0) && (index_rank <= RANK) &&
        cexpr::array::is_prefix_of(index_dimensions, DIMENSIONS)
    )
    {
        if constexpr (index_rank == 1) {
            // return at(mi.first()); // using operator[] is safe here
            return (*this)[mi.first()];
        } else {
            // return at(mi.first()).at(mi.rest()); // using operator[] is safe
            // here
            return (*this)[mi.first()].at(mi.rest()
            ); // using operator[] is safe here
        }
    }

    // specialization for scalar
    [[nodiscard]] constexpr auto&
    at(const MultiIndex<0, Dimensions<0>{}>&) requires(RANK == 0)
    {
        return coefficients[0];
    }
};

static_assert(
    std::is_standard_layout_v<Tensor<float, 0, Dimensions<0>{}>>,
    "Tensor must be of standard layout."
);
static_assert(
    std::is_aggregate_v<Tensor<float, 0, Dimensions<0>{}>>,
    "Tensor must be an aggregate type."
);

static_assert(
    std::is_standard_layout_v<Tensor<float, 1, Dimensions<1>{Dimension{3}}>>,
    "Tensor must be of standard layout."
);
static_assert(
    std::is_aggregate_v<Tensor<float, 1, Dimensions<1>{Dimension{3}}>>,
    "Tensor must be an aggregate type."
);

static_assert(
    std::is_standard_layout_v<
        Tensor<float, 2, Dimensions<2>{Dimension{3}, Dimension{3}}>>,
    "Tensor must be of standard layout."
);
static_assert(
    std::is_aggregate_v<
        Tensor<float, 2, Dimensions<2>{Dimension{3}, Dimension{3}}>>,
    "Tensor must be an aggregate type."
);

} // namespace gttl

#endif