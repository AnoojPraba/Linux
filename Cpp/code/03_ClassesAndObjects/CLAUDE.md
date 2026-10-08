# 03_ClassesAndObjects

Class basics: access specifiers, member functions and the mutable keyword (logical vs bitwise constness).

## Files
- `01_basicClass.cpp` - class with private data, public members, constructor initializer list, getters
- `02_mutableKeyword.cpp` - `mutable` member modified from a const method (cache/counter)
- `NOTES.md` - mutable: logical vs bitwise constness; typical uses (memoisation, call counters, mutexes)

## Build and run
- Single file: `g++ -std=c++17 -Wall -Wextra -pthread 01_basicClass.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `Cpp/code/bin/03_ClassesAndObjects/` (git-ignored).

## Key concepts / interview angles
- `struct` vs `class` differ only in default access (and default inheritance).
- const methods promise logical constness; `mutable` is the sanctioned escape for caches, counters and mutexes.
- Prefer member initializer lists over assignment in the constructor body.
- A `const` method that locks a `mutable std::mutex` is the thread-safe getter idiom.

## Related
- `../04_ConstructorsAndDestructors`
- `../24_Concurrency`
- `../../../C_Basics/code/08_Structures`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.cpp`); keep each `.cpp` self-contained and independently compilable.
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`Cpp/code/bin/` is git-ignored).
