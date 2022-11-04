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

    auto
    operator()(const gttl::Tensor<Scalar, RANK, DIMENSIONS, Traits>& ten) const
    {
        using std::ranges::max_element, std::ranges::views::transform;
        return max_element(transform(ten, Traits::abs));
    }
};

} // namespace odeint
} // namespace numeric
} // namespace boost

#endif
