# 39_SelfBalancingTrees

AVL and Red-Black trees: insertion, rotations and full deletion (including the double-black fixup), with a NOTES.md that records how each implementation was verified.

## Files
- `01_avlTree.c` - AVL insertion with height field, balance factor and LL/RR/LR/RL rotations
- `02_redBlackTree.c` - Red-Black insertion and lookup with colour invariants
- `03_avlTreeDeletion.c` - AVL deletion with rebalancing up the path; demo prints in-order and level-order with balance factors
- `04_redBlackTreeDeletion.c` - Red-Black deletion via BST delete plus double-black fixup (CLRS RB-DELETE-FIXUP cases)
- `NOTES.md` - invariants, AVL vs Red-Black trade-offs, why deletion is harder than insertion, and a "Correctness status" section

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_avlTree.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/39_SelfBalancingTrees/` (git-ignored).

## Key concepts / interview angles
- AVL invariant: `|height(left) - height(right)| <= 1`; stricter balance gives faster lookups, more rotations on update.
- Red-Black invariants: root black, no red-red parent/child, equal black-height on every path; guarantees O(log n) with fewer rotations.
- Insertion needs at most one (double) rotation; deletion can cascade rebalancing up to the root.
- Red-Black deletion has four sibling-colour cases (mirrored).
- Where used: `std::map`/`std::set` (red-black), Linux CFS and kernel rbtree.

## Gotchas
- NOTES.md "Correctness status" documents hand-traced demos and a randomized stress test (not checked in) for the deletion code; the files are large (04 is ~650 lines).

## Related
- `../38_BinaryTree`
- `../40_BTreeAndBPlusTree` - disk-oriented alternative
- `../51_SkipList` - probabilistic alternative
- `../../../Cpp/code/27_STLInternals`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
