# Classes and Objects -- interview notes

## `mutable` keyword (`02_mutableKeyword.cpp`)

- A `const` member function promises not to change the object's
  externally-visible ("logical") state, but the compiler by default enforces
  this via bitwise constness -- it simply forbids writing to any member at
  all inside a `const` method.
- `mutable` on a specific data member carves out an exception: that member
  can still be modified from a `const` method. This is intended for
  implementation-detail bookkeeping that doesn't affect what the object
  looks like from the outside -- e.g. a memoized/cached computed value, an
  access/call counter, or a mutex used only to guard internal synchronization.
- The distinction this demonstrates is "logical constness vs bitwise
  constness": the object's *observable* behavior/result is unchanged
  (logically const), even though some bits inside it did in fact change
  (not bitwise const). `mutable` is how C++ lets you express that gap.
