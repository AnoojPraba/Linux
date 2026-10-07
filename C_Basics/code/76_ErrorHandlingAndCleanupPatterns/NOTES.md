# Error Handling and Cleanup Patterns in C

- C has no exceptions or destructors; error handling is a discipline.
- **Return codes:** `0` / `-errno` (kernel style), or `-1` + `errno` (POSIX
  style), or `NULL`. Never overload a valid value as the error sentinel
  without documenting it. `errno` is thread-local, only meaningful right after
  a call that reported failure, and is not cleared on success.
- **`goto cleanup` ladder:** acquire in order, release in reverse via
  fall-through labels. Pros: one exit path, no duplicated frees, scales with
  resource count. This is the one widely accepted use of `goto`. Set owned
  pointers to NULL once ownership is transferred so cleanup stays safe.
- **`__attribute__((cleanup))`** (GCC/Clang): scope-exit hook = poor man's
  RAII; runs on every return path, in reverse declaration order. Not run on
  `longjmp`, `exit`, `_exit` or a signal kill. Non-standard.
- **`setjmp`/`longjmp`:** non-local jump back to a saved point.
  - Locals changed after `setjmp` and read after `longjmp` are indeterminate
    unless `volatile`.
  - Jumping skips cleanup in the frames it unwinds - leaks, held locks.
  - `longjmp` to a function that already returned is UB.
  - Use `sigsetjmp(env, 1)` / `siglongjmp` to save/restore the signal mask.
  - Legit uses: error recovery in parsers/interpreters, coroutine-ish tricks,
    escaping a signal handler (SIGSEGV recovery is fragile - avoid).
- **Error propagation patterns:** out-parameters for results, return status;
  `errno`-style per-context error (e.g. `ctx->last_error`); error-code enums
  with a `strerror`-like function; `assert` only for programmer errors
  (compiled out by `NDEBUG`!), never for runtime input validation.
- **Partial-failure atomicity:** build into a temp object, publish with one
  assignment/rename (`write temp + rename()` for files).
- **`free(NULL)` and `fclose(NULL)`:** `free(NULL)` is a no-op; `fclose(NULL)`
  is UB - guard it.
- Interview angles:
  - "How do you avoid leaks on error paths in C?" -> goto ladder / cleanup
    attribute / arena that frees everything at once.
  - "What is wrong with `if (fd = open(...) < 0)`?" -> precedence plus
    assignment instead of comparison.
  - "Why doesn't `perror` after a successful call mean anything?" -> `errno`
    is only set on failure, so the value is stale.
  - `atexit` handlers and `exit()` vs `_exit()` (the latter skips stdio flush
    and handlers - correct after `fork` in a child before `exec`).

## Senior interviewer Q&A
**Q: Is `goto` harmful? Show me a legitimate use.**
A: Forward `goto` to a single cleanup ladder is standard (kernel style): each
label releases exactly what was acquired before the failure, in reverse
order, and there is one exit path. Avoid backward gotos and jumping into
scopes; they obscure control flow. Nested `if`s or flag variables are worse as
resource counts grow.

**Q: How do you handle ownership transfer in a cleanup path?**
A: After handing a pointer to the caller (`*out = buf`), set the local to NULL
so the shared cleanup `free(buf)` is a no-op. Same rule for fds (`-1`) -
double-close is a classic bug that can close another thread's reused fd.

**Q: What does `__attribute__((cleanup))` not cover?**
A: `exit()`, `_exit()`, `abort()`, being killed by a signal, and `longjmp`
past the scope. It is a GCC/Clang extension - not portable to MSVC, so
libraries targeting multiple compilers avoid it.

**Q: Explain the `setjmp` volatile gotcha.**
A: Between `setjmp` and `longjmp`, a non-`volatile` automatic variable that
was modified has an indeterminate value after the jump because it may have
lived in a register that `longjmp` restored to its saved contents. Mark it
`volatile` or keep it outside the jumped region.
*Follow-up: why avoid `longjmp` in general?* It skips cleanup of intermediate
frames (leaks, held locks), defeats RAII-like tools, and is UB if the
`setjmp` frame has returned. With signals use `sigsetjmp`.

**Q: `assert` vs returning an error?**
A: `assert` documents programmer-error invariants and disappears under
`-DNDEBUG`, so never put side effects in it or use it to validate external
input. Runtime failures (I/O, user data, OOM) need real error returns.

**Q: How do you design an error-reporting API for a library?**
A: Return a status code (negative errno or an enum), results via out
parameters, a `const char *lib_strerror(int)`, and no global `errno`
dependence for library-specific errors. Document ownership and which
functions can fail. For richer detail, an opaque per-context error struct
(thread-safe, unlike a global).

**Q: `fclose` fails - what do you do?**
A: Buffered writes may only fail at `fclose`/`fflush` (`ENOSPC`, `EIO`).
For data you care about, check its return, and `fsync` the fd before
declaring durability; write-temp + `rename` for atomic replace.

**Q: What breaks if you `fork()` in a multithreaded program?**
A: Only the calling thread exists in the child; locks held by other threads
stay locked forever. Between `fork` and `exec` call only async-signal-safe
functions; consider `posix_spawn`.
