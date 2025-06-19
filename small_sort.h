#ifndef SMALL_SORT_H_INCLUDED
#define SMALL_SORT_H_INCLUDED
#include "sort_aux.h"
SORTER_BEGIN
// Branchless swap; compiler likely generates CMOV to avoid branching penalties.
struct conditional_swap_fn
{
    template <class Iter, class Compare>
    CONSTEXPR_CPP20 inline void
    operator()(Iter a, Iter b, Compare& comp) const
    {
        typedef typename std::iterator_traits<Iter>::value_type value_type;
        bool comp_result = comp(*a, *b);
        value_type tmp = comp_result ? std::move(*a) : std::move(*b);
        *b  = comp_result ? std::move(*b) : std::move(*a);
        *a  = std::move(tmp);
    }
};

struct reverse_conditional_swap_fn
{
    template <class Iter, class Compare>
    CONSTEXPR_CPP20 inline void
    operator()(Iter a, Iter b, Compare& comp) const
    {
        typedef typename std::iterator_traits<Iter>::value_type value_type;
        bool comp_result = comp(*a, *b);
        value_type tmp = comp_result ? std::move(*b) : std::move(*a);
        *b  = comp_result ? std::move(*a) : std::move(*b);
        *a  = std::move(tmp);
    }
};

INLINE_VAR constexpr conditional_swap_fn conditional_swap{};
INLINE_VAR constexpr reverse_conditional_swap_fn reverse_conditional_swap{};

// optimal sorting network for small array

template <class Compare,
          class ConditionalSwap,
          class RandomAccessIterator>
CONSTEXPR_CPP20 inline void
sort2_optimal(RandomAccessIterator x0,
              RandomAccessIterator x1,
              Compare& comp,
              const ConditionalSwap& cond_swap)
{ cond_swap(x0, x1, comp); }

template <class Compare,
          class ConditionalSwap,
          class RandomAccessIterator>
CONSTEXPR_CPP20 void
sort3_optimal(RandomAccessIterator x0,
              RandomAccessIterator x1,
              RandomAccessIterator x2,
              Compare& comp,
              const ConditionalSwap& cond_swap)
{
    cond_swap(x1, x2, comp);
    cond_swap(x0, x2, comp);
    cond_swap(x0, x1, comp);
}

template <class Compare,
          class ConditionalSwap,
          class RandomAccessIterator>
CONSTEXPR_CPP20 void
sort4_optimal(RandomAccessIterator x0,
              RandomAccessIterator x1,
              RandomAccessIterator x2,
              RandomAccessIterator x3,
              Compare& comp,
              const ConditionalSwap& cond_swap)
{
    cond_swap(x0, x1, comp);
    cond_swap(x2, x3, comp);
    cond_swap(x0, x2, comp);
    cond_swap(x1, x3, comp);
    cond_swap(x1, x2, comp);
}

template <class Compare,
          class ConditionalSwap,
          class RandomAccessIterator>
CONSTEXPR_CPP20 void
sort5_optimal(RandomAccessIterator x0,
              RandomAccessIterator x1,
              RandomAccessIterator x2,
              RandomAccessIterator x3,
              RandomAccessIterator x4,
              Compare& comp,
              const ConditionalSwap& cond_swap)
{
    cond_swap(x0, x1, comp);
    cond_swap(x3, x4, comp);
    cond_swap(x2, x4, comp);
    cond_swap(x2, x3, comp);
    cond_swap(x0, x3, comp);
    cond_swap(x1, x4, comp);
    cond_swap(x0, x2, comp);
    cond_swap(x1, x3, comp);
    cond_swap(x1, x2, comp);
}

template <class Compare,
          class ConditionalSwap,
          class RandomAccessIterator>
CONSTEXPR_CPP20 void
sort6_optimal(RandomAccessIterator x0,
              RandomAccessIterator x1,
              RandomAccessIterator x2,
              RandomAccessIterator x3,
              RandomAccessIterator x4,
              RandomAccessIterator x5,
              Compare& comp,
              const ConditionalSwap& cond_swap)
{
    cond_swap(x1, x2, comp);
    cond_swap(x4, x5, comp);
    cond_swap(x0, x2, comp);
    cond_swap(x3, x5, comp);
    cond_swap(x0, x1, comp);
    cond_swap(x3, x4, comp);
    cond_swap(x0, x3, comp);
    cond_swap(x1, x4, comp);
    cond_swap(x2, x5, comp);
    cond_swap(x2, x4, comp);
    cond_swap(x1, x3, comp);
    cond_swap(x2, x3, comp);
}

template <class Compare,
          class ConditionalSwap,
          class RandomAccessIterator>
CONSTEXPR_CPP20 void
sort7_optimal(RandomAccessIterator x0,
              RandomAccessIterator x1,
              RandomAccessIterator x2,
              RandomAccessIterator x3,
              RandomAccessIterator x4,
              RandomAccessIterator x5,
              RandomAccessIterator x6,
              Compare& comp,
              const ConditionalSwap& cond_swap)
{
    cond_swap(x1, x2, comp);
    cond_swap(x3, x4, comp);
    cond_swap(x5, x6, comp);
    cond_swap(x0, x2, comp);
    cond_swap(x3, x5, comp);
    cond_swap(x4, x6, comp);
    cond_swap(x0, x1, comp);
    cond_swap(x4, x5, comp);
    cond_swap(x0, x4, comp);
    cond_swap(x1, x5, comp);
    cond_swap(x2, x6, comp);
    cond_swap(x0, x3, comp);
    cond_swap(x2, x5, comp);
    cond_swap(x1, x3, comp);
    cond_swap(x2, x4, comp);
    cond_swap(x2, x3, comp);
}

template <class Compare,
          class ConditionalSwap,
          class RandomAccessIterator>
CONSTEXPR_CPP20 void
sort8_optimal(RandomAccessIterator x0,
              RandomAccessIterator x1,
              RandomAccessIterator x2,
              RandomAccessIterator x3,
              RandomAccessIterator x4,
              RandomAccessIterator x5,
              RandomAccessIterator x6,
              RandomAccessIterator x7,
              Compare& comp,
              const ConditionalSwap& cond_swap)
{
    cond_swap(x0, x1, comp);
    cond_swap(x2, x3, comp);
    cond_swap(x4, x5, comp);
    cond_swap(x6, x7, comp);
    cond_swap(x0, x2, comp);
    cond_swap(x1, x3, comp);
    cond_swap(x4, x6, comp);
    cond_swap(x5, x7, comp);
    cond_swap(x1, x2, comp);
    cond_swap(x5, x6, comp);
    cond_swap(x0, x4, comp);
    cond_swap(x1, x5, comp);
    cond_swap(x2, x6, comp);
    cond_swap(x3, x7, comp);
    cond_swap(x1, x4, comp);
    cond_swap(x3, x6, comp);
    cond_swap(x2, x4, comp);
    cond_swap(x3, x5, comp);
    cond_swap(x3, x4, comp);
}

template <class Compare,
          class BidirectionalIterator>
CONSTEXPR_CPP20 void
insertion_sort(BidirectionalIterator first,
               BidirectionalIterator last,
               Compare& comp)
{
	if (first != last)
	{
		for (BidirectionalIterator mid = first; ++mid != last;)
		{
			BidirectionalIterator hole = mid;
			typename std::iterator_traits<BidirectionalIterator>::value_type
			val(std::move(*mid));
			if (comp(val, *first)) // found new earliest element, move to front
			{
				std::move_backward(first, mid, ++hole);
				*first = std::move(val);
			}
			else // look for insertion point after first
			{
				for (BidirectionalIterator sift = hole; comp(val, *--sift); hole = sift)
					*hole = std::move(*sift); // move hole down
				*hole = std::move(val);
			}
		}
	}
}

template <class Compare,
          class BidirectionalIterator>
CONSTEXPR_CPP20 inline void
insert_tail(BidirectionalIterator first, BidirectionalIterator tail, Compare& comp)
{
    typedef typename std::iterator_traits<BidirectionalIterator>::value_type value_type;
    BidirectionalIterator sift = tail;
    if (!comp(*tail, *--sift))
        return;
    value_type tmp (std::move(*tail));
    for (;;)
    {
        *tail = std::move(*sift);
        tail = sift;
        if (sift == first || !comp(tmp, *--sift))
            break;
    }
    *tail = std::move(tmp);
}

struct construct
{
    template <class Tp>
    static inline void op(Tp& obj, Tp&& other) // initialize obj
    noexcept(noexcept(::new(static_cast<void*>(std::addressof(obj))) Tp(std::forward<Tp>(other))))
    { ::new(static_cast<void*>(std::addressof(obj))) Tp(std::forward<Tp>(other)); }
};

struct move_assign
{
    template <class Tp>
    static inline void op(Tp& obj, Tp&& other) // move other to obj
    noexcept(noexcept(obj = std::move(other)))
    { obj = std::move(other); }
};

template <class Compare,
          class ConditionalSwap,
          class RandomAccessIterator>
CONSTEXPR_CPP20 inline void
sort1to8(RandomAccessIterator first,
         RandomAccessIterator last,
         Compare& comp,
         const ConditionalSwap& cond_swap)
{
    typedef typename std::iterator_traits<RandomAccessIterator>::difference_type difference_type;
    const difference_type len = last - first;
    switch (len)
    {
    case 0:
    case 1:
        return;
    case 2:
        sort2_optimal(first, --last, comp, cond_swap);
        return;
    case 3:
        sort3_optimal(first, first + 1, --last, comp, cond_swap);
        return;
    case 4:
        sort4_optimal(first, first + 1, first + 2, --last, comp, cond_swap);
        return;
    case 5:
        sort5_optimal(first, first + 1, first + 2, first + 3, --last, comp, cond_swap);
        return;
    case 6:
        sort6_optimal(first, first + 1, first + 2, first + 3, first + 4, --last, comp, cond_swap);
        return;
    case 7:
        sort7_optimal(first, first + 1, first + 2, first + 3, first + 4, first + 5, --last, comp, cond_swap);
        return;
    case 8:
        sort8_optimal(first, first + 1, first + 2, first + 3, first + 4, first + 5, first + 6, --last, comp, cond_swap);
        return;
    }
    // ignore
}

template <class Compare,
          class RandomAccessIterator>
CONSTEXPR_CPP20 void
enforce_order(RandomAccessIterator first,
              RandomAccessIterator last,
              Compare& comp)
{
    typedef typename std::iterator_traits<RandomAccessIterator>::difference_type difference_type;
    RandomAccessIterator iter = first;
    while (last - iter >= BITONIC_BATCH)
    {
        sort8_optimal(iter, iter + 1, iter + 2, iter + 3,
                      iter + 4, iter + 5, iter + 6, iter + 7, comp, conditional_swap);
        iter += static_cast<difference_type>(BATCH);
        sort8_optimal(iter, iter + 1, iter + 2, iter + 3,
                      iter + 4, iter + 5, iter + 6, iter + 7, comp, reverse_conditional_swap);
        iter += static_cast<difference_type>(BATCH);
    }
    if (last - iter >= BATCH)
    {
        sort8_optimal(iter, iter + 1, iter + 2, iter + 3,
                      iter + 4, iter + 5, iter + 6, iter + 7, comp, conditional_swap);
        iter += static_cast<difference_type>(BATCH);
        sort1to8(iter, last, comp, reverse_conditional_swap);
    }
    else
        sort1to8(iter, last, comp, conditional_swap);
}

template <class OpPolicy,
          class Compare,
          class InputIterator,
          class OutputIterator>
CONSTEXPR_CPP20 void
forward_merge(InputIterator first,
              InputIterator last,
              OutputIterator dest,
              Compare& comp)
{
    typedef typename std::iterator_traits<InputIterator>::difference_type difference_type;
    --last;
    difference_type len = last - first;
    for (; len > 0; --len)
    {
        if (comp(*first, *last))
        {
            OpPolicy::op(*dest, std::move(*first));
            ++first;
        }
        else
        {
            OpPolicy::op(*dest, std::move(*last));
            --last;
        }
        ++dest;
    }
    *dest = std::move(*first);
}

template <class OpPolicy,
          class Compare,
          class InputIterator,
          class OutputIterator>
CONSTEXPR_CPP20 void
backward_merge(InputIterator first,
               InputIterator last,
               OutputIterator dest,
               Compare& comp)
{
    typedef typename std::iterator_traits<InputIterator>::difference_type difference_type;
    --last;
    difference_type len = last - first;
    dest += len;
    for (; len > 0; --len)
    {
        if (comp(*first, *last))
        {
            OpPolicy::op(*dest, std::move(*first));
            ++first;
        }
        else
        {
            OpPolicy::op(*dest, std::move(*last));
            --last;
        }
        --dest;
    }
    *dest = std::move(*first);
}

template <class Compare,
          class RandomAccessIterator>
CONSTEXPR_CPP20 inline void
small_sort_network(RandomAccessIterator first,
                   RandomAccessIterator last,
                   Compare& comp)
{
    typedef typename std::iterator_traits<RandomAccessIterator>::difference_type difference_type;
    typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
    difference_type len = last - first;
    if (len <= BATCH)
    {
        sort1to8(first, last, comp, conditional_swap);
        return;
    }
    else
    {
        value_type temp_buf[SMALL_SORT_NETWORK_SCRATCH_LEN]; // uninitialized
        enforce_order(first, last, comp);
        if (len <= BITONIC_BATCH)
        {
            forward_merge<construct>(first, last, temp_buf, comp);
            std::move(temp_buf, temp_buf + len, first);
        }
        else
        {
            forward_merge<construct>(first, first + BITONIC_BATCH, temp_buf, comp);
            backward_merge<construct>(first + BITONIC_BATCH, last, temp_buf + BITONIC_BATCH, comp);
            forward_merge<move_assign>(temp_buf, temp_buf + len, first, comp);
        }
        std::destroy_n(temp_buf, len); // destroy temp_buf[0...len]
    }
}

template <class Compare,
          class InputIterator,
          class OutputIterator>
OutputIterator
merge_move(InputIterator first,
           const InputIterator mid,
           const InputIterator last,
           OutputIterator dest, Compare& comp)
{
    InputIterator next = mid;
    for (;;)
    {
        if (comp(*next, *first))
        {
            *dest = std::move(*next);
            ++dest;
            ++next;
            if (next == last)
                return std::move(first, mid, dest);
        }
        else
        {
            *dest = std::move(*first);
            ++dest;
            ++first;
            if (first == mid)
                return std::move(next, last, dest);
        }
    }
}

template <class Iter>
CONSTEXPR_CPP20 __forceinline Iter&
select(bool cond, Iter& a, Iter& b) noexcept // select iterator by cond
{ return cond ? a : b; }

template <class Compare,
          class RandomAccessIterator,
          class ValueType = typename std::iterator_traits<RandomAccessIterator>::value_type>
CONSTEXPR_CPP20 void
sort4_stable(RandomAccessIterator first,
             ValueType* dst,
             Compare& comp)
{
    typedef typename std::iterator_traits<RandomAccessIterator>::difference_type difference_type;
	RandomAccessIterator a = first;
	RandomAccessIterator b = first;
	RandomAccessIterator c = first + difference_type(2);
	RandomAccessIterator d = c;
	bool comp1 = comp(*next_iter(first), *first);
	bool comp2 = comp(*next_iter(c), *c);

    // create a <= b and c <= d
    a += difference_type(comp1);
	b += difference_type(!comp1);
	c += difference_type(comp2);
	d += difference_type(!comp2);

	// Compare(a, c) and (b, d) to identify max / min.We're left with two
	// unknown elements, but because we are a stable sort we must know which
	// one is leftmost and which one is rightmost.
	// c3, c4 | min max unknown_left unknown_right
	//  0,  0 |  a   d      b          c
	//  0,  1 |  a   b      c          d
	//  1,  0 |  c   d      a          b
	//  1,  1 |  c   b      a          d
	bool comp3 = comp(*c, *a);
	bool comp4 = comp(*d, *b);
	RandomAccessIterator mn = select(comp3, c, a);
	RandomAccessIterator mx = select(comp4, b, d);
	RandomAccessIterator unknown_left  = select(comp3, a, select(comp4, c, b));
	RandomAccessIterator unknown_right = select(comp4, d, select(comp3, b, c));

	// sort the last two elements
	bool comp5 = comp(*unknown_right, *unknown_left);
	RandomAccessIterator lo = select(comp5, unknown_right, unknown_left);
	RandomAccessIterator hi = select(comp5, unknown_left, unknown_right);

	// always moves each element exactly once
	std::construct_at(dst, std::move(*mn)); ++dst;
	std::construct_at(dst, std::move(*lo)); ++dst;
	std::construct_at(dst, std::move(*hi)); ++dst;
	std::construct_at(dst, std::move(*mx));
}

template <class Compare,
          class RandomAccessIterator,
          class ValueType = typename std::iterator_traits<RandomAccessIterator>::value_type>
CONSTEXPR_CPP20 inline void
sort8_stable(RandomAccessIterator first,
             ValueType* scratch_base,
             ValueType* dst,
             Compare& comp)
{
	sort4_stable(first, scratch_base, comp);
	sort4_stable(first + 4, scratch_base + 4, comp);
    // scratch_base[0...8] is now initialized, allowing us to merge back to dst
	merge_move(scratch_base, scratch_base + 4, scratch_base + 8, dst, comp);
}

template <class Compare,
          class RandomAccessIterator>
CONSTEXPR_CPP20 void
small_sort_general(RandomAccessIterator first,
                   RandomAccessIterator last,
                   Compare& comp)
{
    typedef typename std::iterator_traits<RandomAccessIterator>::difference_type difference_type;
    typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;

    const difference_type len = last - first;
    if (len < 2)
        return;

    __builtin_assume(SMALL_SORT_GENERAL_SCRATCH_LEN >= len + 16);
    value_type temp_buf[SMALL_SORT_GENERAL_SCRATCH_LEN]; // uninitialized
    const difference_type half    = len >> 1;
    difference_type presorted_len = 1;

    if (sizeof(value_type) <= 16 && len >= 16)
    {
        sort8_stable(first, temp_buf + len, temp_buf, comp);
        sort8_stable(first + half, temp_buf + (len + 8), temp_buf + half, comp);
        std::destroy_n(temp_buf + len, 16); // destroy temp_buf[len...(len+16)]
        presorted_len = 8;
    }
    else if (len >= 8)
    {
        sort4_stable(first, temp_buf, comp);
        sort4_stable(first + half, temp_buf + half, comp);
        presorted_len = 4;
    }
    else
    {
        std::construct_at(temp_buf, std::move(*first));
        std::construct_at(temp_buf + half, std::move(*(first + half)));
    }

    for (difference_type offset : {difference_type(0), half})
    {
        RandomAccessIterator src = first + offset;
        value_type* dst = temp_buf + offset;
        difference_type desired_len = offset == 0 ? half : len - half;
        // temp_buf is initialized with presorted_len elements
        for (difference_type i = presorted_len; i < desired_len; ++i)
        {
            std::construct_at(dst + i, std::move(*(src + i))); // construct new element
            insert_tail(dst, dst + i, comp); // move the element to the right place
        }
    }
    // temp_buf[0...len] is now initialized, allowing us to merge back to first
    merge_move(temp_buf, temp_buf + half, temp_buf + len, first, comp);
    std::destroy_n(temp_buf, len); // destroy temp_buf[0...len]
}

template <class Compare,
          class RandomAccessIterator>
CONSTEXPR_CPP20 inline void
small_sort(RandomAccessIterator first,
           RandomAccessIterator last,
           Compare& comp)
{
    typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
    if constexpr (use_sorting_network<RandomAccessIterator, Compare>) // for small and trivial types
        small_sort_network(first, last, comp);
    else if constexpr (sizeof(value_type) * SMALL_SORT_GENERAL_SCRATCH_LEN <= MAX_STACK_SIZE) // for median types
        small_sort_general(first, last, comp);
    else // if the 'value_type' is very large, fall back to insertionsort
        insertion_sort(first, last, comp);
}
SORTER_END
#endif // SMALL_SORT_H_INCLUDED
