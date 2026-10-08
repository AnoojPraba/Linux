# 01_BootProcess

Conceptual walkthrough of what happens from power-on to a login prompt (NOTES-only); the classic "what happens when you press the power button" question.

## Files
- `NOTES.md` - six stages (power, BIOS/UEFI + POST, boot loader, kernel + init, services/daemons, login), BIOS vs UEFI, cross-references, interview framing

## Build and run
- Notes-only folder: nothing to compile.

## Key concepts / interview angles
- Explain why each stage exists: power must stabilise; POST validates hardware before trusting software; the bootloader exists because firmware can only load something small.
- BIOS (MBR, real mode, 512-byte first stage) vs UEFI (GPT, ESP, boot manager, Secure Boot).
- Kernel init: memory, scheduler, drivers, root filesystem, then PID 1 (init/systemd).
- Init brings up services; login/display manager ends the sequence.
- Follow-ups: initramfs, device tree on ARM (this machine is a Raspberry Pi), secure boot chain.

## Related
- `../44_FirmwareUpdateAndFlashStorage`
- `../70_InterruptPathAndKernelModules`
- `../62_LinkerAndLoaderMechanics`
- `../04_ProcessLifecycle`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
