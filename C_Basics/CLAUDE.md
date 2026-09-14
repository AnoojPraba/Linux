# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Repository purpose

This is a personal C learning/practice repository — a collection of small, standalone C
programs exploring core language concepts (loops, functions, arrays, search/sort
algorithms, pattern printing, pointers, bit manipulation, variable scope/linkage, tokens,
escape sequences, etc.). There is no test suite or package manifest. `code/` has a
Makefile for bulk builds; each `.c` file is otherwise self-contained and independently
compilable.

## Structure

- `code/` — numbered topic folders (`01_Loops`, `02_Functions`, `03_array`,
  `04_Search_alg`, `05_pattern`, `06_Sorting`, `BitManipulation`, `pointers`) each holding
  small example programs for that topic, plus a top-level `HelloWorld.c`.
- `Notes/` — numbered `.c` files that double as written notes/explanations (e.g.
  `04_MacroPreprocessor.c`, `06_Error_Signals.c`, `09_VariableScope.c`,
  `11_complicatedDeclaration.c`), plus two "Crack the Interview" PDF references.

## Working with this codebase

- `code/Makefile` builds every `.c`/`.cpp` file under `code/` into a matching binary under
  `code/bin/`, mirroring the source's directory structure (e.g.
  `code/BitManipulation/04_get_bitset.c` → `code/bin/BitManipulation/04_get_bitset`).
  From `code/`:
  ```
  make          # build all binaries into bin/
  make clean    # remove bin/
  ```
  `Notes/` is not covered by the Makefile; compile those files directly with gcc if needed,
  e.g. `gcc Notes/09_VariableScope.c -o /tmp/a.out && /tmp/a.out`.
- `code/BitManipulation/utils.h` is a small shared header (hex/decimal string checks,
  `linedisplay()`) used by files in that folder only — other topic folders do not share
  headers and each file is otherwise fully self-contained.
- New example files should follow the existing naming pattern within their folder:
  a two-digit numeric prefix followed by a short descriptive name
  (e.g. `03_OddOrEven.c`, `01_indexremoveproduct.c`).

