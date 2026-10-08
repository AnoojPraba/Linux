# Interrupt Path, Bottom Halves and Kernel Modules

NOTES-only: needs kernel headers/root/hardware this repo's plain-gcc setup can't
assume. The module skeleton is in the code block below (not compiled by `make`).

## From device to handler
1. Device asserts an IRQ line (or sends an MSI/MSI-X message over PCIe).
2. Interrupt controller (APIC / GIC) routes it to a CPU; CPU finishes the
   current instruction, switches to kernel mode, saves minimal state, vectors
   through the IDT (x86) / vector table (ARM).
3. Kernel entry code -> `do_IRQ` / `generic_handle_irq` -> the driver's
   registered handler (`request_irq`). Local interrupts of that line are
   masked while it runs.
- **Top half (hard IRQ):** runs with interrupts (at least that line) disabled.
  Must be SHORT: ack the device, read/clear status, grab the data, schedule the
  rest. No sleeping, no blocking locks, no `GFP_KERNEL`, no `copy_to_user`.
- **Bottom half:** deferred work run with interrupts enabled:
  - **softirq** (NET_RX/NET_TX, TIMER, BLOCK, RCU, TASKLET): per-CPU, runs on
    IRQ exit or in `ksoftirqd`; same type can run concurrently on several CPUs;
    cannot sleep. The network stack lives here (NAPI).
  - **tasklet:** built on softirqs; a given tasklet is serialized (never
    concurrent with itself); cannot sleep. Being phased out.
  - **workqueue:** kernel threads (`kworker`); process context, CAN sleep and
    take mutexes. Use when you need to block.
  - **threaded IRQ** (`request_threaded_irq`): top half minimal, handler body in
    a dedicated kernel thread (also what PREEMPT_RT does for most IRQs).
- **NAPI:** under load a NIC would interrupt per packet (livelock). NAPI masks
  the NIC IRQ after the first one and POLLS the ring from softirq in batches,
  re-enabling IRQs when the queue drains. Related: interrupt coalescing, RSS
  (multiple RX queues spread across CPUs), `irqbalance`, `smp_affinity`.
- **Context rules:** *process context* may sleep; *interrupt/atomic context*
  may not (`might_sleep()` warns: "BUG: scheduling while atomic"). Locks used
  in both contexts need `spin_lock_irqsave` to avoid self-deadlock (IRQ
  arriving while the same CPU holds the lock in process context).
- Observe: `/proc/interrupts`, `/proc/softirqs`, `mpstat -I ALL`,
  `perf record -e irq:*`, `trace-cmd`, `top` (`hi`/`si` columns).
- User-space analogy: signal handlers (`../../../C_Basics/code/80_SignalSafetyAndThreadLocal`) -
  tiny, async-safe, defer real work. Embedded ISR rules in `../61_IOManagementPollingInterruptsDMA`.

## Latency sources (RT/embedded interviews)
Interrupt latency = hardware + time interrupts stay disabled + entry cost;
long `irqsave` sections, SMIs, cache/TLB misses and CPU idle exit latency add
jitter. PREEMPT_RT threads IRQs and makes spinlocks sleepable to bound it.

## Kernel modules
- A module is a relocatable ELF object (`.ko`) linked into the running kernel
  by `insmod`/`modprobe`; runs in ring 0 with full privilege - a bug panics
  the machine. Must match the kernel version/config (`vermagic`); only
  `EXPORT_SYMBOL`'d kernel symbols are linkable; `GPL` license needed for
  `EXPORT_SYMBOL_GPL` symbols (and unlocks taint-free loading).
- Build with kbuild (`obj-m += hello.o`; `make -C /lib/modules/$(uname -r)/build M=$PWD modules`).
- `printk`/`pr_info` -> `dmesg`. No libc, no floating point, small fixed
  kernel stack (8-16 KB), allocate with `kmalloc(size, GFP_KERNEL|GFP_ATOMIC)`.
- Device interfaces: char device (`cdev`, `file_operations`: open/read/write/
  `unlocked_ioctl`/mmap/poll), `/proc`, `/sys` (sysfs attributes), debugfs,
  misc devices, platform drivers bound by device tree (embedded Linux).
- Data exchange with user space: `copy_to_user`/`copy_from_user` (they validate
  the user pointer and can fault/sleep) - never dereference user pointers directly.

```c
/* hello.c - minimal module (kbuild: obj-m += hello.o) */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int __init hello_init(void)
{
    pr_info("hello: loaded\n");
    return 0;                       /* non-zero aborts the load */
}

static void __exit hello_exit(void)
{
    pr_info("hello: unloaded\n");
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("minimal example");
```

## Senior interviewer Q&A
**Q: Why split interrupt handling into top and bottom halves?**
A: Hard IRQ context blocks other interrupts and cannot sleep, so keep it to the
minimum (acknowledge hardware, grab data) to bound interrupt latency; the long
processing runs later in softirq/tasklet/workqueue with interrupts enabled.

**Q: When do you pick a workqueue over a tasklet/softirq?**
A: When the work must sleep (mutexes, memory allocation with GFP_KERNEL, I/O).
Softirq/tasklet run in atomic context and cannot sleep; workqueues run in
process context at the cost of scheduling latency.

**Q: What is an interrupt storm/livelock and how does NAPI help?**
A: A flood of packets causes so many IRQs the CPU does nothing but handle them
(receive livelock). NAPI disables the device IRQ and polls in batches while
load is high, switching back to interrupts when idle.

**Q: Why use `spin_lock_irqsave` in code shared with an ISR?**
A: If an interrupt fires on the CPU that holds the lock in process context and
the ISR tries to take the same lock, it spins forever (deadlock). Disabling
local interrupts while holding the lock prevents that.

**Q: How would you debug a kernel module crash?**
A: Read the oops in `dmesg` (RIP/PC, call trace, taint flags), map the address
with `addr2line`/`gdb vmlinux` or `objdump -dS`, enable `KASAN/KCSAN/lockdep`
in a debug kernel, use `kdump/crash`, `ftrace`/`perf` for timing, and test in a
VM (QEMU) rather than the host.

**Q: How does a user process talk to a driver?**
A: Through a device node (`/dev/foo`) with `read/write/ioctl/mmap/poll`,
or sysfs/procfs/netlink; the syscall enters the VFS, which dispatches to the
driver's `file_operations`. Avoid `ioctl` ABI breakage by versioning structs.

**Q: What is DMA and how does it relate to interrupts?**
A: The device moves data to/from memory without CPU involvement and raises an
interrupt on completion. Requires DMA-capable buffers (`dma_alloc_coherent`,
IOMMU mapping), cache coherence handling, and ring descriptors. See
`../61_IOManagementPollingInterruptsDMA`.
