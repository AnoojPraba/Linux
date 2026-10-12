# 84_DeadlockVictimAndServiceOrdering

Wait-for-graph deadlock detection with victim selection, and topological ordering of service start-up with parallel waves and cycle detection.

## Files
- `01_waitForGraphVictim.c` - DFS colour cycle detection on a wait-for graph, abort the lowest-priority process in the cycle, re-check.
- `02_serviceStartOrder.c` - Kahn's algorithm in waves (parallel start groups), critical path length, cycle leaves services unstarted.
- `NOTES.md` - concise interview-focused notes for this topic

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_waitForGraphVictim.c -o /tmp/x -lm && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/84_DeadlockVictimAndServiceOrdering/` (git-ignored).

## Key concepts / interview angles
- **Wait-for graph:** edge A -> B = A waits for a resource B holds. With single-instance resources deadlock <=> a **cycle**; DFS with white/grey/black colours (edge to grey = back edge) in O(V+E). A chain is not a deadlock. Multi-instance resources: a cycle is only necessary; use the matrix detection (Banker's safety check without Max) - see `../18_ResourceAllocationGraphAndBankersAlgorithm`.
- **Recovery:** terminate all/one-by-one, or preempt + roll back. **Victim cost:** priority, work done, resources held, work remaining, number of rollbacks (include previous aborts to prevent starvation). Databases do exactly this on a transaction wait-for graph. Prevention: global lock ordering (kills circular wait); avoidance: Banker's; practical: `trylock` with back-off, timeouts, `lockdep` in Linux.
- **Topological order:** every node after its prerequisites. Kahn: start in-degree-0 nodes, decrement dependents. Running it in **waves** gives groups that can start in parallel; number of waves = longest dependency chain = boot-time lower bound (`systemd-analyze critical-chain`). Leftover nodes after Kahn = a cycle (and everything behind it); systemd breaks it by dropping a job and logging "ordering cycle". Same algorithm: `make`, package managers. `After=` orders start-up, `Requires=` also couples failure.

## Gotchas
- Victim choice with ties can starve the same process; track rollbacks in the cost.
