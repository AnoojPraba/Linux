# 10_Polymorphism

Virtual functions, override, abstract classes, and why a base destructor must be virtual.

## Files
- `01_shapes.cpp` - abstract Shape with pure virtual function, override in derived classes, dispatch through base pointers
- `02_virtualDestructor.cpp` - BrokenBase (non-virtual dtor) deleted through a base pointer vs the fixed virtual-destructor version
- `NOTES.md` - virtual destructor necessity and rule of thumb

## Build and run
- Single file: `g++ -std=c++17 -Wall -Wextra -pthread 01_shapes.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `Cpp/code/bin/10_Polymorphism/` (git-ignored).

## Key concepts / interview angles
- Dynamic dispatch uses a per-class vtable and a per-object vptr; cost is one indirection and no inlining.
- Deleting a derived object through a non-virtual base destructor is UB; derived cleanup is skipped.
- Use `override` (and `final`) to catch signature mismatches.
- Never call virtual functions from constructors/destructors (dispatch stays at the current class level).
- Pure virtual functions make a class abstract (and can still have a body).

## Gotchas
- `02_virtualDestructor.cpp` deliberately performs the UB `delete` through a non-virtual base; do not "fix" the Broken* classes (the fixed version is shown next to it).
- Run under ASan/valgrind to see leaks it creates: `g++ -g -fsanitize=address ...`.

## Related
- `../../../C_Basics/code/55_VTableEmulation` - how vtables work in C
- `../08_Inheritance`
- `../15_SmartPointers`
- `../31_SOLIDPrinciples`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.cpp`); keep each `.cpp` self-contained and independently compilable.
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`Cpp/code/bin/` is git-ignored).
