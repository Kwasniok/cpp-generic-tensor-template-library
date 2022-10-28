
/**
 * @file
 * @brief tensor (advanced operations)
 */

#ifndef GTTL_TENSOR_ADVANCED_PP
#define GTTL_TENSOR_ADVANCED_PP

#include "basic.hpp"

namespace gttl
{

/*
 * @brief outer product
 */
template <
    typename Scalar,
    std::size_t RANK1,
    std::size_t RANK2,
    std::array<std::size_t, RANK1> DIMENSIONS1,
    std::array<std::size_t, RANK2> DIMENSIONS2>
[[nodiscard]] constexpr Tensor<
    Scalar,
    RANK1 + RANK2,
    cexpr::array::concatenate(DIMENSIONS1, DIMENSIONS2)>
outer_product(
    const Tensor<Scalar, RANK1, DIMENSIONS1>& x,
    const Tensor<Scalar, RANK2, DIMENSIONS2>& y
)
{
    constexpr auto PROD_RANK{RANK1 + RANK2};
    constexpr auto PROD_DIMENSIONS{
        cexpr::array::concatenate(DIMENSIONS1, DIMENSIONS2)};
    using Tensor1 = Tensor<Scalar, RANK1, DIMENSIONS1>;
    using Tensor2 = Tensor<Scalar, RANK2, DIMENSIONS2>;
    using ProdTensor = Tensor<Scalar, PROD_RANK, PROD_DIMENSIONS>;
    using ProdSubTensor = Tensor2;
    // note: Requires that coefficients are the only non-static
    // data member.
    static_assert(
        std::is_standard_layout_v<ProdTensor>,
        "Tensor must be of standard layout."
    );
    static_assert(
        sizeof(ProdTensor) == sizeof(ProdTensor::coefficients),
        "`coefficients`must be only non-static data member of Tensor"
    );
    static_assert(
        std::is_standard_layout_v<ProdSubTensor>,
        "SubTensor must be of standard layout."
    );
    static_assert(
        sizeof(ProdSubTensor) == sizeof(ProdSubTensor::coefficients),
        "`coefficients`must be only non-static data member of SubTensor"
    );

    // loop over elements in x
    //    copy y and multilpy by coefficient from x
    // note: Iterating primarily over the memory index is more compiler-firendly
    //       Tested with gcc 12.2 and clang 14.0 both with -O3
    ProdTensor res; // no initialization required

    // reinterpret cast is checked above
    // NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
    ProdSubTensor* it = reinterpret_cast<ProdSubTensor*>(&res);
    // NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)
    for (std::size_t i{0}; i < Tensor1::size; ++it, ++i) {
        // note: Don't bother using std::memcpy (no difference for gcc/clang)
        *it = y;
        *it *= x.coefficients[i];
    }

    return res;
}

/*
 * @brief tensor contraction
 */
template <
    std::size_t i,
    std::size_t j,
    typename Scalar,
    std::size_t RANK,
    std::array<std::size_t, RANK> DIMENSIONS,
    typename Traits>
[[nodiscard]] constexpr auto
contraction(const Tensor<Scalar, RANK, DIMENSIONS, Traits>& x) requires(
    (RANK >= 2) && (i < RANK) && (j < RANK) && (i != j) &&
    (DIMENSIONS[i] == DIMENSIONS[j])
)
{
    constexpr auto CONTRACTED_RANK{RANK - 2};
    constexpr auto CONTRACTED_DIMENSIONS{
        cexpr::array::multi_erase_at<2, std::array<std::size_t, 2>{i, j}>(
            DIMENSIONS
        )};
    using ThisTensor = Tensor<Scalar, RANK, DIMENSIONS, Traits>;
    using ContractedTensor =
        Tensor<Scalar, CONTRACTED_RANK, CONTRACTED_DIMENSIONS, Traits>;

    // loop over this tensor in memory order and store coefficients in
    // result
    // note: Iterating primarily over the memory index is more compiler-firendly
    //       Tested with gcc 12.2 and clang 14.0 both with -O3
    ContractedTensor res{}; // zero-initialization required!
    for (std::size_t coeff_index{0}; coeff_index < ThisTensor::size;
         ++coeff_index) {
        auto multi_index{ThisTensor::get_multi_index_for_index(coeff_index)};
        if (multi_index.template get<i>() == multi_index.template get<j>()) {
            // calculate destination
            const auto mi_res = multi_index.template multi_erase_at<
                2,
                std::array<std::size_t, 2>{i, j}>();
            // store
            res.at(mi_res) += x.coefficients[coeff_index];
        }
    }

    return res;
}

/*
 * @brief two-tensor conctraction
 * @note Same as contraction of an outer product but more resource efficient.
 */
template <
    std::size_t i,
    std::size_t j,
    typename Scalar,
    std::size_t RANK1,
    std::size_t RANK2,
    Dimensions<RANK1> DIMENSIONS1,
    Dimensions<RANK2> DIMENSIONS2>
[[nodiscard]] constexpr auto
// clang-format off
contraction(const Tensor<Scalar, RANK1, DIMENSIONS1>& x,
    const Tensor<Scalar, RANK2, DIMENSIONS2>& y)
    // clang-format on
    requires(
        (i != j) && (RANK1 + RANK2 >= 2) && (i < (RANK1 + RANK2)) &&
        (j < (RANK1 + RANK2)) &&
        (cexpr::array::concatenate(DIMENSIONS1, DIMENSIONS2)[i] ==
         cexpr::array::concatenate(DIMENSIONS1, DIMENSIONS2)[j])
    )
{
    // there are 4 cases:
    //     i,j both in x
    //     i,j both in y
    //     i in y and j in x
    //     i in x and j in y  <- only 'new' case

    if constexpr (i < RANK1 && j < RANK1) {
        return outer_product(contraction<i, j>(x), y);
    } else if constexpr (i >= RANK1 && j >= RANK1) {
        return outer_product(x, contraction<i - RANK1, j - RANK1>(y));
    } else if constexpr (i > j) {
        return contraction<j, i>(x, y);
    } else {
        // from above: i must be in x and j in y
        using Tensor1 = Tensor<Scalar, RANK1, DIMENSIONS1>;
        using Tensor2 = Tensor<Scalar, RANK2, DIMENSIONS2>;
        using ResTensor = Tensor<
            Scalar,
            RANK1 + RANK2 - 2,
            cexpr::array::multi_erase_at<2, std::array<std::size_t, 2>{i, j}>(
                cexpr::array::concatenate(DIMENSIONS1, DIMENSIONS2)
            )>;

        // note: Iterating primarily over the memory index is more
        //       compiler-firendly
        // loop over all elements in x
        //     loop over all elements in y
        //         if both elements contribute to a product in the result
        //             add product of both elements to element in result
        ResTensor res{}; // zero-initialization required!
        for (std::size_t a{0}; a < Tensor1::size; ++a) {
            const auto mi1 = Tensor1::get_multi_index_for_index(a);
            for (std::size_t b{0}; b < Tensor2::size; ++b) {
                const auto mi2 = Tensor2::get_multi_index_for_index(b);
                if (mi1.template get<i>() == mi2.template get<j - RANK1>()) {
                    const auto mi_both = mi1.concatenate(mi2);
                    const auto mi_res = mi_both.template multi_erase_at<
                        2,
                        std::array<std::size_t, 2>{i, j}>();
                    res.at(mi_res) += x.at(mi1) * y.at(mi2);
                }
            }
        }
        return res;
    }
}

} // namespace gttl

#endif
