# Custom Allocators (C++)

- Minimum interface an allocator-aware container (`std::vector`, `std::list`, etc.)
  needs from an allocator (`std::allocator_traits` fills in defaults for the rest):
  - `value_type` typedef.
  - `T *allocate(std::size_t n)` - returns storage for `n` objects of `T` (uninitialized).
  - `void deallocate(T *p, std::size_t n)` - releases storage previously returned by
    `allocate`.
  - A converting constructor template `Allocator(const Allocator<U>&)` - required for
    "rebind": a `std::list<T, MyAlloc<T>>` internally needs `MyAlloc<ListNode<T>>`, and
    the container builds that via this converting constructor (C++17 no longer requires a
    hand-written `rebind` member template - `allocator_traits` synthesizes it from the
    class template itself).
  - `operator==`/`operator!=` between allocator instances - tells the container whether
    two allocator objects can free each other's memory (needed for container
    move/swap/assignment semantics).
- Allocator-aware container mechanics:
  - The container calls `allocate`/`deallocate` for raw storage, then separately calls
    placement-new / destructor (via `allocator_traits::construct`/`destroy`) to build and
    tear down objects in that storage - allocation and construction are deliberately
    decoupled.
  - Containers propagate (or don't) their allocator across copy/move/swap based on
    `propagate_on_container_*` traits, which default to "don't propagate" unless
    specified - out of scope for a simple demo allocator, which can rely on the defaults.
- Why this matters for embedded/perf-sensitive C++:
  - A bump/arena allocator (as in `01_loggingBumpAllocator.cpp`) avoids per-node
    malloc/free churn and heap fragmentation - allocation is just a pointer bump.
  - Lets code use standard containers (`vector`, `list`, `map`) with predictable,
    pool-bounded memory instead of the general-purpose heap, which matters when heap use
    is restricted, fragmentation must be avoided, or allocation latency must be bounded.
- C analogue: `C_Basics/code/65_CustomAllocator` implements the same
  arena/fixed-size-pool ideas manually (a `void *` bump pointer, a free list) without any
  language-level allocator interface - here the C++ allocator template is just the
  "pluggable" hook that lets `std::vector` and friends use that same strategy
  transparently.
