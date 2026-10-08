# 04_ConstructorsAndDestructors

Special member functions: rule of three, copy-and-swap, rule of zero and explicit; classic senior C++ resource-management questions.

## Files
- `01_ruleOfThree.cpp` - Buffer owning a char*: default/parameterised/copy ctor, destructor, copy assignment
- `02_copyAndSwap.cpp` - exception-safe operator= via by-value parameter and noexcept swap
- `03_ruleOfZero.cpp` - class made of RAII members needs no custom special members
- `04_explicitKeyword.cpp` - implicit conversion via single-argument constructor vs explicit
- `NOTES.md` - copy-and-swap, rule of zero, explicit

## Build and run
- Single file: `g++ -std=c++17 -Wall -Wextra -pthread 01_ruleOfThree.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `Cpp/code/bin/04_ConstructorsAndDestructors/` (git-ignored).

## Key concepts / interview angles
- If you define one of destructor / copy ctor / copy assignment you probably need all three (rule of three; five with move ops).
- Copy-and-swap gives strong exception safety and free self-assignment safety.
- Rule of zero: let `std::string`/`vector`/`unique_ptr` own resources; prefer it by default.
- Mark single-argument constructors `explicit` to block surprising implicit conversions.
- Constructors initialise members in declaration order, not initializer-list order.

## Related
- `../14_RAII`
- `../21_MoveSemantics`
- `../15_SmartPointers`
- `../../../C_Basics/code/15_DynamicMemory`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.cpp`); keep each `.cpp` self-contained and independently compilable.
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`Cpp/code/bin/` is git-ignored).
