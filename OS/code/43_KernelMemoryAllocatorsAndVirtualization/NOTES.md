# Kernel Memory Allocators and Virtualization

Notes-only: the slab allocator, memory interleaving, and
namespace/cgroup-based containers are all kernel-internal or
multi-process-isolation mechanisms that can't be meaningfully demoed as a
small standalone userspace C program without either faking the interesting
part or needing root/kernel-module access - conceptual coverage here
instead, cross-referencing the runnable demos elsewhere in this repo.

## Slab allocator (on top of the buddy system)

- The kernel's own physical-page allocator is a **buddy system**
  (power-of-2 pages) - see `27_MemoryAddressingAndFragmentation` for a
  userspace-scale demo of the same splitting/coalescing mechanics. That's
  fine for page-sized-and-larger allocations, but the kernel constantly
  allocates many small, fixed-size, frequently-churned objects -
  `task_struct` (one per process/thread), inodes, dentries, network buffer
  headers. Handing each of those a whole buddy-system page would waste
  most of the page (internal fragmentation) and pay allocator overhead on
  every single alloc/free.
- **Slab allocator**: sits on top of the buddy system. It requests whole
  pages from the buddy allocator, then carves each page into many
  fixed-size "slots" sized exactly for one specific object type (a
  "cache", e.g. a dedicated cache for `task_struct`). Allocating/freeing an
  object of that type just grabs/returns a slot from its cache - no buddy
  system call, no internal-fragmentation waste for that page, and O(1)
  alloc/free.
- **Object caching**: beyond fast allocation, the slab allocator can keep
  freed objects in a *pre-initialized* state (the "constructor" run once
  per slot, not once per alloc) so that reusing a slot for a new object of
  the same type skips redundant init/teardown work - valuable for objects
  with expensive constructors (e.g. objects containing their own locks).
- **SLUB / SLQB**: later evolutions of the original SLAB allocator in
  Linux. **SLUB** (the current default) simplifies the original design's
  per-CPU queue bookkeeping, reducing metadata overhead and improving
  scalability on many-core machines; **SLQB** was an experimental
  queue-based alternative that was not ultimately merged. The high-level
  problem (fast, low-fragmentation allocation of same-sized kernel objects)
  is the same across all three - they differ in internal bookkeeping and
  per-CPU scalability tradeoffs.

## Memory interleaving

- On multi-channel/multi-bank memory systems, consecutive physical
  addresses can be striped ("interleaved") across multiple memory
  banks/channels rather than filling one bank at a time. This lets
  sequential/bursty access patterns spread load across channels and be
  serviced in parallel, raising effective memory bandwidth versus a purely
  linear single-bank layout.
- This is a distinct axis from NUMA node placement: interleaving spreads
  one region of memory *across* banks/channels for bandwidth, while NUMA
  affinity (see `40_NUMABasics`) is about keeping a thread's memory *within*
  one node for latency. In fact, `numactl --interleave=all` deliberately
  trades away NUMA locality for more even bandwidth across nodes when a
  workload is bandwidth-bound rather than latency-sensitive - the two
  concepts can be in direct tension depending on the workload.

## OS-level virtualization (containers) vs full hardware virtualization (VMs)

- **Full hardware virtualization (VMs)**: a hypervisor (e.g. KVM, Xen,
  VMware) emulates virtual hardware for each guest, and each guest runs its
  own complete OS kernel. Strong isolation (a guest kernel exploit generally
  can't escape to the host or other guests without also breaking the
  hypervisor), at the cost of overhead - each VM boots and runs a full
  kernel, duplicating memory/CPU accounting the host already does.
- **OS-level virtualization (containers)**: all containers share the one
  host kernel; isolation is enforced by kernel features, not a hypervisor -
  **namespaces** (PID, mount, network, UTS, IPC, user) give each container
  its own view of processes/filesystem/network/etc., and **cgroups**
  enforce resource limits (CPU, memory, I/O) per container/group of
  processes. No guest kernel to boot, so containers start near-instantly
  and share the host kernel's memory/scheduler overhead instead of
  duplicating it.
- **Trade-off**: containers are far lower-overhead (no per-guest kernel, near
  native performance, fast startup) but weaker isolation than VMs (a kernel
  vulnerability is shared by every container on that host, since there's
  only one kernel to exploit); VMs pay real overhead for a stronger
  isolation boundary (each guest's kernel is a separate attack surface from
  the others). Choosing between them is a direct security-vs-density
  trade-off, and hybrids exist (e.g. gVisor, Kata Containers) that try to
  interpose a lighter-weight isolation layer than a full traditional VM
  while still not sharing the host kernel directly.
- Cross-reference `42_CustomAllocator` for the userspace-allocator side of
  "avoid per-object overhead by reusing/pooling" - the slab allocator above
  is the same idea applied inside the kernel to its own fixed-size objects.
