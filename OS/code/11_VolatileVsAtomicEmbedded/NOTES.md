# volatile vs _Atomic

- `volatile` only prevents the compiler from caching a value in a register or
  eliding/reordering accesses to it. It does NOT provide atomicity and does
  NOT provide any cross-thread memory ordering guarantee.
- Correct `volatile` use case: a variable modified asynchronously by a signal
  handler (or memory-mapped hardware register in embedded code) and polled by
  normal code on the same thread. `sig_atomic_t` is the guaranteed-safe type
  for signal-handler flags.
- `volatile` does NOT make `x++` atomic - it is still a read-modify-write with
  a race window; two threads can both read the same old value.
- C11 `_Atomic` / `<stdatomic.h>` gives real atomicity (no torn
  read-modify-write) plus a defined memory ordering (default
  `memory_order_seq_cst`), which is what genuine multi-threaded shared-state
  access needs.
- Rule of thumb: signal handler / hardware register flag -> `volatile`.
  Data shared and mutated by multiple threads -> `_Atomic` or a mutex.
  Neither one is a substitute for the other.
- `volatile` + threads is a classic interview trap: Java's `volatile` has
  happens-before semantics; C/C++'s `volatile` does not - don't confuse them.
