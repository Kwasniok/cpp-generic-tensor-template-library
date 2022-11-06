/**
 * @file
 * @brief tensor extension for boost::numeric::odeint
 */

#ifndef GTTL_EXTENSIONS_BOOST_NUMERIC_ODEINT_HPP
#define GTTL_EXTENSIONS_BOOST_NUMERIC_ODEINT_HPP

#include <algorithm>
#include <ranges>

#include <boost/numeric/odeint/algebra/algebra_dispatcher.hpp>
#include <boost/numeric/odeint/algebra/norm_result_type.hpp>

#include "../../../../tensor.hpp"

namespace boost
{
namespace numeric
{
namespace odeint
{
/**
 * select vector space algebra for gttl::Tensor by default
 */
template <
    typename Scalar,
    std::size_t RANK,
    gttl::Dimensions<RANK> DIMENSIONS,
    typename Traits>
struct algebra_dispatcher<gttl::Tensor<Scalar, RANK, DIMENSIONS, Traits>> {
    using algebra_type = vector_space_algebra;
};

/** @brief inf norm for gttl::Tensor */
template <
    typename Scalar,
    std::size_t RANK,
    gttl::Dimensions<RANK> DIMENSIONS,
    typename Traits>

struct vector_space_norm_inf<gttl::Tensor<Scalar, RANK, DIMENSIONS, Traits>> {

    using result_type = decltype(typename Traits::abs{}(Scalar{}));

    result_type
    operator()(const gttl::Tensor<Scalar, RANK, DIMENSIONS, Traits>& ten) const
    {
        // MAINTENANCE: modernize as:
        // return max_element(transform(ten, typename Traits::abs{}));

        using Tensor = gttl::Tensor<Scalar, RANK, DIMENSIONS, Traits>;
        const auto abs = typename Traits::abs{};
        result_type res{0};
        for (std::size_t i{0}; i < Tensor::size; ++i) {
            result_type elem{abs(ten[i])};
            if (elem > res) {
                res = elem;
            }
        }
        return res;
    }
};

} // namespace odeint
} // namespace numeric
} // namespace boost

namespace gttl
{

/**
 * @brief elementwise scalar addition for gttl::Tensor
 * @note This is an undocumented requirement of boost::numeric::odeint.
 */
template <
    typename Scalar,
    std::size_t RANK,
    gttl::Dimensions<RANK> DIMENSIONS,
    typename Traits>

constexpr gttl::Tensor<Scalar, RANK, DIMENSIONS, Traits>
operator+(
    const gttl::Tensor<Scalar, RANK, DIMENSIONS, Traits>& lhs, const Scalar& rhs
)
{
    const auto func = [y = rhs, add = typename Traits::add{}](const Scalar& x) {
        return add(x, y);
    };
    return lhs.elementwise(func);
}

/**
 * @brief elementwise scalar addition for gttl::Tensor
 * @note This is an undocumented requirement of boost::numeric::odeint.
 */
template <
    typename Scalar,
    std::size_t RANK,
    gttl::Dimensions<RANK> DIMENSIONS,
    typename Traits>

constexpr gttl::Tensor<Scalar, RANK, DIMENSIONS, Traits>
operator+(
    const Scalar& lhs, const gttl::Tensor<Scalar, RANK, DIMENSIONS, Traits>& rhs
)
{
    return rhs + lhs;
}

/** @brief elementwise division for gttl::Tensor */
template <
    typename Scalar,
    std::size_t RANK,
    gttl::Dimensions<RANK> DIMENSIONS,
    typename Traits>

constexpr gttl::Tensor<Scalar, RANK, DIMENSIONS, Traits>
operator/(
    const gttl::Tensor<Scalar, RANK, DIMENSIONS, Traits>& lhs,
    const gttl::Tensor<Scalar, RANK, DIMENSIONS, Traits>& rhs
)
{
    return lhs.elementwise(typename Traits::div{}, rhs);
}

/** @brief elementwise absolute value for gttl::Tensor */
template <
    typename Scalar,
    std::size_t RANK,
    gttl::Dimensions<RANK> DIMENSIONS,
    typename Traits>

constexpr gttl::Tensor<Scalar, RANK, DIMENSIONS, Traits>
abs(const gttl::Tensor<Scalar, RANK, DIMENSIONS, Traits>& x)
{
    return x.elementwise(typename Traits::abs{});
}

} // namespace gttl

#endif
