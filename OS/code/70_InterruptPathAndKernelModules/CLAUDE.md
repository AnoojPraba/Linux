# 70_InterruptPathAndKernelModules

Conceptual notes on the interrupt path from device to handler, top/bottom halves, latency sources and writing kernel modules, with a senior Q&A (NOTES-only).

## Files
- `NOTES.md` - device-to-handler path, latency sources (RT/embedded), kernel modules, "Senior interviewer Q&A"

## Build and run
- Notes-only folder: nothing to compile.

## Key concepts / interview angles
- Path: device raises IRQ, interrupt controller, CPU vectors to the top-half handler (fast, interrupts masked), deferred work in softirq/tasklet/workqueue (bottom half).
- Latency sources: interrupt masking, long ISRs, preemption-disabled sections, cache/TLB effects, SMIs; PREEMPT_RT makes most of these preemptible.
- Kernel modules: `insmod`/`modprobe`, `module_init`/`module_exit`, no libc, `printk`, GFP flags, error unwinding; a crash is a kernel panic.
- Threaded IRQs and IRQ affinity.

## Gotchas
- No kernel module code is built here; nothing to compile.

## Related
- `../61_IOManagementPollingInterruptsDMA`
- `../71_EbpfAndTracingBasics`
- `../23_RTOSConceptsAndTaskScheduling`
- `../42_HardwareBuses`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
