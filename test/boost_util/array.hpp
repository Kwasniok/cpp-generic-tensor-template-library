
#ifndef GTTL_TEST_BOOST_UTIL_ARRAY_HPP
#define GTTL_TEST_BOOST_UTIL_ARRAY_HPP

#include <array>
#include <iostream>

namespace std /* required by boost */
{
template <typename T, std::size_t N>
std::ostream&
boost_test_print_type(std::ostream& os, const std::array<T, N>& arr)
{
    os << '{';
    for (const auto& val : arr) {
        os << val << ',';
    }
    os << '}';
    return os;
}
} // namespace std

#endif