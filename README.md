# C++ QuickSort

> **Note:** This project was out of personal interest, thus containing astoundingly immature implementation.

## Complexity

- **O(N log N)** worst-case comparisons.  
- **O(N)** comparisons for fully ascending or descending inputs.  
- **O(N log K)** comparisons when sorting **K** unique values. 

## Techniques

### Partition

Two partitioning strategies are used:

- A [bitsetpartition](https://github.com/minjaehwang/bitsetsort) for arithmetic types.  
- A branchy ~Hoare-style partition~ [fulcrum_partition](https://github.com/scandum/crumsort?tab=readme-ov-file) for large or expensive-to-move types.  

### Pivot Selection

- Recursive median selection using √N sampling, based on [glidesort](https://github.com/orlp/glidesort) by Orson Peters.

### Small Sort  
#### Trivial types:  

 - For N <= 8,  use a sorting networks with conditional moves.   
 - For N > 8 and N <= 32, use Bitonic Order Merge Sort for faster sorting.

#### Nontrivial and median-sized types:

- Use a stable sorting network combined with insertion sort.
- The implementation in [small_sort_general](https://github.com/Voultapher/sort-research-rs/tree/main/ipnsort) is adapted from ipnsort,  
designed for nontrivial types that still fit comfortably in stack-allocated scratch space.

#### Large or complex types:
- If the type is too large for stack allocation, fallback to insertion sort.

