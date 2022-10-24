#define BOOST_TEST_MODULE gttl::MultiIndex

#include "../boost_util/array.hpp"
#include <boost/test/included/unit_test.hpp>
#include <gttl/multi_index.hpp>

using gttl::literals::operator"" _D;

BOOST_AUTO_TEST_CASE(default_constructor)
{
    constexpr gttl::Dimensions<2> dims2{2_D, 3_D};

    gttl::MultiIndex<2, dims2> index{};
    BOOST_TEST(index.get<0>() == 0);
    BOOST_TEST(index.get<1>() == 0);
}

BOOST_AUTO_TEST_CASE(constructor_concatenate_index)
{

    constexpr gttl::Dimensions<2> dims2{3_D, 2_D};
    constexpr gttl::Dimensions<3> dims3{4_D, 3_D, 2_D};

    gttl::Index<4> first{3_D};
    gttl::MultiIndex<2, dims2> rest{2_D, 1_D};
    gttl::MultiIndex<3, dims3> index{first, rest};
    BOOST_TEST(index.get<0>() == 3);
    BOOST_TEST(index.get<1>() == 2);
    BOOST_TEST(index.get<2>() == 1);
}

BOOST_AUTO_TEST_CASE(constructor_concatenate_value)
{

    constexpr gttl::Dimensions<2> dims2{3_D, 2_D};
    constexpr gttl::Dimensions<3> dims3{4_D, 3_D, 2_D};

    gttl::MultiIndex<2, dims2> rest{2_D, 1_D};
    gttl::MultiIndex<3, dims3> index{3_D, rest};
    BOOST_TEST(index.get<0>() == 3);
    BOOST_TEST(index.get<1>() == 2);
    BOOST_TEST(index.get<2>() == 1);
}

BOOST_AUTO_TEST_CASE(constructor_variadic)
{
    constexpr gttl::Dimensions<2> dims2{2_D, 3_D};

    {
        gttl::MultiIndex<2, dims2> index{1_D, 2_D};
        BOOST_TEST(index.get<0>() == 1);
        BOOST_TEST(index.get<1>() == 2);
    }
    {
        /* rest dimension is default constructed */
        gttl::MultiIndex<2, dims2> index{1_D};
        BOOST_TEST(index.get<0>() == 1);
        BOOST_TEST(index.get<1>() == 0);
    }
}

BOOST_AUTO_TEST_CASE(conversion_from_array)
{
    constexpr gttl::Dimensions<3> dims3{4_D, 4_D, 4_D};
    const std::array values = {1_D, 2_D, 3_D};

    auto index = gttl::MultiIndex<3, dims3>{values};

    BOOST_TEST(index.get<0>() == 1);
    BOOST_TEST(index.get<1>() == 2);
    BOOST_TEST(index.get<2>() == 3);
}

BOOST_AUTO_TEST_CASE(conversion_to_array)
{
    constexpr gttl::Dimensions<3> dims3{3_D, 3_D, 1_D};
    const gttl::MultiIndex<3, dims3> index{
        1_D, // == 1
        2_D, // == 2
        3_D, // == 0
    };

    auto arr = std::array<gttl::Dimension, 3>(index);

    BOOST_TEST(arr[0] == 1);
    BOOST_TEST(arr[1] == 2);
    BOOST_TEST(arr[2] == 0);
}

BOOST_AUTO_TEST_CASE(assignment_array)
{
    constexpr gttl::Dimensions<3> dims3{4_D, 4_D, 4_D};
    const std::array values = {1_D, 2_D, 3_D};

    gttl::MultiIndex<3, dims3> index;
    index = values;

    BOOST_TEST(index.get<0>() == 1);
    BOOST_TEST(index.get<1>() == 2);
    BOOST_TEST(index.get<2>() == 3);
}

BOOST_AUTO_TEST_CASE(first_const)
{
    constexpr gttl::Dimensions<3> dims3{3_D, 4_D, 5_D};

    const gttl::MultiIndex<3, dims3> index{1_D, 2_D, 3_D};
    gttl::Index<3_D> i = index.first();
    BOOST_TEST(i == 1);
}

BOOST_AUTO_TEST_CASE(first_mut)
{
    constexpr gttl::Dimensions<3> dims3{3_D, 4_D, 5_D};

    gttl::MultiIndex<3, dims3> index{1_D, 2_D, 3_D};
    index.first() = 0_D;
    BOOST_TEST(index.first() == 0);
}

BOOST_AUTO_TEST_CASE(rest_const)
{
    constexpr gttl::Dimensions<3> dims3{3_D, 4_D, 5_D};
    constexpr gttl::Dimensions<2> dims2{4_D, 5_D};

    const gttl::MultiIndex<3, dims3> index{1_D, 2_D, 3_D};
    const gttl::MultiIndex<2, dims2> rest{2_D, 3_D};
    gttl::MultiIndex<2, dims2> is = index.rest();

    BOOST_TEST(is == rest);
}

BOOST_AUTO_TEST_CASE(rest_mut)
{
    constexpr gttl::Dimensions<3> dims3{5_D, 6_D, 7_D};
    constexpr gttl::Dimensions<2> dims2{6_D, 7_D};

    gttl::MultiIndex<3, dims3> index{1_D, 2_D, 3_D};
    gttl::MultiIndex<2, dims2> rest{4_D, 5_D};
    index.rest() = rest;

    BOOST_TEST(index.rest().get<0>() == 4);
    BOOST_TEST(index.rest().get<1>() == 5);
}

BOOST_AUTO_TEST_CASE(get_const)
{
    constexpr gttl::Dimensions<3> dims3{4_D, 4_D, 4_D};
    const gttl::MultiIndex<3, dims3> index{1_D, 2_D, 3_D};

    BOOST_TEST(index.get<0>() == 1);
    BOOST_TEST(index.get<1>() == 2);
    BOOST_TEST(index.get<2>() == 3);
}

BOOST_AUTO_TEST_CASE(get_mut)
{
    constexpr gttl::Dimensions<3> dims3{8_D, 8_D, 8_D};
    gttl::MultiIndex<3, dims3> index{1_D, 2_D, 3_D};

    index.get<0>() = 4_D;
    index.get<1>() = 5_D;
    index.get<2>() = 6_D;

    BOOST_TEST(index.get<0>() == 4);
    BOOST_TEST(index.get<1>() == 5);
    BOOST_TEST(index.get<2>() == 6);
}

BOOST_AUTO_TEST_CASE(equality_with_array)
{
    using A = gttl::Dimensions<3>;
    constexpr gttl::Dimensions<3> dims3{2_D, 1_D, 4_D};
    gttl::MultiIndex<3, dims3> index{
        1_D, // == 1
        2_D, // == 0
        3_D, // == 3
    };

    BOOST_TEST(index == A({1_D, 0_D, 3_D}));
    BOOST_TEST(!(index == A{1_D, 2_D, 3_D}));
}

BOOST_AUTO_TEST_CASE(inequality_with_array)
{
    using A = gttl::Dimensions<3>;
    constexpr gttl::Dimensions<3> dims3{2_D, 1_D, 4_D};
    gttl::MultiIndex<3, dims3> index{
        1_D, // == 1
        2_D, // == 0
        3_D, // == 3
    };

    BOOST_TEST(index != A({1_D, 2_D, 3_D}));
    BOOST_TEST(!(index != A{1_D, 0_D, 3_D}));
}

BOOST_AUTO_TEST_CASE(prefix_increment)
{
    using A = gttl::Dimensions<3>;
    constexpr gttl::Dimensions<3> dims3{2_D, 1_D, 3_D};
    gttl::MultiIndex<3, dims3> index{};

    // second = overflow
    decltype(++index) ret = ++index;
    BOOST_TEST(index == A({0_D, 0_D, 1_D}));
    BOOST_TEST(ret.second == false);
    ret = ++index;
    BOOST_TEST(index == A({0_D, 0_D, 2_D}));
    BOOST_TEST(ret.second == false);
    ret = ++index;
    BOOST_TEST(index == A({1_D, 0_D, 0_D}));
    BOOST_TEST(ret.second == false);
    ret = ++index;
    BOOST_TEST(index == A({1_D, 0_D, 1_D}));
    BOOST_TEST(ret.second == false);
    ret = ++index;
    BOOST_TEST(index == A({1_D, 0_D, 2_D}));
    BOOST_TEST(ret.second == false);
    ret = ++index;
    BOOST_TEST(index == A({0_D, 0_D, 0_D}));
    BOOST_TEST(ret.second == true);
}

BOOST_AUTO_TEST_CASE(prefix_decrement)
{
    using A = gttl::Dimensions<3>;
    constexpr gttl::Dimensions<3> dims3{2_D, 1_D, 3_D};
    gttl::MultiIndex<3, dims3> index{1_D, 0_D, 2_D};

    // second = underflow
    decltype(--index) ret = --index;
    BOOST_TEST(index == A({1_D, 0_D, 1_D}));
    BOOST_TEST(ret.second == false);
    ret = --index;
    BOOST_TEST(index == A({1_D, 0_D, 0_D}));
    BOOST_TEST(ret.second == false);
    ret = --index;
    BOOST_TEST(index == A({0_D, 0_D, 2_D}));
    BOOST_TEST(ret.second == false);
    ret = --index;
    BOOST_TEST(index == A({0_D, 0_D, 1_D}));
    BOOST_TEST(ret.second == false);
    ret = --index;
    BOOST_TEST(index == A({0_D, 0_D, 0_D}));
    BOOST_TEST(ret.second == false);
    ret = --index;
    BOOST_TEST(index == A({1_D, 0_D, 2_D}));
    BOOST_TEST(ret.second == true);
}

BOOST_AUTO_TEST_CASE(concatenate_empty_empty)
{
    constexpr gttl::Dimensions<0> dims0{};
    const gttl::MultiIndex<0, dims0> index0{};

    BOOST_TEST(index0.concatenate(index0) == index0);
}

BOOST_AUTO_TEST_CASE(concatenate_empty_non_empty)
{
    constexpr gttl::Dimensions<0> dims_l{};
    constexpr gttl::Dimensions<2> dims_r{5_D, 6_D};
    constexpr gttl::Dimensions<2> dims_res{5_D, 6_D};
    const gttl::MultiIndex<0, dims_l> index_l{};
    const gttl::MultiIndex<2, dims_r> index_r{4_D, 5_D};
    const gttl::MultiIndex<2, dims_res> index_res{4_D, 5_D};

    BOOST_TEST(index_l.concatenate(index_r) == index_res);
}

BOOST_AUTO_TEST_CASE(concatenate_non_empty_empty)
{
    constexpr gttl::Dimensions<3> dims_l{2_D, 3_D, 4_D};
    constexpr gttl::Dimensions<0> dims_r{};
    constexpr gttl::Dimensions<3> dims_res{2_D, 3_D, 4_D};
    const gttl::MultiIndex<3, dims_l> index_l{1_D, 2_D, 3_D};
    const gttl::MultiIndex<0, dims_r> index_r{};
    const gttl::MultiIndex<3, dims_res> index_res{1_D, 2_D, 3_D};

    BOOST_TEST(index_l.concatenate(index_r) == index_res);
}

BOOST_AUTO_TEST_CASE(concatenate_non_empty_non_empty)
{
    constexpr gttl::Dimensions<3> dims_l{2_D, 3_D, 4_D};
    constexpr gttl::Dimensions<2> dims_r{5_D, 6_D};
    constexpr gttl::Dimensions<5> dims_res{2_D, 3_D, 4_D, 5_D, 6_D};
    const gttl::MultiIndex<3, dims_l> index_l{1_D, 2_D, 3_D};
    const gttl::MultiIndex<2, dims_r> index_r{4_D, 5_D};
    const gttl::MultiIndex<5, dims_res> index_res{1_D, 2_D, 3_D, 4_D, 5_D};

    BOOST_TEST(index_l.concatenate(index_r) == index_res);
}

BOOST_AUTO_TEST_CASE(multi_insert_at_empty_empty)
{
    constexpr gttl::Dimensions<0> dims0{};
    const gttl::MultiIndex<0, dims0> index0{};
    constexpr std::array<std::size_t, 0> positions0{};

    const auto res = index0.template multi_insert_at<0, positions0>(index0);
    BOOST_TEST(res == index0);
}

BOOST_AUTO_TEST_CASE(multi_insert_at_empty_non_empty)
{
    constexpr gttl::Dimensions<0> dims_l{};
    constexpr gttl::Dimensions<3> dims_r{4_D, 5_D, 6_D};
    const gttl::MultiIndex<0, dims_l> index_l{};
    const gttl::MultiIndex<3, dims_r> index_r{1_D, 2_D, 3_D};
    constexpr std::array positions{0uz, 0uz, 0uz};

    const auto res = index_l.template multi_insert_at<3, positions>(index_r);
    BOOST_TEST(res == index_r);
}

BOOST_AUTO_TEST_CASE(multi_insert_at_non_empty_empty)
{
    constexpr gttl::Dimensions<3> dims_l{4_D, 5_D, 6_D};
    constexpr gttl::Dimensions<0> dims_r{};
    const gttl::MultiIndex<3, dims_l> index_l{1_D, 2_D, 3_D};
    const gttl::MultiIndex<0, dims_r> index_r{};
    constexpr std::array<std::size_t, 0> positions{};

    const auto res = index_l.template multi_insert_at<0, positions>(index_r);
    BOOST_TEST(res == index_l);
}

BOOST_AUTO_TEST_CASE(multi_insert_at_non_empty_non_empty)
{
    constexpr gttl::Dimensions<3> dims_l{4_D, 5_D, 6_D};
    constexpr gttl::Dimensions<3> dims_r{7_D, 8_D, 9_D};
    constexpr gttl::Dimensions<6> dims_res{
        4_D, // 0
        7_D,
        9_D,
        5_D, // 1
        6_D, // 2
        8_D,
    };
    const gttl::MultiIndex<3, dims_l> index_l{1_D, 2_D, 3_D};
    const gttl::MultiIndex<3, dims_r> index_r{4_D, 5_D, 6_D};
    const gttl::MultiIndex<6, dims_res> index_res{
        1_D, // 0
        4_D,
        6_D,
        2_D, // 1
        3_D, // 2
        5_D,
    };
    constexpr std::array positions{1uz, 3uz, 1uz};

    const auto res = index_l.template multi_insert_at<3, positions>(index_r);
    BOOST_TEST(res == index_res);
}

// TODO: multi_erase_at
BOOST_AUTO_TEST_CASE(multi_erase_at_empty_empty)
{
    constexpr gttl::Dimensions<0> dims0{};
    const gttl::MultiIndex<0, dims0> index0{};
    constexpr std::array<std::size_t, 0> positions0{};

    const auto res = index0.template multi_erase_at<0, positions0>();
    BOOST_TEST(res == index0);
}

BOOST_AUTO_TEST_CASE(multi_erase_at_non_empty_empty)
{
    constexpr gttl::Dimensions<3> dims_l{4_D, 5_D, 6_D};
    const gttl::MultiIndex<3, dims_l> index_l{1_D, 2_D, 3_D};
    constexpr std::array<std::size_t, 0> positions{};

    const auto res = index_l.template multi_erase_at<0, positions>();
    BOOST_TEST(res == index_l);
}

BOOST_AUTO_TEST_CASE(multi_erase_at_non_empty_non_empty)
{
    constexpr gttl::Dimensions<6> dims_l{4_D, 5_D, 6_D, 7_D, 8_D, 9_D};
    constexpr gttl::Dimensions<3> dims_res{5_D, 6_D, 8_D};
    const gttl::MultiIndex<6, dims_l> index_l{1_D, 2_D, 3_D, 4_D, 5_D, 6_D};
    const gttl::MultiIndex<3, dims_res> index_res{
        // 1_D, // 0
        2_D,
        3_D,
        // 4_D, // 3
        5_D,
        // 6_D, // 5
    };
    constexpr std::array positions{3uz, 0uz, 5uz};

    const auto res = index_l.template multi_erase_at<3, positions>();
    BOOST_TEST(res == index_res);
}
