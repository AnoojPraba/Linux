# SearchingAlgorithms

- **`lower_bound`** = first element >= value, **`upper_bound`** = first element > value, difference = count of equals; need sorted data; O(log n) on random-access iterators, O(n) iterator steps on a `list`. `nth_element` = quickselect, O(n) average. `binary_search` only answers present/absent.
- **Search variants** (sorted, indexable): jump O(sqrt n), exponential O(log i) (unbounded/streams), interpolation O(log log n) uniform but O(n) skewed. Midpoint `lo + (hi - lo) / 2`.

## More algorithms in this folder
- **Binary search family:** `lower_bound` (first `>= key`), `upper_bound` (first `> key`), `equal_range` (both), `binary_search` (bool), `partition_point` (any monotone predicate). Count = `ub - lb`; insert position = `lower_bound`; floor/ceiling from the two bounds. Midpoint: `lo + (hi - lo) / 2` or `std::midpoint` (C++20).
- **STL requirements:** the range must be sorted/partitioned by the same ordering; O(log n) steps on any iterator but O(log n) *comparisons* with O(n) iterator movement on `list`; sets/maps have member `lower_bound`/`find`/`contains` that use the tree.
- **Linear search:** `find`, `find_if`, `count_if`, `any_of`/`all_of`/`none_of`; sentinel (one comparison per element); self-organizing (move-to-front with `list::splice`, transpose) for skewed access; works on forward-only data with no ordering.
- **Ternary/Fibonacci:** ternary search finds the extremum of a **unimodal** function (discard a third per step; real or integer domain); not useful for sorted arrays. Fibonacci search splits at Fibonacci numbers (additions only, good locality).
- **Rotated arrays:** compare `a[mid]` with `a[hi]` for the minimum; to search decide which half is sorted. Duplicates degrade the worst case to O(n). **Peak/mountain** and single-element-in-pairs searches binary-search on a property, not a value.
- **2D:** staircase from the top-right corner O(R+C) for row/column-sorted; a fully sorted matrix is one sorted sequence (index `mid / cols`, `mid % cols`).
- **Binary search on the answer:** `first_true(lo, hi, ok)` for a monotone feasibility predicate: isqrt, minimum speed/capacity, split array largest sum, aggressive cows; cost O(log range * check).
- **Other searches:** `search`/`find_end`/`find_first_of`/`adjacent_find`/`mismatch`/`includes`; exponential search for unbounded or streamed sequences (gallop then binary search, O(log i)).
