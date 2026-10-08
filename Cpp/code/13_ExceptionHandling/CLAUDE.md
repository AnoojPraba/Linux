# 13_ExceptionHandling

Exceptions: custom exception classes, throw/catch order and what() overrides.

## Files
- `01_exceptions.cpp` - DivideByZeroException derived from std::exception with a noexcept what(); catch specific before general

## Build and run
- Single file: `g++ -std=c++17 -Wall -Wextra -pthread 01_exceptions.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `Cpp/code/bin/13_ExceptionHandling/` (git-ignored).

## Key concepts / interview angles
- Catch by `const&`; order handlers most-derived first (otherwise slicing/shadowing).
- Stack unwinding runs destructors, which is why RAII is the basis of exception safety.
- Never let exceptions escape destructors (they are `noexcept` by default); `terminate` if one does during unwinding.
- Exception safety levels: basic, strong, no-throw.
- Cost model: zero-cost when not thrown (table-based), expensive when thrown.

## Related
- `../14_RAII`
- `../23_NoexceptAndSTL`
- `../04_ConstructorsAndDestructors/02_copyAndSwap.cpp`
- `../../../C_Basics/code/76_ErrorHandlingAndCleanupPatterns`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.cpp`); keep each `.cpp` self-contained and independently compilable.
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`Cpp/code/bin/` is git-ignored).
