# 64_StaticAndSharedLibraries

Building and linking the same library statically (.a) and dynamically (.so), with rpath and inspection tools.

## Files
- `01_useStatic.c` - program linked against libgreet.a
- `02_useShared.c` - program linked against libgreet.so, resolved at load time
- `libgreet.c` - library implementation (addNumbers and a greeting; no main())
- `libgreet.h` - library header
- `NOTES.md` - building by hand, .a vs .so trade-offs, inspecting shared libraries

## Build and run
- Static: `gcc -Wall -c libgreet.c -o /tmp/libgreet.o && ar rcs /tmp/libgreet.a /tmp/libgreet.o && gcc -Wall -I. 01_useStatic.c /tmp/libgreet.a -o /tmp/useStatic && /tmp/useStatic`.
- Shared: `gcc -Wall -fPIC -shared libgreet.c -o /tmp/libgreet.so && gcc -Wall -I. 02_useShared.c -L/tmp -lgreet -Wl,-rpath,/tmp -o /tmp/useShared && /tmp/useShared` (the Makefile uses `-Wl,-rpath,$ORIGIN`).
- `make` from `..` builds `bin/64_StaticAndSharedLibraries/{01_useStatic,02_useShared}` with the library artefacts beside them.
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/64_StaticAndSharedLibraries/` (git-ignored).

## Key concepts / interview angles
- Static: code copied into the binary, no runtime dependency, bigger binaries, no shared security updates.
- Shared: one copy in memory (page cache), updated independently, resolved by ld.so at load time (search order: rpath/RUNPATH, LD_LIBRARY_PATH, ldconfig cache).
- `-fPIC` is needed for shared objects (position-independent code via GOT/PLT).
- Inspect with `ldd`, `readelf -d`, `nm -D`, `LD_DEBUG=libs`.
- Symbol versioning and SONAME for ABI compatibility.

## Related
- `../62_LinkerAndLoaderMechanics`
- `../63_DynamicLoading`
- `../../../C_Basics/code/30_MultiFile`
- `../../../C_Basics/code/71_CMakeIntroduction`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
