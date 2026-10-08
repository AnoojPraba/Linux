# 25_CustomSharedPtr

Hand-rolled shared_ptr and weak_ptr with an atomic two-count control block, to explain what std::shared_ptr does.

## Files
- `01_sharedPtrBasic.cpp` - SharedPtr with a control block and atomic reference counting
- `02_weakPtrCycleBreaking.cpp` - adds WeakPtr; strong count destroys the object, strong and weak both zero destroy the control block; breaks a cycle
- `NOTES.md` - exception safety in construction, atomic refcount pattern, why WeakPtr exists, two-count control block

## Build and run
- Single file: `g++ -std=c++17 -Wall -Wextra -pthread 01_sharedPtrBasic.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `Cpp/code/bin/25_CustomSharedPtr/` (git-ignored).

## Key concepts / interview angles
- Control block holds strong count, weak count, deleter; the object and control block have separate lifetimes.
- Increment can be relaxed, decrement needs acq_rel so the last owner sees all prior writes before deleting.
- Reference cycles leak with plain shared_ptr; weak_ptr breaks them and `lock()` upgrades safely.
- `make_shared` fuses object and control block into one allocation.
- The pointer copy is thread-safe, the pointee is not.

## Related
- `../15_SmartPointers`
- `../24_Concurrency/04_atomics.cpp`
- `../21_MoveSemantics`
- `../../../OS/code/20_Atomics`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.cpp`); keep each `.cpp` self-contained and independently compilable.
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`Cpp/code/bin/` is git-ignored).
