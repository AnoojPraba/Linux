# 72_CrossCompilationBasics

Conceptual notes on cross-compilation: toolchain triplets, sysroot, CMake toolchain files and QEMU emulation (no code).

## Files
- `NOTES.md` - what cross-compilation is, toolchain triplets, sysroot, CMAKE_TOOLCHAIN_FILE, QEMU emulation

## Build and run
- Notes-only folder: nothing to compile.

## Key concepts / interview angles
- Build/host/target machines; triplet `arch-vendor-os-abi` (e.g. `aarch64-linux-gnu`).
- The sysroot supplies target headers and libs; point the compiler at it with `--sysroot`.
- A CMake toolchain file sets compilers and `CMAKE_FIND_ROOT_PATH` modes.
- `qemu-user` runs foreign binaries for quick tests.

## Related
- `../71_CMakeIntroduction`
- `../29_CompilerPipelineWalkthrough`
- `../../../OS/code/62_LinkerAndLoaderMechanics`
- `../../../OS/code/01_BootProcess`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
