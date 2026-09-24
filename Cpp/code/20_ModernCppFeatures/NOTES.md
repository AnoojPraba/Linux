# Modern C++ Features (C++11-C++20)

- `auto`: deduces from the initializer (like a template parameter); `decltype(expr)`
  yields the exact declared type of an expression, useful in trailing return types
  (`auto f(A a, B b) -> decltype(a + b)`) when the return type depends on the parameters.
- Range-based `for`: works on anything with `begin()`/`end()` (or ADL-found free
  functions); structured bindings (`auto [k, v] : map`) unpack pairs/tuples/structs
  without naming intermediate variables.
- `constexpr`: evaluated at compile time when all inputs are constant expressions,
  otherwise falls back to a normal runtime call. `if constexpr` discards the untaken
  branch at compile time (no instantiation), which is what makes template code that
  would otherwise fail to compile for some types actually compile.
- `std::optional<T>`: represents "may or may not have a value" without sentinel values
  or exceptions; `std::variant<T...>`: a type-safe tagged union, `std::get` throws
  `std::bad_variant_access` on the wrong type; `std::any`: type-erased single-value
  container, `std::any_cast` throws `std::bad_any_cast` on mismatch.
- `std::string_view`: a non-owning (pointer + length) view into existing character data.
  Avoids copies for read-only string parameters, but can dangle if the underlying
  storage is freed, reallocated, or was a temporary — never store a string_view longer
  than the data it views.
- Rule of five: if a class needs a custom destructor (because it owns a raw resource),
  it should also define copy ctor/assign and move ctor/assign (or `= delete` them).
  Move operations should be `noexcept` so containers like `std::vector` prefer moving
  over copying during reallocation.

## C++20 features (ranges, concepts)

- C++20-specific material (concepts, ranges) has been moved to its own folder,
  `30_Cpp20Features/`, since this toolchain (GCC 8.5.0) can't actually compile C++20 —
  see that folder's NOTES.md for the details and environment limitation.
