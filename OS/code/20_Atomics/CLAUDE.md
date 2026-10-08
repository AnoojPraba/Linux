# 20_Atomics

C11 stdatomic and thread-local storage: atomic counters, memory ordering, patterns, pitfalls, with a senior Q&A.

## Files
- `01_stdatomicCounter.c` - atomic_int increment from several threads, no mutex
- `02_threadLocalStorage.c` - _Thread_local per-thread copy of a variable
- `NOTES.md` - what an atomic gives you, atomics vs volatile vs mutex, patterns, pitfalls, TLS, "Senior interviewer Q&A"

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_stdatomicCounter.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/20_Atomics/` (git-ignored).

## Key concepts / interview angles
- An atomic RMW is indivisible; memory_order choices (relaxed, acquire/release, seq_cst) control visibility of surrounding writes.
- Default seq_cst is safe; relaxed is for independent counters only.
- CAS loops and ABA problem; `atomic_is_lock_free` for the type.
- False sharing between adjacent atomics on one cache line (see `../39_FalseSharing`).
- Atomics are not volatile and not a substitute for protecting multi-word invariants.

## Related
- `../11_VolatileVsAtomicEmbedded`
- `../66_MemoryModelLitmusTests`
- `../37_LockFreeRingBuffer`
- `../../../Cpp/code/24_Concurrency/04_atomics.cpp`
- `../39_FalseSharing`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
