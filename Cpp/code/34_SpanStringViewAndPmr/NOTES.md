# std::span, std::string_view and std::pmr

Build: this folder compiles with `-std=c++20` (see the Makefile rule).

## `std::span<T, Extent>` (C++20)
- Non-owning `(T* data, size_t size)` view over contiguous storage: C arrays,
  `std::vector`, `std::array`. Replaces `(T* p, size_t n)` parameter pairs; pass
  by value (16 bytes). `span<const T>` = read-only; `span<T, N>` has a
  compile-time size.
- `first(n)`, `last(n)`, `subspan(off, n)` slice without copying; `as_bytes()` /
  `as_writable_bytes()` view the object representation.
- **Not bounds-checked by `operator[]`** (UB on out of range); size is checked by
  the type system only for static extents. Prefer range-for or `at`-like checks
  (`span::at` arrives in C++26).
- Lifetime: dangles if the underlying container reallocates, is destroyed, or is
  a temporary. `v.push_back` / `reserve` after taking a span invalidates it.

## `std::string_view` (C++17)
- Non-owning `(const char*, size)` view: cheap substring/trim/split/parse with
  zero allocation; accepts `std::string`, literals, `char*`.
- **Not null-terminated:** `sv.data()` handed to C APIs (`atoi`, `fopen`) can
  overrun - build a `std::string` first.
- Classic traps: returning a `string_view` to a local/temporary `string`;
  storing a view into a map key while the owner string changes; `string_view`
  from `std::string + "x"` temporaries; comparing lifetimes in coroutines.
- Pass `string_view` (not `const string&`) for read-only text parameters - avoids
  constructing a `std::string` from literals.
- C equivalent idea: `struct { const char *p; size_t n; }` slices - the safer
  pattern many C codebases adopt (see `../../../C_Basics/code/07_Strings`).

## `std::pmr` polymorphic allocators (C++17)
- `std::pmr::memory_resource` is an abstract interface (`do_allocate`,
  `do_deallocate`, `do_is_equal`). Containers like `std::pmr::vector<T>` hold a
  `polymorphic_allocator` pointing at a resource chosen AT RUNTIME, so the
  container type does not change with the allocation strategy (unlike
  `std::vector<T, MyAlloc>`).
- Standard resources: `new_delete_resource()` (global heap),
  `monotonic_buffer_resource` (bump allocator over a buffer, falls back to an
  upstream, frees all at once), `unsynchronized_pool_resource` /
  `synchronized_pool_resource` (size-class pools; the latter thread-safe),
  `null_memory_resource()` (always throws - enforces "no heap").
- Use cases: per-request/per-frame arenas, stack-based scratch containers,
  deterministic latency (no malloc), cache locality, easy leak-freedom,
  counting/debugging allocators, avoiding allocator-induced contention.
- Propagation: `pmr` containers pass their resource to pmr-aware elements
  (`pmr::vector<pmr::string>` uses the same arena for the strings) via
  uses-allocator construction; a `std::string` element would silently use the
  global heap.
- `02_pmrArena.cpp` counts upstream allocations: heap-backed container hits the
  heap dozens of times; a stack arena makes zero upstream calls until the buffer
  is exhausted.
- Costs/risks: a virtual call per allocation; monotonic resources never reuse
  freed memory; objects must not outlive their arena; mixing resources across
  containers on move/assign can force element-wise copies (allocators must
  compare equal for cheap moves).
- Compare with C custom allocators/arenas (`../../../OS/code/45_CustomAllocator`,
  `../../../C_Basics/code/81_MallocInternalsAndAllocators`) - same idea with
  language support for containers.

## Senior interviewer Q&A
**Q: When would you use `std::span` instead of `std::vector<T>&` or iterators?**
A: When a function only needs to read/write a contiguous range and should accept
any owner (C array, vector, array, part of a buffer). It expresses intent, avoids
templating on the container, and carries the size with the pointer.

**Q: What is the danger of `string_view`?**
A: It does not own anything. Returning a view of a local string, holding a view
across a reallocation of the owner, or taking a view of a temporary produces
dangling reads. Also, no null terminator.

**Q: Why prefer `pmr` over `std::allocator` template parameters?**
A: Type erasure of the allocator at runtime: `pmr::vector<int>` is one type
regardless of arena, so APIs and libraries do not template on allocators;
switching strategy is just choosing a resource. Cost is an indirect call.

**Q: How would you eliminate heap allocations in a hot request handler?**
A: Per-request `monotonic_buffer_resource` over a stack/thread-local buffer, pmr
containers/strings for all temporaries, `string_view`/`span` for inputs, reserve
capacity up front, and measure with a counting resource or `malloc` hooks to
prove zero heap calls.

**Q: Span vs gsl::span vs array_view?**
A: `std::span` is the standard successor of Microsoft GSL's `span`; it differs
mainly in not being bounds-checked by default. `array_view` was the earlier
proposal name.

**Q: Why can't you store `std::span<const char>` and `std::string_view` freely
in a long-lived struct?**
A: Because they borrow. Store owners (`std::string`, `std::vector`) in long-lived
objects and views only as parameters or short-lived locals, unless you can
document and enforce the owner's lifetime (e.g. arena-backed or interned strings).
