#define BOOST_TEST_MODULE gttl::concepts

#include <boost/mpl/list.hpp>
#include <boost/test/included/unit_test.hpp>
#include <gttl/concepts.hpp>

namespace bmpl = boost::mpl;

BOOST_AUTO_TEST_CASE(test_positives)
{
    static_assert(gttl::internal::
                      convertible_to_but_not_same_as<int, float, double>);
    static_assert(gttl::internal::
                      convertible_to_but_not_same_as<float, float, double>);
}

BOOST_AUTO_TEST_CASE(test_negatives_due_to_beeing_same_as)
{
    static_assert(!gttl::internal::
                      convertible_to_but_not_same_as<int, float, int>);
    static_assert(!gttl::internal::
                      convertible_to_but_not_same_as<float, float, float>);
}

BOOST_AUTO_TEST_CASE(test_negatives_due_to_beeing_not_convertible_to)
{
    static_assert(!gttl::internal::
                      convertible_to_but_not_same_as<float, void, int>);
}
