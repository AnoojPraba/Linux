# 51_SkipList

Skip list: a probabilistic ordered structure with expected O(log n) search/insert/delete and no rebalancing.

## Files
- `01_skipList.c` - multi-level forward pointers, randomLevel() coin-flip level selection, insert/search/delete
- `NOTES.md` - balanced-tree performance without rotations, randomised levels, simplicity vs balanced BSTs, real-world use (Redis sorted sets)

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_skipList.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/51_SkipList/` (git-ignored).

## Key concepts / interview angles
- Each node gets a random height (geometric, p = 0.5 or 0.25); expected O(log n), worst case O(n) but with negligible probability.
- No rotations, simpler concurrent variants (lock-free skip lists, e.g. Java ConcurrentSkipListMap).
- Redis ZSET uses a skip list plus a hash map; LevelDB/RocksDB memtables too.
- Costs more memory per node (multiple forward pointers) than a BST.

## Gotchas
- Level choice uses rand() with no srand() call in the file, so runs are deterministic (default seed).

## Related
- `../39_SelfBalancingTrees`
- `../38_BinaryTree`
- `../../../SystemDesign/topics/30_LSMTreesAndStorageEngines`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
