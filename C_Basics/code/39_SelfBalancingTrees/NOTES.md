# Self-Balancing Trees

- Plain BSTs (`38_BinaryTree`) degrade to O(n) on sorted/near-sorted input:
  inserting keys in increasing order builds a right-leaning chain with no
  branching, so search/insert/delete become linear scans instead of
  O(log n).
- AVL balance-factor invariant: for every node, `height(left) - height(right)`
  stays within `[-1, 1]`. Whenever an insertion pushes a node's balance
  factor outside that range, a rotation restores it, keeping the tree height
  O(log n).
- AVL rotations (named by the shape of the imbalance):
  - LL (left-left heavy): single right rotation.
  - RR (right-right heavy): single left rotation.
  - LR (left-right heavy): left rotation on the left child, then right
    rotation on the node.
  - RL (right-left heavy): right rotation on the right child, then left
    rotation on the node.
- AVL vs Red-Black tradeoffs: AVL trees are more rigidly balanced (stricter
  height bound), giving faster lookups, but do more rotation work on
  insert/delete. Red-Black trees are more loosely balanced (allow up to ~2x
  height), which means fewer rotations on average, making them a common
  choice for write-heavy structures (e.g. Linux kernel scheduler, C++
  `std::map`/`std::set`).
- Red-Black coloring rules/invariants, now backed by code in
  `02_redBlackTree.c`:
  - Every node is colored red or black.
  - The root is always black.
  - Red nodes cannot have red children (no two reds in a row on any path).
  - Every root-to-null-leaf path has the same number of black nodes
    ("black-height"), which is what bounds the tree's height.
- `02_redBlackTree.c` implements insertion (BST insert of a red leaf,
  followed by `fixupInsert`'s standard uncle-color case analysis: a red
  uncle triggers recoloring and pushes the fixup up toward the root; a
  black uncle triggers a rotation - with an extra rotation first for the
  "triangle"/LR-RL shapes - followed by recoloring) and `find(root, key)`,
  a plain BST search unaffected by color. `main()` exercises LL, RR, LR,
  and RL imbalance shapes plus both red-uncle and black-uncle recoloring
  cases, and prints the in-order traversal with each node's color to verify
  both sortedness and the coloring invariants.
- Lookup in a red-black tree is O(log n) worst-case, guaranteed by the same
  kind of black-height bound that AVL's stricter height bound provides -
  both structures give the same asymptotic search cost, they just differ
  in how much rebalancing work insertion/deletion do to maintain it (see
  the tradeoff note below).
- AVL vs Red-Black tradeoffs: AVL trees are more rigidly balanced (stricter
  height bound), giving faster lookups, but do more rotation work on
  insert/delete. Red-Black trees are more loosely balanced (allow up to ~2x
  height), which means fewer rotations on average, making them a common
  choice for write-heavy structures (e.g. Linux kernel scheduler, C++
  `std::map`/`std::set`).
- Red-Black deletion is intentionally omitted here: it is notoriously
  intricate, with many more rebalancing cases than insertion (recoloring
  plus rotations depending on sibling color/children, and a "double-black"
  fixup that has to walk back up the tree). Insertion and lookup - now both
  implemented - are what interview questions on this topic actually ask
  about most; deletion is better understood conceptually than reproduced
  from scratch here.
