# I/O Management: Polling, Interrupts, and DMA

Notes-only for the polling/interrupt/DMA comparison and buffering technique
detail (a small demo contrasting polling vs an event-driven wait is in
`01_pollingVsBlockingWait.c`, and a demo of the rules for writing a correct
ISR body is in `02_isrRules.c`, see the section below); the deeper
device-driver/ioctl and buffering-vs-spooling ground is covered by the
cross-referenced folders below rather than repeated here.

## Polling (programmed I/O)

- The CPU repeatedly reads a device's status register in a busy-wait loop,
  checking whether it's ready for the next data transfer, until it is.
  Simple to implement and has essentially zero latency once the device
  actually becomes ready (no interrupt dispatch/context-switch overhead),
  but wastes CPU cycles the entire time the device isn't ready - the CPU
  can do nothing else while polling.

## Interrupt-driven I/O

- Instead of the CPU checking the device, the device raises a hardware
  interrupt when it's ready (or when a transfer completes); the CPU is free
  to run other work in the meantime, and the interrupt service routine
  (ISR) handles the request when it fires.
- **Overhead trade-off vs polling**: interrupt-driven I/O wins for slow or
  infrequent devices (keyboard, mouse, most disk/network completions) since
  the CPU would otherwise waste huge amounts of time busy-waiting between
  rare events. Polling can actually win for extremely fast, high-frequency
  I/O, because each interrupt has real fixed overhead (trap into the
  kernel, save/restore context, dispatch to the ISR, return) - if a device
  produces data faster than that overhead can be amortized, polling in a
  tight loop (as high-throughput networking code, e.g. DPDK-style
  poll-mode drivers, deliberately does) avoids paying the per-event
  interrupt tax entirely. This overhead-crossover point is a classic
  interview discussion point.

## DMA (Direct Memory Access)

- A DMA controller transfers data directly between a device and main
  memory without the CPU being involved in each individual byte/word
  transfer. The CPU only programs the DMA controller once per transfer
  (source address, destination address, and length), then the DMA
  controller performs the entire bulk transfer independently.
- The CPU receives exactly ONE interrupt when the whole transfer completes,
  instead of one interrupt per byte/word as plain interrupt-driven I/O
  would require for the same volume of data. For bulk transfers (disk I/O,
  network packet DMA rings, graphics framebuffers) this is a massive
  reduction in interrupt overhead and frees the CPU to do other work for
  the entire duration of the transfer, not just between individual
  interrupts.

## Device drivers

- The kernel abstraction layer between the generic I/O subsystem
  (`read()`/`write()`/`ioctl()` calls applications make) and the actual
  device-specific hardware control logic. See `08_SystemCalls`'s
  `04_deviceManagementIoctl.c` for a runnable `ioctl()` demo - not repeated
  here.
- **Character devices**: accessed as an unstructured byte stream, no fixed
  block size, typically not seekable in a meaningful random-access sense
  (serial ports, keyboards, `/dev/null`, `/dev/random`).
- **Block devices**: accessed in fixed-size blocks with kernel-managed
  buffering/caching in between (disks, SSDs) - random access by block
  number is a first-class operation, and the kernel's page/buffer cache
  sits in front of the actual device to avoid redundant physical I/O.

## I/O buffering techniques

`60_SpoolingBufferingAndFreeSpace` already covers the buffering-vs-spooling
distinction (buffering smooths one ongoing stream, spooling queues whole
independent jobs) - this adds the specific buffering techniques themselves:

- **Single buffering**: one buffer between the device and the consuming
  process. Simple, but the producer and consumer can't overlap - the buffer
  must be fully drained (or filled) before the other side can touch it
  again, so I/O and processing are strictly serialized.
- **Double buffering**: two buffers used alternately - while one buffer is
  being filled by the device (or consumed by the process), the other is
  being consumed (or filled) at the same time. This overlaps I/O with
  processing instead of serializing them, which is the whole point: the
  producer and consumer can each work on a different buffer concurrently.
- **Circular buffering**: more than two buffers (or one buffer logically
  split into many slots) arranged in a ring, with separate read/write
  cursors that wrap around. Generalizes double buffering to allow multiple
  outstanding chunks in flight at once, which helps absorb bursty
  production/consumption rate mismatches without stalling either side as
  quickly as a two-buffer scheme would. See `37_LockFreeRingBuffer` for a
  concrete lock-free circular buffer implementation.

## Rules for writing a correct ISR body

This is a distinct, narrower angle from the polling-vs-interrupt tradeoff
above: given that a device raises an interrupt, what rules govern the code
inside the ISR itself? `02_isrRules.c` demonstrates this using a POSIX signal
handler, the closest thing to a real ISR reachable from userspace C - its
top-of-file comment explicitly notes that a real embedded ISR has additional
hardware-specific constraints this demo can't show, since it can only
illustrate the constraints that ARE demonstrable in a signal-handler context.

- **Shared state must be `volatile sig_atomic_t`**: the flag/data a handler
  sets and the main code reads must use this type, exactly as already
  covered in `11_VolatileVsAtomicEmbedded/01_volatileNotAtomic.c` - not
  repeated here beyond reusing the pattern correctly.
- **Keep the handler minimal**: the handler should only set a flag or copy a
  small piece of data, then return immediately. Any real (potentially slow)
  work is deferred to the main loop, which checks the flag afterward and does
  the actual processing outside the interrupt context. `02_isrRules.c`'s
  `handleSignal()` does only the minimal flag-set/data-copy; `main()`'s loop
  does the deferred `processDeferredWork()` call.
- **Rules a real embedded ISR must additionally follow**, beyond what a
  userspace signal handler can demonstrate:
  - never call a blocking function - the rest of the system may effectively
    be waiting on this ISR to finish.
  - never call a non-reentrant function - most of the standard C library,
    including `malloc()`/`printf()`, is not guaranteed async-signal-safe or
    reentrant, so calling it from a real ISR risks corrupting state shared
    with the code it interrupted.
  - keep execution time as short as possible - the ISR (or at least its
    interrupt line/priority level) blocks other interrupts for its entire
    duration.
  - avoid floating point on architectures where interrupt entry/exit doesn't
    automatically save/restore FPU context, unless it's saved/restored
    manually.
  - clear/acknowledge the interrupt source register before returning, or the
    same interrupt condition immediately re-fires.
