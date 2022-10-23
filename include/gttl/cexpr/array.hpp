/**
 * @file
 * @brief collection of `constexpr`functions related to `std::array`
 */

#ifndef GTTL_CEXPR_ARRAY_HPP
#define GTTL_CEXPR_ARRAY_HPP

#include <algorithm>
#include <array>
#include <type_traits>

namespace gttl
{
namespace cexpr
{
namespace array
{

/**
 * @brief Checks if value is contained by array.
 */
template <typename T, std::size_t N>
constexpr bool
contains(const T& value, const std::array<T, N>& arr)
{
    // MAINTENANCE: use ranges
    return std::find(std::begin(arr), std::end(arr), value) != std::end(arr);
}

/**
 * @brief Checks if all values inside the array are larger than zero.
 */
template <typename T, std::size_t N>
constexpr bool
all_strictly_positive(const std::array<T, N>& arr)
{
    // MAINTENANCE: generalize & use prediactes
    bool res = true;
    for (std::size_t i = 0; i < arr.size(); ++i) {
        res = res && (arr[i] > 0);
    }
    return res;
}

/**
 * @brief Checks if all values inside the array are less than bound.
 */
template <typename T, std::size_t N>
constexpr bool
all_less_than(const T& bound, const std::array<T, N>& arr)
{
    // MAINTENANCE: generalize & use prediactes
    bool res = true;
    for (std::size_t i = 0; i < arr.size(); ++i) {
        res = res && (arr[i] < bound);
    }
    return res;
}

/**
 * @brief Checks if all values inside the array are unique.
 * @note Uses `operator==()`.
 */
template <typename T, std::size_t N>
constexpr bool
all_unique(const std::array<T, N>& arr)
{
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = 0; j < N; ++j) {
            if (i != j && arr[i] == arr[j]) {
                return false;
            }
        }
    }
    return true;
}

/**
 * @brief Sums all values inside array.
 * @note Empty array returns zero.
 */
template <typename T, std::size_t N>
constexpr auto
sum(const std::array<T, N>& arr)
{
    // MAINTENANCE: generalize & use fold
    T res{static_cast<T>(0)};
    for (std::size_t i = 0; i < arr.size(); ++i) {
        res += arr[i];
    }
    return res;
}

/**
 * @brief Multiplies all values inside array.
 * @note Empty array returns one.
 */
template <typename T, std::size_t N>
constexpr auto
prod(const std::array<T, N>& arr)
{
    // MAINTENANCE: generalize & use fold
    T res{static_cast<T>(1)};
    for (std::size_t i = 0; i < arr.size(); ++i) {
        res *= arr[i];
    }
    return res;
}

/**
 * @brief Checks if the sub array is identical to the beginning of the array.
 * @note True, when both arrays are equal.
 */
template <typename T, std::size_t SUB_N, std::size_t N>
constexpr bool
is_prefix_of(const std::array<T, SUB_N>& sub_arr, const std::array<T, N>& arr)
{
    if constexpr (SUB_N > N) {
        return false;
    }
    for (std::size_t i{0}; i < SUB_N; ++i) {
        if (sub_arr[i] != arr[i]) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Concatenates two arrays.
 */
template <typename T, std::size_t N1, std::size_t N2>
constexpr std::array<T, N1 + N2>
concatenate(const std::array<T, N1>& arr1, const std::array<T, N2>& arr2)
{
    std::array<T, N1 + N2> res{};
    for (std::size_t i = 0; i < N1; ++i) {
        res[i] = arr1[i];
    }
    for (std::size_t i = 0; i < N2; ++i) {
        res[N1 + i] = arr2[i];
    }
    return res;
}

/**
 * @brief Inserts element at front.
 */
template <typename T, std::size_t N>
constexpr std::array<T, N + 1>
concatenate(const T& x, const std::array<T, N>& arr)
{
    std::array<T, N + 1> res{};
    res[0] = x;
    for (std::size_t i = 0; i < N; ++i) {
        res[i + 1] = arr[i];
    }
    return res;
}

/**
 * @brief Inserts element at back.
 */
template <typename T, std::size_t N>
constexpr std::array<T, N + 1>
concatenate(const std::array<T, N>& arr, const T& x)
{
    std::array<T, N + 1> res{};
    res[N] = x;
    for (std::size_t i = 0; i < N; ++i) {
        res[i] = arr[i];
    }
    return res;
}

/**
 * @brief Front element.
 */
template <typename T, std::size_t N>
constexpr const T&
first(const std::array<T, N>& arr) requires(N > 0)
{
    return arr[0];
}

/**
 * @brief All except the front element.
 */
template <typename T, std::size_t N>
constexpr std::array<T, N - 1>
rest(const std::array<T, N>& arr) requires(N > 0)
{
    std::array<T, N - 1> res{};
    for (std::size_t i = 0; i < N - 1; ++i) {
        res[i] = arr[i + 1];
    }
    return res;
}

/**
 * @brief Back element.
 */
template <typename T, std::size_t N>
constexpr const T&
last(const std::array<T, N>& arr) requires(N > 0)
{
    return arr[N - 1];
}

/**
 * @brief All except the back element.
 */
template <typename T, std::size_t N>
constexpr std::array<T, N - 1>
most(const std::array<T, N>& arr) requires(N > 0)
{
    std::array<T, N - 1> res{};
    for (std::size_t i = 0; i < N - 1; ++i) {
        res[i] = arr[i];
    }
    return res;
}

/**
 * @brief Prefix array with elements of given amount.
 */
template <std::size_t AMOUNT, typename T, std::size_t N>
constexpr std::array<T, AMOUNT>
take(const std::array<T, N>& arr) requires(N >= AMOUNT)
{
    std::array<T, AMOUNT> res{};
    for (std::size_t i = 0; i < AMOUNT; ++i) {
        res[i] = arr[i];
    }
    return res;
}

/**
 * @brief Suffix array missing elements of given amount.
 */
template <std::size_t AMOUNT, typename T, std::size_t N>
constexpr std::array<T, N - AMOUNT>
drop(const std::array<T, N>& arr) requires(N >= AMOUNT)
{
    std::array<T, N - AMOUNT> res{};
    for (std::size_t i = 0; i < N - AMOUNT; ++i) {
        res[i] = arr[i + AMOUNT];
    }
    return res;
}

/**
 * @brief Insert an element at given position.
 * @note `arr[POSITION]`becomes `x`.
 */
template <std::size_t POSITION, typename T, std::size_t N>
constexpr std::array<T, N + 1>
insert_at(const T& x, const std::array<T, N>& arr) requires(POSITION <= N)
{
    std::array<T, N + 1> res{};
    res[POSITION] = x;
    for (std::size_t i = 0; i < N; ++i) {
        res[i + (i >= POSITION ? 1 : 0)] = arr[i];
    }
    return res;
}

/**
 * @brief Insert multiple elements at given positions.
 * @see insert_at
 * @note For all `i` `arr[POSITION[i]]`becomes `xs[i]`.
 */
template <
    std::size_t INSERT_SIZE,
    std::array<std::size_t, INSERT_SIZE> POSITIONS,
    typename T,
    std::size_t N>
constexpr std::array<T, N + INSERT_SIZE>
multi_insert_at(
    const std::array<T, INSERT_SIZE>& xs, const std::array<T, N>& arr
) requires(all_less_than(N + 1, POSITIONS))
{
    // MAINTENANCE: simplify/refactor algorithm

    // ZIP

    // array of tuples of position, meta position value (= position in xs)
    std::array<std::tuple<std::size_t, std::size_t, T>, INSERT_SIZE> sorted_xs;
    // zip positions and values into array
    for (std::size_t i{0}; i < INSERT_SIZE; ++i) {
        sorted_xs[i] = {POSITIONS[i], i, xs[i]};
    }

    // STABLE SORT

    // stable sort by position
    // note: actual std::stable_sort is non-constexpr! (C++20)
    //       This is the reason for the meta postion value.
    constexpr auto less = [](const auto& tx, const auto& ty) {
        const auto& [posx, ix, x] = tx;
        const auto& [posy, iy, y] = ty;
        if (posx == posy) {
            return ix < iy;
        } else {
            return posx < posy;
        }
    };
    std::sort(std::begin(sorted_xs), std::end(sorted_xs), less);

    // COPY REMAINING ELEMENTS

    std::array<T, N + INSERT_SIZE> res;
    for (std::size_t i = 0, i_arr = 0, i_sorted_xs = 0; i < N + INSERT_SIZE;
         ++i) {
        if (i_sorted_xs < INSERT_SIZE) {
            const auto& [pos, _, x] = sorted_xs[i_sorted_xs];
            if (pos != i_arr) {
                res[i] = arr[i_arr++];
            } else {
                res[i] = x;
                ++i_sorted_xs;
            }
        } else {
            res[i] = arr[i_arr++];
        }
    }

    return res;
}

/**
 * @brief Erase elements at given position.
 */
template <std::size_t POSITION, typename T, std::size_t N>
constexpr std::array<T, N - 1>
erase_at(const std::array<T, N>& arr) requires(N > 0)
{
    std::array<T, N - 1> res{};
    for (std::size_t i = 0; i < N - 1; ++i) {
        res[i] = arr[i + (i < POSITION ? 0 : 1)];
    }
    return res;
}

/**
 * @brief Erase multiple elements at given positions.
 * @see erase_at
 */
template <
    std::size_t ERASE_SIZE,
    std::array<std::size_t, ERASE_SIZE> POSITIONS,
    typename T,
    std::size_t N>
constexpr std::array<T, N - ERASE_SIZE>
multi_erase_at(const std::array<T, N>& arr) requires(
    (ERASE_SIZE <= N) && all_unique(POSITIONS) && all_less_than(N, POSITIONS)
)
{
    std::array<T, N - ERASE_SIZE> res;
    for (std::size_t i = 0, j = 0; i < N; ++i) {
        if (!contains(i, POSITIONS)) {
            res[j++] = arr[i];
        }
    }
    return res;
}

} // namespace array
} // namespace cexpr
} // namespace gttl

#endif
