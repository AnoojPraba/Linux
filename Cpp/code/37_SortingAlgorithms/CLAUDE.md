# 37_SortingAlgorithms

Sorting in modern C++: quick sort pivot strategies, merge sort and the STL sorting algorithms.

## Files
- `01_quickSortPivots.cpp` - Last/median-of-three/random pivots on sorted input (O(n^2) vs O(n log n)), smaller-half recursion and three-way partitioning for duplicates.
- `02_mergeSort.cpp` - Top-down merge sort with a temporary buffer; stable, O(n log n) always.
- `03_stlSearchAndOrder.cpp` - sort, stable_sort, binary_search, lower_bound/upper_bound, nth_element.
- `04_elementarySorts.cpp` - Bubble, selection and insertion sort as templates, with comparison and swap counts, adaptivity and stability shown.
- `05_cocktailCombGnomeSort.cpp` - Cocktail shaker, comb and gnome sort: simple sorts that improve on bubble sort in different ways.
- `06_stabilityComparatorsAndProjections.cpp` - std::sort versus stable_sort, strict weak ordering rules, multi-key comparators with tuples and C++20 ranges::sort with projections.
- `07_shellSort.cpp` - Shell sort with several gap sequences (Shell, Knuth, Ciura) and a comparison count for each.
- `08_heapSort.cpp` - Heap sort written by hand (sift-down) and with std::make_heap/sort_heap, plus partial_sort for the top k.
- `09_countingRadixBucketSort.cpp` - Non-comparison sorts: counting sort, LSD radix sort and bucket sort for integers and floating-point values.
- `10_partitionAlgorithms.cpp` - Partitioning: std::partition, stable_partition, partition_point and the Dutch national flag with three groups.
- `11_naturalMergeSort.cpp` - Natural merge sort with std::inplace_merge: detect ascending runs and merge neighbours, the principle of Timsort.
- `12_introsort.cpp` - Generic introsort: median-of-three quick sort with a depth limit, heap-sort fallback and insertion sort for small ranges.
- `13_quickselectAndTopK.cpp` - Selection without a full sort: hand-written quickselect, std::nth_element, std::partial_sort and a heap for the top k.
- `14_countInversions.cpp` - Counting inversions with merge sort (O(n log n)) and with a Fenwick tree over compressed values.
- `15_cycleAndPancakeSort.cpp` - Cycle sort (minimum number of writes) and pancake sort (only prefix reversals).
- `16_externalMergeSort.cpp` - External sorting simulated in memory: sort chunk-sized runs, then k-way merge them with a priority_queue.
- `17_sortingComparisonCounts.cpp` - Race of sorting algorithms by counted comparisons on random, sorted and reversed input, using a counting comparator.
- `NOTES.md` - concise interview-focused notes for this topic

## Build and run
- Single file: `g++ -std=c++20 -Wall -Wextra -pthread 01_quickSortPivots.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `Cpp/code/bin/37_SortingAlgorithms/` (git-ignored).

## Key concepts / interview angles
- **Quick sort** = partition around a pivot. Last-element pivot on sorted input is O(n^2) (n^2/2 comparisons); **median of three** fixes sorted/reverse input; a **random pivot** gives expected O(n log n) for every input; **recurse into the smaller half, loop on the larger** keeps stack depth O(log n); **three-way partition** (less/equal/greater) makes many duplicates ~O(n).
- **`std::sort` is introsort:** quick sort that switches to heap sort when depth exceeds ~2 log n (worst case O(n log n)) and insertion sort for tiny ranges. Not stable. `std::stable_sort` is a merge sort (needs a buffer). `list::sort` relinks nodes.
- **Merge sort:** always O(n log n), stable, O(n) extra for arrays, ideal for lists and external data. Use `<=` when merging to keep stability.
- Choosing: merge sort for stability/lists/external; heap sort for O(1) space + worst-case bound; counting/radix for small integer keys; `std::sort` by default.

## Gotchas
- std::sort needs a strict weak ordering; a comparator using <= is undefined behavior.
- A last-element pivot recursion on sorted input of 100k elements overflows the stack.
