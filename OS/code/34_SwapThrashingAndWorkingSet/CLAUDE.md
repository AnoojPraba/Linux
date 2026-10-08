# 34_SwapThrashingAndWorkingSet

Page replacement beyond FIFO/LRU: Optimal, Clock (second chance) and Belady's anomaly, plus swapping, thrashing and the working-set model.

## Files
- `01_optimalPageReplacement.c` - Belady's OPT: evict the page used farthest in the future (needs the whole reference string)
- `02_clockPageReplacement.c` - circular frames with a hand and reference bits
- `03_beladysAnomaly.c` - FIFO with 3 vs 4 frames on the classic string shows more faults with more frames
- `NOTES.md` - algorithm survey, Belady's anomaly, thrashing, working set

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_optimalPageReplacement.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/34_SwapThrashingAndWorkingSet/` (git-ignored).

## Key concepts / interview angles
- OPT is unimplementable online but is the benchmark.
- Clock approximates LRU cheaply with one reference bit per frame.
- Belady's anomaly affects FIFO, not stack algorithms (LRU, OPT).
- Thrashing: the working sets exceed RAM so the system spends its time swapping; fix by reducing multiprogramming, working-set/PFF control.
- Linux uses active/inactive LRU lists and swappiness.

## Related
- `../29_MemoryManagement/01_pageReplacement.c`
- `../33_VirtualMemoryDeepDive`
- `../41_NUMABasics`
- `../../../C_Basics/code/37_LRUCache`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
