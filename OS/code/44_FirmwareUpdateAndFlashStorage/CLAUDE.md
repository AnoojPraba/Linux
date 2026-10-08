# 44_FirmwareUpdateAndFlashStorage

Conceptual notes on OTA firmware updates (A/B banks), signing/verification and flash constraints (NOTES-only).

## Files
- `NOTES.md` - dual-bank A/B updates, firmware signing and verification, flash memory constraints

## Build and run
- Notes-only folder: nothing to compile.

## Key concepts / interview angles
- A/B banks allow atomic switch and rollback; confirm the new image before committing.
- Verify signature (and version for anti-rollback) in the bootloader before executing.
- Flash: erase before write, block-granular erase, limited erase cycles, wear levelling, power-fail safety.
- Resumable, power-fail-safe update protocols; delta updates save bandwidth.

## Related
- `../01_BootProcess`
- `../43_EmbeddedReliabilityAndPowerManagement`
- `../58_FilesystemInternals`
- `../../../SystemDesign/topics/23_DeploymentStrategies`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
