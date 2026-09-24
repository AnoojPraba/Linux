# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Repository purpose

This is a personal C++ learning/practice repository — a collection of small, standalone
C++ programs exploring core language concepts (classes, constructors/destructors, operator
overloading, inheritance, polymorphism, templates, STL containers/algorithms, smart
pointers, RAII, exceptions, lambdas, move semantics, references, namespaces, static
members, friend functions, multiple/virtual inheritance, streams, type casting, etc.) plus
classic object-oriented design patterns (creational, structural, behavioral). There is no
test suite or package manifest. `code/` has a Makefile for bulk builds; each `.cpp` file is
otherwise self-contained and independently compilable.

## Structure

- `code/` — numbered topic folders (`01_Namespaces`, `02_References`,
  `03_ClassesAndObjects`, ... `24_Concurrency`, `25_CustomSharedPtr`,
  `26_TemplateMetaprogramming`, `27_STLInternals`, `28_PImplIdiom`,
  `29_CustomAllocatorCpp`, `30_Cpp20Features`, `31_SOLIDPrinciples`,
  `32_RpcMechanismsCpp`), numbered in
  easiest-to-hardest study order, each holding
  small example programs for that topic, plus `DesignPatterns/<Category>/<PatternName>.cpp`
  for classic design pattern examples grouped under `Creational/`, `Structural/`, and
  `Behavioral/`. Some topic folders also have a `NOTES.md` with concise,
  interview-focused bullet points (what the topic is, key gotchas/tradeoffs, complexity
  facts) — add one when a topic benefits from quick-reference notes beyond the code
  itself.
  `24_Concurrency` was extended with memory-ordering levels
  (relaxed/acquire-release/seq_cst) and a lock-striped thread-safe LRU cache;
  `26_TemplateMetaprogramming` was extended with hand-rolled type erasure
  (`MyAny`) and SFINAE member detection via `void_t`. `25_CustomSharedPtr` is a
  new folder implementing an atomic-refcounted `SharedPtr`/`WeakPtr` pair from
  scratch, for studying what `std::shared_ptr` does internally.

## Working with this codebase

- `code/Makefile` builds every `.cpp` file under `code/` into a matching binary under
  `code/bin/`, mirroring the source's directory structure (e.g.
  `code/10_Polymorphism/01_shapes.cpp` → `code/bin/10_Polymorphism/01_shapes`).
  From `code/`:
  ```
  make          # build all binaries into bin/
  make clean    # remove bin/
  ```
- New example files should follow the existing naming pattern within their folder:
  a two-digit numeric prefix followed by a short descriptive name
  (e.g. `01_basicClass.cpp`, `02_copyConstructor.cpp`). Design pattern files are named
  after the pattern itself (e.g. `Singleton.cpp`, `Observer.cpp`).
- Each topic folder is otherwise fully self-contained — no shared headers between topic
  folders.
