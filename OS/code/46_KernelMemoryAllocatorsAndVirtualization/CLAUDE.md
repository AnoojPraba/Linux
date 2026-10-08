# 46_KernelMemoryAllocatorsAndVirtualization

Conceptual notes on kernel allocators (buddy and slab), memory interleaving, and OS-level vs hardware virtualization (NOTES-only).

## Files
- `NOTES.md` - slab allocator on top of the buddy system, memory interleaving, containers vs VMs

## Build and run
- Notes-only folder: nothing to compile.

## Key concepts / interview angles
- Buddy system hands out physically contiguous power-of-two page blocks; slab/SLUB caches fixed-size kernel objects on top of it to cut fragmentation and init cost.
- `kmalloc` (physically contiguous, size classes) vs `vmalloc` (virtually contiguous).
- Memory interleaving spreads consecutive addresses across channels/banks for bandwidth.
- Containers share one kernel (namespaces + cgroups); VMs run separate kernels on virtual hardware.

## Related
- `../28_MemoryAddressingAndFragmentation/02_buddySystem.c`
- `../47_HypervisorsAndVirtualMachines`
- `../68_ContainersFromScratch`
- `../../../C_Basics/code/81_MallocInternalsAndAllocators/02_slabSizeClasses.c`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
