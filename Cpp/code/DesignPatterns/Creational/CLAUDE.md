# Creational

Creational design patterns in C++17: object-creation strategies (Abstract Factory, Builder, Factory Method, Prototype, Singleton), one self-contained demo per pattern.

## Files
- `AbstractFactory.cpp` - families of related UI widgets with Light and Dark theme factories
- `Builder.cpp` - step-by-step Pizza assembly with chained setters and a final build
- `FactoryMethod.cpp` - notification creation without exposing concrete classes
- `Prototype.cpp` - `unique_ptr<Shape> clone() const` to copy shapes polymorphically
- `Singleton.cpp` - Logger::instance() with a function-local static; copy operations deleted

## Build and run
- Single file: `g++ -std=c++17 -Wall -Wextra -pthread AbstractFactory.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `Cpp/code` builds each file into `bin/DesignPatterns/<Group>/<Name>` (git-ignored).

## Key concepts / interview angles
- Factory Method: subclass or function decides the concrete type; callers use only the interface.
- Abstract Factory: factory per family guarantees consistent products (all Dark or all Light).
- Builder: avoids telescoping constructors; return `*this` for chaining.
- Prototype: virtual `clone()` is the polymorphic copy constructor (avoids slicing).
- Singleton: Meyers singleton is thread-safe since C++11; discuss global state, testability and the alternative of dependency injection.

## Related
- `../../05_StaticMembers`
- `../../24_Concurrency/09_callOnce.cpp`
- `../../31_SOLIDPrinciples`
- `../../15_SmartPointers`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.cpp`); keep each `.cpp` self-contained and independently compilable.
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`Cpp/code/bin/` is git-ignored).
