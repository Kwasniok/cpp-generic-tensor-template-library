#define BOOST_TEST_MODULE gttl::Tensor

#include <cstdint>

#include "../../boost_util/array.hpp"
#include <boost/mpl/list.hpp>
#include <boost/test/included/unit_test.hpp>
#include <gttl/tensor/basic.hpp>

namespace bmpl = boost::mpl;

using gttl::literals::operator"" _D;
using scalar_types = bmpl::list<float, double, long double>;

constexpr auto square = [](const auto& x) constexpr
{
    return x * x;
};

constexpr auto add = [](const auto& x, const auto& y) constexpr
{
    return x + y;
};

// There will be a lot of 'magic' numbers here.
// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)

BOOST_AUTO_TEST_CASE_TEMPLATE(default_constructor_scalar, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<0> dims{};
    gttl::Tensor<Scalar, 0, dims> tensor{};
    std::array<Scalar, 1> values{0};

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(tensor),
        std::end(tensor),
        std::begin(values),
        std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(default_constructor_vector, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<1> dims{3_D};
    gttl::Tensor<Scalar, 1, dims> tensor{};
    std::array<Scalar, 3> values{0, 0, 0};

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(tensor),
        std::end(tensor),
        std::begin(values),
        std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(default_constructor_ten3, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<3> dims{4_D, 3_D, 2_D};
    gttl::Tensor<Scalar, 3, dims> tensor{};
    std::array<Scalar, 24> values{
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    };

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(tensor),
        std::end(tensor),
        std::begin(values),
        std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(value_constructor_scalar, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<0> dims{};
    gttl::Tensor<Scalar, 0, dims> tensor{1};
    std::array<Scalar, 1> values{1};

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(tensor),
        std::end(tensor),
        std::begin(values),
        std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(value_constructor_vector, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<1> dims{3_D};
    gttl::Tensor<Scalar, 1, dims> tensor{1, 2, 3};
    std::array<Scalar, 3> values{1, 2, 3};

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(tensor),
        std::end(tensor),
        std::begin(values),
        std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(value_constructor_ten3, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<3> dims{4_D, 3_D, 2_D};
    gttl::Tensor<Scalar, 3, dims> tensor{
        +1, +2, +3, +4, +5, +6, +7, +8, +9, 10, 11, 12,
        13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
    };
    std::array<Scalar, 24> values{
        +1, +2, +3, +4, +5, +6, +7, +8, +9, 10, 11, 12,
        13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
    };

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(tensor),
        std::end(tensor),
        std::begin(values),
        std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
    partial_value_constructor_vector, Scalar, scalar_types
)
{
    constexpr gttl::Dimensions<1> dims{3_D};
    gttl::Tensor<Scalar, 1, dims> tensor{1, 2, /*3*/};
    std::array<Scalar, 3> values{1, 2, 0};

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(tensor),
        std::end(tensor),
        std::begin(values),
        std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
    partial_value_constructor_ten3, Scalar, scalar_types
)
{
    constexpr gttl::Dimensions<3> dims{4_D, 3_D, 2_D};
    gttl::Tensor<Scalar, 3, dims> tensor{
        +1, +2, +3, +4, +5, +6, +7, +8, +9, /*10, 11, 12,
        13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,*/
    };
    std::array<Scalar, 24> values{
        +1, +2, +3, +4, +5, +6, +7, +8, +9, +0, +0, +0,
        +0, +0, +0, +0, +0, +0, +0, +0, +0, +0, +0, +0,
    };

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(tensor),
        std::end(tensor),
        std::begin(values),
        std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
    subtensor_constructor_vector, Scalar, scalar_types
)
{
    constexpr gttl::Dimensions<0> dims0{};
    using S = gttl::Tensor<Scalar, 0, dims0>;

    constexpr gttl::Dimensions<1> dims{3_D};
    // note: It is important, that this call is unambigous!
    // see: value_constructor_vector
    gttl::Tensor<Scalar, 1, dims> tensor{S{1}, S{2}, S{3}};
    std::array<Scalar, 3> values{1, 2, 3};

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(tensor),
        std::end(tensor),
        std::begin(values),
        std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(subtensor_constructor_ten3, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<3> dims{4_D, 3_D, 2_D};
    constexpr gttl::Dimensions<2> subdims{3_D, 2_D};
    using SubTensor = gttl::Tensor<Scalar, 2, subdims>;
    gttl::Tensor<Scalar, 3, dims> tensor{
        SubTensor{+1, +2, +3, +4, +5, +6},
        SubTensor{+7, +8, +9, 10, 11, 12},
        SubTensor{13, 14, 15, 16, 17, 18},
        SubTensor{19, 20, 21, 22, 23, 24},
    };
    std::array<Scalar, 24> values{
        +1, +2, +3, +4, +5, +6, +7, +8, +9, 10, 11, 12,
        13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
    };

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(tensor),
        std::end(tensor),
        std::begin(values),
        std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
    partial_subtensor_constructor_vector, Scalar, scalar_types
)
{
    constexpr gttl::Dimensions<0> dims0{};
    using S = gttl::Tensor<Scalar, 0, dims0>;

    constexpr gttl::Dimensions<1> dims{3_D};
    gttl::Tensor<Scalar, 1, dims> tensor{S{1}, S{2}, /*3*/};
    std::array<Scalar, 3> values{1, 2, 0};

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(tensor),
        std::end(tensor),
        std::begin(values),
        std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
    partial_subtensor_constructor_ten3, Scalar, scalar_types
)
{
    constexpr gttl::Dimensions<3> dims{4_D, 3_D, 2_D};
    constexpr gttl::Dimensions<2> subdims{3_D, 2_D};
    using SubTensor = gttl::Tensor<Scalar, 2, subdims>;
    gttl::Tensor<Scalar, 3, dims> tensor{
        SubTensor{+1, +2, +3, +4, +5, +6}, SubTensor{+7, +8, +9, 10, 11, 12},
        /*SubTensor{13, 14, 15, 16, 17, 18},
        SubTensor{19, 20, 21, 22, 23, 24},*/
    };
    std::array<Scalar, 24> values{
        +1, +2, +3, +4, +5, +6, +7, +8, +9, 10, 11, 12,
        /*13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,*/
    };

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(tensor),
        std::end(tensor),
        std::begin(values),
        std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(array_conversion_scalar, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<0> dims{};
    gttl::Tensor<Scalar, 0, dims> tensor{1};
    using A = std::array<Scalar, 1>;
    A values{1};
    const auto& res = static_cast<const A&>(tensor);

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(res), std::end(res), std::begin(values), std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(array_conversion_vector, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<1> dims{3_D};
    gttl::Tensor<Scalar, 1, dims> tensor{1, 2, 3};
    using A = std::array<Scalar, 3>;
    A values{1, 2, 3};
    const auto& res = static_cast<const A&>(tensor);

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(res), std::end(res), std::begin(values), std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(array_conversion_ten3, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<3> dims{4_D, 3_D, 2_D};
    gttl::Tensor<Scalar, 3, dims> tensor{
        +1, +2, +3, +4, +5, +6, +7, +8, +9, 10, 11, 12,
        13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
    };
    using A = std::array<Scalar, 24>;
    A values{
        +1, +2, +3, +4, +5, +6, +7, +8, +9, 10, 11, 12,
        13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
    };
    const auto& res = static_cast<const A&>(tensor);

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(res), std::end(res), std::begin(values), std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
    scalar_const_conversion_scalar, Scalar, scalar_types
)
{
    constexpr gttl::Dimensions<0> dims{};
    const gttl::Tensor<Scalar, 0, dims> tensor{7};
    Scalar value = tensor;
    BOOST_TEST(value == 7);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
    scalar_mut_conversion_scalar, Scalar, scalar_types
)
{
    constexpr gttl::Dimensions<0> dims{};
    gttl::Tensor<Scalar, 0, dims> tensor{7};
    Scalar& value = tensor;
    value += 8;
    BOOST_TEST(value == 15);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(scalar_assignment, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<0> dims{};
    gttl::Tensor<Scalar, 0, dims> tensor{};
    tensor = {9};
    BOOST_TEST(tensor == 9);
    tensor = 11;
    BOOST_TEST(tensor == 11);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(iterate_ten3, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<3> dims{4_D, 3_D, 2_D};
    gttl::Tensor<Scalar, 3, dims> tensor{
        +1, +2, +3, +4, +5, +6, +7, +8, +9, 10, 11, 12,
        13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
    };
    std::array<Scalar, 24> values{
        99, +2, +3, +4, +5, +6, +7, +8, +9, 10, 11, 12,
        13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
    };

    *std::begin(tensor) = 99;

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(tensor),
        std::end(tensor),
        std::begin(values),
        std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(const_iterate_ten3, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<3> dims{4_D, 3_D, 2_D};
    gttl::Tensor<Scalar, 3, dims> tensor{
        +1, +2, +3, +4, +5, +6, +7, +8, +9, 10, 11, 12,
        13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
    };
    std::array<Scalar, 24> values{
        +1, +2, +3, +4, +5, +6, +7, +8, +9, 10, 11, 12,
        13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
    };

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::cbegin(tensor),
        std::cend(tensor),
        std::cbegin(values),
        std::cend(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(reverse_iterate_ten3, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<3> dims{4_D, 3_D, 2_D};
    gttl::Tensor<Scalar, 3, dims> tensor{
        +1, +2, +3, +4, +5, +6, +7, +8, +9, 10, 11, 12,
        13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
    };
    std::array<Scalar, 24> values{
        +1, +2, +3, +4, +5, +6, +7, +8, +9, 10, 11, 12,
        13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 99,
    };

    *std::rbegin(tensor) = 99;

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::rbegin(tensor),
        std::rend(tensor),
        std::rbegin(values),
        std::rend(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(reverse_const_iterate_ten3, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<3> dims{4_D, 3_D, 2_D};
    gttl::Tensor<Scalar, 3, dims> tensor{
        +1, +2, +3, +4, +5, +6, +7, +8, +9, 10, 11, 12,
        13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
    };
    std::array<Scalar, 24> values{
        +1, +2, +3, +4, +5, +6, +7, +8, +9, 10, 11, 12,
        13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
    };

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::crbegin(tensor),
        std::crend(tensor),
        std::crbegin(values),
        std::crend(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(swap_scalar, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<0> dims{};
    gttl::Tensor<Scalar, 0, dims> tensor1{1};
    gttl::Tensor<Scalar, 0, dims> tensor2{2};
    std::array<Scalar, 1> values1{1};
    std::array<Scalar, 1> values2{2};

    tensor1.swap(tensor2);

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(tensor1),
        std::end(tensor1),
        std::begin(values2),
        std::end(values2)
    );
    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(tensor2),
        std::end(tensor2),
        std::begin(values1),
        std::end(values1)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
    make_multi_index_range_scalar, Scalar, scalar_types
)
{
    constexpr gttl::Dimensions<0> dims{};
    gttl::MultiIndexRange<0, dims> range{
        gttl::Tensor<Scalar, 0, dims>::make_index_range(),
    };
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
    make_multi_index_range_vector, Scalar, scalar_types
)
{
    constexpr gttl::Dimensions<1> dims{3_D};
    gttl::MultiIndexRange<1, dims> range{
        gttl::Tensor<Scalar, 1, dims>::make_index_range(),
    };
}

BOOST_AUTO_TEST_CASE_TEMPLATE(make_multi_index_range_ten3, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<3> dims{4_D, 3_D, 2_D};
    gttl::MultiIndexRange<3, dims> range{
        gttl::Tensor<Scalar, 3, dims>::make_index_range(),
    };
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
    get_multi_index_for_index_scalar, Scalar, scalar_types
)
{
    constexpr gttl::Dimensions<0> dims{};
    [[maybe_unused]] gttl::MultiIndex<0, dims> multi_index{
        gttl::Tensor<Scalar, 0, dims>::get_multi_index_for_index(0),
    };
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
    get_multi_index_for_index_vector, Scalar, scalar_types
)
{
    constexpr static gttl::Dimensions<1> dims{3_D};
    using T = gttl::Tensor<Scalar, 1, dims>;
    using MI = gttl::MultiIndex<1, dims>;

    for (std::size_t i{0}; i < 3; ++i) {
        MI multi_index = T::get_multi_index_for_index(i);
        BOOST_TEST(multi_index == MI{i});
    }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
    get_multi_index_for_index_ten3, Scalar, scalar_types
)
{
    constexpr static gttl::Dimensions<3> dims{3_D, 4_D, 5_D};
    using T = gttl::Tensor<Scalar, 3, dims>;
    using MI = gttl::MultiIndex<3, dims>;

    for (std::size_t i{0}; i < 3; ++i) {
        for (std::size_t j{0}; j < 4; ++j) {
            for (std::size_t k{0}; k < 5; ++k) {
                MI multi_index =
                    T::get_multi_index_for_index(i * 20 + j * 5 + k);
                BOOST_TEST(multi_index == (MI{i, j, k}));
            }
        }
    }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(swap_vector, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<1> dims{3_D};
    gttl::Tensor<Scalar, 1, dims> tensor1{+1, +2, +3};
    gttl::Tensor<Scalar, 1, dims> tensor2{-1, -2, -3};
    std::array<Scalar, 3> values1{+1, +2, +3};
    std::array<Scalar, 3> values2{-1, -2, -3};

    tensor1.swap(tensor2);

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(tensor1),
        std::end(tensor1),
        std::begin(values2),
        std::end(values2)
    );
    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(tensor2),
        std::end(tensor2),
        std::begin(values1),
        std::end(values1)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(swap_ten3, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<3> dims{4_D, 3_D, 2_D};
    gttl::Tensor<Scalar, 3, dims> tensor1{
        +1, +2, +3, +4, +5, +6, +7, +8, +9, 10, 11, 12,
        13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
    };
    gttl::Tensor<Scalar, 3, dims> tensor2{
        -1, -2, 3,  -4, -5, -6, -7, -8, -9, 30, 31, 32,
        33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44,
    };
    std::array<Scalar, 24> values1{
        +1, +2, +3, +4, +5, +6, +7, +8, +9, 10, 11, 12,
        13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
    };
    std::array<Scalar, 24> values2{
        -1, -2, 3,  -4, -5, -6, -7, -8, -9, 30, 31, 32,
        33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44,
    };

    tensor1.swap(tensor2);

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(tensor1),
        std::end(tensor1),
        std::begin(values2),
        std::end(values2)
    );
    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(tensor2),
        std::end(tensor2),
        std::begin(values1),
        std::end(values1)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
    const_scalar_at_indicies_ten3, Scalar, scalar_types
)
{
    constexpr gttl::Dimensions<3> dims{4_D, 3_D, 2_D};
    using T = gttl::Tensor<Scalar, 3, dims>;
    const T tensor{
        +1, +2, +3, +4, +5, +6, // 0
        +7, +8, +9, 10, 11, 12, // 1
        13, 14, 15, 16, 17, 18, // 2
        19, 20, 21, 22, 23, 24, // 3
    };

    BOOST_TEST(tensor.at(0, 0, 0) == +1);
    BOOST_TEST(tensor.at(0, 0, 1) == +2);
    BOOST_TEST(tensor.at(0, 1, 0) == +3);
    BOOST_TEST(tensor.at(0, 1, 1) == +4);
    BOOST_TEST(tensor.at(0, 2, 0) == +5);
    BOOST_TEST(tensor.at(0, 2, 1) == +6);
    BOOST_TEST(tensor.at(1, 0, 0) == +7);
    BOOST_TEST(tensor.at(1, 0, 1) == +8);
    BOOST_TEST(tensor.at(1, 1, 0) == +9);
    BOOST_TEST(tensor.at(1, 1, 1) == 10);
    BOOST_TEST(tensor.at(1, 2, 0) == 11);
    BOOST_TEST(tensor.at(1, 2, 1) == 12);
    BOOST_TEST(tensor.at(2, 0, 0) == 13);
    BOOST_TEST(tensor.at(2, 0, 1) == 14);
    BOOST_TEST(tensor.at(2, 1, 0) == 15);
    BOOST_TEST(tensor.at(2, 1, 1) == 16);
    BOOST_TEST(tensor.at(2, 2, 0) == 17);
    BOOST_TEST(tensor.at(2, 2, 1) == 18);
    BOOST_TEST(tensor.at(3, 0, 0) == 19);
    BOOST_TEST(tensor.at(3, 0, 1) == 20);
    BOOST_TEST(tensor.at(3, 1, 0) == 21);
    BOOST_TEST(tensor.at(3, 1, 1) == 22);
    BOOST_TEST(tensor.at(3, 2, 0) == 23);
    BOOST_TEST(tensor.at(3, 2, 1) == 24);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mut_scalar_at_indicies_ten3, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<3> dims{4_D, 3_D, 2_D};
    using T = gttl::Tensor<Scalar, 3, dims>;
    /*mut*/ T tensor{};

    tensor.at(0, 0, 0) = +1;
    tensor.at(0, 0, 1) = +2;
    tensor.at(0, 1, 0) = +3;
    tensor.at(0, 1, 1) = +4;
    tensor.at(0, 2, 0) = +5;
    tensor.at(0, 2, 1) = +6;
    tensor.at(1, 0, 0) = +7;
    tensor.at(1, 0, 1) = +8;
    tensor.at(1, 1, 0) = +9;
    tensor.at(1, 1, 1) = 10;
    tensor.at(1, 2, 0) = 11;
    tensor.at(1, 2, 1) = 12;
    tensor.at(2, 0, 0) = 13;
    tensor.at(2, 0, 1) = 14;
    tensor.at(2, 1, 0) = 15;
    tensor.at(2, 1, 1) = 16;
    tensor.at(2, 2, 0) = 17;
    tensor.at(2, 2, 1) = 18;
    tensor.at(3, 0, 0) = 19;
    tensor.at(3, 0, 1) = 20;
    tensor.at(3, 1, 0) = 21;
    tensor.at(3, 1, 1) = 22;
    tensor.at(3, 2, 0) = 23;
    tensor.at(3, 2, 1) = 24;

    BOOST_TEST(tensor.at(0, 0, 0) == +1);
    BOOST_TEST(tensor.at(0, 0, 1) == +2);
    BOOST_TEST(tensor.at(0, 1, 0) == +3);
    BOOST_TEST(tensor.at(0, 1, 1) == +4);
    BOOST_TEST(tensor.at(0, 2, 0) == +5);
    BOOST_TEST(tensor.at(0, 2, 1) == +6);
    BOOST_TEST(tensor.at(1, 0, 0) == +7);
    BOOST_TEST(tensor.at(1, 0, 1) == +8);
    BOOST_TEST(tensor.at(1, 1, 0) == +9);
    BOOST_TEST(tensor.at(1, 1, 1) == 10);
    BOOST_TEST(tensor.at(1, 2, 0) == 11);
    BOOST_TEST(tensor.at(1, 2, 1) == 12);
    BOOST_TEST(tensor.at(2, 0, 0) == 13);
    BOOST_TEST(tensor.at(2, 0, 1) == 14);
    BOOST_TEST(tensor.at(2, 1, 0) == 15);
    BOOST_TEST(tensor.at(2, 1, 1) == 16);
    BOOST_TEST(tensor.at(2, 2, 0) == 17);
    BOOST_TEST(tensor.at(2, 2, 1) == 18);
    BOOST_TEST(tensor.at(3, 0, 0) == 19);
    BOOST_TEST(tensor.at(3, 0, 1) == 20);
    BOOST_TEST(tensor.at(3, 1, 0) == 21);
    BOOST_TEST(tensor.at(3, 1, 1) == 22);
    BOOST_TEST(tensor.at(3, 2, 0) == 23);
    BOOST_TEST(tensor.at(3, 2, 1) == 24);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
    const_matrix_at_indicies_ten3, Scalar, scalar_types
)
{
    constexpr gttl::Dimensions<3> dims{4_D, 3_D, 2_D};
    using T = gttl::Tensor<Scalar, 3, dims>;
    const T tensor{
        +1, +2, +3, +4, +5, +6, // 0
        +7, +8, +9, 10, 11, 12, // 1
        13, 14, 15, 16, 17, 18, // 2
        19, 20, 21, 22, 23, 24, // 3
    };
    constexpr gttl::Dimensions<2> dims_sub{3_D, 2_D};
    using TSub = gttl::Tensor<Scalar, 2, dims_sub>;
    std::array<TSub, 4> values{
        TSub{+1, +2, +3, +4, +5, +6},
        TSub{+7, +8, +9, 10, 11, 12},
        TSub{13, 14, 15, 16, 17, 18},
        TSub{19, 20, 21, 22, 23, 24},
    };

    using A = const std::array<Scalar, 6>&;
    for (std::size_t i{0}; i < 4; ++i) {
        // note: compare as arrays for proper floating-point comparison
        BOOST_TEST(A(tensor.at(i)) == A(values.at(i)));
    }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mut_matrix_at_indicies_ten3, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<3> dims{4_D, 3_D, 2_D};
    using T = gttl::Tensor<Scalar, 3, dims>;
    /*mut*/ T tensor{};
    constexpr gttl::Dimensions<2> dims_sub{3_D, 2_D};
    using TSub = gttl::Tensor<Scalar, 2, dims_sub>;
    std::array<TSub, 4> values{
        TSub{+1, +2, +3, +4, +5, +6},
        TSub{+7, +8, +9, 10, 11, 12},
        TSub{13, 14, 15, 16, 17, 18},
        TSub{19, 20, 21, 22, 23, 24},
    };

    using A = const std::array<Scalar, 6>&;
    for (std::size_t i{0}; i < 4; ++i) {
        tensor.at(i) = values.at(i);
        // note: compare as arrays for proper floating-point comparison
        BOOST_TEST(A(tensor.at(i)) == A(values.at(i)));
    }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(const_matrix_slice_ten3, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<3> dims{4_D, 3_D, 2_D};
    using T = gttl::Tensor<Scalar, 3, dims>;
    const T tensor{
        +1, +2, +3, +4, +5, +6, // 0
        +7, +8, +9, 10, 11, 12, // 1
        13, 14, 15, 16, 17, 18, // 2
        19, 20, 21, 22, 23, 24, // 3
    };
    constexpr gttl::Dimensions<2> dims_sub{3_D, 2_D};
    using TSub = gttl::Tensor<Scalar, 2, dims_sub>;
    std::array<TSub, 4> values{
        TSub{+1, +2, +3, +4, +5, +6},
        TSub{+7, +8, +9, 10, 11, 12},
        TSub{13, 14, 15, 16, 17, 18},
        TSub{19, 20, 21, 22, 23, 24},
    };

    using A = const std::array<Scalar, 6>&;
    for (std::size_t i{0}; i < 4; ++i) {
        // note: compare as arrays for proper floating-point comparison
        BOOST_TEST(A(tensor.at(i)) == A(values.at(i)));
    }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mut_matrix_slice_ten3, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<3> dims{4_D, 3_D, 2_D};
    using T = gttl::Tensor<Scalar, 3, dims>;
    /*mut*/ T tensor{};
    constexpr gttl::Dimensions<2> dims_sub{3_D, 2_D};
    using TSub = gttl::Tensor<Scalar, 2, dims_sub>;
    std::array<TSub, 4> values{
        TSub{+1, +2, +3, +4, +5, +6},
        TSub{+7, +8, +9, 10, 11, 12},
        TSub{13, 14, 15, 16, 17, 18},
        TSub{19, 20, 21, 22, 23, 24},
    };

    using A = const std::array<Scalar, 6>&;
    for (std::size_t i{0}; i < 4; ++i) {
        tensor.at(i) = values.at(i);
        // note: compare as arrays for proper floating-point comparison
        BOOST_TEST(A(tensor.at(i)) == A(values.at(i)));
    }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
    const_scalar_at_multi_index_ten3, Scalar, scalar_types
)
{
    constexpr gttl::Dimensions<3> dims{4_D, 3_D, 2_D};
    using T = gttl::Tensor<Scalar, 3, dims>;
    const T tensor{
        +1, +2, +3, +4, +5, +6, +7, +8, +9, 10, 11, 12,
        13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
    };
    std::array<Scalar, 24> values{
        +1, +2, +3, +4, +5, +6, +7, +8, +9, 10, 11, 12,
        13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
    };

    std::size_t i{0};
    for (const auto& mi : T::make_index_range()) {
        BOOST_TEST(tensor.at(mi) == values.at(i));
        ++i;
    }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
    mut_scalar_at_multi_index_ten3, Scalar, scalar_types
)
{
    constexpr gttl::Dimensions<3> dims{4_D, 3_D, 2_D};
    using T = gttl::Tensor<Scalar, 3, dims>;
    /*mut*/ T tensor{};
    std::array<Scalar, 24> values{
        +1, +2, +3, +4, +5, +6, +7, +8, +9, 10, 11, 12,
        13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
    };

    std::size_t i{0};
    for (auto& mi : T::make_index_range()) {
        tensor.at(mi) = values.at(i);
        BOOST_TEST(tensor.at(mi) == values.at(i));
        ++i;
    }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
    const_matrix_at_multi_index_ten3, Scalar, scalar_types
)
{
    constexpr gttl::Dimensions<3> dims{4_D, 3_D, 2_D};
    using T = gttl::Tensor<Scalar, 3, dims>;
    const T tensor{
        +1, +2, +3, +4, +5, +6, +7, +8, +9, 10, 11, 12,
        13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
    };
    constexpr gttl::Dimensions<2> dims_sub{3_D, 2_D};
    using TSub = gttl::Tensor<Scalar, 2, dims_sub>;
    std::array<TSub, 4> values{
        TSub{+1, +2, +3, +4, +5, +6},
        TSub{+7, +8, +9, 10, 11, 12},
        TSub{13, 14, 15, 16, 17, 18},
        TSub{19, 20, 21, 22, 23, 24},
    };

    using Range = gttl::MultiIndexRange<1, gttl::Dimensions<1>{4_D}>;
    using A = const std::array<Scalar, 6>&;
    std::size_t i{0};
    for (const auto& mi : Range{}) {
        // note: compare as arrays for proper floating-point comparison
        BOOST_TEST(A(tensor.at(mi)) == A(values.at(i)));
        ++i;
    }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
    mut_matrix_at_multi_index_ten3, Scalar, scalar_types
)
{
    constexpr gttl::Dimensions<3> dims{4_D, 3_D, 2_D};
    using T = gttl::Tensor<Scalar, 3, dims>;
    /*mut*/ T tensor{};
    constexpr gttl::Dimensions<2> dims_sub{3_D, 2_D};
    using TSub = gttl::Tensor<Scalar, 2, dims_sub>;
    std::array<TSub, 4> values{
        TSub{+1, +2, +3, +4, +5, +6},
        TSub{+7, +8, +9, 10, 11, 12},
        TSub{13, 14, 15, 16, 17, 18},
        TSub{19, 20, 21, 22, 23, 24},
    };

    using Range = gttl::MultiIndexRange<1, gttl::Dimensions<1>{4_D}>;
    using A = const std::array<Scalar, 6>&;
    std::size_t i{0};
    for (const auto& mi : Range{}) {
        tensor.at(mi) = values.at(i);
        // note: compare as arrays for proper floating-point comparison
        BOOST_TEST(A(tensor.at(mi)) == A(values.at(i)));
        ++i;
    }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(inplace_elementwise_scalar, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<0> dims{};
    gttl::Tensor<Scalar, 0, dims> tensor{5};
    std::array<Scalar, 1> values{25};

    tensor.inplace_elementwise(square);

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(tensor),
        std::end(tensor),
        std::begin(values),
        std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(inplace_elementwise_vector, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<1> dims{3_D};
    gttl::Tensor<Scalar, 1, dims> tensor{1, 2, 3};
    std::array<Scalar, 3> values{1, 4, 9};

    tensor.inplace_elementwise(square);

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(tensor),
        std::end(tensor),
        std::begin(values),
        std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
    inplace_elementwise_2scalars, Scalar, scalar_types
)
{
    constexpr gttl::Dimensions<0> dims{};
    gttl::Tensor<Scalar, 0, dims> tensor0{4};
    gttl::Tensor<Scalar, 0, dims> tensor1{5};
    std::array<Scalar, 1> values{9};

    tensor0.inplace_elementwise(add, tensor1);

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(tensor0),
        std::end(tensor0),
        std::begin(values),
        std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
    inplace_elementwise_2vectors, Scalar, scalar_types
)
{
    constexpr gttl::Dimensions<1> dims{3_D};
    gttl::Tensor<Scalar, 1, dims> tensor0{1, 2, 3};
    gttl::Tensor<Scalar, 1, dims> tensor1{4, 5, 6};
    std::array<Scalar, 3> values{5, 7, 9};

    tensor0.inplace_elementwise(add, tensor1);

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(tensor0),
        std::end(tensor0),
        std::begin(values),
        std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(elementwise_2scalars, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<0> dims{};
    using T = gttl::Tensor<Scalar, 0, dims>;
    T tensor1{4};
    T tensor2{5};
    std::array<Scalar, 1> values{9};

    T res = tensor1.elementwise(add, tensor2);

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(res), std::end(res), std::begin(values), std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(elementwise_2vectors, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<1> dims{3_D};
    using T = gttl::Tensor<Scalar, 1, dims>;
    T tensor1{1, 2, 3};
    T tensor2{4, 5, 6};
    std::array<Scalar, 3> values{5, 7, 9};

    T res = tensor1.elementwise(add, tensor2);

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(res), std::end(res), std::begin(values), std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(plus_assignment_scalars, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<0> dims{};
    gttl::Tensor<Scalar, 0, dims> tensor0{4};
    gttl::Tensor<Scalar, 0, dims> tensor1{5};
    std::array<Scalar, 1> values{9};

    tensor0 += tensor1;

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(tensor0),
        std::end(tensor0),
        std::begin(values),
        std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(plus_assignment_vectors, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<1> dims{3_D};
    gttl::Tensor<Scalar, 1, dims> tensor0{1, 2, 3};
    gttl::Tensor<Scalar, 1, dims> tensor1{4, 5, 6};
    std::array<Scalar, 3> values{5, 7, 9};

    tensor0 += tensor1;

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(tensor0),
        std::end(tensor0),
        std::begin(values),
        std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(minus_assignment_scalars, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<0> dims{};
    gttl::Tensor<Scalar, 0, dims> tensor0{4};
    gttl::Tensor<Scalar, 0, dims> tensor1{5};
    std::array<Scalar, 1> values{-1};

    tensor0 -= tensor1;

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(tensor0),
        std::end(tensor0),
        std::begin(values),
        std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(minus_assignment_vectors, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<1> dims{3_D};
    gttl::Tensor<Scalar, 1, dims> tensor0{1, 2, 3};
    gttl::Tensor<Scalar, 1, dims> tensor1{4, 5, 6};
    std::array<Scalar, 3> values{-3, -3, -3};

    tensor0 -= tensor1;

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(tensor0),
        std::end(tensor0),
        std::begin(values),
        std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(inplace_elem_mul_scalars, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<0> dims{};
    gttl::Tensor<Scalar, 0, dims> tensor0{4};
    gttl::Tensor<Scalar, 0, dims> tensor1{5};
    std::array<Scalar, 1> values{20};

    tensor0.inplace_elem_mul(tensor1);

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(tensor0),
        std::end(tensor0),
        std::begin(values),
        std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(inplace_elem_mul_vectors, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<1> dims{3_D};
    gttl::Tensor<Scalar, 1, dims> tensor0{1, 2, 3};
    gttl::Tensor<Scalar, 1, dims> tensor1{4, 5, 6};
    std::array<Scalar, 3> values{4, 10, 18};

    tensor0.inplace_elem_mul(tensor1);

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(tensor0),
        std::end(tensor0),
        std::begin(values),
        std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(inplace_elem_div_scalars, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<0> dims{};
    gttl::Tensor<Scalar, 0, dims> tensor0{4};
    gttl::Tensor<Scalar, 0, dims> tensor1{5};
    std::array<Scalar, 1> values{Scalar{4} / Scalar{5}};

    tensor0.inplace_elem_div(tensor1);

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(tensor0),
        std::end(tensor0),
        std::begin(values),
        std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(inplace_elem_div_vectors, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<1> dims{3_D};
    gttl::Tensor<Scalar, 1, dims> tensor0{1, 2, 3};
    gttl::Tensor<Scalar, 1, dims> tensor1{4, 5, 6};
    std::array<Scalar, 3> values{
        Scalar{1} / Scalar{4},
        Scalar{2} / Scalar{5},
        Scalar{3} / Scalar{6},
    };

    tensor0.inplace_elem_div(tensor1);

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(tensor0),
        std::end(tensor0),
        std::begin(values),
        std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
    multiplies_assignment_scalar_scalar, Scalar, scalar_types
)
{
    constexpr gttl::Dimensions<0> dims{};
    gttl::Tensor<Scalar, 0, dims> tensor{2};
    const Scalar fac{7};
    std::array<Scalar, 1> values{14};

    tensor *= fac;

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(tensor),
        std::end(tensor),
        std::begin(values),
        std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
    multiplies_assignment_vector_scalar, Scalar, scalar_types
)
{
    constexpr gttl::Dimensions<1> dims{3_D};
    gttl::Tensor<Scalar, 1, dims> tensor{1, 2, 3};
    const Scalar fac{7};
    std::array<Scalar, 3> values{7, 14, 21};

    tensor *= fac;

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(tensor),
        std::end(tensor),
        std::begin(values),
        std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(plus_scalars, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<0> dims{};
    using T = gttl::Tensor<Scalar, 0, dims>;
    const T tensor1{4};
    const T tensor2{5};
    std::array<Scalar, 1> values{9};

    T res = tensor1 + tensor2;

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(res), std::end(res), std::begin(values), std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(plus_vectors, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<1> dims{3_D};
    using T = gttl::Tensor<Scalar, 1, dims>;
    const T tensor1{1, 2, 3};
    const T tensor2{4, 5, 6};
    std::array<Scalar, 3> values{5, 7, 9};

    T res = tensor1 + tensor2;

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(res), std::end(res), std::begin(values), std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(minus_scalars, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<0> dims{};
    using T = gttl::Tensor<Scalar, 0, dims>;
    const T tensor1{4};
    const T tensor2{5};
    std::array<Scalar, 1> values{-1};

    T res = tensor1 - tensor2;

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(res), std::end(res), std::begin(values), std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(minus_vectors, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<1> dims{3_D};
    using T = gttl::Tensor<Scalar, 1, dims>;
    const T tensor1{1, 2, 3};
    const T tensor2{4, 5, 6};
    std::array<Scalar, 3> values{-3, -3, -3};

    T res = tensor1 - tensor2;

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(res), std::end(res), std::begin(values), std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(elem_mul_scalars, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<0> dims{};
    using T = gttl::Tensor<Scalar, 0, dims>;
    const T tensor1{4};
    const T tensor2{5};
    std::array<Scalar, 1> values{20};

    T res = tensor1.elem_mul(tensor2);

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(res), std::end(res), std::begin(values), std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(elem_mul_vectors, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<1> dims{3_D};
    using T = gttl::Tensor<Scalar, 1, dims>;
    const T tensor1{1, 2, 3};
    const T tensor2{4, 5, 6};
    std::array<Scalar, 3> values{4, 10, 18};

    T res = tensor1.elem_mul(tensor2);

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(res), std::end(res), std::begin(values), std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(elem_div_scalars, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<0> dims{};
    using T = gttl::Tensor<Scalar, 0, dims>;
    const T tensor1{4};
    const T tensor2{5};
    std::array<Scalar, 1> values{Scalar{4} / Scalar{5}};

    T res = tensor1.elem_div(tensor2);

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(res), std::end(res), std::begin(values), std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(elem_div_vectors, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<1> dims{3_D};
    using T = gttl::Tensor<Scalar, 1, dims>;
    const T tensor1{1, 2, 3};
    const T tensor2{4, 5, 6};
    std::array<Scalar, 3> values{
        Scalar{1} / Scalar{4},
        Scalar{2} / Scalar{5},
        Scalar{3} / Scalar{6},
    };

    T res = tensor1.elem_div(tensor2);

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(res), std::end(res), std::begin(values), std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(negate_scalar, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<0> dims{};
    using T = gttl::Tensor<Scalar, 0, dims>;
    const T tensor{4};
    std::array<Scalar, 1> values{-4};

    T res = -tensor;

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(res), std::end(res), std::begin(values), std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(negate_vector, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<1> dims{3_D};
    using T = gttl::Tensor<Scalar, 1, dims>;
    const T tensor{1, 2, 3};
    std::array<Scalar, 3> values{-1, -2, -3};

    T res = -tensor;

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(res), std::end(res), std::begin(values), std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(multiplies_scalar_scalar, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<0> dims{};
    using T = gttl::Tensor<Scalar, 0, dims>;
    const T tensor{2};
    const Scalar fac{7};
    std::array<Scalar, 1> values{14};

    T res;

    res = tensor * fac;
    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(res), std::end(res), std::begin(values), std::end(values)
    );

    res = fac * tensor;
    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(res), std::end(res), std::begin(values), std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(multiplies_vector_scalar, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<1> dims{3_D};
    using T = gttl::Tensor<Scalar, 1, dims>;
    T tensor{1, 2, 3};
    const Scalar fac{7};
    std::array<Scalar, 3> values{7, 14, 21};

    T res = tensor * fac;

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(res), std::end(res), std::begin(values), std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(multiplies_scalar_vector, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<1> dims{3_D};
    using T = gttl::Tensor<Scalar, 1, dims>;
    T tensor{1, 2, 3};
    const Scalar fac{7};
    std::array<Scalar, 3> values{7, 14, 21};

    T res = fac * tensor;

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(res), std::end(res), std::begin(values), std::end(values)
    );
}

// note: If the platform generates power of two sized memory layouts only,
//       this will be power of two sized as well.
//       This is fine, since there should be no padding issues.
// note: This type attempts to provoke padding and alignment issues as much as
//       possible.
struct NonPowerOfTwoSizedScalar {
    using value_type = std::int_least8_t; // as small as possible

    value_type dummy1{-1};
    value_type i{0};
    value_type dummy2{-2};

    constexpr NonPowerOfTwoSizedScalar() = default;

    constexpr NonPowerOfTwoSizedScalar(const value_type value) : i{value} { }

    constexpr bool
    operator<=>(const NonPowerOfTwoSizedScalar&) const = default;
};

std::ostream&
operator<<(std::ostream& os, const NonPowerOfTwoSizedScalar& rhs)
{
    os << static_cast<int>(rhs.i);
    return os;
}

// MAINTENANCE:  C++23 provides fixed-width floating-point scalars in <stdfloat>
//               Add these for improved test coverage.
using layout_scalar_types = bmpl::list<
    NonPowerOfTwoSizedScalar,
    std::int_least8_t,
    std::int_least16_t,
    float,
    double,
    long double>;

BOOST_AUTO_TEST_CASE_TEMPLATE(layout, Scalar, layout_scalar_types)
{
    // note: There is a possibility that subtensors get an additional padding.
    //       This test tries to detect any issues related to that in addition to
    //       the static asserts provided in the include file.

    // note: Use (sub)tensor sizes which are not powers of 2 to test for
    //       paddinig.
    constexpr gttl::Dimensions<2> dims{3_D, 3_D};
    constexpr gttl::Dimensions<1> dims_sub{3_D};
    using Tensor = gttl::Tensor<Scalar, 2, dims>;
    using SubTensor = gttl::Tensor<Scalar, 1, dims_sub>;
    Tensor tensor{};
    SubTensor subtensor0{1, 2, 3};
    SubTensor subtensor1{4, 5, 6};
    SubTensor subtensor2{7, 8, 9};
    std::array<Scalar, 9> values{1, 2, 3, 4, 5, 6, 7, 8, 9};

    // Note: Must call operator[] to trigger static asserts!
    // note: Unusual order is intended!
    //       Writing to the middle section last may reveal padding issues.
    //       E.g. if the subtensors have an additional padding at the end.
    tensor[0] = subtensor0;
    tensor[2] = subtensor2;
    tensor[1] = subtensor1;

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(tensor),
        std::end(tensor),
        std::begin(values),
        std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(punning_related, Scalar, layout_scalar_types)
{
    // note: An older version of Tensor caused a bug due o the strict aliasing
    //       violation when using `reinterpret_cast`. This test chack the
    //       specific scenario the bug was caused in.
    // note: The bug was observerd on ARM64 but not x86 platforms using gcc and
    //       clang.
    constexpr gttl::Dimensions<2> dims{3_D, 3_D};
    using Tensor = gttl::Tensor<Scalar, 2, dims>;
    Tensor tensor{}; // initialize as zero
    std::array<Scalar, 9> values{1, 0, 0, 0, 2, 0, 0, 0, 3};

    for (auto i{0}; i < 3; ++i) {
        tensor[i][i] = i + 1;
    }

    // note: In the bug tensor[0][0] was not initialized at this stage.

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(tensor),
        std::end(tensor),
        std::begin(values),
        std::end(values)
    );
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
