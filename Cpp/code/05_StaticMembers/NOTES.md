# Static Members -- interview notes

## Static initialization order fiasco (`02_staticInitOrderFiasco.cpp`)

- C++ does NOT guarantee the order in which non-local static objects (static
  storage duration, namespace/global scope) are constructed across DIFFERENT
  translation units. Within a single TU, order follows declaration order --
  but across TUs it's unspecified, and can even change with link order.
- If a static object's constructor in one TU depends on a static object
  defined in another TU already being initialized, the program's correctness
  depends on that unspecified order -- a crash or garbage result is possible
  if the dependency hasn't run yet.
- Honest caveat: truly demonstrating the fiasco needs two separate `.cpp`
  files linked into one binary. This repo's Makefile builds each `.cpp` file
  independently into its own binary (see `Cpp/code/Makefile`'s generic
  `$(BIN_DIR)/%: %.cpp` rule), so `02_staticInitOrderFiasco.cpp` is a
  single-file illustrative simplification with comments marking where the
  conceptual TU split would be, rather than an actual cross-TU crash.

## The fix: construct on first use

- Wrap the static object in a function that returns a reference to a
  function-local `static`:
  ```cpp
  Logger &getLogger()
  {
      static Logger instance("prefix");
      return instance;
  }
  ```
- C++11 guarantees function-local statics are initialized the first time
  control passes through their declaration -- lazily, on first call -- which
  sidesteps the cross-TU ordering problem entirely: there's no "startup order"
  to get wrong, because construction happens on demand.
- This initialization is also guaranteed thread-safe since C++11 ("magic
  statics"): if two threads call `getLogger()` for the first time
  concurrently, the standard guarantees only one of them actually runs the
  constructor, and the other blocks until it's done.
