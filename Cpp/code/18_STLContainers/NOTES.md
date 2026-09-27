# STL Containers -- interview notes

## Big theme

The STL gives you battle-tested, usually more optimized versions of the classic
data structures you'd otherwise hand-roll (linked list, stack, queue, heap,
hash table -- see the from-scratch C implementations in the sibling C_Basics
repo). Interviewers generally want to see BOTH:

- You understand what's happening under the hood (array-based heap sift-up/down,
  hash table bucket/resize logic, linked-list node relinking), and
- You can still reach for the idiomatic, correct, modern-C++ STL type day-to-day
  instead of reinventing it.

Knowing both sides answers the classic follow-up: "now show me how you'd
actually do this in real code."

## Container adaptors (easy to get wrong)

`std::stack`, `std::queue`, and `std::priority_queue` are container
**adaptors**, not independent data structures. Each one wraps an underlying
container and restricts its interface down to a specific access pattern:

- `std::stack<T, Container = std::deque<T>>` -- LIFO via push/pop/top.
- `std::queue<T, Container = std::deque<T>>` -- FIFO via push/pop/front/back.
- `std::priority_queue<T, Container = std::vector<T>, Compare = std::less<T>>`
  -- heap-ordered access via push/pop/top, maintained with push_heap/pop_heap
  style logic over the underlying container.

Because they're adaptors, you can rebind the second template parameter to a
different underlying container (e.g. `stack<int, vector<int>>` or
`stack<int, list<int>>`), as long as it supports the operations the adaptor
needs. This is a different concept from `std::list`, `std::vector`,
`std::unordered_map`, etc., which are genuine standalone containers.

## Complexity/behavior cheat sheet

- `std::list`: doubly-linked list. O(1) insert/erase given an iterator
  (unlike `std::vector`'s O(n) due to shifting), but no random access and
  weaker cache locality. `splice` moves nodes between lists without copying.
- `std::priority_queue`: conceptually the same array-based binary heap you'd
  build from scratch (see C_Basics' Heap folder for that from-scratch
  version) -- the STL version just hides the heap-invariant bookkeeping.
- `std::unordered_set` / `std::unordered_map`: hash tables where bucket
  count, load factor, and rehashing are all handled automatically -- the
  container rehashes into a larger bucket array once `load_factor()` would
  exceed `max_load_factor()`, instead of the caller manually detecting "too
  full" and resizing like a from-scratch hash table would.

## Small String Optimization -- SSO (`07_smallStringOptimization.cpp`)

- Most real-world strings are short. Allocating on the heap for every single
  `std::string`, even a 3-character one, would waste time on an allocation
  plus add a pointer indirection on every access. SSO avoids both: short
  strings are stored directly inside the `std::string` object's own memory
  (a small internal buffer), with no heap allocation at all.
- `sizeof(std::string)` is constant regardless of content (it's the size of
  the internal buffer/pointer/length/capacity bookkeeping struct, not the
  string's characters). You can empirically tell whether a given string is
  using SSO by comparing its `.data()` pointer against its own object
  address: for a short string under SSO, the data pointer falls inside the
  object's own `sizeof(std::string)` bytes; for a long string, the data
  pointer points to a separate heap allocation, far from the object itself.
- This is a **libstdc++-specific implementation detail**, not something the
  C++ standard guarantees -- the exact SSO buffer size and mechanism vary by
  standard library implementation (libstdc++, libc++, MSVC STL all differ).
  The general small-buffer-optimization concept (avoid a heap allocation for
  short strings) is common across implementations, but you should never
  assume anything about `std::string`'s memory layout in portable code, even
  though poking at it like this is a fun way to understand what's going on
  underneath.
