# 31_SOLIDPrinciples

The five SOLID principles, each as a violation followed by a refactor.

## Files
- `01_singleResponsibility.cpp` - Report doing computation, formatting and saving, split into separate classes
- `02_openClosed.cpp` - if/else chain over shape types replaced by polymorphism
- `03_liskovSubstitution.cpp` - Square derived from Rectangle breaks the contract; corrected design
- `04_interfaceSegregation.cpp` - fat interface split into focused ones
- `05_dependencyInversion.cpp` - high-level class depending on an abstraction instead of a concrete class
- `NOTES.md` - SOLID summary and cross-references to DesignPatterns/

## Build and run
- Single file: `g++ -std=c++17 -Wall -Wextra -pthread 01_singleResponsibility.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `Cpp/code/bin/31_SOLIDPrinciples/` (git-ignored).

## Key concepts / interview angles
- SRP: one reason to change per class.
- OCP: extend by adding types, not editing a switch; usually via virtual dispatch or strategy.
- LSP: derived classes must honour base contracts (the Square/Rectangle example).
- ISP: many small interfaces over one fat one.
- DIP: depend on abstractions; inject dependencies (enables test doubles).
- Be ready to say when strict adherence is over-engineering.

## Related
- `../DesignPatterns/Behavioral/Strategy.cpp`
- `../DesignPatterns/Creational/FactoryMethod.cpp`
- `../10_Polymorphism`
- `../08_Inheritance`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.cpp`); keep each `.cpp` self-contained and independently compilable.
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`Cpp/code/bin/` is git-ignored).
