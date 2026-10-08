# 37_LockFreeRingBuffer

Bounded lock-free queues: an SPSC ring buffer with plain atomics and a Vyukov-style MPMC queue, with a senior Q&A.

## Files
- `01_spscRingBuffer.c` - single producer/single consumer; each index has one writer so atomic loads/stores suffice
- `02_mpmcRingBuffer.c` - Vyukov bounded MPMC queue with a sequence number per slot and CAS on indices
- `NOTES.md` - SPSC, MPMC, and "Senior interviewer Q&A" (correctness, memory order, false sharing, one empty slot, power-of-two capacity, spin vs block, testing)

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_spscRingBuffer.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/37_LockFreeRingBuffer/` (git-ignored).

## Key concepts / interview angles
- SPSC needs release on publish and acquire on consume, not CAS; the demo uses default seq_cst (could be weakened).
- Keep head and tail on separate cache lines or they false-share.
- Power-of-two capacity turns modulo into a mask; one wasted slot (or a count) disambiguates full from empty.
- MPMC: per-slot sequence numbers make slots self-describing; MPMC is not always faster than a mutex queue under low contention.
- Decide how to wait when empty/full: spin, yield, futex.
- Test with TSan and stress with randomised producers/consumers.

## Related
- `../20_Atomics`
- `../39_FalseSharing`
- `../38_ConcurrentDataStructures`
- `../65_FutexAndSeqlock`
- `../../../C_Basics/code/32_StackAndQueue/08_uartRingBuffer.c`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
