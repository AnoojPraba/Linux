Three features that refine memory layout or cross-thread behavior beyond
what the earlier structs/dynamic-memory/threads notes already cover.

Atomics and thread-local storage (C11, `<stdatomic.h>`):

*   An atomic_int guarantees each operation on it runs as one indivisible
    step, without an explicit `pthread_mutex_t` - contrast with the
    mutex-protected plain int counter in `49_Threads/02_mutex.c`, which
    protects the same kind of shared counter but with an explicit
    lock/unlock. See `code/52_Atomics/01_stdatomicCounter.c`.
*   `_Thread_local` gives each thread its own independent copy of a
    variable - unlike an ordinary global, which every thread shares and
    needs a mutex to touch safely. No locking is needed for a
    thread-local variable, because there's nothing shared to race on. See
    `code/52_Atomics/02_threadLocalStorage.c`.

Variable-length and flexible-size arrays:

*   A VLA's size is a runtime value, not a compile-time constant, but it's
    still stack-allocated (freed automatically on scope exit, unlike
    malloc'd memory) - and unlike malloc, there's no way to detect a VLA
    that's too large; it simply overflows the stack. See
    `code/20_AdvancedArrays/01_variableLengthArray.c`.
*   A flexible array member (a struct's last member, declared with no
    size) isn't counted in `sizeof(struct)` at all, letting one malloc
    allocate both a struct's fixed fields and its variable-length payload
    as a single contiguous block - better cache locality and one free()
    instead of a struct-plus-separate-malloc'd-pointer design. See
    `code/20_AdvancedArrays/02_flexibleArrayMember.c`.

Bit-fields in structs:

*   A bit-field member claims exactly N bits instead of a whole byte/word;
    the compiler packs consecutive bit-fields together into as few bytes
    as possible. Useful for mirroring a hardware register or wire-format
    layout, at the cost of a bit order/padding that's implementation
    defined - unlike the explicit shifting/masking already used in
    `05_BitManipulation/`, which is fully portable. See
    `code/25_BitFields/01_bitFieldStruct.c`.
