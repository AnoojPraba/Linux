# STL Internals

- `std::vector` growth: doubles capacity (implementation-defined factor, commonly 2x,
  libstdc++ uses 2x) when it needs to grow, giving amortized O(1) push_back even though
  any single growing push_back is O(n) (copy/move all elements to the new buffer).
  `reserve()` avoids repeated reallocations when the final size is known up front.
- Iterator invalidation:
  - `vector`: reallocation (growth past capacity) invalidates ALL iterators, pointers,
    and references. `erase()` invalidates the erased element's iterator and everything
    after it (elements shift down); the iterator `erase()` returns is valid.
  - `list`/`map`/`set`: node-based, so inserting/erasing other elements never
    invalidates iterators to elements that were not themselves erased — this is the
    key tradeoff against vector's contiguous storage.
- `std::map` vs `std::unordered_map`:
  - `map`: red-black tree, keys always iterated in sorted order,
    find/insert/erase are O(log n).
  - `unordered_map`: hash table, no ordering guarantee, average O(1)
    find/insert/erase but O(n) worst case (hash collisions/rehashing); iteration order
    is unspecified and can change after rehashing.
  - Prefer `unordered_map` for raw lookup speed when order doesn't matter; prefer `map`
    when sorted iteration or stable ordering is required.
- Custom iterators: the minimum useful iterator needs `operator*`, `operator++`,
  `operator==`/`operator!=`, and typically the five `iterator_traits` typedefs
  (`iterator_category`, `value_type`, `difference_type`, `pointer`, `reference`) so
  generic `<algorithm>`/range-based-for code can use it. `input_iterator_tag` is the
  weakest category (single-pass, forward-only); `forward_iterator_tag` and above allow
  multi-pass algorithms.
