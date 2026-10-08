# 60_SpoolingBufferingAndFreeSpace

Conceptual notes on buffering, spooling and free-space management (bitmap, linked list, grouping) (NOTES-only).

## Files
- `NOTES.md` - buffering, spooling (SPOOL), free space management, related topics

## Build and run
- Notes-only folder: nothing to compile.

## Key concepts / interview angles
- Buffering smooths speed mismatch between producer and consumer (single, double, circular).
- Spooling queues output for a device that cannot interleave jobs (printer queue).
- Free-space structures: bitmap (fast contiguous search), linked list, grouping, counting, space maps.
- Trade-offs: bitmap size vs scan speed.

## Related
- `../59_DiskSchedulingAlgorithms`
- `../57_FileSystemStructuresAndAllocation`
- `../61_IOManagementPollingInterruptsDMA`
- `../../../C_Basics/code/32_StackAndQueue`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
