# 08_Inheritance

Single inheritance, constructor chaining, protected members and object slicing.

## Files
- `01_animals.cpp` - base/derived classes with protected members and base-initializer-list chaining
- `02_objectSlicing.cpp` - copying a Derived into a Base by value drops derived data
- `NOTES.md` - object slicing and the by-reference/pointer fix

## Build and run
- Single file: `g++ -std=c++17 -Wall -Wextra -pthread 01_animals.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `Cpp/code/bin/08_Inheritance/` (git-ignored).

## Key concepts / interview angles
- Constructors run base-first, destructors in reverse order.
- Slicing: pass and store polymorphic types by reference, pointer or smart pointer, never by value.
- Make base destructors `virtual` when deleting through a base pointer (see `../10_Polymorphism`).
- Prefer composition over inheritance unless modelling is-a.

## Related
- `../09_MultipleInheritanceAndVirtualBase`
- `../10_Polymorphism`
- `../31_SOLIDPrinciples`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.cpp`); keep each `.cpp` self-contained and independently compilable.
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`Cpp/code/bin/` is git-ignored).
