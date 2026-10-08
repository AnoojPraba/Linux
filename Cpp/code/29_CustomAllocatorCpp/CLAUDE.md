# 29_CustomAllocatorCpp

A minimal std-conforming bump/arena allocator that logs every allocation made by std::vector.

## Files
- `01_loggingBumpAllocator.cpp` - allocator over a fixed buffer with allocate/deallocate logging, used with vector
- `NOTES.md` - minimum allocator interface, allocator-aware container mechanics, why it matters for embedded/perf

## Build and run
- Single file: `g++ -std=c++17 -Wall -Wextra -pthread 01_loggingBumpAllocator.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `Cpp/code/bin/29_CustomAllocatorCpp/` (git-ignored).

## Key concepts / interview angles
- Minimal allocator: `value_type`, `allocate`, `deallocate`, rebind-able converting constructor, equality operators.
- Bump allocation is O(1) and frees everything at once; individual deallocate is a no-op.
- `std::pmr` (C++17) makes allocators a runtime property (see `../34_SpanStringViewAndPmr`).
- Watch vector regrowth: each reallocation requests a larger slice and wastes the old one in a bump arena.

## Related
- `../34_SpanStringViewAndPmr`
- `../../../OS/code/45_CustomAllocator`
- `../../../C_Basics/code/81_MallocInternalsAndAllocators`
- `../27_STLInternals`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.cpp`); keep each `.cpp` self-contained and independently compilable.
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`Cpp/code/bin/` is git-ignored).
