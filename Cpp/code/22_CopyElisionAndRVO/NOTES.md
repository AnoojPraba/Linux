# Copy Elision and RVO/NRVO

- Copy elision: skipping a copy/move construction and building the object directly in
  its final storage location.
- Guaranteed by the C++17 standard (not just an optimization anymore):
  - Returning a prvalue of the function's return type (`return Traced();` or
    `return Type(args);`) - no temporary is ever materialized, the object is constructed
    directly in the caller's storage. No copy/move constructor call is observable, so the
    type does not even need to be copyable/movable for this to compile.
  - Passing a prvalue as a by-value function argument, and other prvalue-materialization
    contexts.
- NOT guaranteed by the standard (just extremely common compiler behavior):
  - NRVO (named return value optimization): `return local;` where `local` is a named
    local variable. The compiler MAY construct `local` directly in the caller's return
    slot and skip the move/copy entirely, but the standard permits (does not require)
    this. GCC/Clang/MSVC all do it at normal optimization levels, but a conforming
    compiler could still call the move constructor - or even the copy constructor if the
    type isn't movable.
  - Because NRVO is not guaranteed, the type must still have an accessible copy or move
    constructor for `return local;` to compile, even when the compiler ends up eliding it.
- `std::move(local)` on a return statement is usually a pessimization:
  - It converts the named local into an xvalue, which disqualifies it from NRVO
    consideration - the compiler can no longer elide, and must fall back to the move
    constructor.
  - Guaranteed elision only applies to prvalues; std::move produces an xvalue, so it
    never helps and can only hurt (best case: same as an un-elided move; worst case:
    disables an elision the compiler would otherwise have performed).
  - Correct guidance: never `std::move` a local variable in a `return` statement - just
    `return local;` and let the compiler choose move or elision itself.
