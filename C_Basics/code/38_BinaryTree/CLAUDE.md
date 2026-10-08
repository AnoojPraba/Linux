# 38_BinaryTree

Binary search trees and the classic tree interview problems: insert/search, traversals, level order, deletion, LCA, diameter, invert, balanced/symmetric checks.

## Files
- `01_bstInsertSearch.c` - BST invariant; insert returns the possibly new root
- `02_treeTraversals.c` - in/pre/post-order DFS (in-order yields ascending values)
- `03_heightAndLevelOrder.c` - tree height and BFS level order using an explicit queue
- `04_bstDeletion.c` - three deletion cases (leaf, one child, two children via in-order successor)
- `05_lowestCommonAncestor.c` - LCA in a BST by walking down using ordering; also general-tree variant
- `06_diameter.c` - height-based diameter in one pass
- `07_invertTree.c` - recursively swap children
- `08_balancedAndSymmetric.c` - single-pass height-balanced check and mirror (symmetric) check
- `NOTES.md` - BST deletion cases, LCA, diameter, invert, balanced and symmetric checks

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_bstInsertSearch.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/38_BinaryTree/` (git-ignored).

## Key concepts / interview angles
- BST ops are O(h): O(log n) when balanced, O(n) on sorted input; see `../39_SelfBalancingTrees`.
- Deleting a node with two children: replace with the in-order successor (leftmost of right subtree) then delete that node.
- In-order traversal of a BST is sorted; level order needs a queue, DFS uses the call stack (depth = recursion risk).
- Balanced check in O(n): return height or -1 on imbalance instead of recomputing heights.
- Diameter = max over nodes of left height + right height.

## Related
- `../39_SelfBalancingTrees`
- `../32_StackAndQueue` - queue for BFS
- `../41_Heap`
- `../44_Trie`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
