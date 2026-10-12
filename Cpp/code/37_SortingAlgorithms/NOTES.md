# SortingAlgorithms

- **Quick sort** = partition around a pivot. Last-element pivot on sorted input is O(n^2) (n^2/2 comparisons); **median of three** fixes sorted/reverse input; a **random pivot** gives expected O(n log n) for every input; **recurse into the smaller half, loop on the larger** keeps stack depth O(log n); **three-way partition** (less/equal/greater) makes many duplicates ~O(n).
- **`std::sort` is introsort:** quick sort that switches to heap sort when depth exceeds ~2 log n (worst case O(n log n)) and insertion sort for tiny ranges. Not stable. `std::stable_sort` is a merge sort (needs a buffer). `list::sort` relinks nodes.
- **Merge sort:** always O(n log n), stable, O(n) extra for arrays, ideal for lists and external data. Use `<=` when merging to keep stability.
- Choosing: merge sort for stability/lists/external; heap sort for O(1) space + worst-case bound; counting/radix for small integer keys; `std::sort` by default.

## More algorithms in this folder
- **Elementary sorts:** bubble (stops early if a pass swaps nothing: O(n) on sorted data), selection (at most n-1 swaps, good when writes are expensive, not stable), insertion (O(n) when nearly sorted, stable, the building block of Timsort/introsort for small ranges).
- **Shell sort:** gapped insertion sort; the gap sequence matters (n/2 halving O(n^2), Knuth 3h+1 ~O(n^1.5), Ciura empirical best). In place, not stable.
- **Heap sort:** build a max-heap in O(n), repeatedly swap the root to the end and sift down; O(n log n) always, O(1) space, not stable, cache-unfriendly. `make_heap`/`sort_heap`; `partial_sort` for the k smallest in O(n log k).
- **Non-comparison sorts:** counting O(n + k), **LSD radix** (stable counting pass per digit/byte), bucket sort expected O(n) for uniform data. They beat the n log n lower bound only because keys are restricted.
- **Cycle/pancake/comb/gnome/cocktail:** curiosities with interview value: cycle sort minimizes writes, pancake sorts with prefix reversals (at most 2n-3), comb shrinks the gap by ~1.3 (removes 'turtles'), cocktail is bidirectional bubble.
- **Natural merge / Timsort:** merge existing ascending runs (`inplace_merge`); sorted input is one run (O(n)). Timsort adds minimum-run insertion sort, run-stack invariants and galloping; stable; used by Python and Java object sorts.
- **Introsort:** quick sort + depth limit 2*log2 n -> heap sort fallback + insertion sort below ~16 elements; this is `std::sort` (O(n log n) worst case, not stable). `stable_sort` is a merge sort needing a buffer.
- **Partitioning:** `std::partition` (unordered, fast), `stable_partition` (keeps order inside groups), `partition_point` (binary search on a partitioned range), Dutch national flag (three groups in one pass).
- **Selection:** quickselect/`nth_element` expected O(n); `partial_sort` O(n log k); a size-k min-heap for streams. **Inversions:** merge sort counts them in O(n log n); Fenwick tree over compressed ranks also works; measures how unsorted data is.
- **External sorting:** sort memory-sized runs, then k-way merge with a heap holding one head per run; passes = log_k(runs).
- **Comparators:** must be a strict weak ordering (`comp(a,a)` false); `<=` or inconsistent comparators are undefined behaviour in `std::sort`. Use `std::tie` for multi-key comparisons and C++20 `ranges::sort` with projections (`&Student::age`). Stability only from `stable_sort` or by adding the original index as a key.
