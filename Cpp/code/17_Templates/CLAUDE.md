# 17_Templates

Function and class templates: generic code resolved at compile time.

## Files
- `01_functionAndClassTemplates.cpp` - maxOf<T> function template and a class template; instantiation per type

## Build and run
- Single file: `g++ -std=c++17 -Wall -Wextra -pthread 01_functionAndClassTemplates.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `Cpp/code/bin/17_Templates/` (git-ignored).

## Key concepts / interview angles
- Templates are instantiated per type at compile time: no runtime cost, code bloat risk, errors surface at instantiation.
- Template definitions normally live in headers (the compiler needs the body to instantiate).
- Argument deduction vs explicit arguments; specialisation and overload resolution.
- `concepts` (C++20) constrain templates for readable errors.
- Deeper: SFINAE, variadics and metaprogramming in `../26_TemplateMetaprogramming`.

## Related
- `../26_TemplateMetaprogramming`
- `../27_STLInternals`
- `../30_Cpp20Features`
- `../../../C_Basics/code/31_Generics` - C11 _Generic comparison

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.cpp`); keep each `.cpp` self-contained and independently compilable.
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`Cpp/code/bin/` is git-ignored).
