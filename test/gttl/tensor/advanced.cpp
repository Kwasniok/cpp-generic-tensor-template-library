#define BOOST_TEST_MODULE gttl::Tensor

#include "../../boost_util/array.hpp"
#include <boost/mpl/list.hpp>
#include <boost/test/included/unit_test.hpp>
#include <gttl/tensor/advanced.hpp>

namespace bmpl = boost::mpl;

using gttl::literals::operator"" _D;
using scalar_types = bmpl::list<float, double, long double>;

// There will be a lot of 'magic' numbers here.
// NOLINTBEGIN(cppcoreguidelines-avoid-magic_numbers)

BOOST_AUTO_TEST_CASE_TEMPLATE(contraction_scalar_scalar, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<0> dims1{};
    constexpr gttl::Dimensions<0> dims2{};
    constexpr gttl::Dimensions<0> dims_res{};
    using T1 = gttl::Tensor<Scalar, 0, dims1>;
    using T2 = gttl::Tensor<Scalar, 0, dims2>;
    using TRes = gttl::Tensor<Scalar, 0, dims_res>;
    const T1 tensor1{1};
    const T2 tensor2{2};
    std::array<Scalar, 1> values{2};

    TRes res = outer_product(tensor1, tensor2);
    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(res), std::end(res), std::begin(values), std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(contraction_vector_matrix, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<1> dims1{2_D};
    constexpr gttl::Dimensions<2> dims2{3_D, 4_D};
    using T1 = gttl::Tensor<Scalar, 1, dims1>;
    using T2 = gttl::Tensor<Scalar, 2, dims2>;
    const T1 tensor1{1, 2};
    // clang-format off
    const T2 tensor2{
        +10, +20, +30, +40,
        +50, +60, +70, +80,
        +90, 100, 110, 120,
    };
    // clang-format on

    // vector x matrix
    {
        constexpr gttl::Dimensions<3> dims_res{2_D, 3_D, 4_D};
        using TRes = gttl::Tensor<Scalar, 3, dims_res>;

        // clang-format off
        std::array<Scalar, 2 * 3 * 4> values{
            +10, +20, +30, +40,
            +50, +60, +70, +80,
            +90, 100, 110, 120,

            +20, +40, +60, +80,
            100, 120, 140, 160,
            180, 200, 220, 240,
        };
        // clang-format on

        TRes res = outer_product(tensor1, tensor2);
        BOOST_CHECK_EQUAL_COLLECTIONS(
            std::begin(res), std::end(res), std::begin(values), std::end(values)
        );
    }
    // matrix x vector
    {
        constexpr gttl::Dimensions<3> dims_res{3_D, 4_D, 2_D};
        using TRes = gttl::Tensor<Scalar, 3, dims_res>;

        // clang-format off
        std::array<Scalar, 3 * 4 * 2> values{
            +10, +20,
            +20, +40,
            +30, +60,
            +40, +80,

            +50, 100,
            +60, 120,
            +70, 140,
            +80, 160,

            +90, 180,
            100, 200,
            110, 220,
            120, 240,
        };
        // clang-format on

        TRes res = outer_product(tensor2, tensor1);
        BOOST_CHECK_EQUAL_COLLECTIONS(
            std::begin(res), std::end(res), std::begin(values), std::end(values)
        );
    }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(contraction_matrix_ten4, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<2> dims1{2_D, 3_D};
    constexpr gttl::Dimensions<4> dims2{3_D, 2_D, 1_D, 4_D};
    using T1 = gttl::Tensor<Scalar, 2, dims1>;
    using T2 = gttl::Tensor<Scalar, 4, dims2>;
    // clang-format off
    const T1 tensor1{
        1, 2, 3,
        4, 5, 6,
    };
    const T2 tensor2{
        +10, +20, +30, +40,
        +50, +60, +70, +80,

        +90, 100, 110, 120,
        130, 140, 150, 160,

        170, 180, 190, 200,
        210, 220, 230, 240,
    };
    // clang-format on

    // matrix x rank-4 tensor
    {
        constexpr gttl::Dimensions<6> dims_res{2_D, 3_D, 3_D, 2_D, 1_D, 4_D};
        using TRes = gttl::Tensor<Scalar, 6, dims_res>;

        // clang-format off
        std::array<Scalar, 2 * 3 * 3 * 2 * 1 * 4> values{
        +10, +20, +30, +40,
        +50, +60, +70, +80,
        +90, 100, 110, 120,
        130, 140, 150, 160,
        170, 180, 190, 200,
        210, 220, 230, 240,

        +20, +40, +60, +80,
        100, 120, 140, 160,
        180, 200, 220, 240,
        260, 280, 300, 320,
        340, 360, 380, 400,
        420, 440, 460, 480,

        +30, +60, +90, 120,
        150, 180, 210, 240,
        270, 300, 330, 360,
        390, 420, 450, 480,
        510, 540, 570, 600,
        630, 660, 690, 720,


        +40, +80, 120, 160,
        200, 240, 280, 320,
        360, 400, 440, 480,
        520, 560, 600, 640,
        680, 720, 760, 800,
        840, 880, 920, 960,

          50,  100,  150,  200,
         250,  300,  350,  400,
         450,  500,  550,  600,
         650,  700,  750,  800,
         850,  900,  950, 1000,
        1050, 1100, 1150, 1200,

          60,  120,  180,  240,
         300,  360,  420,  480,
         540,  600,  660,  720,
         780,  840,  900,  960,
        1020, 1080, 1140, 1200,
        1260, 1320, 1380, 1440, 
        };
        // clang-format on

        TRes res = outer_product(tensor1, tensor2);
        BOOST_CHECK_EQUAL_COLLECTIONS(
            std::begin(res), std::end(res), std::begin(values), std::end(values)
        );
    }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(contraction_matrix, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<2> dims{3_D, 3_D};
    constexpr gttl::Dimensions<0> dims_res{};
    using T = gttl::Tensor<Scalar, 2, dims>;
    using TRes = gttl::Tensor<Scalar, 0, dims_res>;
    // clang-format off
    const T tensor{
        1, 2, 3,
        4, 5, 6,
        7, 8, 9,
    };
    // clang-format on
    std::array<Scalar, 1> values{1 + 5 + 9};

    TRes res = contraction<0, 1>(tensor);

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(res), std::end(res), std::begin(values), std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(contraction_ten3, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<3> dims{3_D, 4_D, 3_D};
    constexpr gttl::Dimensions<1> dims_res{4_D};
    using T = gttl::Tensor<Scalar, 3, dims>;
    using TRes = gttl::Tensor<Scalar, 1, dims_res>;
    // fill tensor with natural numbers 1,...
    T tensor;
    std::iota(std::begin(tensor), std::end(tensor), 1);

    std::array<Scalar, 4> values{
        +1 + 14 + 27, // = 42
        +4 + 17 + 30, // = 51
        +7 + 20 + 33, // = 60
        10 + 23 + 36, // = 69
    };

    TRes res;

    res = contraction<0, 2>(tensor);
    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(res), std::end(res), std::begin(values), std::end(values)
    );

    // reverse order of indicies
    res = contraction<2, 0>(tensor);
    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(res), std::end(res), std::begin(values), std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(contraction_ten5, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<5> dims{3_D, 2_D, 3_D, 1_D, 2_D};
    constexpr gttl::Dimensions<3> dims_res{3_D, 3_D, 1_D};
    using T = gttl::Tensor<Scalar, 5, dims>;
    using TRes = gttl::Tensor<Scalar, 3, dims_res>;
    // fill tensor with natural numbers 1,...
    T tensor;
    std::iota(std::begin(tensor), std::end(tensor), 1);

    std::array<Scalar, 9> values{9, 13, 17, 33, 37, 41, 57, 61, 65};

    TRes res;

    res = contraction<1, 4>(tensor);
    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(res), std::end(res), std::begin(values), std::end(values)
    );

    // reverse order of indicies
    res = contraction<4, 1>(tensor);
    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(res), std::end(res), std::begin(values), std::end(values)
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(contraction_matrix_scalar, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<2> dims1{3_D, 3_D};
    constexpr gttl::Dimensions<0> dims2{};
    constexpr gttl::Dimensions<0> dims_res{};
    using T1 = gttl::Tensor<Scalar, 2, dims1>;
    using T2 = gttl::Tensor<Scalar, 0, dims2>;
    using TRes = gttl::Tensor<Scalar, 0, dims_res>;
    // clang-format off
    const T1 tensor1{
        1, 2, 3,
        4, 5, 6,
        7, 8, 9,
    };
    // clang-format on
    const T2 tensor2{10};
    std::array<Scalar, 1> values{10 + 50 + 90};

    {
        // matrix left
        TRes res = contraction<0, 1>(tensor1, tensor2);
        BOOST_CHECK_EQUAL_COLLECTIONS(
            std::begin(res), std::end(res), std::begin(values), std::end(values)
        );
    }
    {
        // matrix right
        TRes res = contraction<1, 0>(tensor2, tensor1);
        BOOST_CHECK_EQUAL_COLLECTIONS(
            std::begin(res), std::end(res), std::begin(values), std::end(values)
        );
    }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(contraction_matrix_vector, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<2> dims1{3_D, 2_D};
    constexpr gttl::Dimensions<1> dims2{3_D};
    constexpr gttl::Dimensions<1> dims_res{2_D};
    using T1 = gttl::Tensor<Scalar, 2, dims1>;
    using T2 = gttl::Tensor<Scalar, 1, dims2>;
    using TRes = gttl::Tensor<Scalar, 1, dims_res>;
    // clang-format off
    const T1 tensor1{
        1, 2,
        3, 4,
        5, 6,
    };
    // clang-format on
    const T2 tensor2{10, 20, 30};
    std::array<Scalar, 2> values{10 + 60 + 150, 20 + 80 + 180};

    {
        // matrix left, vector right
        TRes res;
        res = contraction<0, 2>(tensor1, tensor2);
        BOOST_CHECK_EQUAL_COLLECTIONS(
            std::begin(res), std::end(res), std::begin(values), std::end(values)
        );
        res = contraction<2, 0>(tensor1, tensor2);
        BOOST_CHECK_EQUAL_COLLECTIONS(
            std::begin(res), std::end(res), std::begin(values), std::end(values)
        );
    }
    {
        // vector left, matrix right
        TRes res;
        res = contraction<0, 1>(tensor2, tensor1);
        BOOST_CHECK_EQUAL_COLLECTIONS(
            std::begin(res), std::end(res), std::begin(values), std::end(values)
        );
        res = contraction<1, 0>(tensor2, tensor1);
        BOOST_CHECK_EQUAL_COLLECTIONS(
            std::begin(res), std::end(res), std::begin(values), std::end(values)
        );
    }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(contraction_matrix_ten3, Scalar, scalar_types)
{
    constexpr gttl::Dimensions<2> dims1{3_D, 2_D};
    constexpr gttl::Dimensions<3> dims2{2_D, 3_D, 4_D};
    using T1 = gttl::Tensor<Scalar, 2, dims1>;
    using T2 = gttl::Tensor<Scalar, 3, dims2>;
    // clang-format off
    const T1 tensor1{
        1, 2,
        3, 4,
        5, 6,
    };
    const T2 tensor2{
         10,  20,  30,  40,
         50,  60,  70,  80,
         90, 100, 110, 120,

        130, 140, 150, 160,
        170, 180, 190, 200,
        210, 220, 230, 240,
    };
    // clang-format on

    {
        // matrix left, rank-3 tensor right
        constexpr gttl::Dimensions<3> dims_res{2_D, 2_D, 4_D};
        using TRes = gttl::Tensor<Scalar, 3, dims_res>;
        // clang-format off
        std::array<Scalar, 2 * 2 * 4> values{
             610,  700,  790,  880,
            1690, 1780, 1870, 1960,
             760,  880, 1000, 1120,
            2200, 2320, 2440, 2560,
        };
        // clang-format on
        TRes res;
        res = contraction<0, 3>(tensor1, tensor2);
        BOOST_CHECK_EQUAL_COLLECTIONS(
            std::begin(res), std::end(res), std::begin(values), std::end(values)
        );
        res = contraction<3, 0>(tensor1, tensor2);
        BOOST_CHECK_EQUAL_COLLECTIONS(
            std::begin(res), std::end(res), std::begin(values), std::end(values)
        );
    }
    {
        // rank-3 tensor left, matrix right
        constexpr gttl::Dimensions<3> dims_res{2_D, 4_D, 2_D};
        using TRes = gttl::Tensor<Scalar, 3, dims_res>;
        // clang-format off
        std::array<Scalar, 2 * 4 * 2> values{
             610,  760,
             700,  880,
             790, 1000,
             880, 1120,

             1690, 2200,
             1780, 2320,
             1870, 2440,
             1960, 2560,
        };
        // clang-format on
        TRes res;
        res = contraction<1, 3>(tensor2, tensor1);
        BOOST_CHECK_EQUAL_COLLECTIONS(
            std::begin(res), std::end(res), std::begin(values), std::end(values)
        );
        res = contraction<1, 3>(tensor2, tensor1);
        BOOST_CHECK_EQUAL_COLLECTIONS(
            std::begin(res), std::end(res), std::begin(values), std::end(values)
        );
    }
}

// NOLINTEND(cppcoreguidelines-avoid-magic_numbers)
