# 32_SegmentationAndOverlays

Conceptual notes on segmentation, segmentation vs paging, hybrid schemes and overlays (NOTES-only).

## Files
- `NOTES.md` - segmentation, segmentation vs paging, x86 hybrid, why "segmentation fault" is named so, overlays

## Build and run
- Notes-only folder: nothing to compile.

## Key concepts / interview angles
- Segments are logical units (code, data, stack) with base+limit; external fragmentation, but natural protection/sharing.
- Paging has no external fragmentation but ignores program structure; x86 protected mode combined both, x86-64 mostly flat.
- "Segfault" originates from a segment-limit violation; today it means an invalid page access (SIGSEGV).
- Overlays: a pre-virtual-memory technique to run programs larger than RAM by swapping code regions manually.

## Related
- `../30_Paging`
- `../31_PageTableEntriesAndTLB`
- `../28_MemoryAddressingAndFragmentation`
- `../../../C_Basics/code/79_StackFramesAndCallingConvention`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
