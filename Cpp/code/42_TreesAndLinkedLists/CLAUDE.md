# 42_TreesAndLinkedLists

Binary search tree with unique_ptr, AVL rotations and Floyd's linked-list cycle detection.

## Files
- `01_bstUniquePtr.cpp` - BST insert/search/traversals/height; unique_ptr children free the whole tree automatically.
- `02_avlTree.cpp` - AVL insertion with the four rotation cases; 1000 sorted keys give height 10.
- `03_listCycleAndMiddle.cpp` - Slow/fast pointers: middle of a linked list and cycle detection.
- `NOTES.md` - concise interview-focused notes for this topic

## Build and run
- Single file: `g++ -std=c++20 -Wall -Wextra -pthread 01_bstUniquePtr.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `Cpp/code/bin/42_TreesAndLinkedLists/` (git-ignored).

## Key concepts / interview angles
- **BST:** left < node < right; O(h); in-order = sorted; degenerates to O(n) on sorted input. `std::map`/`set` are balanced trees (usually red-black). Owning children with `unique_ptr` frees the tree automatically, but destroying a very deep tree recursively can overflow the stack.
- **AVL:** balance factor in {-1,0,1}, height ~1.44 log2 n. After insert: LL -> right rotation, RR -> left rotation, LR -> rotate child left then node right, RL mirror. Rotations are O(1) and keep in-order order. Red-black rotates less on updates, B-trees minimize disk reads.
- **Floyd:** slow +1, fast +2; they meet iff there is a cycle; when fast hits the end, slow is the middle. O(n) time, O(1) space.

## Gotchas
- Destroying a very deep unique_ptr tree recursively can overflow the stack.
