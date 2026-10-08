# 30_Cpp20Features

C++20 concepts and ranges, with a C++17 fallback implementation and a note about the toolchain limitation.

## Files
- `01_conceptsFallback.cpp` - enable_if plus static_assert equivalent of `template <std::integral T> T add(T, T)`
- `NOTES.md` - environment limitation, concepts, ranges

## Build and run
- Single file: `g++ -std=c++17 -Wall -Wextra -pthread 01_conceptsFallback.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `Cpp/code/bin/30_Cpp20Features/` (git-ignored).

## Key concepts / interview angles
- Concepts name and check template requirements with readable diagnostics, replacing most SFINAE.
- Ranges: lazy view pipelines (`views::filter | views::transform`) over containers.
- Other C++20: `<=>`, `consteval`, coroutines, modules, `std::span`.

## Gotchas
- NOTES.md and the .cpp header say the toolchain is GCC 8.5.0 and lacks <concepts>/<ranges>; this machine now has g++ 12.2, where `-std=c++20` and both headers work. The checked-in demo is still the C++17 fallback.

## Related
- `../26_TemplateMetaprogramming`
- `../33_Cpp20Coroutines`
- `../34_SpanStringViewAndPmr`
- `../19_STLAlgorithms`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.cpp`); keep each `.cpp` self-contained and independently compilable.
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`Cpp/code/bin/` is git-ignored).
