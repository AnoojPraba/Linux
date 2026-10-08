# 47_HypervisorsAndVirtualMachines

Conceptual notes on Type 1/Type 2 hypervisors, full vs paravirtualization and hardware-assisted virtualization (NOTES-only).

## Files
- `NOTES.md` - Type 1 vs Type 2, full vs paravirtualization, contrast with containers, hardware virtualization extensions

## Build and run
- Notes-only folder: nothing to compile.

## Key concepts / interview angles
- Type 1 runs on bare metal (Xen, ESXi, KVM as part of the kernel); Type 2 runs as an application on a host OS.
- Paravirtualization modifies the guest to make hypercalls; full virtualization relies on trap-and-emulate or hardware assist.
- VT-x/AMD-V (ARM EL2) plus nested/extended page tables (EPT/NPT) remove most shadow-paging cost.
- Containers vs VMs trade isolation strength for density and start-up time.

## Related
- `../46_KernelMemoryAllocatorsAndVirtualization`
- `../68_ContainersFromScratch`
- `../../../SystemDesign/topics/35_ContainersAndKubernetesBasics`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
