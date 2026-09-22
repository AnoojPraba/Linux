# Template Metaprogramming

- Variadic templates: `template <typename... Args>` captures a parameter pack; fold
  expressions (C++17) collapse a pack with an operator in one expression, e.g.
  `(args + ...)` or `((std::cout << args), ...)`, replacing manual recursive unpacking.
- SFINAE ("Substitution Failure Is Not An Error"): when substituting template arguments
  produces an invalid type/expression in an overload's signature, the compiler silently
  drops that overload instead of erroring, letting other overloads be considered.
  `std::enable_if<Condition, T>::type` is the classic tool: it only has a nested `type`
  member when `Condition` is true, so it fails substitution (removing the overload)
  otherwise.
- CRTP (Curiously Recurring Template Pattern): `class Derived : public Base<Derived>`
  gives the base compile-time access to the derived type via `static_cast<Derived*>(this)`,
  enabling "static polymorphism" — dispatch resolved at compile time, no vtable/virtual
  call overhead, but no runtime polymorphism (can't store a `Base*` to different
  Derived types uniformly since each Base<Derived> is a distinct type).
- Type traits (`<type_traits>`): `std::is_same<A, B>` compares types at compile time;
  `std::conditional<Cond, A, B>::type` picks a type (not a value) based on a compile-time
  bool; `std::enable_if` constrains templates. In C++14+ prefer the `_v`/`_t` suffixed
  shorthands (`is_same_v<A,B>`, `conditional_t<...>`) over `::value`/`::type`.

## C++20 concepts

- Concepts are the C++20 replacement for SFINAE-based constraints — see
  `29_Cpp20Features/NOTES.md` for the concepts/ranges writeup and
  `29_Cpp20Features/01_conceptsFallback.cpp` for the C++17 `enable_if` equivalent of the
  `add()` example above (this toolchain, GCC 8.5.0, can't compile real concepts).
