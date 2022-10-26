#define BOOST_TEST_MODULE gttl::Tensor

#include "../../boost_util/array.hpp"
#include <boost/mpl/list.hpp>
#include <boost/test/included/unit_test.hpp>
#include <gttl/tensor/advanced.hpp>

namespace bmpl = boost::mpl;

using gttl::literals::operator"" _D;
using scalar_types = bmpl::list<float, double, long double>;

BOOST_AUTO_TEST_CASE_TEMPLATE(contraction_matrix, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<2> dims{3_D, 3_D};
    constexpr gttl::Dimensions<0> dims_res{};
    using T = gttl::Tensor<Scalar, 2, dims>;
    using TRes = gttl::Tensor<Scalar, 0, dims_res>;
    // clang-format off
    const T tensor{
        1, 2, 3,
        4, 5, 6,
        7, 8, 9,
    };
    // clang-format on
    std::array<Scalar, 1> values{1 + 5 + 9};

    TRes res = contraction<0, 1>(tensor);

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(res), std::end(res), std::begin(values), std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(contraction_ten3, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<3> dims{3_D, 4_D, 3_D};
    constexpr gttl::Dimensions<1> dims_res{4_D};
    using T = gttl::Tensor<Scalar, 3, dims>;
    using TRes = gttl::Tensor<Scalar, 1, dims_res>;
    // fill tensor with natural numbers 1,...
    T tensor;
    std::iota(std::begin(tensor), std::end(tensor), 1);

    std::array<Scalar, 4> values{
        +1 + 14 + 27, // = 42
        +4 + 17 + 30, // = 51
        +7 + 20 + 33, // = 60
        10 + 23 + 36, // = 69
    };

    TRes res;

    res = contraction<0, 2>(tensor);
    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(res), std::end(res), std::begin(values), std::end(values)
    );

    // reverse order of indicies
    res = contraction<2, 0>(tensor);
    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(res), std::end(res), std::begin(values), std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(contraction_ten5, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<5> dims{3_D, 2_D, 3_D, 1_D, 2_D};
    constexpr gttl::Dimensions<3> dims_res{3_D, 3_D, 1_D};
    using T = gttl::Tensor<Scalar, 5, dims>;
    using TRes = gttl::Tensor<Scalar, 3, dims_res>;
    // fill tensor with natural numbers 1,...
    T tensor;
    std::iota(std::begin(tensor), std::end(tensor), 1);

    std::array<Scalar, 9> values{9, 13, 17, 33, 37, 41, 57, 61, 65};

    TRes res;

    res = contraction<1, 4>(tensor);
    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(res), std::end(res), std::begin(values), std::end(values)
    );

    // reverse order of indicies
    res = contraction<4, 1>(tensor);
    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(res), std::end(res), std::begin(values), std::end(values)
    );
}
