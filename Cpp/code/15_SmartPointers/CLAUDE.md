# 15_SmartPointers

Standard smart pointers: unique_ptr (exclusive), shared_ptr (shared count) and weak_ptr (non-owning observer).

## Files
- `01_smartPointers.cpp` - unique_ptr ownership transfer, shared_ptr reference counts, weak_ptr::lock() to test expiry (inspectWeak)

## Build and run
- Single file: `g++ -std=c++17 -Wall -Wextra -pthread 01_smartPointers.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `Cpp/code/bin/15_SmartPointers/` (git-ignored).

## Key concepts / interview angles
- Default to `unique_ptr` (zero overhead); use `shared_ptr` only for genuinely shared ownership.
- `weak_ptr` breaks shared_ptr cycles and gives safe observation via `lock()`.
- Prefer `make_unique`/`make_shared` (exception safety, single allocation for shared).
- Passing: by raw pointer/reference for no ownership, by value to transfer ownership.
- Control-block cost and atomic refcounts of `shared_ptr`.

## Related
- `../25_CustomSharedPtr`
- `../14_RAII`
- `../21_MoveSemantics`
- `../DesignPatterns/Creational/Singleton.cpp`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.cpp`); keep each `.cpp` self-contained and independently compilable.
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`Cpp/code/bin/` is git-ignored).
