# TreesAndLinkedLists

- **BST:** left < node < right; O(h); in-order = sorted; degenerates to O(n) on sorted input. `std::map`/`set` are balanced trees (usually red-black). Owning children with `unique_ptr` frees the tree automatically, but destroying a very deep tree recursively can overflow the stack.
- **AVL:** balance factor in {-1,0,1}, height ~1.44 log2 n. After insert: LL -> right rotation, RR -> left rotation, LR -> rotate child left then node right, RL mirror. Rotations are O(1) and keep in-order order. Red-black rotates less on updates, B-trees minimize disk reads.
- **Floyd:** slow +1, fast +2; they meet iff there is a cycle; when fast hits the end, slow is the middle. O(n) time, O(1) space.
