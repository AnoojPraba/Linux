# 22_CopyElisionAndRVO

RVO/NRVO and C++17 guaranteed copy elision, observed with a class that logs every special member call.

## Files
- `01_rvoAndNrvo.cpp` - instrumented type showing which constructions are elided for prvalue returns vs named locals
- `NOTES.md` - what is guaranteed in C++17 (prvalue returns) vs merely allowed (NRVO)

## Build and run
- Single file: `g++ -std=c++17 -Wall -Wextra -pthread 01_rvoAndNrvo.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `Cpp/code/bin/22_CopyElisionAndRVO/` (git-ignored).

## Key concepts / interview angles
- C++17: returning a prvalue constructs directly in the caller's storage; no copy/move needed.
- NRVO (`return local;`) is permitted, not required; the compiler can still fall back to a move.
- `return std::move(x)` defeats NRVO; return the name.
- Output is compiler/flag dependent (e.g. `-fno-elide-constructors` shows the full picture).

## Gotchas
- Observed counts depend on the compiler and optimisation flags.

## Related
- `../21_MoveSemantics`
- `../02_References`
- `../04_ConstructorsAndDestructors`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.cpp`); keep each `.cpp` self-contained and independently compilable.
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`Cpp/code/bin/` is git-ignored).
