# Binary Search Trees

## BST deletion cases

Deleting a value from a BST (`04_bstDeletion.c`) has three cases, depending on
how many children the node being removed has:

1. **Leaf node (no children)** - simply free the node and set the parent's
   link to it to `NULL`. Nothing else in the tree needs to change.
2. **One child** - splice the child directly into the position of the node
   being removed (return the child pointer up to the caller, who reattaches
   it in place of the deleted node). This preserves the BST property because
   the child's whole subtree was already correctly ordered relative to the
   deleted node's parent.
3. **Two children** - the node can't just be removed, since both its left and
   right subtrees need a new home. Instead, find the node's in-order
   successor (the smallest value in the right subtree, i.e. the leftmost
   node of that subtree), copy the successor's value into the node being
   "deleted", then recursively delete the successor from the right subtree.
   The successor is guaranteed to have at most one child (it has no left
   child by definition, since it's the leftmost node), so that recursive
   delete always terminates in case 1 or case 2 - it can never recurse into
   another two-child case.

Using the in-order *predecessor* (largest value in the left subtree) instead
of the successor works equally well; the choice is arbitrary.

## Lowest Common Ancestor (`05_lowestCommonAncestor.c`)

In a BST, ordering can be exploited directly: walk down from the root, going
left if both targets are smaller than the current node, right if both are
larger, and stopping (that node is the LCA) as soon as the targets split
across both sides or one of them equals the current node. This is O(h) with
no need to search both subtrees. A plain (non-BST) binary tree has no
ordering to exploit and needs the more general recursive approach instead:
search both subtrees for the two targets, and if a node's left and right
subtree searches both come back non-null, that node is the LCA. Both
versions are implemented in the file.

## Diameter of a binary tree (`06_diameter.c`)

The diameter is the longest path between any two nodes, which may or may
not pass through the root. The naive approach recomputes `height()` from
scratch at every node while walking the tree - each `height()` call is
O(n), across O(n) nodes, so the whole thing is O(n^2). The optimized
single-pass approach instead has each recursive call return the subtree's
height for its parent's use, while updating a running max-diameter as a
side effect - every node's height is computed exactly once, so the whole
thing is O(n).

## Invert a binary tree (`07_invertTree.c`)

Swap every node's left and right children, recursively. It became a famous
interview meme ("can you invert a binary tree?") precisely because it's
trivial to state but still directly tests basic comfort with tree
recursion - candidates who freeze on it likely haven't internalized
recursive tree traversal at all.

## Balanced and symmetric checks (`08_balancedAndSymmetric.c`)

- Height-balanced: every node's left and right subtree heights must differ
  by at most 1, checked at EVERY node, not just the root - a single-pass
  helper returns each subtree's height while flagging imbalance as a side
  effect, avoiding an O(n^2) naive recheck-height-everywhere approach.
- Symmetric: a tree is symmetric if it's a mirror image of itself around
  its center, checked by recursively comparing the left subtree against
  the right subtree with left/right swapped at each level.
