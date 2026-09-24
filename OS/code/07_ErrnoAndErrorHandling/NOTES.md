# errno and Error Handling

- `errno` is set by library/syscall functions ONLY to indicate the reason for
  a failure - it is never cleared on success, so checking it without first
  confirming the call failed is a bug (a stale errno from an earlier failure
  can leak through).
- Correct pattern: check the function's own failure return value first
  (e.g. `-1` from `open()`, `NULL` from `malloc()`), THEN read `errno`.
- `perror(prefix)` prints `prefix: <strerror(errno) text>` to stderr - the
  quickest way to report a failed syscall.
- `strerror(errno)` returns a human-readable string for programmatic use
  (building your own error message instead of printing directly).
- `errno` is thread-local in any POSIX-conformant threaded program (it's
  effectively `__thread int errno` under the hood via a per-thread location),
  so it is safe to use from multiple threads without external locking - each
  thread has its own.
- `strerror()` itself is not guaranteed thread-safe in older POSIX (a
  concurrent call could overwrite a static buffer); use `strerror_r()` in
  strictly thread-safe/reentrant code.
- Common gotcha: some functions (e.g. `strtol()`) use `0`/valid-looking
  return values to mean both "success" and "failure" - you must explicitly
  set `errno = 0` before the call and check it afterward to disambiguate.
