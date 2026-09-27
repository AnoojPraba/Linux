# Constructors and Destructors -- interview notes

## Copy-and-swap idiom (`02_copyAndSwap.cpp`)

- `operator=` takes its parameter BY VALUE, so the (potentially throwing) copy
  happens while constructing that parameter, entirely before the function body
  runs. If the copy throws, `*this` has never been touched -- strong exception
  safety.
- `swap()` itself must be `noexcept` and use only non-throwing operations
  (pointer/scalar swaps) so nothing can go wrong once control reaches it.
- Self-assignment (`x = x;`) is handled for free with no explicit
  `if (this == &other)` check: the by-value parameter is just a copy of `x`,
  and swapping it back into `*this` is a harmless no-op.

## Rule of Zero (`03_ruleOfZero.cpp`)

- Contrast with the Rule of Three demo in `01_ruleOfThree.cpp`: `Buffer` there
  owns a raw resource directly (`char *`), so it must hand-write its
  destructor, copy constructor, and copy assignment operator.
- A class whose members are ALL RAII types (`std::string`, `std::vector`,
  `std::unique_ptr`, etc.) needs no custom destructor/copy/move members at
  all -- the compiler-generated ones simply call each member's own already
  correct special member functions.
- Rule of Zero is the modern preferred default. Rule of Three/Five only kicks
  in once a class directly owns a raw resource that no RAII wrapper already
  covers.

## `explicit` keyword (`04_explicitKeyword.cpp`)

- A single-argument (non-`explicit`) constructor doubles as an implicit
  conversion: passing an `int` where a class type is expected silently
  compiles, even when that's not what the caller meant.
- Marking that constructor `explicit` disables the implicit conversion --the
  same accidental call now fails to compile, catching the mistake at build
  time instead of at runtime (or never).
- Rule of thumb: mark single-argument constructors `explicit` unless the
  implicit conversion is genuinely intended (e.g. wrapper types meant to be
  interchangeable with the wrapped type).
