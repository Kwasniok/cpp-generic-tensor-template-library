
#ifndef GTTL_TEST_BOOST_UTIL_PAIR_HPP
#define GTTL_TEST_BOOST_UTIL_PAIR_HPP

#include <iostream>
#include <utility>

namespace std /* required by boost */
{
template <typename T, typename U>
std::ostream&
boost_test_print_type(std::ostream& os, const std::pair<T, U>& pr)
{
    os << '{';
    os << pr.first;
    os << ',';
    os << pr.second;
    os << '}';
    return os;
}
} // namespace std

#endif