# Hypervisors and Virtual Machines

Notes-only: hypervisor internals need either real hardware virtualization
extensions or a nested-virtualization setup this repo can't assume - this
covers the concepts, cross-referencing the runnable/related demos elsewhere.

## Type 1 (bare-metal) vs Type 2 (hosted) hypervisors

- **Type 1 (bare-metal)**: the hypervisor runs directly on the physical
  hardware - there is no general-purpose host OS underneath it at all; the
  hypervisor itself is effectively the OS. Examples: Xen, VMware ESXi,
  Microsoft Hyper-V. Lower overhead (no host OS layer to go through for
  privileged operations), which is why datacenter/cloud virtualization
  (the underlying platform for most public-cloud VM offerings) is
  overwhelmingly Type 1.
- **Type 2 (hosted)**: the hypervisor runs as an ordinary application on top
  of a full host OS (Windows, Linux, macOS), and guest VMs run under that
  application. Examples: VirtualBox, VMware Workstation, Parallels Desktop.
  Easier to set up on a desktop/laptop (install like any other app, no need
  to dedicate the whole machine to the hypervisor), but pays extra overhead
  from the host OS layer sitting between the guest and the real hardware.

## Full virtualization vs paravirtualization

- **Full virtualization**: the hypervisor presents each guest with
  virtual hardware that looks just like real hardware - the guest OS runs
  completely unmodified and is unaware it's virtualized. Privileged
  instructions the guest kernel issues (expecting to talk to real hardware)
  must be intercepted/trapped and emulated by the hypervisor; on x86 this is
  practical at reasonable performance only with hardware support for
  trapping those instructions (Intel VT-x / AMD-V), since some
  privileged x86 instructions historically didn't trap cleanly when run
  in user-mode ("ring compression") without such support.
- **Paravirtualization**: the guest OS is modified to be aware it's running
  under a hypervisor - instead of issuing normal privileged instructions
  that must be caught and emulated, it makes explicit "hypercalls" (like a
  syscall, but guest-to-hypervisor instead of userspace-to-kernel) for
  operations that need the hypervisor's involvement. Trades requiring a
  modified guest kernel (can't run an arbitrary unmodified OS) for
  meaningfully lower virtualization overhead, since there's no need to trap
  and decode instructions the hypervisor never sees coming. Classic Xen
  paravirtualized guests are the textbook example.

## Contrast with OS-level virtualization (containers)

Both VMs and containers virtualize/isolate workloads, but at different
layers - see `43_KernelMemoryAllocatorsAndVirtualization` for the detailed
comparison (namespaces, cgroups, containers sharing one host kernel vs each
VM booting its own full guest kernel, and the security/overhead trade-off
between the two). This folder deliberately focuses on the hypervisor/VM
side of that comparison rather than repeating it.

## Hardware-assisted virtualization extensions

- **Intel VT-x / AMD-V**: CPU extensions adding a new privilege mode for the
  hypervisor itself (VMX root/non-root mode on Intel) so the hypervisor
  doesn't need software tricks (binary translation, ring compression) to
  trap sensitive guest instructions - the CPU traps them directly in
  hardware.
- **Extended/Nested Page Tables (Intel EPT / AMD NPT)**: hardware support
  for a second level of address translation - guest-virtual to
  guest-physical (walked by the guest's own page tables, same as
  `30_PageTableEntriesAndTLB` describes) and then guest-physical to
  host-physical (walked by the hypervisor's page tables). Without this,
  the hypervisor would need to trap and emulate every guest page-table
  update ("shadow paging") to keep a single combined translation
  consistent - far more overhead than letting hardware walk both levels
  directly. This is the same page-table-walk/TLB-caching machinery covered
  in `30_PageTableEntriesAndTLB`, just with an extra translation layer
  added for the guest-physical-to-host-physical step.
