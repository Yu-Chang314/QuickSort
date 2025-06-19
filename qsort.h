#pragma once
#include "small_sort.h"

SORTER_BEGIN
template <class Compare,
          class RandomAccessIterator,
          class DistanceType = typename std::iterator_traits<RandomAccessIterator>::difference_type>
CONSTEXPR_CPP20 void
sift_down(RandomAccessIterator first,
          RandomAccessIterator last,
          Compare& comp,
          DistanceType node)
{
    typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
    const DistanceType len = last - first;
    __builtin_assume(node < len);
    value_type tmp(std::move(*(first + node)));
    for (;;)
    {
        DistanceType child = (node << 1);
        if (++child >= len)
            break;
        child += static_cast<DistanceType>(child + 1 < len
                     ? comp(*(first + child), *(first + (child + 1))) : 0);
        if (!comp(tmp, *(first + child)))
            break;
        *(first + node) = std::move(*(first + child));
        node = child;
    }
    *(first + node) = std::move(tmp);
}

template <class Compare,
          class RandomAccessIterator>
CONSTEXPR_CPP20 void
heap_sort(RandomAccessIterator first,
          RandomAccessIterator last,
          Compare& comp)
{
    typedef typename std::iterator_traits<RandomAccessIterator>::difference_type difference_type;
    const difference_type len = last - first;
    __builtin_assume(len >= 2);
    for (difference_type idx = len + (len >> 1); idx > 0; --idx)
    {
        difference_type sift_idx = idx >= len ? idx - len : (std::iter_swap(first, first + idx), 0);
        sift_down(first, first + std::min(idx, len), comp, sift_idx);
    }
}

template <class Compare,
          class RandomAccessIterator>
CONSTEXPR_CPP20 __forceinline RandomAccessIterator
median_of_three(RandomAccessIterator a,
                RandomAccessIterator b,
                RandomAccessIterator c,
                Compare& comp)
{
    bool x = comp(*a, *b);
    bool y = comp(*a, *c);
    // If x=y=0 then b, c <= a. In this case we want to return max(b, c).
    // If x=y=1 then a < b, c. In this case we want to return min(b, c).
    // By toggling the outcome of b < c using XOR x we get this behavior.
    if (x == y)
    {
        bool z = comp(*b, *c);
        return (z ^ x) ? c : b;
    }
    return a;
}

template <class Compare,
          class RandomAccessIterator,
          class DistanceType = typename std::iterator_traits<RandomAccessIterator>::difference_type>
CONSTEXPR_CPP20 RandomAccessIterator
median_of_three_recursive(RandomAccessIterator a,
                          RandomAccessIterator b,
                          RandomAccessIterator c,
                          Compare& comp,
                          DistanceType step)
{
    if ((step << 3) >= PSEUDO_MEDIAN_REC_THRESHOLD)
    {
        const DistanceType next_step   = step >> 3;
        const DistanceType four_steps  = next_step << 2;
        const DistanceType seven_steps = next_step * 7;
        a = median_of_three_recursive(a, a + four_steps, a + seven_steps, comp, next_step);
        b = median_of_three_recursive(b, b + four_steps, b + seven_steps, comp, next_step);
        c = median_of_three_recursive(c, c + four_steps, c + seven_steps, comp, next_step);
    }
    return median_of_three(a, b, c, comp);
}

template <class Compare,
          class RandomAccessIterator>
CONSTEXPR_CPP20 inline void
choose_pivot(RandomAccessIterator first,
             RandomAccessIterator last,
             Compare& comp)
{
    typedef typename std::iterator_traits<RandomAccessIterator>::difference_type difference_type;
    const difference_type len  = last - first;
    const difference_type step = len >> 3;
    RandomAccessIterator mid = len < PSEUDO_MEDIAN_REC_THRESHOLD
        ? median_of_three(first, first + (step << 2), first + step * 7, comp)
        : median_of_three_recursive(first, first + (step << 2), first + step * 7, comp, step);
    std::iter_swap(first, mid);
}

template <class Compare,
          class RandomAccessIterator>
CONSTEXPR_CPP20 RandomAccessIterator
hoare_branchy_cyclic(RandomAccessIterator first,
                     RandomAccessIterator last,
                     Compare& comp)
{
    typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
    value_type pivot(std::move(*first));
    RandomAccessIterator begin = first;
    while (++first != last && comp(*first, pivot));
    while (first != last && !comp(*--last, pivot));
    if (first < last)
    {
        value_type tmp(std::move(*first));
        do
        {
            *first = std::move(*last);
            // always safe here, but need to find the correct position for tmp
            while (comp(*++first, pivot) && first < last);
            if (!(first < last))
                break;
            *last = std::move(*first);
            // safe because last will eventually be overwritten
            while (!comp(*--last, pivot));
        } while (first < last);
        *first = std::move(tmp);
    }
    *begin = std::move(*--first);
    *first = std::move(pivot);
    return first;
}

template <class RandomAccessIterator>
CONSTEXPR_CPP20 inline void
swap_bitmap_cyclic(RandomAccessIterator first,
                RandomAccessIterator last,
                uint64_t& left_bitset,
                uint64_t& right_bitset)
{
    typedef typename std::iterator_traits<RandomAccessIterator>::difference_type difference_type;
    typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;

    if (left_bitset == 0 || right_bitset == 0)
        return;

    difference_type tz_left = count_tail_zero(left_bitset);
    left_bitset = clear_lowest_bit(left_bitset);

    difference_type tz_right = count_tail_zero(right_bitset);
    right_bitset = clear_lowest_bit(right_bitset);

    RandomAccessIterator l = first + tz_left;
    RandomAccessIterator r = last - tz_right;

    value_type tmp(std::move(*l));
    *l = std::move(*r);

    while (left_bitset != 0 && right_bitset != 0)
    {
        tz_left = count_tail_zero(left_bitset);
        left_bitset = clear_lowest_bit(left_bitset);
        tz_right = count_tail_zero(right_bitset);
        right_bitset = clear_lowest_bit(right_bitset);
        l = first + tz_left;
        *r = std::move(*l);
        r = last - tz_right;
        *l = std::move(*r);
    }
    *r = std::move(tmp);
}

template <class Compare,
          class RandomAccessIterator,
          class ValueType = typename std::iterator_traits<RandomAccessIterator>::value_type>
CONSTEXPR_CPP20 inline void
populate_left_bitset(RandomAccessIterator iter,
                     Compare& comp,
                     ValueType& pivot,
                     uint64_t& left_bitset)
{
    for (int j = 0; j < BLOCK_SIZE;)
    {
        bool comp_result = !comp(*iter, pivot);
        left_bitset |= (static_cast<uint64_t>(comp_result) << j);
        ++j;
        ++iter;
    }
}

template <class Compare,
          class RandomAccessIterator,
          class ValueType = typename std::iterator_traits<RandomAccessIterator>::value_type>
CONSTEXPR_CPP20 inline void
populate_right_bitset(RandomAccessIterator iter,
                      Compare& comp,
                      ValueType& pivot,
                      uint64_t& right_bitset)
{
    for (int j = 0; j < BLOCK_SIZE;)
    {
        bool comp_result = comp(*iter, pivot);
        right_bitset |= (static_cast<uint64_t>(comp_result) << j);
        ++j;
        --iter;
    }
}

template <class Compare,
          class RandomAccessIterator,
          class ValueType = typename std::iterator_traits<RandomAccessIterator>::value_type>
CONSTEXPR_CPP20 inline void
bitset_partition_partial_blocks(RandomAccessIterator& first,
                                RandomAccessIterator& lm1,
                                Compare& comp,
                                ValueType& pivot,
                                uint64_t& left_bitset,
                                uint64_t& right_bitset)
{
    typedef typename std::iterator_traits<RandomAccessIterator>::difference_type difference_type;
    difference_type remaining_len = lm1 - first + 1;
    difference_type l_size, r_size;

    if (left_bitset == 0 && right_bitset == 0)
    {
        l_size = remaining_len >> 1;
        r_size = remaining_len - l_size;
    }
    else if (left_bitset == 0)
    {
        l_size = remaining_len - BLOCK_SIZE;
        r_size = BLOCK_SIZE;
    }
    else
    {
        l_size = BLOCK_SIZE;
        r_size = remaining_len - BLOCK_SIZE;
    }

    if (left_bitset == 0)
    {
        RandomAccessIterator iter = first;
        for (int j = 0; j < l_size; ++j)
        {
            bool comp_result = !comp(*iter, pivot);
            left_bitset |= (static_cast<uint64_t>(comp_result) << j);
            ++iter;
        }
    }

    if (right_bitset == 0)
    {
        RandomAccessIterator iter = lm1;
        for (int j = 0; j < r_size; ++j)
        {
            bool comp_result = comp(*iter, pivot);
            right_bitset |= (static_cast<uint64_t>(comp_result) << j);
            --iter;
        }
    }

    swap_bitmap_cyclic(first, lm1, left_bitset, right_bitset);
    first += (left_bitset == 0) ? l_size : difference_type(0);
    lm1 -= (right_bitset == 0) ? r_size : difference_type(0);
}

template <class RandomAccessIterator>
CONSTEXPR_CPP20 inline void
swap_bitmap_pos_within(RandomAccessIterator& first,
                       RandomAccessIterator& lm1,
                       uint64_t& left_bitset,
                       uint64_t& right_bitset)
{
    typedef typename std::iterator_traits<RandomAccessIterator>::difference_type difference_type;
    if (left_bitset)
    {
        while (left_bitset != 0)
        {
            difference_type tz_left = BLOCK_SIZE - 1 - count_left_zero(left_bitset);
            left_bitset &= (static_cast<uint64_t>(1) << tz_left) - 1;
            std::iter_swap(first + tz_left, lm1);
            --lm1;
        }
        first = next_iter(lm1);
    }
    else if (right_bitset)
    {
        while (right_bitset != 0)
        {
            difference_type tz_right = BLOCK_SIZE - 1 - count_left_zero(right_bitset);
            right_bitset &= (static_cast<uint64_t>(1) << tz_right) - 1;
            std::iter_swap(lm1 - tz_right, first);
            ++first;
        }
    }
}

template <class Compare,
          class RandomAccessIterator>
CONSTEXPR_CPP20 RandomAccessIterator
bitset_partition(RandomAccessIterator first,
                 RandomAccessIterator last,
                 Compare& comp)
{
    typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
    typedef typename std::iterator_traits<RandomAccessIterator>::difference_type difference_type;
    RandomAccessIterator begin = first;
    value_type pivot(std::move(*first));
    while (++first < last && comp(*first, pivot));
    while (first < last && !comp(*--last, pivot));
    if (first < last) // Is [first, last) already partitioned?
    {
        std::iter_swap(first, last); // Swap first pair we find above.
        ++first;
        RandomAccessIterator lm1 = last;
        --lm1;
        // 'last' is not inclusive in [first, last). From now on, it uses last minus one
        // to be inclusive both side.
        uint64_t left_bitset  = 0;
        uint64_t right_bitset = 0;
        while (lm1 - first >= 2 * BLOCK_SIZE - 1)
        {
            // Record the comparison outcomes for the elements currently on the left side.
            if (left_bitset == 0)
                populate_left_bitset(first, comp, pivot, left_bitset);
            // Record the comparison outcomes for the elements currently on the right side.
            if (right_bitset == 0)
                populate_right_bitset(lm1, comp, pivot, right_bitset);
             // Swap the elements recorded to be the candidates for swapping in the bitsets.
            swap_bitmap_cyclic(first, lm1, left_bitset, right_bitset);
            first += (left_bitset == 0) ? difference_type(BLOCK_SIZE) : difference_type(0);
            lm1 -= (right_bitset == 0) ? difference_type(BLOCK_SIZE) : difference_type(0);
        }
        // Now, we have a less-than a block worth of elements on at least one of the sides.
        bitset_partition_partial_blocks(first, lm1, comp, pivot, left_bitset, right_bitset);
        // At least one the bitsets would be empty.  For the non-empty one, we need to
        // properly partition the elements that appear within that bitset.
        swap_bitmap_pos_within(first, lm1, left_bitset, right_bitset);
    }
    // Move the pivot to the right space.
    *begin = std::move(*--first);
    *first = std::move(pivot);
    return first;
}

template <class Compare,
          class RandomAccessIterator>
CONSTEXPR_CPP20
inline RandomAccessIterator
partition_by_choosed_pivot(RandomAccessIterator first,
                           RandomAccessIterator last,
                           Compare&& comp)
{
    if constexpr (use_branchless_sort<RandomAccessIterator, Compare>)
        return bitset_partition(first, last, comp);
    else
        return hoare_branchy_cyclic(first, last, comp);
}

template <class Compare>
struct reverse_predicate
{
    Compare& comp;
    template <class Tp1, class Tp2>
    [[nodiscard]] constexpr auto
    operator()(Tp1&& left, Tp2&& right) const
    { return !comp(right, left); }
};

template <class Compare,
          class RandomAccessIterator>
CONSTEXPR_CPP20 inline void
quick_sort(RandomAccessIterator first,
           RandomAccessIterator last,
           Compare& comp,
           typename std::iterator_traits<RandomAccessIterator>::difference_type depth_limit,
           typename std::iterator_traits<RandomAccessIterator>::pointer ancestor_pivot = nullptr)
{
    for (;;)
    {
        // smallsort is faster for small array.
        if (last - first <= SSORT_MAX)
        {
             small_sort(first, last, comp);
            return;
        }

        // if too many bad pivots choices we made, simply fall back to heapsort
        // to guarantee O(nlogn) worst case.
        if (depth_limit == 0)
        {
            heap_sort(first, last, comp);
            return;
        }

        --depth_limit; // allow 2log2(n) divisions.

        // calculate the approximate median of 3 elements by median of 3 or
        // recursively from an approximation of each, if they're large enough.
        // this algorithm is taken from glidesort by Orson Peters.
        choose_pivot(first, last, comp);

        // if the chosen pivot is equal to the predecessor, we change the strategy,
        // putting the equal elements in the left partition, greater elements in
        // the right partition.
        if (ancestor_pivot && !comp(*ancestor_pivot, *first))
        {
            first = partition_by_choosed_pivot(first, last, reverse_predicate{comp});
            ancestor_pivot = nullptr;
            ++first;
            continue;
        }

        RandomAccessIterator mid = partition_by_choosed_pivot(first, last, comp);
        __builtin_assume(mid < last);
        // sort the left partition first using recursion and do tail recursion elimination for
        // the right-hand partition.
        quick_sort(first, mid, comp, depth_limit, ancestor_pivot);
        ancestor_pivot = std::to_address(mid);
        first = ++mid;
    }
}

template <class Compare,
          class RandomAccessIterator>
CONSTEXPR_CPP20 inline std::pair<RandomAccessIterator, bool>
find_existing_run(RandomAccessIterator first,
                  RandomAccessIterator last,
                  Compare& comp)
{
    if (last - first < 2) // 0 or 1 element is sorted
        return std::pair<RandomAccessIterator, bool>(last, false);
    RandomAccessIterator mid = first;
    bool is_strictly_descending = comp(*++mid, *first);
    ++mid;
    if (is_strictly_descending)
        while (mid != last && comp(*mid, *prev_iter(mid)))
            ++mid;
    else
        while (mid != last && !comp(*mid, *prev_iter(mid)))
			++mid;
    return std::pair<RandomAccessIterator, bool>(mid, is_strictly_descending);
}

template <class Tp>
inline constexpr Tp log2i(Tp val) noexcept
{
#if __cplusplus >= 201703L
    return std::bit_width(std::make_unsigned_t<Tp>(val)) - 1;
#else
    const int sz = sizeof(+val);
    int w = sz * __CHAR_BIT__ - 1;
    if (sz == sizeof(long long))
        w -= __builtin_clzll(+val);
    else if (sz == sizeof(long))
        w -= __builtin_clzl(+val);
    else if (sz == sizeof(int))
        w -= __builtin_clz(+val);
    return w;
#endif
}

template <class RandomAccessIterator, class Compare>
CONSTEXPR_CPP20 inline void
qsort(const RandomAccessIterator first,
	  const RandomAccessIterator last,
	  Compare comp)
{
    const auto [mid, descending] = find_existing_run(first, last, comp);

    if (mid == last) // strictly ascending ==> no operation
    {
        if (descending) // strictly descending ==> reverse
            std::reverse(first, last);
        return;
    }
    else if (mid - first >= last - mid) // first half are sorted, sort last half and merge them
    {
        if (descending)
            std::reverse(first, mid);
        quick_sort(mid, last, comp, log2i(last - mid) << 1);
        std::inplace_merge(first, mid, last, comp);
        return;
     }
     quick_sort(first, last, comp, log2i(last - first) << 1);
}

template <class RandomAccessIterator>
CONSTEXPR_CPP20 inline void
qsort(const RandomAccessIterator first,
      const RandomAccessIterator last)
{
    typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
    qsort(first, last, std::less<value_type>{});
}
SORTER_END


