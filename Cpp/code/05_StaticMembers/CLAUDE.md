# 05_StaticMembers

Static data/member functions and the static initialization order fiasco with its construct-on-first-use fix.

## Files
- `01_staticMembers.cpp` - static data member (shared counter) and static member function
- `02_staticInitOrderFiasco.cpp` - single-file simplification of the cross-TU init-order hazard, with comments marking the conceptual TU split, and the function-local-static fix
- `NOTES.md` - the fiasco, why a real demo needs two TUs, construct-on-first-use

## Build and run
- Single file: `g++ -std=c++17 -Wall -Wextra -pthread 01_staticMembers.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `Cpp/code/bin/05_StaticMembers/` (git-ignored).

## Key concepts / interview angles
- Static members are shared by all instances; define them out of class (or `inline` in C++17).
- Init order of non-local statics is unspecified across translation units.
- Fix: function-local static (Meyers singleton); initialisation is thread-safe since C++11.
- Destruction-order problems exist too (use-after-destruction at exit).

## Gotchas
- The fiasco is only simulated in one file: the Makefile builds each `.cpp` into its own binary, so a true cross-TU demo is not possible here.

## Related
- `../DesignPatterns/Creational/Singleton.cpp`
- `../24_Concurrency`
- `../../../C_Basics/code/17_StorageClasses`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.cpp`); keep each `.cpp` self-contained and independently compilable.
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`Cpp/code/bin/` is git-ignored).
