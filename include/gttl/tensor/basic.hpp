
/**
 * @file
 * @brief tensor (basic operations)
 */

#ifndef GTTL_TENSOR_BASIC_HPP
#define GTTL_TENSOR_BASIC_HPP

#include <algorithm>
#include <concepts>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <limits>
#include <ranges>

#include "../dimensions.hpp"
#include "../field_traits.hpp"
#include "../multi_index.hpp"
#include "../multi_index_range.hpp"

// NOLINTBEGIN(cppcoreguidelines-pro-type-union-access)
namespace gttl
{

/**
 * @brief Tensor in standard representation as an array of scalar values
 * (coefficients).
 * @note Is a standard layout struct.
 * @note Mimics an aggregate type.
 */
template <
    typename Scalar,
    std::size_t RANK,
    Dimensions<RANK> DIMENSIONS,
    typename Traits = field_traits<Scalar>>

requires(cexpr::array::all_strictly_positive(DIMENSIONS)) struct Tensor {

    using scalar_type = Scalar;
    constexpr static std::size_t rank{RANK};
    constexpr static Dimensions<RANK> dimensions{DIMENSIONS};
    using traits_type = Traits;

    using multi_index_type = MultiIndex<RANK, DIMENSIONS>;
    using multi_index_range_type = MultiIndexRange<RANK, DIMENSIONS>;

    constexpr static Dimension size{cexpr::array::prod(DIMENSIONS)};

    using subtensor_type =
        Tensor<Scalar, RANK - 1, cexpr::array::drop<1>(DIMENSIONS)>;

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
    using coefficient_array_type = std::array<Scalar, size>;

    using subtensor_array_type = std::array<subtensor_type, DIMENSIONS[0]>;

    static_assert(
        std::is_standard_layout_v<subtensor_type>,
        "`subtensor_type` must be of standard layout."
    );
    static_assert(
        sizeof(subtensor_type) == sizeof(Scalar) * subtensor_type::size,
        "No padding is allowed in `subtensor_type`."
    );
    static_assert(
        sizeof(coefficient_array_type) == sizeof(subtensor_array_type),
        "`coefficient_array_type` and `subtensor_type` must have matching "
        "memory layout."
    );

    union {
        coefficient_array_type coefficients;
        subtensor_array_type subtensors;
    };

    /** @brief zero-initialize coefficients */
    constexpr Tensor() { std::ranges::fill(coefficients, Traits::zero); }

    /** @brief aggregate type-like initialization (from coefficients) */
    constexpr Tensor(std::initializer_list<Scalar> coefficients)
    {
        std::ranges::copy(coefficients, std::begin(this->coefficients));
        // fill rest with zeros
        auto it = std::begin(this->coefficients);
        std::advance(it, coefficients.size());
        std::fill(it, std::end(this->coefficients), Traits::zero);
    }

    /** @brief aggregate type-like initialization (from subtensors) */
    constexpr Tensor(std::initializer_list<subtensor_type> subtensors)
    {
        std::ranges::copy(subtensors, std::begin(this->subtensors));
        // fill rest with zeros
        auto it = std::begin(this->subtensors);
        std::advance(it, subtensors.size());
        for (auto end = std::end(this->subtensors); it != end; ++it) {
            *it = subtensor_type{};
        }
    }

    constexpr Tensor(const Tensor& rhs) : coefficients{rhs.coefficients} { }

    constexpr Tensor(Tensor&& rhs) : coefficients{std::move(rhs.coefficients)}
    {
    }

    constexpr Tensor&
    operator=(const Tensor& rhs)
    {
        coefficients = rhs.coefficients;
        return *this;
    }

    constexpr Tensor&
    operator=(Tensor&& rhs)
    {
        coefficients = std::move(rhs.coefficients);
        return *this;
    }

    constexpr ~Tensor() { coefficients.~coefficient_array_type(); }

    /** @brief trivially converts to a read-only array of coefficients */
    constexpr operator const coefficient_array_type&() const
    {
        return coefficients;
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
        const Dimension i_first =
            static_cast<Dimension>(index / subtensor_type::size);
        const std::size_t i_rest{index % subtensor_type::size};
        // note: Implicit modulo operation for i_first in constructor.
        return {i_first, subtensor_type::get_multi_index_for_index(i_rest)};
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
    at(const std::size_t i_first, const Ts... i_rest) const
    {
        if constexpr (sizeof...(Ts) == 0) {
            return subtensors.at(i_first);
        } else {
            return subtensors.at(i_first).at(i_rest...);
        }
    }

    // might throw: std::out_of_range
    template <typename... Ts>
    [[nodiscard]] constexpr auto&
    at(const std::size_t i_first, const Ts... i_rest)
    {
        if constexpr (sizeof...(Ts) == 0) {
            return subtensors.at(i_first);
        } else {
            return subtensors.at(i_first).at(i_rest...);
        }
    }

    [[nodiscard]] constexpr const auto&
    operator[](const std::size_t i_first) const
    {
        return subtensors[i_first];
    }

    [[nodiscard]] constexpr auto&
    operator[](const std::size_t i_first) requires(RANK > 0)
    {
        return subtensors[i_first];
    }

    // return by reference in case of large subtensors
    // might throw: std::out_of_range
    template <std::size_t index_rank, Dimensions<index_rank> index_dimensions>
    [[nodiscard]] constexpr const auto&
    at(const MultiIndex<index_rank, index_dimensions>& mi) const requires(
        (index_rank > 0) && (index_rank <= RANK) &&
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

    // might throw: std::out_of_range
    template <std::size_t index_rank, Dimensions<index_rank> index_dimensions>
    [[nodiscard]] constexpr auto&
    at(const MultiIndex<index_rank, index_dimensions>& mi) requires(
        (index_rank > 0) && (index_rank <= RANK) &&
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

// NOLINTEND(cppcoreguidelines-pro-type-union-access)

/**
 * @brief specialization for rank-zero tensor (a.k.a. scalar)
 */
template <typename Scalar, Dimensions<0> DIMENSIONS, typename Traits>

requires(cexpr::array::all_strictly_positive(DIMENSIONS)
) struct Tensor<Scalar, 0, DIMENSIONS, Traits> {

    using scalar_type = Scalar;
    constexpr static std::size_t rank{0};
    constexpr static Dimensions<0> dimensions{DIMENSIONS};
    using traits_type = Traits;

    using multi_index_type = MultiIndex<0, DIMENSIONS>;
    using multi_index_range_type = MultiIndexRange<0, DIMENSIONS>;

    constexpr static Dimension size{1};
    using coefficient_array_type = std::array<Scalar, size>;
    coefficient_array_type coefficients;

    /** @brief trivially converts to a read-only array of coefficients */
    constexpr operator const coefficient_array_type&() const
    {
        return coefficients;
    }

    // rank zero tensor is trivialy convertible to scalar
    constexpr operator Scalar() const { return coefficients[0]; }

    // rank zero tensor is trivialy convertible to scalar
    constexpr
    operator Scalar&()
    {
        return coefficients[0];
    }

    // rank zero tensor is trivialy constructable from scalar
    constexpr Tensor&
    operator=(const Scalar& value)
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
    get_multi_index_for_index(std::size_t)
    {
        return {};
    }

    constexpr void
    swap(Tensor& other) noexcept(std::is_nothrow_swappable_v<Scalar>)
    {
        coefficients.swap(other.coefficients);
    }

    // specialization for scalar
    [[nodiscard]] constexpr auto
    at(const MultiIndex<0, Dimensions<0>{}>&) const
    {
        return coefficients[0];
    }

    // specialization for scalar
    [[nodiscard]] constexpr auto&
    at(const MultiIndex<0, Dimensions<0>{}>&)
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
     * @brief in-place elementwise application of (n+1)-ary operation on
     scalars
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
        coefficients[0] = op(coefficients[0], xs.coefficients[0]...);
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
        res.coefficients[0] = op(coefficients[0], xs.coefficients[0]...);
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
    std::is_standard_layout_v<
        Tensor<float, 2, Dimensions<2>{Dimension{3}, Dimension{3}}>>,
    "Tensor must be of standard layout."
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
