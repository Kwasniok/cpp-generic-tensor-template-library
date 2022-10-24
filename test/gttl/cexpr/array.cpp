#define BOOST_TEST_MODULE gttl::cexpr::array

#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/included/unit_test.hpp>
#include <gttl/cexpr/array.hpp>

namespace bdata = boost::unit_test::data;
namespace gca = gttl::cexpr::array;

template <std::size_t N>
using size_array = std::array<std::size_t, N>;

const std::array<int, 0> empty{};
const size_array<0> size_empty{};

BOOST_DATA_TEST_CASE(contains_empty, bdata::xrange(3), x)
{
    BOOST_TEST(!gca::contains(x, empty));
}

BOOST_DATA_TEST_CASE(contains_non_empty, bdata::make({7, 253, 2}), x)
{
    const std::array arr{7, 253, 2};
    BOOST_TEST(gca::contains(x, arr));
}

BOOST_DATA_TEST_CASE(contains_not_non_empty, bdata::make({646, -53, 0}), x)
{
    const std::array arr{7, 253, 2};
    BOOST_TEST(!gca::contains(x, arr));
}

BOOST_AUTO_TEST_CASE(all_strictly_positive_empty)
{
    BOOST_TEST(gca::all_strictly_positive(empty));
}

BOOST_AUTO_TEST_CASE(all_strictly_positive_non_empty)
{
    const std::array arr{7, 253, 2};
    BOOST_TEST(gca::all_strictly_positive(arr));
}

BOOST_AUTO_TEST_CASE(all_strictly_positive_not_non_empty)
{
    const std::array arr{7, -253, 2};
    BOOST_TEST(!gca::all_strictly_positive(arr));
}

BOOST_DATA_TEST_CASE(all_less_than_empty, bdata::make({-123, 0, 54}), bound)
{
    BOOST_TEST(gca::all_less_than(bound, empty));
}

BOOST_DATA_TEST_CASE(all_less_than_non_empty, bdata::make({254, 999}), bound)
{
    const std::array arr{7, 253, 2};
    BOOST_TEST(gca::all_less_than(bound, arr));
}

BOOST_DATA_TEST_CASE(all_less_than_not_non_empty, bdata::make({253, 0}), bound)
{
    const std::array arr{7, 253, 2};
    BOOST_TEST(!gca::all_less_than(bound, arr));
}

BOOST_AUTO_TEST_CASE(all_unique_empty)
{
    BOOST_TEST(gca::all_unique(empty));
}

BOOST_AUTO_TEST_CASE(all_unique_non_empty)
{
    const std::array arr{-7, 1, 8};
    BOOST_TEST(gca::all_unique(arr));
}

BOOST_AUTO_TEST_CASE(all_unique_not_non_empty)
{
    const std::array arr{0, 1, 2, 3, 1, 5};
    BOOST_TEST(!gca::all_unique(arr));
}

BOOST_AUTO_TEST_CASE(sum_empty)
{
    BOOST_TEST(gca::sum(empty) == 0);
}

BOOST_AUTO_TEST_CASE(sum_non_empty)
{
    const std::array arr{1, -20, 500};
    BOOST_TEST(gca::sum(arr) == 481);
}

BOOST_AUTO_TEST_CASE(prod_empty)
{
    BOOST_TEST(gca::prod(empty) == 1);
}

BOOST_AUTO_TEST_CASE(prod_non_empty)
{
    const std::array arr{2, -3, 5};
    BOOST_TEST(gca::prod(arr) == -30);
}

BOOST_AUTO_TEST_CASE(is_prefix_of_empty_empty)
{
    BOOST_TEST(gca::is_prefix_of(empty, empty));
}

BOOST_AUTO_TEST_CASE(is_prefix_of_empty_non_empty)
{
    const std::array arr{2, -3, 5};
    BOOST_TEST(gca::is_prefix_of(empty, arr));
}

BOOST_AUTO_TEST_CASE(is_prefix_of_non_empty_non_empty1)
{
    const std::array sub{2};
    const std::array arr{2, -3, 5};
    BOOST_TEST(gca::is_prefix_of(sub, arr));
}

BOOST_AUTO_TEST_CASE(is_prefix_of_non_empty_non_empty2)
{
    const std::array sub{2, -3};
    const std::array arr{2, -3, 5};
    BOOST_TEST(gca::is_prefix_of(sub, arr));
}

BOOST_AUTO_TEST_CASE(is_prefix_of_same)
{
    const std::array arr{2, -3, 5};
    BOOST_TEST(gca::is_prefix_of(arr, arr));
}

BOOST_AUTO_TEST_CASE(is_prefix_of_not)
{
    const std::array sub{2, -3, 5, 2};
    const std::array arr{2, -3, 5};
    BOOST_TEST(!gca::is_prefix_of(sub, arr));
}

BOOST_AUTO_TEST_CASE(concatenate_empty_empty)
{
    BOOST_TEST(gca::concatenate(empty, empty) == empty);
}

BOOST_AUTO_TEST_CASE(concatenate_empty_non_empty)
{
    const std::array arr{2, -3, 5};
    BOOST_TEST(gca::concatenate(empty, arr) == arr);
    BOOST_TEST(gca::concatenate(arr, empty) == arr);
}

BOOST_AUTO_TEST_CASE(concatenate_non_empty_non_empty)
{
    const std::array arr1{2, -3};
    const std::array arr2{5, 7};
    const std::array arr{2, -3, 5, 7};
    BOOST_TEST(gca::concatenate(arr1, arr2) == arr);
}

BOOST_AUTO_TEST_CASE(concatenate_value_empty)
{
    const std::array arr{2};
    BOOST_TEST(gca::concatenate(2, empty) == arr);
    BOOST_TEST(gca::concatenate(empty, 2) == arr);
}

BOOST_AUTO_TEST_CASE(concatenate_value_non_empty)
{
    const std::array arr1{2, -3, 5};
    const std::array arr2{7, 2, -3, 5};
    const std::array arr3{2, -3, 5, 7};
    BOOST_TEST(gca::concatenate(7, arr1) == arr2);
    BOOST_TEST(gca::concatenate(arr1, 7) == arr3);
}

BOOST_AUTO_TEST_CASE(first)
{
    const std::array arr{2, -3, 5};
    BOOST_TEST(gca::first(arr) == 2);
}

BOOST_AUTO_TEST_CASE(rest)
{
    const std::array arr1{2, -3, 5};
    const std::array arr2{-3, 5};
    BOOST_TEST(gca::rest(arr1) == arr2);
}

BOOST_AUTO_TEST_CASE(last)
{
    const std::array arr{2, -3, 5};
    BOOST_TEST(gca::last(arr) == 5);
}

BOOST_AUTO_TEST_CASE(most)
{
    const std::array arr1{2, -3, 5};
    const std::array arr2{2, -3};
    BOOST_TEST(gca::most(arr1) == arr2);
}

BOOST_AUTO_TEST_CASE(take)
{
    const std::array arr1{2};
    const std::array arr2{2, -3};
    const std::array arr3{2, -3, 5};
    BOOST_TEST(gca::take<0>(arr3) == empty);
    BOOST_TEST(gca::take<1>(arr3) == arr1);
    BOOST_TEST(gca::take<2>(arr3) == arr2);
    BOOST_TEST(gca::take<3>(arr3) == arr3);
}

BOOST_AUTO_TEST_CASE(drop)
{
    const std::array arr1{5};
    const std::array arr2{-3, 5};
    const std::array arr3{2, -3, 5};
    BOOST_TEST(gca::drop<0>(arr3) == arr3);
    BOOST_TEST(gca::drop<1>(arr3) == arr2);
    BOOST_TEST(gca::drop<2>(arr3) == arr1);
    BOOST_TEST(gca::drop<3>(arr3) == empty);
}

BOOST_AUTO_TEST_CASE(insert_at_empty)
{
    const std::array arr{7};
    BOOST_TEST(gca::insert_at<0>(7, empty) == arr);
}

BOOST_AUTO_TEST_CASE(insert_at_non_empty)
{
    const std::array arr{2, -3, 5};
    const std::array arr0{7, 2, -3, 5};
    const std::array arr1{2, 7, -3, 5};
    const std::array arr2{2, -3, 7, 5};
    const std::array arr3{2, -3, 5, 7};
    BOOST_TEST(gca::insert_at<0>(7, arr) == arr0);
    BOOST_TEST(gca::insert_at<1>(7, arr) == arr1);
    BOOST_TEST(gca::insert_at<2>(7, arr) == arr2);
    BOOST_TEST(gca::insert_at<3>(7, arr) == arr3);
}

BOOST_AUTO_TEST_CASE(multi_insert_at_empty_empty)
{
    BOOST_TEST((gca::multi_insert_at<0, size_empty>(empty, empty)) == empty);
}

BOOST_AUTO_TEST_CASE(multi_insert_singleton_empty)
{
    constexpr size_array<1> pos1{0};
    const std::array arr1{1};
    BOOST_TEST((gca::multi_insert_at<1, pos1>(arr1, empty)) == arr1);
}

BOOST_AUTO_TEST_CASE(multi_insert_non_empty_empty)
{
    /* check preservation of relative order */
    constexpr size_array<3> pos3{0, 0, 0};
    const std::array arr{-1, -2, -3};
    BOOST_TEST((gca::multi_insert_at<3, pos3>(arr, empty)) == arr);
}

BOOST_AUTO_TEST_CASE(multi_insert_at_singleton_non_empty)
{
    const std::array arr{10, 20, 30};

    constexpr size_array<1> pos10{0};
    constexpr size_array<1> pos11{1};
    constexpr size_array<1> pos12{2};
    constexpr size_array<1> pos13{3};
    const std::array arr1{-1};
    const std::array arr10{-1, 10, 20, 30};
    const std::array arr11{10, -1, 20, 30};
    const std::array arr12{10, 20, -1, 30};
    const std::array arr13{10, 20, 30, -1};
    BOOST_TEST((gca::multi_insert_at<1, pos10>(arr1, arr)) == arr10);
    BOOST_TEST((gca::multi_insert_at<1, pos11>(arr1, arr)) == arr11);
    BOOST_TEST((gca::multi_insert_at<1, pos12>(arr1, arr)) == arr12);
    BOOST_TEST((gca::multi_insert_at<1, pos13>(arr1, arr)) == arr13);
}

BOOST_AUTO_TEST_CASE(multi_insert_at_non_empty_non_empty)
{
    /* check preservation of relative order */
    const std::array arr{10, 20, 30};
    constexpr size_array<3> pos3{1, 2, 1};
    const std::array arr_in{-1, -2, -3};
    const std::array arr_out{
        10, // 0
        -1,
        -3,
        20, // 1
        -2,
        30, // 2
    };
    BOOST_TEST((gca::multi_insert_at<3, pos3>(arr_in, arr)) == arr_out);
}

BOOST_AUTO_TEST_CASE(erase_at)
{
    const std::array arr0{-7, 20, 30, 50};
    const std::array arr1{20, -7, 30, 50};
    const std::array arr2{20, 30, -7, 50};
    const std::array arr3{20, 30, 50, -7};
    const std::array arr{20, 30, 50};
    BOOST_TEST(gca::erase_at<0>(arr0) == arr);
    BOOST_TEST(gca::erase_at<1>(arr1) == arr);
    BOOST_TEST(gca::erase_at<2>(arr2) == arr);
    BOOST_TEST(gca::erase_at<3>(arr3) == arr);
}

BOOST_AUTO_TEST_CASE(multi_erase_at_empty_empty)
{
    BOOST_TEST((gca::multi_erase_at<0, size_empty>(empty)) == empty);
}

BOOST_AUTO_TEST_CASE(multi_erase_at_singleton_non_empty)
{
    constexpr size_array<1> pos0{0};
    constexpr size_array<1> pos1{1};
    constexpr size_array<1> pos2{2};
    constexpr size_array<1> pos3{3};
    const std::array arr0{-1, 10, 20, 30};
    const std::array arr1{10, -1, 20, 30};
    const std::array arr2{10, 20, -1, 30};
    const std::array arr3{10, 20, 30, -1};
    const std::array arr{10, 20, 30};
    BOOST_TEST((gca::multi_erase_at<1, pos0>(arr0)) == arr);
    BOOST_TEST((gca::multi_erase_at<1, pos1>(arr1)) == arr);
    BOOST_TEST((gca::multi_erase_at<1, pos2>(arr2)) == arr);
    BOOST_TEST((gca::multi_erase_at<1, pos3>(arr3)) == arr);
}

BOOST_AUTO_TEST_CASE(multi_erase_at_non_empty_non_empty)
{
    const std::array arr{-0, 1, 2, -3, 4, 5, -6, -7, 8, 9};
    constexpr size_array<4> pos4{0, 7, 3, 6};
    const std::array arr_out{
        // -0,
        1,
        2,
        // -3,
        4,
        5,
        // -6,
        // -7,
        8,
        9,
    };
    BOOST_TEST((gca::multi_erase_at<4, pos4>(arr)) == arr_out);
}
