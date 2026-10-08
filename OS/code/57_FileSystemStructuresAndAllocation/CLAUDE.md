# 57_FileSystemStructuresAndAllocation

Conceptual notes on directory structures, path names, file allocation methods and access methods (NOTES-only).

## Files
- `NOTES.md` - directory structures, path names, allocation (contiguous, linked, indexed), access methods

## Build and run
- Notes-only folder: nothing to compile.

## Key concepts / interview angles
- Allocation: contiguous (fast, external fragmentation), linked/FAT (no random access), indexed/inode (random access, multi-level pointers).
- Directory structures: single-level, two-level, tree, DAG with links, general graph.
- Access: sequential, direct, indexed.
- B-trees / extents in modern filesystems (ext4, XFS, btrfs).

## Related
- `../58_FilesystemInternals`
- `../59_DiskSchedulingAlgorithms`
- `../60_SpoolingBufferingAndFreeSpace`
- `../../../C_Basics/code/40_BTreeAndBPlusTree`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
