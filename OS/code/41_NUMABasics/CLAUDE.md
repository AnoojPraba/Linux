# 41_NUMABasics

Conceptual notes on NUMA: local vs remote memory access, node affinity, first-touch policy and libnuma (NOTES-only).

## Files
- `NOTES.md` - UMA vs NUMA, local vs remote cost, node affinity, first-touch, libnuma

## Build and run
- Notes-only folder: nothing to compile.

## Key concepts / interview angles
- Remote access costs roughly 1.5-2x local (NOTES figure); bandwidth contention on interconnects.
- Linux first-touch policy places a page on the node of the thread that first writes it, so initialise data on the thread that will use it.
- Tools: `numactl --hardware`, `numactl --cpunodebind --membind`, `numastat`, libnuma (`numa_alloc_onnode`).
- Interacts with thread pinning, memory allocators and the scheduler.

## Gotchas
- A Raspberry Pi is a single-node (UMA) system, so nothing here can be measured on this machine.

## Related
- `../40_CacheCoherenceMESI`
- `../39_FalseSharing`
- `../46_KernelMemoryAllocatorsAndVirtualization`
- `../34_SwapThrashingAndWorkingSet`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
