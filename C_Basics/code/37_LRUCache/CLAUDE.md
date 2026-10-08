# 37_LRUCache

O(1) LRU cache combining a doubly-linked list (recency order) with a hash map (key to node); also discusses thread-safe variants.

## Files
- `01_lruCacheDllHashMap.c` - get/put with DLL head = most recent, tail = eviction candidate, hash map key -> node pointer
- `NOTES.md` - why neither structure alone suffices, the combination, thread safety (single mutex vs lock striping)

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_lruCacheDllHashMap.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/37_LRUCache/` (git-ignored).

## Key concepts / interview angles
- Hash map gives O(1) lookup; DLL gives O(1) move-to-front and tail eviction; array or plain list alone cannot do both.
- get: lookup, detach, reinsert at head. put: update or insert at head and evict the tail at capacity.
- Thread safety: one big mutex is simplest; shard by key hash for scalability; note that get mutates order so even reads need a write lock.
- Variants: LFU, 2Q, CLOCK approximations of LRU.

## Related
- `../34_HashTable`
- `../33_LinkedList/03_doublyLinkedList.c`
- `../../../SystemDesign/topics/03_CachingStrategies`
- `../../../SystemDesign/topics/15_DesignCaseStudyDistributedCache`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
