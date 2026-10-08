# 74_ContainerOfAndIntrusiveLists

Linux-kernel style container_of/offsetof, intrusive lists and flexible array members, with a senior Q&A.

## Files
- `01_container_of.c` - container_of macro recovering the enclosing struct from an embedded member pointer; intrusive list node
- `02_flexible_array.c` - C99 flexible array member: header and payload in one allocation
- `NOTES.md` - container_of, offsetof and intrusive lists, plus "Senior interviewer Q&A" (container_of pitfalls, intrusive vs non-intrusive, sentinel lists, thread safety, flexible array vs data[1], hlist sizing)

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_container_of.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/74_ContainerOfAndIntrusiveLists/` (git-ignored).

## Key concepts / interview angles
- `container_of(ptr, type, member)` = `(type *)((char *)ptr - offsetof(type, member))`.
- Intrusive list: the link lives inside the object, so one object can sit on several lists, no per-node allocation and O(1) unlink from the node itself.
- A circular list with a sentinel head removes empty/head/tail special cases.
- The kernel `hlist` (single pointer head) halves hash-table bucket size.
- Thread safety needs external locking or RCU; list pointers are not atomic.

## Related
- `../33_LinkedList`
- `../55_VTableEmulation`
- `../22_AdvancedArrays`
- `../34_HashTable`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
