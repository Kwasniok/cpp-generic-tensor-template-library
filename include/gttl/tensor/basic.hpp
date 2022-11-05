
/**
 * @file
 * @brief tensor (basic operations)
 */

#ifndef GTTL_TENSOR_BASIC_HPP
#define GTTL_TENSOR_BASIC_HPP

#include <concepts>
#include <iostream>
#include <limits>
#include <ranges>

#include "../dimensions.hpp"
#include "../field_traits.hpp"
#include "../multi_index.hpp"
#include "../multi_index_range.hpp"

namespace gttl
{

/**
 * Tensor in standard representation as array of scalar values.
 * @note Is a standard layout struct.
 * @note Is an aggregate type.
 */
template <
    typename Scalar,
    std::size_t RANK,
    Dimensions<RANK> DIMENSIONS,
    typename Traits = field_traits<Scalar>>

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

    /*
     * @brief returns multi-index associated with the index-th coefficient
     * @note It is asserted but NOT checked that 0 <= index < size.
     */
    constexpr static multi_index_type
    get_multi_index_for_index(std::size_t index)
    {
        // MAINTENANCE: consider making this function private/protected
        if constexpr (RANK == 0) {
            return {};
        } else {
            using SubTensor =
                Tensor<Scalar, RANK - 1, cexpr::array::rest(DIMENSIONS)>;
            static_assert(
                SubTensor::size < std::numeric_limits<Dimension>::max(),
                "Type Dimension must be large enough to index all top level "
                "elements."
            );
            const Dimension i_first =
                static_cast<Dimension>(index / SubTensor::size);
            const std::size_t i_rest{index % SubTensor::size};
            // note: Implicit modulo operation for i_first in constructor.
            return {i_first, SubTensor::get_multi_index_for_index(i_rest)};
        }
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
        // reinterpret cast is checked above
        // NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
        const std::array<SubTensor, DIMENSIONS[0]>& subtensors =
            reinterpret_cast<const std::array<SubTensor, DIMENSIONS[0]>&>(*this
            );
        // NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)
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
        // reinterpret cast is checked above
        // NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
        std::array<SubTensor, DIMENSIONS[0]>& subtensors =
            reinterpret_cast<std::array<SubTensor, DIMENSIONS[0]>&>(*this);
        // NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)
        if constexpr (sizeof...(Ts) == 0) {
            return subtensors.at(i_first);
        } else {
            return subtensors.at(i_first).at(i_rest...);
        }
    }

    [[nodiscard]] constexpr const auto&
    operator[](const std::size_t i_first) const requires(RANK > 0)
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
        // reinterpret cast is checked above
        // NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
        const std::array<SubTensor, DIMENSIONS[0]>& subtensors =
            reinterpret_cast<const std::array<SubTensor, DIMENSIONS[0]>&>(*this
            );
        // NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)
        return subtensors[i_first];
    }

    [[nodiscard]] constexpr auto&
    operator[](const std::size_t i_first) requires(RANK > 0)
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
        // reinterpret cast is checked above
        // NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
        std::array<SubTensor, DIMENSIONS[0]>& subtensors =
            reinterpret_cast<std::array<SubTensor, DIMENSIONS[0]>&>(*this);
        // NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)
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
            return (*this)[mi.first()].at(mi.rest());
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
            return (*this)[mi.first()].at(mi.rest());
        }
    }

    // specialization for scalar
    [[nodiscard]] constexpr auto&
    at(const MultiIndex<0, Dimensions<0>{}>&) requires(RANK == 0)
    {
        return coefficients[0];
    }

    constexpr Tensor&
    inplace_elementwise(auto op)
    {
        for (auto& c : coefficients) {
            c = op(c);
        }
        return *this;
    }

    /*
     * @brief in-place elementwise application of (n+1)-ary operation on scalars
     * @note `this` is the implicit first argument.
     * @note `this` is the storage location.
     */
    template <
        // all operands must be tensors of same type
        std::same_as<Tensor>... Ts>

    constexpr Tensor&
    inplace_elementwise(auto op, const Ts&... xs) requires requires
    {
        // check if op is n-ary scalar operation
        // clang-format off
    { op(coefficients[0], xs.coefficients[0]...) } -> std::same_as<Scalar>;
        // clang-format on
    }

    {
        for (std::size_t i{0}; i < size; ++i) {
            coefficients[i] = op(coefficients[i], xs.coefficients[i]...);
        }
        return *this;
    }

    /*
     * @brief elementwise application of (n+1)-ary operation on scalars
     * @note `this` is the implicit first argument.
     * @note The return value is the storage location.
     */
    template <
        // all operands must be tensors of same type
        std::same_as<Tensor>... Ts>

    constexpr Tensor
    elementwise(auto op, const Ts&... xs) const requires requires
    {
        // check if op is n-ary scalar operation
        // clang-format off
    { op(coefficients[0], xs.coefficients[0]...) } -> std::same_as<Scalar>;
        // clang-format on
    }

    {
        Tensor res; // initialization is NOT required!
        for (std::size_t i{0}; i < size; ++i) {
            res.coefficients[i] = op(coefficients[i], xs.coefficients[i]...);
        }
        return res;
    }

    constexpr Tensor&
    operator+=(const Tensor& rhs)
    {
        this->inplace_elementwise(typename Traits::add{}, rhs);
        return *this;
    }

    constexpr Tensor&
    operator-=(const Tensor& rhs)
    {
        this->inplace_elementwise(typename Traits::sub{}, rhs);
        return *this;
    }

    // note: avoid *= due to confusion with other products
    constexpr Tensor&
    inplace_elem_mul(const Tensor& rhs)
    {
        this->inplace_elementwise(typename Traits::mul{}, rhs);
        return *this;
    }

    // note: avoid /= since it is not common mathematical operation
    constexpr Tensor&
    inplace_elem_div(const Tensor& rhs)
    {
        this->inplace_elementwise(typename Traits::div{}, rhs);
        return *this;
    }

    /**
     * @brief in-place scalar multiplication
     */
    constexpr Tensor&
    operator*=(const Scalar& rhs)
    {
        const auto f = [fac = rhs](const Scalar& x) { return x * fac; };
        this->inplace_elementwise(f);
        return *this;
    }

    constexpr Tensor
    operator+(const Tensor& rhs) const
    {
        return this->elementwise(typename Traits::add{}, rhs);
    }

    constexpr Tensor
    operator-(const Tensor& rhs) const
    {
        return this->elementwise(typename Traits::sub{}, rhs);
    }

    constexpr Tensor
    elem_mul(const Tensor& rhs) const
    {
        return this->elementwise(typename Traits::mul{}, rhs);
    }

    constexpr Tensor
    elem_div(const Tensor& rhs) const
    {
        return this->elementwise(typename Traits::div{}, rhs);
    }

    constexpr Tensor
    operator-() const
    {
        return this->elementwise(typename Traits::neg{});
    }

    /**
     * @brief scalar multiplication
     */
    constexpr Tensor
    operator*(const Scalar& rhs) const
    {
        const auto f = [fac = rhs](const Scalar& x) { return x * fac; };
        return this->elementwise(f);
    }
};

template <
    typename Scalar,
    std::size_t RANK,
    Dimensions<RANK> DIMENSIONS,
    typename Traits>
constexpr Tensor<Scalar, RANK, DIMENSIONS, Traits>
operator*(
    const Scalar& lhs, const Tensor<Scalar, RANK, DIMENSIONS, Traits>& rhs
)
{
    return rhs * lhs;
}

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

static_assert(std::ranges::contiguous_range<Tensor<float, 0, Dimensions<0>{}>>);
static_assert(std::ranges::contiguous_range<
              Tensor<float, 1, Dimensions<1>{Dimension{3}}>>);
static_assert(std::ranges::contiguous_range<
              Tensor<float, 2, Dimensions<2>{Dimension{3}, Dimension{3}}>>);

static_assert(std::ranges::viewable_range<Tensor<float, 0, Dimensions<0>{}>>);
static_assert(std::ranges::viewable_range<
              Tensor<float, 1, Dimensions<1>{Dimension{3}}>>);
static_assert(std::ranges::viewable_range<
              Tensor<float, 2, Dimensions<2>{Dimension{3}, Dimension{3}}>>);

template <
    typename Scalar,
    std::size_t RANK,
    Dimensions<RANK> DIMENSIONS,
    typename Traits>
std::ostream&
operator<<(
    std::ostream& os, const Tensor<Scalar, RANK, DIMENSIONS, Traits>& x
) requires(RANK > 0)
{
    os << '{';
    os << x[0];
    for (std::size_t i{1}; i < DIMENSIONS[0]; ++i) {
        os << ',' << x[i];
    }
    os << '}';
    return os;
}

template <typename Scalar, typename Traits>
std::ostream&
operator<<(
    std::ostream& os, const Tensor<Scalar, 0, Dimensions<0>{}, Traits>& x
)
{
    os << x.coefficients[0];
    return os;
}

} // namespace gttl

#endif
