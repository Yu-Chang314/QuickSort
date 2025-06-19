#ifndef SORT_AUX_H_INCLUDED
#define SORT_AUX_H_INCLUDED

#include <algorithm>
#include <type_traits>
#include <bit>
#include <cstddef>
#define SORTER_BEGIN namespace sorter {
#define SORTER_END }

#if __cplusplus > 201703L
#define CONSTEXPR_CPP20 constexpr
#else
#define CONSTEXPR_CPP20
#endif // C++20

#if __cplusplus > 201402L
#define INLINE_VAR inline
#else
#define INLINE_VAR
#endif //C++17

SORTER_BEGIN
INLINE_VAR constexpr size_t MAX_STACK_SIZE = 4096; // default to ~1 page
INLINE_VAR constexpr int BATCH = 8;
INLINE_VAR constexpr int BITONIC_BATCH = 16;
INLINE_VAR constexpr int SSORT_MAX  = 32;
INLINE_VAR constexpr int BLOCK_SIZE = 64;
INLINE_VAR constexpr int PSEUDO_MEDIAN_REC_THRESHOLD = 64;
INLINE_VAR constexpr int SMALL_SORT_GENERAL_SCRATCH_LEN = 48;
INLINE_VAR constexpr int SMALL_SORT_NETWORK_SCRATCH_LEN = 32;
// INLINE_VAR constexpr size_t MAX_BRANCHLESS_SIZE = 96;

template <class Tp>
struct is_simple_comparator : std::false_type {};
template <class Tp>
struct is_simple_comparator<std::less<Tp>> : std::true_type {};
template <class Tp>
struct is_simple_comparator<std::greater<Tp>> : std::true_type {};
#if __cplusplus > 201703L
template <>
struct is_simple_comparator<std::ranges::less> : std::true_type {};
template <>
struct is_simple_comparator<std::ranges::greater> : std::true_type {};
#endif // C++20

template <class Iter,
          class Compare,
          class Tp = typename std::iterator_traits<Iter>::value_type>
constexpr bool use_branchless_sort = std::is_trivially_copyable<Tp>::value &&
                                     std::is_arithmetic<Tp>::value &&
                                     is_simple_comparator<typename std::remove_cvref<Compare>::type>::value;

template <class Iter,
          class Compare,
          class Tp = typename std::iterator_traits<Iter>::value_type>
constexpr bool use_sorting_network = std::is_trivially_copy_constructible<Tp>::value &&
                                     std::is_trivially_copy_assignable<Tp>::value &&
                                     sizeof(Tp) <= 4 * sizeof(size_t) &&
                                     is_simple_comparator<typename std::remove_cvref<Compare>::type>::value;

[[nodiscard]] CONSTEXPR_CPP20 __forceinline
unsigned clear_lowest_bit(unsigned x) noexcept { return x & (x - 1); }

[[nodiscard]] CONSTEXPR_CPP20 __forceinline
unsigned long clear_lowest_bit(unsigned long x) noexcept { return x & (x - 1); }

[[nodiscard]] CONSTEXPR_CPP20 __forceinline
unsigned long long clear_lowest_bit(unsigned long long x) noexcept { return x & (x - 1); }

[[nodiscard]] CONSTEXPR_CPP20 __forceinline
int count_tail_zero(unsigned x) noexcept { return __builtin_ctz(x); }

[[nodiscard]] CONSTEXPR_CPP20 __forceinline
int count_tail_zero(unsigned long x) noexcept { return __builtin_ctzl(x); }

[[nodiscard]] CONSTEXPR_CPP20 __forceinline
int count_tail_zero(unsigned long long x) noexcept { return __builtin_ctzll(x); }

[[nodiscard]] CONSTEXPR_CPP20 __forceinline
int count_left_zero(unsigned x) noexcept { return __builtin_clz(x); }

[[nodiscard]] CONSTEXPR_CPP20 __forceinline
int count_left_zero(unsigned long x) noexcept { return __builtin_clzl(x); }

[[nodiscard]] CONSTEXPR_CPP20 __forceinline
int count_left_zero(unsigned long long x) noexcept { return __builtin_clzll(x); }

template <class BidirectionalIterator>
CONSTEXPR_CPP20 inline
BidirectionalIterator
prev_iter(BidirectionalIterator first)
{ return --first; }

template <class ForwardIterator>
CONSTEXPR_CPP20 inline
ForwardIterator
next_iter(ForwardIterator first)
{ return ++first; }
SORTER_END
#endif // SORT_AUX_H_INCLUDED
