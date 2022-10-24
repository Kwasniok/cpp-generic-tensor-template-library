#define BOOST_TEST_MODULE gttl::Index

#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/included/unit_test.hpp>
#include <gttl/index.hpp>

namespace bdata = boost::unit_test::data;

using gttl::literals::operator"" _D;

BOOST_AUTO_TEST_CASE(default_constructor)
{
    gttl::Index<3_D> index{};
    BOOST_TEST(index == 0);
}

BOOST_DATA_TEST_CASE(constructor_one_dim, bdata::xrange(4_D), d)
{
    gttl::Index<1_D> index{d};
    BOOST_TEST(index == 0);
}

BOOST_DATA_TEST_CASE(
    constructor, bdata::xrange(4_D) ^ bdata::make({0, 1, 2, 0}), d, val
)
{
    gttl::Index<3_D> index{d};
    BOOST_TEST(index == val);
}

BOOST_DATA_TEST_CASE(
    assignment, bdata::xrange(4_D) ^ bdata::make({0, 1, 2, 0}), d, val
)
{
    gttl::Index<3_D> index{};
    index = d;
    BOOST_TEST(index == val);
}

BOOST_DATA_TEST_CASE(conversion_one_dim, bdata::xrange(4_D), d)
{
    gttl::Index<1_D> index{d};
    auto value = static_cast<gttl::Dimension>(index);
    BOOST_TEST(value == 0);
}

BOOST_DATA_TEST_CASE(
    conversion, bdata::xrange(4_D) ^ bdata::make({0, 1, 2, 0}), d, val
)
{
    gttl::Index<3_D> index{d};
    auto value = static_cast<gttl::Dimension>(index);
    BOOST_TEST(value == val);
}

BOOST_AUTO_TEST_CASE(prefix_increment_one_dim)
{
    gttl::Index<1_D> index{};
    BOOST_TEST(++index == 0);
    BOOST_TEST(++index == 0);
    BOOST_TEST(++index == 0);
}

BOOST_AUTO_TEST_CASE(prefix_increment)
{
    gttl::Index<3_D> index{};
    BOOST_TEST(++index == 1);
    BOOST_TEST(++index == 2);
    BOOST_TEST(++index == 0);
    BOOST_TEST(++index == 1);
    BOOST_TEST(++index == 2);
    BOOST_TEST(++index == 0);
}

BOOST_AUTO_TEST_CASE(prefix_decrement_one_dim)
{
    gttl::Index<1_D> index{};
    BOOST_TEST(--index == 0);
    BOOST_TEST(--index == 0);
    BOOST_TEST(--index == 0);
}

BOOST_AUTO_TEST_CASE(prefix_decrement)
{
    gttl::Index<3_D> index{};
    BOOST_TEST(--index == 2);
    BOOST_TEST(--index == 1);
    BOOST_TEST(--index == 0);
    BOOST_TEST(--index == 2);
    BOOST_TEST(--index == 1);
    BOOST_TEST(--index == 0);
}