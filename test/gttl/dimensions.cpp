#define BOOST_TEST_MODULE gttl::Dimensions

#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/included/unit_test.hpp>
#include <gttl/dimensions.hpp>

namespace bdata = boost::unit_test::data;

BOOST_DATA_TEST_CASE(
    constructor_single, bdata::xrange(static_cast<gttl::Dimension>(4)), n
)
{
    [[maybe_unused]] gttl::Dimension dim{n};
}

BOOST_AUTO_TEST_CASE(constructor_multi)
{
    [[maybe_unused]] gttl::Dimensions<3> dims{
        static_cast<gttl::Dimension>(1),
        static_cast<gttl::Dimension>(2),
        static_cast<gttl::Dimension>(3),
    };
}

BOOST_AUTO_TEST_CASE(literal_single)
{
    using gttl::literals::operator"" _D;
    [[maybe_unused]] auto dim{1_D};
}