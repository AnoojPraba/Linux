# 84_NestedFunctionsGccExtension

Nested functions: not standard C, a GCC extension; closures over the enclosing frame and the trampoline GCC builds when you take the address.

## Files
- `01_nestedFunction.c` - A function defined inside another (GCC extension) reading the enclosing locals, called directly and through a function pointer (trampoline).
- `NOTES.md` - concise interview-focused notes for this topic

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_nestedFunction.c -o /tmp/x -lm && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/84_NestedFunctionsGccExtension/` (git-ignored).

## Key concepts / interview angles
- **Standard C forbids** defining a function inside another; GCC allows it. Not portable (clang rejects it), so avoid in portable code.
- A nested function can read and write the enclosing function's locals: it receives a hidden pointer to the enclosing stack frame (a limited closure).
- **Direct call:** the compiler knows the frame, nothing special is generated.
- **Address taken and passed on** (`qsort`, callbacks): a plain function pointer has no room for the frame pointer, so GCC emits a **trampoline** - a few bytes of code on the stack that load the frame pointer and jump to the function. That needs an **executable stack** (linker warning "requires executable stack"), a security weakening; hardened toolchains refuse or use heap trampolines.
- **Lifetime:** the pointer is valid only while the enclosing function is active. Calling it after return is undefined behavior (same trap as returning the address of a local).

## Gotchas
- Linking prints `requires executable stack (because the .note.GNU-stack section is executable)`; that is expected for this file.
- Compile with `-Wno-trampolines` to silence the warning in builds that enable it.
