# Nested Functions (GCC Extension) and Trampolines

- **Standard C forbids** defining a function inside another; GCC allows it. Not portable (clang rejects it), so avoid in portable code.
- A nested function can read and write the enclosing function's locals: it receives a hidden pointer to the enclosing stack frame (a limited closure).
- **Direct call:** the compiler knows the frame, nothing special is generated.
- **Address taken and passed on** (`qsort`, callbacks): a plain function pointer has no room for the frame pointer, so GCC emits a **trampoline** - a few bytes of code on the stack that load the frame pointer and jump to the function. That needs an **executable stack** (linker warning "requires executable stack"), a security weakening; hardened toolchains refuse or use heap trampolines.
- **Lifetime:** the pointer is valid only while the enclosing function is active. Calling it after return is undefined behavior (same trap as returning the address of a local).
- **Portable alternative:** pass an explicit `void *context` through the callback (as `qsort_r` does) or use a file-scope function with a small state struct.
