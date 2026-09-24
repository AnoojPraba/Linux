# PImpl Idiom

- PImpl (pointer to implementation): the public class holds only a
  `std::unique_ptr<Impl>` to a forward-declared struct; the real members live in `Impl`,
  defined only in the .cpp file.
- Why use it:
  - Compile-time firewall: changing `Impl`'s private members/headers does not touch the
    public header, so dependents don't recompile - big win for build times in large
    codebases.
  - ABI stability: the public class's size/layout (one pointer) never changes even if
    `Impl` grows, so shared libraries can add private fields without breaking binary
    compatibility with already-compiled callers.
  - Hides private members and any heavy/private headers from the public header entirely.
- Tradeoffs:
  - Extra indirection on every member access (pointer dereference).
  - One heap allocation per object (the `Impl`), plus allocation on every copy.
  - Destructor, copy/move special members must be defined out-of-line (in the .cpp,
    where `Impl` is complete) - `= default` in the header would fail because
    `unique_ptr`'s deleter needs a complete type at instantiation.
  - Move operations are cheap (just move the pointer); copy operations still require a
    deep copy of `Impl`.
- C analogue: `C_Basics/code/49_OpaquePointer` does the same thing with a `struct handle *`
  and a `.c`/`.h` split - PImpl is the same "hide the layout behind a pointer" idea,
  expressed with RAII (`unique_ptr`) instead of manual malloc/free.
