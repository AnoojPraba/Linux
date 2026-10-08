# 34_SpanStringViewAndPmr

std::span, std::string_view and std::pmr arena allocation: non-owning views and allocation control, with a senior Q&A.

## Files
- `01_spanAndStringView.cpp` - span over contiguous memory ("array + size" without copying) and string_view parameters
- `02_pmrArena.cpp` - pmr containers over a stack arena (monotonic_buffer_resource); same container type regardless of allocator
- `NOTES.md` - span (C++20), string_view (C++17), pmr (C++17), "Senior interviewer Q&A"

## Build and run
- Needs C++20: `g++ -std=c++20 -Wall -Wextra -pthread 01_spanAndStringView.cpp -o /tmp/x && /tmp/x`.
- `make` from `..` builds it with `-std=c++20`.
- `make` from `..` (the parent code/ dir) builds everything into `Cpp/code/bin/34_SpanStringViewAndPmr/` (git-ignored).

## Key concepts / interview angles
- `span`/`string_view` are non-owning: never return one that points into a temporary or a container that may reallocate.
- `string_view` is not guaranteed null-terminated; do not pass `.data()` to C APIs blindly.
- pmr: `memory_resource*` is a runtime property, so `pmr::vector<int>` has one type regardless of arena; monotonic arena frees all at once.
- Eliminate hot-path heap allocation with a stack arena plus `pmr` containers.
- `span<const T>` vs `const vector<T>&`: accepts arrays, vectors and subranges.

## Related
- `../20_ModernCppFeatures/05_stringView.cpp`
- `../29_CustomAllocatorCpp`
- `../../../C_Basics/code/81_MallocInternalsAndAllocators`
- `../18_STLContainers/07_smallStringOptimization.cpp`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.cpp`); keep each `.cpp` self-contained and independently compilable.
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`Cpp/code/bin/` is git-ignored).
