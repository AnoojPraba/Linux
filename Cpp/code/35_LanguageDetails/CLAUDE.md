# 35_LanguageDetails

Namespaces and ADL, iterator categories, if/switch initializers, container adapters, multilevel/diamond inheritance, escape sequences, C strings, input validation, array decay, enum and precedence, loops and switch.

## Files
- `01_namespacesAndAdl.cpp` - Anonymous/inline/nested namespaces, aliases, using-declaration vs directive, argument-dependent lookup.
- `02_iteratorCategories.cpp` - Detect input..contiguous categories with concepts, std::distance/advance cost, invalidation.
- `03_initStatementsAndBindings.cpp` - if/switch with initializer, structured bindings, [[fallthrough]], lock_guard scoped to an if.
- `04_containerAdaptersAndChoices.cpp` - stack/queue/priority_queue over chosen containers, list::splice, transparent set lookup, deque reference stability, forward_list.
- `05_multilevelAndDiamondInheritance.cpp` - Constructor/destructor order, slicing and the virtual-base diamond.
- `06_escapeSequencesAndRawStrings.cpp` - \n \t \\ \" \0 \x41, string with embedded NUL, raw string literals.
- `07_cStringsVsStdString.cpp` - strlen/sizeof, strcat/strncpy/strcmp/strchr/strstr, std::string +, +=, append.
- `08_cinInputValidation.cpp` - cin fail state: clear() + ignore() loop (reads stdin).
- `09_arrayDecayAnd2dArrays.cpp` - Array-to-pointer decay in parameters, reference-to-array template, 2-D arrays, std::array, vector<vector>.
- `10_enumClassAndPrecedence.cpp` - Plain enum vs enum class, precedence pitfalls (x & 1 == 0, 1 << 2 + 1).
- `11_doWhileSwitchBreakContinue.cpp` - do-while, switch fall-through, break/continue in nested loops.
- `NOTES.md` - concise interview-focused notes for this topic

## Build and run
- Single file: `g++ -std=c++20 -Wall -Wextra -pthread 01_namespacesAndAdl.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `Cpp/code/bin/35_LanguageDetails/` (git-ignored).

## Key concepts / interview angles
- **Namespaces:** anonymous namespace = internal linkage (replacement for file `static`); inline namespace = versioned API/ABI (`std::__cxx11`); using-declaration brings one name, using-directive all (never in a header). **ADL:** an unqualified call also searches the namespaces of its arguments' types - why `swap`/`begin`/`operator<<` are defined next to the type; `using std::swap; swap(a, b);` idiom.
- **Iterator categories:** input, forward, bidirectional, random access, contiguous (C++17/20). `std::sort` needs random access (not `list`); `lower_bound` on a `list` is O(n) steps; `distance`/`advance` O(1) only for random access. C++20 concepts (`std::random_access_iterator`) + `if constexpr` replace tag dispatch. Invalidation: `vector` push may invalidate all; `deque` push keeps references; `list`/`map` keep everything except erased.
- **Init statements:** `if (init; cond)` / `switch (init; v)` scope the variable to the statement (lock held exactly for the if); structured bindings take apart pair/tuple/struct (use `const auto &` in loops to avoid copies); `[[fallthrough]]` marks intentional fall-through.
- **Adapters:** `stack` needs push_back/pop_back, `queue` needs pop_front (deque/list, not vector), `priority_queue` is a heap in a vector (max by default). `list::splice` moves nodes in O(1) and keeps iterators; `deque` keeps references on end pushes; transparent comparator (`std::less<>`) lets `set<string>::find("literal")` skip building a string.
- **Inheritance:** construction base -> derived, destruction reverse; virtual calls in a constructor do not reach derived overrides; slicing loses derived data; base destructor must be virtual; `override`/`final`; a derived name hides all base overloads. **Diamond:** `virtual` base = one shared sub-object, constructed by the most derived class, extra vptr/offset cost; prefer composition or data-less interfaces.

## Gotchas
- The input-validation program reads stdin; run it with `printf 'abc\n-5\n42\n7x\n8\n' | ./bin/41_LanguageDetails/08_cinInputValidation`.
- Embedded NUL in a literal truncates a const char* conversion.
