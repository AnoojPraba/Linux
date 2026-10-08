# 71_CMakeIntroduction

Introduction to CMake using a two-file C program and a minimal CMakeLists.txt.

## Files
- `01_hello.c` - main() calling a helper
- `02_helper.c` - addTwo helper (no main(); part of the same program)
- `02_helper.h` - helper declaration
- `CMakeLists.txt` - cmake_minimum_required 3.10, project(CMakeIntroduction C), add_executable(cmake_intro_demo 01_hello.c 02_helper.c)
- `NOTES.md` - why CMake over hand-written Makefiles, vocabulary, build commands

## Build and run
- Out-of-source CMake build (keep output outside the repo): `cmake -S . -B /tmp/cmake_intro && cmake --build /tmp/cmake_intro && /tmp/cmake_intro/cmake_intro_demo`.
- Without CMake: `gcc -Wall -Wextra -std=gnu11 01_hello.c 02_helper.c -o /tmp/x`.
- The parent Makefile has an explicit rule linking both files into `bin/71_CMakeIntroduction/01_hello`.
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/71_CMakeIntroduction/` (git-ignored).

## Key concepts / interview angles
- Out-of-source builds keep the source tree clean (`-S`/`-B`).
- Targets and properties (`target_link_libraries`, `target_include_directories`) beat global flags.
- CMake generates build files (Make/Ninja); it is not itself the build tool.
- Cross compiling uses a toolchain file (see `../72_CrossCompilationBasics`).

## Gotchas
- Never run `cmake .` in this folder; it would drop CMakeCache.txt and generated files into the repo.

## Related
- `../72_CrossCompilationBasics`
- `../30_MultiFile`
- `../29_CompilerPipelineWalkthrough`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
