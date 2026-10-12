# Search Algorithms

## Ternary search

Ternary search (`04_ternarySearch.c`) splits the search range into three
parts instead of two, but it isn't a generally better sorted-array search
than binary search. Its real use case is finding the maximum or minimum of a
unimodal function (one that strictly increases then strictly decreases, or
vice versa), where comparing two interior points tells you which third of the
range can't contain the extremum.

For plain sorted-array lookup, binary search is still generally preferred:
ternary search does 2 comparisons per level (to evaluate both `mid1` and
`mid2`) versus binary search's 1 comparison per level, and splitting into 3
parts instead of 2 doesn't make up for that - the extra comparisons roughly
cancel out the smaller per-level range reduction, giving no real practical
advantage over binary search.

## More algorithms in this folder
- **Binary search family:** `lower_bound` (first `a[i] >= key`) and `upper_bound` (first `> key`) with half-open `[lo, hi)`; count = `ub - lb`; first/last occurrence; insert position; floor/ceiling. Always `mid = lo + (hi - lo) / 2`. Loop invariants matter: decide whether the answer lies in `[lo, hi]` or `[lo, hi)` and keep to it.
- **Fibonacci search:** like binary search but splits at Fibonacci numbers using only additions/subtractions and touches nearby elements first (better cache/seek behaviour on some media); O(log n).
- **Rotated sorted array:** find the minimum by comparing `a[mid]` with `a[hi]`; search by deciding which half is sorted and whether the key lies in it. Duplicates break the O(log n) guarantee (`hi--` on ties, worst case O(n)).
- **No explicit target:** peak element (compare `a[mid]` with `a[mid+1]`, move uphill), mountain array peak, single non-duplicate in a sorted array of pairs (check pair alignment by index parity). The predicate only has to be monotone.
- **2D search:** rows and columns sorted -> start at the top-right corner, go left if too big, down if too small, O(R + C); a fully sorted (row-continuing) matrix is one sorted array of R*C elements indexed `mid / C, mid % C`.
- **Binary search on the answer:** when 'is X feasible?' is monotone in X, search X: integer sqrt (compare `mid <= x / mid`), bisection root finding (sign change), minimum speed/capacity/time problems (koko bananas, ship packages, split array largest sum) - O(log range * cost of the check).
- **Linear-search variants:** sentinel (drop the bounds test), self-organizing lists (move-to-front, transpose, count) help skewed access patterns; for repeated lookups use sorting + binary search, a hash table or a tree instead.
