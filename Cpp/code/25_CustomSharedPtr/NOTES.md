# Custom SharedPtr / WeakPtr

Hand-rolled reimplementation of `std::shared_ptr`/`std::weak_ptr`'s essential mechanics,
for learning what the standard library is actually doing underneath. See
`15_SmartPointers/01_smartPointers.cpp` for the real `std::shared_ptr`/`std::weak_ptr`
usage this reimplements.

- Exception safety during construction: `SharedPtr(T *pointer)` allocates the control
  block after the raw pointer already exists, so if `new ControlBlock<T>(...)` throws
  (e.g. `std::bad_alloc`), the caller's `new T(...)` has already succeeded and would leak
  unless the constructor call site is written carefully (e.g.
  `SharedPtr<T>(new T(...))` as one expression, so nothing else can throw between the
  `new T` and the SharedPtr taking ownership). More generally, any control-block-owning
  smart pointer must ensure no code path leaves a raw pointer allocated but unowned.
  The real fix production code reaches for is `make_shared`-style *combined allocation*:
  allocate the control block and the managed object together in a single heap block, so
  there's only one allocation to fail atomically and no window where one exists without
  the other. This demo's separate allocations (raw object first, control block second)
  are fine for learning the refcounting mechanics but aren't exception-safe/production
  quality as written.
- Atomic refcounting pattern (`01_sharedPtrBasic.cpp`): incrementing the strong count on
  copy can use `memory_order_relaxed` because the new reference is only usable via a
  handoff (copy construction/assignment) that already establishes ordering by other
  means (e.g. being on the same thread, or protected by a mutex) -- the increment itself
  introduces no new cross-thread visibility requirement. Decrementing to zero is the
  operation that actually destroys shared state, so it needs `memory_order_acq_rel`
  (release paired with an acquire fence on whichever thread observes the count reaching
  zero): the release half publishes this thread's prior work on the object so it
  happens-before destruction, and the acquire half ensures the destroying thread also
  sees every other thread's now-finished uses of the object.
- Why WeakPtr exists (`02_weakPtrCycleBreaking.cpp`): reference counting (what
  `SharedPtr` does) cannot collect cycles -- if A holds a `SharedPtr` to B and B holds a
  `SharedPtr` back to A, both objects' strong counts stay above zero forever even after
  nothing external references either one, leaking both. `WeakPtr` breaks such cycles by
  letting one side (conventionally: child -> parent, since the parent already owns the
  child) observe the other without contributing to its strong count. `.lock()` attempts
  to atomically promote a `WeakPtr` to a `SharedPtr`, succeeding only if the strong count
  hasn't already hit zero (implemented here with a compare-exchange loop on the strong
  count), and returns an empty `SharedPtr` if the object is already gone.
  Trade-off vs. a tracing garbage collector: refcounting (with manual `WeakPtr` cycle
  breaks) gives deterministic, immediate destruction and no stop-the-world pauses, but
  requires the programmer to notice and manually break cycles; a tracing GC collects
  cycles automatically but destruction timing is non-deterministic and collection can
  introduce pause times.
- Two-count control block: strong count reaching zero destroys the *managed object*
  (calling its destructor) but not necessarily the control block itself, since a
  `WeakPtr` might still be alive and need to ask "is the object still there?" via
  `strongCount == 0`. The control block itself is only freed once *both* strong and weak
  counts have reached zero, i.e. once no `SharedPtr` and no `WeakPtr` reference it anymore.
