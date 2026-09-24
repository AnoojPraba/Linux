# Sorting

- Time complexity (average / worst):
  - Quicksort: O(n log n) average, O(n^2) worst (already-sorted input with a
    bad pivot choice, e.g. always picking the last element).
  - Merge sort: O(n log n) average and worst - the split/merge structure
    doesn't depend on input order.
  - Heap sort: O(n log n) average and worst - same guarantee as merge sort,
    without the extra memory.
- Stability (equal elements keep their relative order):
  - Merge sort: stable (the merge step takes from the left run on ties).
  - Quicksort: not stable (partitioning swaps elements across the pivot).
  - Heap sort: not stable (sift-down swaps break relative order).
- In-place vs extra memory:
  - Quicksort: in-place, O(log n) stack space for recursion.
  - Merge sort: needs O(n) extra buffer for the merge step (or O(n) with
    linked lists, where merging can be done pointer-only).
  - Heap sort: in-place, O(1) extra space (heap is built directly in the
    array).
- When each is preferred in practice:
  - Quicksort: fastest average case in practice, low constant factors and
    good cache locality; the basis of most stdlib sorts (`qsort`, C++
    `std::sort`) once combined with introsort-style worst-case fallback and
    small-array insertion sort.
  - Merge sort: guaranteed O(n log n) worst case and stable, so it's the
    right choice for linked lists (no random access needed, merge is just
    pointer rewiring) and external sorting (merging sorted chunks that don't
    fit in memory).
  - Heap sort: guaranteed O(n log n) worst case with O(1) extra space, useful
    when memory is tight and worst-case guarantees matter more than average
    speed - but worse cache behavior and higher constants than quicksort due
    to the non-local heap access pattern.
