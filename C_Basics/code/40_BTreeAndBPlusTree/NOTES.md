# B-Tree and B+ Tree

- Notes-only folder: a small illustrative implementation would either be too
  trivial to show the real node-splitting/merging logic, or long enough to
  duplicate a textbook chapter. Understanding the structure and its
  motivation matters more here than a from-scratch implementation.
- Why B-trees exist: they minimize disk/block I/O. Each node maps to one
  disk block/page, and a high branching factor (many keys per node) keeps
  the tree's height very small even for millions of entries, so a lookup
  touches only a handful of blocks instead of `O(log2 n)` blocks the way a
  binary tree would.
- B-tree structure:
  - Each node holds multiple sorted keys (and child pointers between them),
    not just one key like a binary tree node.
  - Insertion that overflows a node's key capacity splits it, pushing the
    median key up into the parent - this split can cascade up to the root,
    which is how the tree grows in height (always at the root, keeping all
    leaves at the same depth).
  - All leaves sit at the same depth, guaranteeing worst-case `O(log n)`
    operations with a very shallow tree in practice.
- B+ Tree distinction:
  - Data records live only in leaf nodes; internal nodes hold only routing
    keys (copies) used to direct searches - they carry no payload.
  - Leaf nodes are linked together (a singly or doubly linked list across
    leaves), enabling fast ordered range scans without walking back up
    through internal nodes.
  - Because internal nodes carry no data, they're smaller and fit more keys
    per block, further increasing branching factor and shrinking height.
- Why B+ trees back real DB indexes/filesystems: the combination of shallow
  height (few block reads for point lookups) and linked leaves (efficient
  range scans, e.g. `WHERE col BETWEEN a AND b`) matches exactly how
  relational databases implement indexes and how filesystems implement
  directory/extent structures.
- Cross-references:
  - `../../OS/code/54_FileSystemStructuresAndAllocation` covers how
    filesystems organize on-disk structures and allocation, the same
    block-oriented motivation that drives B-tree design.
  - `../../SystemDesign/topics/04_DatabaseIndexingAndQueryOptimization`
    covers how B+ trees back real database indexes and how query planners
    use them.
