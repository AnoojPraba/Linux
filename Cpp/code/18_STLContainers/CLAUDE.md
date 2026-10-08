# 18_STLContainers

STL containers and adaptors with complexity and behaviour trade-offs, including unordered containers and small string optimisation.

## Files
- `01_containers.cpp` - vector, map, set, unordered_map insertion/iteration/lookup
- `02_priorityQueue.cpp` - priority_queue as a heap adaptor over vector (max-heap by default)
- `03_list.cpp` - std::list doubly linked list; O(1) insert/erase given an iterator
- `04_stackAdaptor.cpp` - std::stack adaptor (deque by default)
- `05_queueAdaptor.cpp` - std::queue FIFO adaptor (deque by default)
- `06_unorderedSetMap.cpp` - hash containers: buckets, load_factor, rehash
- `07_smallStringOptimization.cpp` - checks whether string data lives inside the object (SSO) or on the heap
- `NOTES.md` - big theme, container adaptors gotchas, complexity/behaviour cheat sheet, SSO

## Build and run
- Single file: `g++ -std=c++17 -Wall -Wextra -pthread 01_containers.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `Cpp/code/bin/18_STLContainers/` (git-ignored).

## Key concepts / interview angles
- Choose by access pattern: `vector` by default (cache locality), `deque` for both-end growth, `list` rarely, `map` ordered O(log n), `unordered_map` average O(1).
- Adaptors (`stack`, `queue`, `priority_queue`) restrict an underlying container; they expose no iterators.
- `vector` reallocation invalidates iterators/references; erase-remove idiom; reserve to avoid regrowth.
- `unordered_*`: load factor and rehash; hash quality and iteration order unspecified.
- SSO: short strings (about 15 chars in libstdc++) avoid heap allocation; a small string move is a copy.

## Gotchas
- SSO capacity and layout depend on the standard library implementation (libstdc++ here).

## Related
- `../27_STLInternals`
- `../19_STLAlgorithms`
- `../../../C_Basics/code/34_HashTable`
- `../../../C_Basics/code/41_Heap`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.cpp`); keep each `.cpp` self-contained and independently compilable.
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`Cpp/code/bin/` is git-ignored).
