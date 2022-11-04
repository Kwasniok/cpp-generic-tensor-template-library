#define BOOST_TEST_MODULE gttl::field_traits

#include <boost/mpl/list.hpp>
#include <boost/test/included/unit_test.hpp>
#include <gttl/field_traits.hpp>

namespace bmpl = boost::mpl;

using scalar_types = bmpl::list<float, double, long double>;

BOOST_AUTO_TEST_CASE_TEMPLATE(test_add, Scalar, scalar_types)
{
    using Traits = gttl::field_traits<Scalar>;
    auto op = typename Traits::add{};
    auto x = Scalar{4};
    auto y = Scalar{5};

    BOOST_TEST(op(x, y) == 9);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_sub, Scalar, scalar_types)
{
    using Traits = gttl::field_traits<Scalar>;
    auto op = typename Traits::sub{};
    auto x = Scalar{4};
    auto y = Scalar{5};

    BOOST_TEST(op(x, y) == -1);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_mul, Scalar, scalar_types)
{
    using Traits = gttl::field_traits<Scalar>;
    auto op = typename Traits::mul{};
    auto x = Scalar{4};
    auto y = Scalar{5};

    BOOST_TEST(op(x, y) == 20);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_div, Scalar, scalar_types)
{
    using Traits = gttl::field_traits<Scalar>;
    auto op = typename Traits::div{};
    auto x = Scalar{4};
    auto y = Scalar{5};

    BOOST_TEST(op(x, y) == Scalar{4} / Scalar{5});
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_neg, Scalar, scalar_types)
{
    using Traits = gttl::field_traits<Scalar>;
    auto op = typename Traits::neg{};
    auto x = Scalar{4};

    BOOST_TEST(op(x) == -4);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_zero, Scalar, scalar_types)
{
    using Traits = gttl::field_traits<Scalar>;
    BOOST_TEST(Traits::zero == 0);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_one, Scalar, scalar_types)
{

    using Traits = gttl::field_traits<Scalar>;
    BOOST_TEST(Traits::one == 1);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_abs, Scalar, scalar_types)
{
    using Traits = gttl::field_traits<Scalar>;
    auto op = typename Traits::abs{};

    BOOST_TEST(op(Scalar{+0}) == 0);
    BOOST_TEST(op(Scalar{-0}) == 0);
    BOOST_TEST(op(Scalar{+4}) == 4);
    BOOST_TEST(op(Scalar{-4}) == 4);
}
