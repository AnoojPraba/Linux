# 40_CacheCoherenceMESI

Conceptual notes on the MESI cache coherence protocol and why it matters for concurrent code (NOTES-only).

## Files
- `NOTES.md` - why coherence is needed, the four MESI states, typical transitions, implications for concurrent code

## Build and run
- Notes-only folder: nothing to compile.

## Key concepts / interview angles
- Modified, Exclusive, Shared, Invalid per cache line per core; writes need ownership (RFO / invalidate others).
- Cache-line ping-pong is the cost of contended writes and of false sharing.
- Coherence is not consistency: memory ordering still needs barriers/atomics (store buffers).
- Variants: MOESI/MESIF; snooping vs directory-based at scale (NUMA).

## Related
- `../39_FalseSharing`
- `../41_NUMABasics`
- `../66_MemoryModelLitmusTests`
- `../20_Atomics`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
