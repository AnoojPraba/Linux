# 14_RAII

Resource Acquisition Is Initialisation: a FILE* guard whose destructor always closes the file.

## Files
- `01_fileGuard.cpp` - FileGuard opens in the constructor, closes in the destructor, non-copyable; writes /tmp/raii_example.txt

## Build and run
- Single file: `g++ -std=c++17 -Wall -Wextra -pthread 01_fileGuard.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `Cpp/code/bin/14_RAII/` (git-ignored).

## Key concepts / interview angles
- Tie resource lifetime to object lifetime: constructor acquires, destructor releases, including on exceptions and early returns.
- Delete (or define) copy operations so ownership is unambiguous; move is the transfer mechanism.
- The standard library applies it everywhere: `lock_guard`, `unique_ptr`, `fstream`.
- C++ alternative to the C `goto cleanup` ladder.

## Gotchas
- Writes a scratch file under `/tmp/`.

## Related
- `../15_SmartPointers`
- `../04_ConstructorsAndDestructors/03_ruleOfZero.cpp`
- `../13_ExceptionHandling`
- `../../../C_Basics/code/76_ErrorHandlingAndCleanupPatterns`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.cpp`); keep each `.cpp` self-contained and independently compilable.
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`Cpp/code/bin/` is git-ignored).
