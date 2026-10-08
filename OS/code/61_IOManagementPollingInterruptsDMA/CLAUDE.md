# 61_IOManagementPollingInterruptsDMA

Polling vs interrupt-driven I/O vs DMA, device drivers, buffering and rules for writing an ISR, simulated in user space.

## Files
- `01_pollingVsBlockingWait.c` - a background thread sets a flag after a delay to mimic device completion; busy-poll vs blocking wait
- `02_isrRules.c` - a POSIX signal handler as the closest user-space ISR: defer work to the main loop
- `NOTES.md` - polling, interrupts, DMA, device drivers, I/O buffering, rules for a correct ISR body

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_pollingVsBlockingWait.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/61_IOManagementPollingInterruptsDMA/` (git-ignored).

## Key concepts / interview angles
- Polling wastes CPU but has low latency and is predictable; interrupts free the CPU but cost context-save and latency; DMA moves bulk data without CPU copies, interrupting on completion.
- ISR rules: short, no blocking, no malloc/printf, only touch volatile/atomic shared state, defer work (top half/bottom half, tasklets, workqueues).
- Interrupt coalescing and NAPI switch between interrupts and polling under load.
- The demos cannot raise real interrupts; they only illustrate the structure.

## Gotchas
- The demos use `sleep` and signals, so each takes about a second.

## Related
- `../70_InterruptPathAndKernelModules`
- `../42_HardwareBuses`
- `../11_VolatileVsAtomicEmbedded`
- `../06_SignalHandling`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
