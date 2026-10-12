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

## Bubble / selection / insertion sort

- Time complexity: all three are O(n^2) average and worst case - each does a
  quadratic number of comparisons (and, for bubble/insertion, a similar order
  of element moves) over the input.
- Stability:
  - Bubble sort: stable. It only swaps adjacent elements, and only when the
    left one is strictly greater, so two equal elements are never swapped
    past each other.
  - Insertion sort: stable, for the same reason - the shift loop only moves
    an element past entries strictly greater than the key, so equal elements
    keep their original relative order.
  - Selection sort: NOT stable. It swaps the current position directly with
    the minimum found anywhere in the remaining unsorted region, which can
    jump an element past equal elements that were between them, breaking
    their original relative order.
- Why they're rarely used in production for large data: O(n^2) is far worse
  than O(n log n) once n is large, so quicksort/mergesort/heapsort (or a
  hybrid like introsort/Timsort) dominate in general-purpose library sorts.
- Exception - insertion sort as a hybrid base case: Timsort and introsort
  both fall back to insertion sort once a subarray shrinks below a small
  threshold (commonly ~16-32 elements). This is because insertion sort has
  very low constant-factor overhead (no recursion, no partitioning/merging
  bookkeeping) and runs close to O(n) on nearly-sorted data - exactly the
  kind of small, partially-ordered runs that show up as the base case of a
  divide-and-conquer sort or as real-world "mostly sorted" input.

## Counting sort and radix sort

- Both are non-comparison sorts: instead of comparing elements pairwise
  (which bounds any comparison sort at Omega(n log n)), they exploit
  knowledge of the key structure to place elements directly.
- Counting sort: counts occurrences of each key in a known bounded range
  [0, k), turns the counts into prefix sums (each key's final position), then
  places elements accordingly - O(n + k) time, O(n + k) extra space, and
  stable if the placement pass runs back-to-front.
- Radix sort: repeatedly counting-sorts by one digit at a time, from least to
  most significant, over d digits - O(d * (n + b)) time where b is the digit
  base, i.e. O(nd) when the base is a small constant. It relies on the
  digit-wise counting sort being stable so that sorting the next (more
  significant) digit doesn't undo the ordering already established by the
  previous digits.
- When these beat O(n log n) comparison sorts: when the key range k (or digit
  count d) is small/bounded relative to n, O(n + k) or O(nd) can beat
  O(n log n) - e.g. sorting exam scores 0-100, or fixed-width integer keys.
- Limitations: they are not general-purpose comparison sorts - they only work
  for integer/discrete keys in a known, bounded range (or that can be broken
  into fixed-width digits), not arbitrary comparable types like strings of
  varying keys or floating-point values without extra encoding. They also
  need O(n + k) extra memory for the count/output arrays, which can be
  wasteful if k is large relative to n.

## More algorithms in this folder
- **Shell sort:** insertion sort over decreasing gaps; the gap sequence decides the complexity (halving O(n^2), Knuth 3h+1 about O(n^1.5), Sedgewick/Ciura better). In place, not stable, no recursion - used in embedded code (uClibc `qsort`).
- **Bucket sort:** scatter into buckets by value range, sort each, concatenate; expected O(n + k) for uniformly distributed input, O(n^2) if everything lands in one bucket. Counting/radix sort are the integer-key relatives (stable, O(n + k) and O(d(n + k))).
- **Simple sorts:** cocktail (bidirectional bubble), gnome (insertion by swapping backwards), comb (bubble with shrinking gap ~1.3 removes 'turtles'), **cycle sort** (minimum number of writes: position = count of smaller items; matters for flash memory), pancake sort (prefix reversals, at most 2n-3 flips). All quadratic except comb (~n log n typical).
- **Sorting networks:** odd-even transposition and **bitonic** sort use a fixed data-independent compare-exchange schedule (n a power of two), so they map to SIMD/GPU/hardware; bitonic is O(n log^2 n) with O(log^2 n) depth.
- **Dutch national flag:** three pointers `lt/i/gt`; elements < pivot go left, > pivot go right, equals stay in the middle; one pass, O(1) space. It is sort-0-1-2 and the partition step of **three-way quick sort** (huge win with many duplicates).
- **Natural merge sort / Timsort:** find existing ascending runs and merge them (a sorted input is a single run: O(n)); Timsort adds minimum run length via insertion sort, merge-order invariants and galloping mode; it is stable and used by Python/Java for objects.
- **Introsort:** quick sort with a depth limit of 2*log2(n) that switches to heap sort (worst case O(n log n)) and leaves ranges <= 16 to insertion sort - the algorithm behind C++ `std::sort`. Pivot by median of three, recurse on the smaller side.
- **Quickselect:** partition like quick sort but continue only into the side containing the k-th element: expected O(n), worst O(n^2) (median-of-medians gives O(n) worst case). Gives the median, top-k, percentiles. `nth_element` in C++.
- **Counting inversions** with merge sort measures how unsorted data is (0 = sorted, n(n-1)/2 = reversed); also Kendall tau distance between rankings. **External sorting:** sort memory-sized chunks into runs, then k-way merge with a min-heap (O(k) memory); the number of passes is log_k(runs).
- **Stability and comparators:** stable sorts keep equal keys in input order (merge, insertion, bubble, counting); `qsort` is not guaranteed stable, so add the original index as the last key. A comparator must be a strict weak ordering (never return `a - b` for values that can overflow). Indirect sorts order indexes or pointers to avoid moving large records.
- **Choosing:** n small or nearly sorted -> insertion; guaranteed O(n log n) and O(1) space -> heap; stability or linked lists -> merge; fastest average in memory -> quick/introsort; small integer keys -> counting/radix; huge data -> external merge; need only top-k -> heap or quickselect.
