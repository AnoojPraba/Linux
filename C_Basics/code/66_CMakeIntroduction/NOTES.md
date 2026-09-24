# CMake Introduction

Companion demo: `01_hello.c` calls `addTwo()` from `02_helper.c`/
`02_helper.h`, analogous to `30_MultiFile`'s hand-linked example but built
here via CMake instead of an explicit `gcc` command line.

Verified locally: `cmake --version` reported 3.23.2, and
`mkdir build && cd build && cmake .. && make` successfully configured and
built `cmake_intro_demo`, which ran and printed `addTwo(1) = 3`. The `build/`
directory was deleted afterward - it is a generated artifact and should
never be committed.

## Why CMake instead of a hand-written Makefile

- **Cross-platform generation.** One `CMakeLists.txt` can generate Unix
  Makefiles, Ninja build files, Visual Studio projects, or Xcode projects,
  instead of maintaining separate hand-written build files per platform.
- **Out-of-source builds.** Configuring and building inside a separate
  `build/` directory (as above) keeps generated object files, CMake cache
  files, and the final binary out of the source tree - `rm -rf build` fully
  resets the build with no risk of deleting source.
- **Dependency discovery.** `find_package()` locates and configures
  third-party libraries (compiler flags, include paths, link libraries) in
  a standard way, instead of every project reinventing its own detection
  logic the way a Makefile would need to.

## Basic vocabulary

- `add_executable(name src1.c src2.c ...)` - defines a binary target built
  from the given sources.
- `add_library(name src1.c ...)` - defines a static or shared library
  target instead of an executable.
- `target_link_libraries(target lib1 lib2 ...)` - links other libraries
  (project-local or external) into a target.
- `target_include_directories(target PUBLIC dir)` - adds a header search
  path scoped to a specific target, instead of a global `-I` flag.
- `CMAKE_BUILD_TYPE` - set to `Debug` or `Release` (e.g.
  `cmake -DCMAKE_BUILD_TYPE=Release ..`) to control optimization/debug-info
  flags without hand-editing compiler flags.

## Build commands

```
mkdir build && cd build && cmake .. && make
```

or, using CMake's generator-agnostic build invocation:

```
cmake --build .
```
