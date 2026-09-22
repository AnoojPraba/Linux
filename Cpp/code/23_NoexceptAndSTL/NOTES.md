# noexcept and the STL

- Strong exception guarantee: an operation either fully succeeds, or fails with no
  observable side effects (state rolled back to before the call). `vector::push_back`
  offers this guarantee when it reallocates.
- Why containers fall back to copy when move isn't noexcept:
  - During reallocation, `vector` must transfer every existing element from the old
    buffer to a new, larger one.
  - If it moved elements and a move threw partway through, the vector would be stuck:
    some elements already moved-from (and possibly destroyed) in the old buffer, the new
    buffer half-populated - there is no way to roll back a move, so the strong guarantee
    would be broken.
  - Copying, by contrast, leaves the old buffer untouched until every element has been
    successfully copied; if a copy throws partway through, the partially-built new buffer
    is discarded and the original vector is unaffected - guarantee preserved.
  - So `vector` (via `std::move_if_noexcept`) only moves an element type during
    reallocation if its move constructor is `noexcept` (or if the type has no copy
    constructor at all, leaving no safer alternative); otherwise it copies, even though a
    move constructor exists.
- `std::is_nothrow_move_constructible<T>` is the trait that answers "will `T`'s move
  constructor be selected here" - useful both as a `static_assert` sanity check and as a
  building block inside generic code that wants the same conditional behavior.
- When to mark noexcept:
  - Mark move constructors/move assignment `noexcept` whenever they truly cannot throw
    (typically: they only move members / swap pointers, no allocation). This is the
    single biggest practical reason to use `noexcept` - it unlocks move-based
    reallocation in `vector` and similar containers, turning O(n) copies into O(n) cheap
    moves.
  - Do not mark a function `noexcept` if it can genuinely throw (e.g. it allocates) -
    violating a `noexcept` promise calls `std::terminate` immediately, which is far worse
    than an uncaught exception propagating normally.
  - Destructors are implicitly `noexcept` by default - do not disable that.
