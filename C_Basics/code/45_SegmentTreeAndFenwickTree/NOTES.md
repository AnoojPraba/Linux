# Segment Tree and Fenwick Tree (BIT)

- Both support range queries and point updates in O(log n), a big
  improvement over a naive array where a range-sum query is O(n).
- Fenwick Tree (Binary Indexed Tree) simplicity: it's just an array (no
  explicit tree nodes/pointers), 1-indexed, where each index implicitly owns
  a range determined by its lowest set bit. The `index & -index` trick
  ("lowbit") isolates that lowest set bit to walk up (`update`, adding the
  lowbit) or down (`prefixSum`, subtracting the lowbit) the implicit tree.
  It's compact and fast but fundamentally limited to operations with an
  invertible combine step (sum, XOR) since range queries are built from
  prefix-sum subtraction.
- Segment Tree flexibility: an explicit binary tree (often stored in an
  array sized `4 * n` for simplicity) where each node stores an aggregate
  over its range. It generalizes to any associative combine function -
  range-min, range-max, range-gcd, range-sum - since queries directly
  combine child results rather than relying on subtraction. It also
  supports lazy propagation: deferring range updates by stamping a "pending
  update" on a node and only pushing it down to children when a later query
  or update actually needs to descend into them, turning O(n) range updates
  into O(log n).
- Why O(log n) matters vs naive O(n): for large n (millions of elements)
  and many queries/updates, the difference between O(log n) (~20 steps for
  a million elements) and O(n) (a million steps) per operation is the
  difference between an interactive system and a timeout.
