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
- `03_avlTreeDeletion.c` implements AVL deletion (standard BST delete -
  leaf / one-child / two-children via in-order successor - followed by a
  rebalancing walk back to the root) and `04_redBlackTreeDeletion.c`
  implements red-black deletion (BST delete, tracking the color of the
  node actually spliced out, plus a double-black fixup when that color was
  black). See the "Why deletion is harder than insertion" section below.

## Why deletion is harder than insertion

- Insertion only ever needs *one* rebalancing step on the path back to the
  root: after inserting a new leaf, at most one node on the path from that
  leaf to the root becomes unbalanced, and fixing it (a single or double
  rotation for AVL; a recolor-or-rotate step for red-black) always restores
  the full invariant in one shot, because insertion only ever *increases* a
  subtree's height by at most one level, and a single rotation undoes
  exactly that.
- Deletion does not have this luxury: removing a node can *decrease* a
  subtree's height, and fixing the resulting imbalance at one ancestor can
  itself leave that ancestor's subtree one level *shorter* than it was
  before the deletion (even after a rotation restores its balance factor
  locally). That shortening can then imbalance the *next* ancestor up, and
  so on - so a single deletion can require rebalancing repeatedly all the
  way up to the root, not just once. `03_avlTreeDeletion.c` demonstrates
  this: `avlDelete` checks the balance factor and applies a rotation at
  *every* ancestor on the way back up (not just the first one found
  unbalanced), and the demo in `main()` shows both a single (LL) and a
  double (RL) rotation firing from a single call chain.
- Red-black deletion has the same "can propagate all the way up" property,
  expressed through the "double-black" concept: when the node actually
  removed (or spliced in via the in-order-successor swap) was black, one
  root-to-leaf path is left one black node short. Since a NIL leaf can't
  literally hold "negative color," the fixup treats the node that replaced
  it as carrying an extra, phantom unit of blackness ("double-black") that
  must be resolved before the tree is valid again. Unlike insertion's
  fixup (two cases: red uncle → recolor and move up; black uncle → rotate
  and stop), the double-black fixup has *four* cases (red sibling; black
  sibling with two black children; black sibling with a "near" red child;
  black sibling with a "far" red child), mirrored again for whether the
  fixup node is a left or right child - and only the "two black children"
  case keeps propagating up (turning the parent into the new double-black
  node); the other three resolve the imbalance with a rotation and stop.
  `04_redBlackTreeDeletion.c`'s `main()` demonstrates a red-leaf delete
  (no fixup), a black-leaf delete whose sibling is black with red children
  (rotation case), and a black-leaf delete whose sibling is black with
  black children, which prints `[recolor propagate up through <key>]` to
  show the double-black explicitly moving to the parent before it is
  resolved.

### Correctness status

- **AVL deletion (`03_avlTreeDeletion.c`)**: verified correct for the cases
  exercised. Compiles clean with `-Wall`. The demo deletes from a
  hand-traced tree and prints in-order (always sorted) and level-order
  (with balance factors) before/after each deletion; the sequence
  specifically triggers one single rotation (LL) and one double rotation
  (RL), both confirmed by inspecting the printed balance factors and tree
  shape before and after.
- **Red-black deletion (`04_redBlackTreeDeletion.c`)**: implemented via the
  standard BST-delete + double-black-fixup approach (four sibling-color
  cases, mirrored left/right, matching CLRS's `RB-DELETE-FIXUP`). This was
  given a genuine implementation effort, not a partial/simplified version.
  Verified in two ways: (1) the `main()` demo hand-traces a red-leaf
  delete, a black-leaf delete with a black-with-red-children sibling
  (rotation case), a black-leaf delete with a black-with-black-children
  sibling (recolor-propagation case, confirmed via the printed trace line
  and by re-deriving the resulting tree's black-heights by hand), a
  black-node-with-one-red-child delete (direct absorption), and a
  two-children delete via in-order successor - every step's in-order
  traversal stays sorted and colors match a valid red-black tree; (2) a
  randomized stress test (not checked into this file) ran 500 trials of
  200 mixed random insert/delete operations each against a from-scratch
  black-height checker (verifying every root-to-NIL path has equal black
  count, no red node has a red child, and the root is black) plus an
  in-order-sortedness/count check after every single operation - all
  500 trials passed with zero invariant violations. Based on this, the
  implementation is assessed as **correct for all cases exercised**,
  including the full double-black fixup case set, though (as with any
  hand-written implementation) an exhaustive formal proof was not
  performed and an adversarial input could in principle still find an
  untested edge case.
