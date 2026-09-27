# Cross-Compilation Basics

## What cross-compilation is

Cross-compilation is building a binary on one machine (the "host") that will
run on a different machine or architecture (the "target"), using a compiler
that runs on the host but emits code for the target's instruction set. It's
needed whenever the target either can't run a compiler itself (an embedded
MCU with no OS) or building natively there would be too slow/impractical
(cross-compiling for an embedded Linux board from a fast desktop instead of
building on the board itself).

This repo's own build, by contrast, has only ever been built NATIVELY: it was
verified on an x86_64 desktop using its own native `gcc`, and separately on a
Raspberry Pi (ARM) using the Pi's own native `gcc` - each machine compiled for
itself. Neither of those is cross-compilation, since in both cases the
compiler ran on the same architecture it was generating code for. True
cross-compilation would instead be building the ARM binary on the x86_64
desktop without ever running a compiler on the Pi at all - which becomes
mandatory for a target that can't self-host a compiler at all, such as a
bare-metal MCU with no operating system to run a compiler under.

## Toolchain triplets

A cross-compiler is named after a triplet describing what it targets, e.g.
`arm-linux-gnueabihf-gcc`:

- `arm` - the target architecture.
- `linux` - the target OS (or `none`/`elf` for bare-metal targets with no OS).
- `gnueabihf` - the ABI (here, the GNU EABI with hardware floating point).

## Sysroot

A sysroot is a directory tree that mirrors the target's `/usr/include` and
`/usr/lib` (headers and libraries built for the target architecture). The
cross-compiler is pointed at this sysroot instead of the host's own
`/usr/include`/`/usr/lib`, so it never accidentally picks up the host's
wrong-architecture headers or libraries when compiling or linking.

## CMake's CMAKE_TOOLCHAIN_FILE

CMake supports cross-compilation via a toolchain file that sets the target
compiler, target system name, and sysroot path, passed on the command line:

```
cmake -DCMAKE_TOOLCHAIN_FILE=/path/to/arm-toolchain.cmake ..
```

A toy example of such a file:

```cmake
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
set(CMAKE_FIND_ROOT_PATH /opt/arm-sysroot)
```

This contrasts with `71_CMakeIntroduction`'s demo, which never passes
`CMAKE_TOOLCHAIN_FILE` and simply configures/builds natively with the host's
default compiler.

## QEMU emulation

QEMU lets a cross-compiled binary be tested without the real target hardware:

- **User-mode emulation** (`qemu-arm ./binary`) runs a single foreign-
  architecture binary directly on the host, translating its instructions on
  the fly - useful for quickly testing one cross-compiled executable.
- **Full-system emulation** (`qemu-system-arm ...`) boots an entire emulated
  machine (CPU, memory, peripherals) so a complete embedded Linux image can
  be tested end-to-end without any physical board.
