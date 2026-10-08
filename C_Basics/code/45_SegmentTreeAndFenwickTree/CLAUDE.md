# 45_SegmentTreeAndFenwickTree

Range-query structures: a range-sum segment tree and a Fenwick (binary indexed) tree, both O(log n) per query/update.

## Files
- `01_segmentTree.c` - recursive build, range sum query and point update on an array-backed tree
- `02_fenwickTree.c` - 1-indexed BIT with prefix sums via lowbit
- `NOTES.md` - why O(log n) matters, Fenwick simplicity vs segment-tree flexibility

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_segmentTree.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/45_SegmentTreeAndFenwickTree/` (git-ignored).

## Key concepts / interview angles
- Prefix sums give O(1) query but O(n) update; these trade for O(log n) both.
- Fenwick: smaller and faster, but best for invertible operations (sum); `i & -i` isolates the lowest set bit.
- Segment tree supports min/max/gcd and lazy propagation for range updates.

## Related
- `../41_Heap`
- `../38_BinaryTree`
- `../46_DynamicProgramming`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
