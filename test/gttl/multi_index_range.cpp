
#define BOOST_TEST_MODULE gttl::MultiIndexRange

#include "../boost_util/array.hpp"
#include <boost/test/included/unit_test.hpp>
#include <gttl/multi_index_range.hpp>

using gttl::literals::operator"" _D;

BOOST_AUTO_TEST_CASE(iterate_empty)
{
    constexpr gttl::Dimensions<0> dims0{};
    constexpr std::array<std::size_t, 0> empty{};

    // note: dimensions behave multiplicative!
    for (auto index : gttl::MultiIndexRange<0, dims0>{}) {
        BOOST_TEST(index == empty);
    }
}

BOOST_AUTO_TEST_CASE(iterate_non_empty)
{
    using A = std::array<gttl::Dimension, 3>;
    constexpr gttl::Dimensions<3> dims3{2_D, 1_D, 3_D};
    constexpr std::array values{
        A{0_D, 0_D, 0_D},
        A{0_D, 0_D, 1_D},
        A{0_D, 0_D, 2_D},
        A{1_D, 0_D, 0_D},
        A{1_D, 0_D, 1_D},
        A{1_D, 0_D, 2_D},
    };

    std::size_t i{0};
    for (auto index : gttl::MultiIndexRange<3, dims3>{}) {
        BOOST_TEST(index == values.at(i));
        ++i;
    }
    BOOST_TEST(i == 6);
}
