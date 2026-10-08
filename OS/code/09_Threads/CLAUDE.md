# 09_Threads

POSIX threads: creation, mutexes, condition variables, counting semaphores, lock ordering and an odd/even printing exercise.

## Files
- `01_pthreadBasics.c` - threads share the address space (no copy-on-write as with fork)
- `02_mutex.c` - mutex protects `counter++` read-modify-write
- `03_conditionVariable.c` - single-slot producer/consumer with a condition variable and predicate
- `04_semaphore.c` - counting semaphore limits concurrent workers to MAX_CONCURRENT
- `05_deadlockAvoidance.c` - avoids deadlock by acquiring locks in a fixed global order
- `06_odd_even_thread.c` - two threads print 1..10 alternately using a mutex and condition variable

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_pthreadBasics.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/09_Threads/` (git-ignored).

## Key concepts / interview angles
- Threads share heap and globals but have their own stack and registers; fork copies the address space.
- Mutex gives mutual exclusion; condition variable needs a predicate loop (spurious wakeups); semaphore counts permits.
- Deadlock needs circular wait: break it with lock ordering (or try-lock/timeouts).
- Always join or detach threads; check return codes of pthread_*.
- Odd/even printing is a standard ping-pong exercise: one mutex, one condvar, a turn variable.

## Gotchas
- Compile with `-pthread` (required here).
- Check races with `gcc -fsanitize=thread -pthread -g`.

## Related
- `../12_RaceConditionAndCriticalSection`
- `../13_ClassicalSyncAlgorithms`
- `../15_MutexVsSemaphoreAndMonitors`
- `../../../Cpp/code/24_Concurrency`
- `../../../C_Basics/code/66_ThreadSanitizerDemo`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
