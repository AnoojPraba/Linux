# 40_BTreeAndBPlusTree

Conceptual notes on B-trees and B+ trees: why databases and filesystems use them (no code).

## Files
- `NOTES.md` - why B-trees minimise block I/O, structure, B+ tree distinction (data in leaves, linked leaves), why B+ trees back DB indexes and filesystems, cross-references

## Build and run
- Notes-only folder: nothing to compile.

## Key concepts / interview angles
- One node = one disk page, with a high fan-out, so tree height is tiny (3-4 levels for millions of keys).
- B+ tree: internal nodes hold keys only, all data in linked leaves, so range scans are sequential.
- Splits and merges keep nodes at least half full; know insert/split mechanics.
- Contrast with LSM trees (write-optimised) in SystemDesign.

## Related
- `../../../SystemDesign/topics/04_DatabaseIndexingAndQueryOptimization`
- `../../../SystemDesign/topics/30_LSMTreesAndStorageEngines`
- `../../../OS/code/57_FileSystemStructuresAndAllocation`
- `../39_SelfBalancingTrees`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
