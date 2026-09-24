# Compiler Pipeline Walkthrough

Companion demo: `01_pipelineDemo.c` - a trivial "hello world plus one
function call" program with nothing interesting in it, so that the value of
this folder is running each of the four stages below on it and inspecting
the artifact each one produces, rather than reading the source itself.

## 1. Preprocessing - `gcc -E`

`gcc -E 01_pipelineDemo.c` runs only the preprocessor: macro expansion,
`#include` inlining, and conditional compilation (`#if`/`#ifdef`). The
output is the full translation unit with every header textually pasted in -
for a file that just includes `<stdio.h>`, that is typically thousands of
lines, almost all of it declarations pulled in from the C library headers.
Don't paste that output into a file here; instead run it yourself and page
through it:

```
gcc -E 01_pipelineDemo.c | less
```

Scroll past the expanded header noise near the top and look near the
bottom - `addOne` and `main` are still there, essentially unchanged, since
this file has no macros of its own to expand.

## 2. Compilation - `gcc -S`

```
gcc -S 01_pipelineDemo.c
```

This produces `01_pipelineDemo.s`, the generated x86 assembly. This is
where the vast majority of optimization work happens (`-O0` through `-O3`
all change this stage's output, not the later ones) - inlining, register
allocation, instruction selection, and dead-code elimination all happen
while going from preprocessed C to assembly.

## 3. Assembly - `gcc -c` / `as`

```
gcc -c 01_pipelineDemo.c
```

This produces `01_pipelineDemo.o`, an object file - assembly translated
into machine code. It is not yet runnable: `printf` is referenced but not
defined anywhere in this translation unit, so the object file has an
unresolved external symbol that only gets resolved at the next stage.
(`objdump -t 01_pipelineDemo.o` shows the undefined `printf` symbol.)

## 4. Linking - `gcc file.o -o file` / `ld`

```
gcc 01_pipelineDemo.o -o 01_pipelineDemo
```

The linker resolves symbols across every object file and library involved -
here, it finds `printf`'s definition in libc and patches the call in
`01_pipelineDemo.o` to point at it - and produces the final executable.
Run each of the four commands above yourself, in order, to see the .s, .o,
and final binary artifacts appear.

## Cross-reference: linking with multiple translation units

This single-file walkthrough only touches linking in passing (resolving a
call into libc). For linking that combines *this project's own* object
files, see `30_MultiFile` (a `main.c` and `mathutils.c` linked together) and
`49_OpaquePointer`/`59_ExternC` (linking a caller against a separately
compiled implementation file, including a C/C++ linkage-naming example in
the latter).
