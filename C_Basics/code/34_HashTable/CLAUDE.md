# 34_HashTable

Hash tables from scratch: separate chaining and open addressing with tombstones, with a senior Q&A (resizing, probing, hash flooding, thread safety).

## Files
- `01_hashTableChaining.c` - buckets are singly linked lists; collisions grow the list
- `02_hashTableOpenAddressing.c` - linear probing with EMPTY / OCCUPIED / DELETED (tombstone) slot states
- `NOTES.md` - load factor and resizing, chaining vs open addressing, hash function quality, tombstone gotcha, plus "Senior interviewer Q&A" (9 questions)

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_hashTableChaining.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/34_HashTable/` (git-ignored).

## Key concepts / interview angles
- Load factor = n / buckets; resize near 0.7 (open addressing) and rehash, giving amortised O(1) insert.
- Open addressing is cache friendly but clustering grows with load; chaining degrades gracefully.
- Tombstones: clearing a deleted slot to EMPTY would break later lookups that probed past it.
- Worst case O(n) with a bad hash or hash-flooding attack; defences are keyed/randomised hashes (SipHash).
- Thread-safe map: lock striping or per-bucket locks, or concurrent resizing strategies.
- Robin Hood and quadratic/double hashing are covered next in `../36_AdvancedHashingAndCacheAwareStructures`.

## Related
- `../33_LinkedList`
- `../36_AdvancedHashingAndCacheAwareStructures`
- `../37_LRUCache`
- `../35_ProbabilisticDataStructures`
- `../../../Cpp/code/27_STLInternals`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
