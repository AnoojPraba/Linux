# 36_CopyOnWrite

Copy-on-write after fork(): parent and child share physical pages until one writes.

## Files
- `01_forkCow.c` - fork then write in the child; shows the parent's value is unchanged

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_forkCow.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/36_CopyOnWrite/` (git-ignored).

## Key concepts / interview angles
- After fork both page tables map the same read-only frames; a write fault triggers a private copy of just that page.
- Makes fork+exec cheap; Redis-style snapshotting (fork to persist) relies on it.
- Cost shows up as page-fault latency and memory growth if the parent keeps writing.
- `vfork`/`posix_spawn` avoid even copying page tables.

## Related
- `../02_Processes`
- `../33_VirtualMemoryDeepDive`
- `../35_MmapFile`
- `../../../SystemDesign/topics/30_LSMTreesAndStorageEngines`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
