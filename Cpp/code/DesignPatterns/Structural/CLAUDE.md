# Structural

Structural design patterns in C++17: composing classes and objects (Adapter, Composite, Decorator, Facade, Proxy), one self-contained demo per pattern.

## Files
- `Adapter.cpp` - LegacyAviPlayer adapted to the ModernPlayer interface
- `Composite.cpp` - files and folders share one interface; folders own children via unique_ptr
- `Decorator.cpp` - coffee with stackable condiments wrapping the base without modifying it
- `Facade.cpp` - home-theatre facade coordinating several subsystems for "watch a movie"
- `Proxy.cpp` - lazy-loading, access-controlled proxy for a large image (RealImage built on first display)

## Build and run
- Single file: `g++ -std=c++17 -Wall -Wextra -pthread Adapter.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `Cpp/code` builds each file into `bin/DesignPatterns/<Group>/<Name>` (git-ignored).

## Key concepts / interview angles
- Adapter: wrap an incompatible interface you cannot change.
- Composite: treat leaves and containers uniformly; recursion over the tree.
- Decorator: same interface wrapping the component; composition instead of subclass explosion.
- Facade: one simple entry point over a complex subsystem (does not forbid direct use).
- Proxy: same interface as the real subject; virtual (lazy), protection, remote or caching proxies.

## Related
- `../../08_Inheritance`
- `../../28_PImplIdiom`
- `../../15_SmartPointers`
- `../../31_SOLIDPrinciples`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.cpp`); keep each `.cpp` self-contained and independently compilable.
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`Cpp/code/bin/` is git-ignored).
