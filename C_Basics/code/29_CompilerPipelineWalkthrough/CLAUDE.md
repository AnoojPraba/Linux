# 29_CompilerPipelineWalkthrough

Hands-on walkthrough of the four compilation stages (preprocess, compile, assemble, link) run by hand on a trivial program.

## Files
- `01_pipelineDemo.c` - trivial addOne() plus main, deliberately boring so the stages can be inspected
- `NOTES.md` - step by step: `gcc -E`, `gcc -S`, `gcc -c` (objdump -t shows undefined printf), link; cross-reference to multi-file linking

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_pipelineDemo.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/29_CompilerPipelineWalkthrough/` (git-ignored).

## Key concepts / interview angles
- Stages: `gcc -E` (macros/includes) -> `-S` (assembly) -> `-c` (object file) -> link with libc.
- An object file lists undefined symbols (e.g. `printf`) that the linker resolves.
- Inspect with `objdump -d`, `nm`, `readelf`.

## Gotchas
- Follow NOTES.md in this folder and write intermediates to /tmp, not here, to avoid adding build output to the repo.

## Related
- `../30_MultiFile`
- `../71_CMakeIntroduction`
- `../../../OS/code/62_LinkerAndLoaderMechanics`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
