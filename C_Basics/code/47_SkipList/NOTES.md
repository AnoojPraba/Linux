# Skip List

- Achieving balanced-tree performance without rotations: instead of
  rebalancing on every insert/delete like AVL/Red-Black trees
  (`39_SelfBalancingTrees`), a skip list layers multiple linked lists on top
  of a base sorted list. Higher levels skip over many elements, letting
  search drop down a level whenever the next node would overshoot the
  target - giving expected O(log n) search/insert/delete.
- Randomized level assignment: each node's height is chosen independently
  via repeated "coin flips" (here, `rand() % 2 == 0` to climb another
  level), forming a geometric distribution. This is what statistically
  keeps roughly half as many nodes at each successive level, without any
  explicit rebalancing step.
- Simplicity vs balanced BSTs: no rotations, no balance-factor/coloring
  invariants to maintain - insert/delete are just pointer relinking at each
  level the node participates in. The tradeoff is probabilistic rather than
  guaranteed worst-case performance (an unlucky sequence of coin flips can
  degrade toward a plain linked list), though this is rare in practice and
  bounded with high probability.
- Real-world use: Redis sorted sets (`ZSET`) use a skip list internally for
  ordered range queries; LevelDB and RocksDB use skip lists for their
  in-memory memtable because concurrent/lock-free insertion is easier to
  reason about on a skip list than on a rotating balanced tree.
