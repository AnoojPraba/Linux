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

- Type erasure (`06_customAnyTypeErasure.cpp`): hides a concrete type behind a small
  abstract interface (an inner `Concept` base with pure virtual `clone()`) implemented by
  a templated `Model<T>` that actually holds the `T`. The outer `MyAny` class stores a
  single `std::unique_ptr<Concept>`, so one non-template class can hold a value of any
  copyable type. This is exactly the technique `std::any` is built on internally.
  Contrast with `std::variant` (see `20_ModernCppFeatures/04_optionalVariantAny.cpp`):
  `variant` is a closed-set, tagged union — it can only ever hold one of a fixed list of
  alternative types named in its template arguments, stored inline with no heap
  allocation, and visited exhaustively (`std::visit`, `07_stdVisitVariant.cpp`).
  `std::any`/`MyAny` is open-set true type erasure — it can hold literally any copyable
  type decided at the call site, at the cost of a heap allocation and needing a
  typeid/type_info check (`anyCast`) to get the value back out, with no compile-time
  exhaustiveness checking.
- `has_serialize<T>` (`07_sfinaeHasMember.cpp`): expression SFINAE via the `void_t`
  idiom. `std::void_t<decltype(expr)>` collapses to `void` when `expr` is well-formed for
  the given `T`, letting a specialization of a trait be selected only when a particular
  expression (here, `T{}.serialize()`) compiles; the primary template's `false_type`
  is picked via ordinary SFINAE otherwise. This kind of "does T have member X" detection
  is a common building block for generic serialization frameworks, ORMs, and
  compile-time interface checks. C++20 concepts (`30_Cpp20Features/01_conceptsFallback.cpp`,
  `30_Cpp20Features/NOTES.md`) express the same "does T support this expression" check far
  more directly via `requires { expr; }`, but this toolchain (GCC 8.5.0, `-std=c++17` only)
  can't compile real concepts, hence the `void_t` idiom here.

## C++20 concepts

- Concepts are the C++20 replacement for SFINAE-based constraints — see
  `30_Cpp20Features/NOTES.md` for the concepts/ranges writeup and
  `30_Cpp20Features/01_conceptsFallback.cpp` for the C++17 `enable_if` equivalent of the
  `add()` example above (this toolchain, GCC 8.5.0, can't compile real concepts).
