# C++20 Features (concepts, ranges)

All C++20-specific material lives here rather than scattered across `20_ModernCppFeatures`
and `26_TemplateMetaprogramming`, since this toolchain can't actually compile C++20.

## Environment limitation

- Toolchain here is GCC 8.5.0: `-std=c++20` is not a recognized flag (only `-std=c++2a`
  is accepted), and even under `-std=c++2a` both `<ranges>` and `<concepts>` fail with
  "No such file or directory" — libstdc++ 8 ships neither header.
- Because the shared Makefile builds everything with `-std=c++17`, no real
  concepts/ranges code is checked in here — `01_conceptsFallback.cpp` shows the C++17
  `enable_if` + `static_assert` equivalent instead, with comments marking where real
  C++20 syntax would go.
- To exercise real C++20 concepts/ranges, use GCC >= 10 or Clang >= 10/16 with
  `-std=c++20`.

## Concepts

- Concepts replace SFINAE (`enable_if`) as the way to constrain templates, with much
  clearer compiler errors. Syntax: `template <std::integral T> T add(T a, T b)`, or an
  ad-hoc `requires` clause: `template <typename T> requires std::integral<T> T add(...)`.
- `01_conceptsFallback.cpp` implements the C++17 equivalent: `enable_if` gates the
  overload from substitution, and a `static_assert` inside the body gives a readable
  error message — this is exactly the ergonomics gap concepts were designed to close.
- Standard library concepts live in `<concepts>` (`std::integral`, `std::floating_point`,
  `std::same_as`, `std::convertible_to`, etc.); custom concepts are defined with the
  `concept` keyword and a `requires` expression.

## Ranges

- `<ranges>` (C++20) provides composable, lazy view adaptors over any range
  (`begin()`/`end()`-compatible type), e.g.
  `for (auto x : data | std::views::filter(pred) | std::views::transform(f))`
  — no intermediate containers are allocated; adaptors are evaluated lazily as iterated.
- Ranges also add constrained algorithm overloads (`std::ranges::sort(container)`)
  that take a range directly instead of an iterator pair, and are concept-checked.
- Not demonstrated here (unavailable on this toolchain) — see the environment
  limitation note above.
