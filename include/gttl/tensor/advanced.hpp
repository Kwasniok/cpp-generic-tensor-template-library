
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
            auto multi_index_res = multi_index.template multi_erase_at<
                2,
                std::array<std::size_t, 2>{i, j}>();
            // store
            res.at(multi_index_res) += x.coefficients[coeff_index];
        }
    }

    return res;
}

} // namespace gttl

#endif
