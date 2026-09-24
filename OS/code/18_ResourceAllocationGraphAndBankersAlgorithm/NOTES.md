# Resource Allocation Graph and Banker's Algorithm

- Builds on/formalizes `17_DeadlockDetectionAvoidance`'s conceptual mention
  of Banker's algorithm - this folder gives it a working implementation and
  covers the graph model deadlock theory is usually taught with.

## Resource Allocation Graph (RAG)

- A directed graph with two kinds of nodes - processes and resource types -
  and two kinds of edges:
  - **Request edge** (process -> resource): the process is waiting for an
    instance of that resource.
  - **Assignment edge** (resource -> process): an instance of that resource
    is currently held by that process.
- **Cycle detection**:
  - If every resource type in the graph has exactly **one instance**, a
    cycle in the RAG means deadlock, definitively - the cycle is both
    necessary and sufficient.
  - If a resource type has **multiple instances**, a cycle is only
    necessary, not sufficient, for deadlock - a cycle can exist and still
    resolve, because another instance of the contended resource type might
    free up along the cycle without needing every process in it to
    progress simultaneously. Determining deadlock in the multi-instance
    case requires a Banker's-style safety check rather than just cycle
    detection.

## Banker's algorithm

- Named for the analogy to a banker who won't lend money (grant resources)
  if doing so could leave the bank unable to satisfy some future legitimate
  demand.
- Requires each process to declare its **maximum claim** on each resource
  type in advance (`maxClaim` in `01_bankersAlgorithm.c`) - impractical for
  most real systems, which is why this is taught more for the CS-theory
  background than used directly in production (as also noted in
  `17_DeadlockDetectionAvoidance/NOTES.md`).
- **Safety algorithm** (`findSafeSequence()`): starting from the current
  `available` vector, repeatedly find any unfinished process whose
  remaining `need` (`maxClaim - allocation`) fits within what's currently
  available, simulate it running to completion (add its `allocation` back
  to `available`), and repeat. If every process can be "finished" this way,
  the state is **safe** and a safe finishing order (safe sequence) exists.
  If at some point no unfinished process's need fits, the state is
  **unsafe** - not necessarily deadlocked yet, but capable of deadlocking.
- **Request-handling logic** (`requestResources()`): to decide whether to
  grant an incremental request, tentatively apply it, rerun the safety
  algorithm, and only keep the grant if the resulting state is still safe;
  otherwise roll it back and deny the request. A request that exceeds a
  process's declared remaining need, or exceeds current availability, is
  rejected outright without even running the safety check.
- `01_bankersAlgorithm.c`'s `main()` demonstrates both a request granted
  (stays safe) and a request denied (would create an unsafe state),
  matching the two request outcomes the algorithm needs to support.
- This is deadlock **avoidance** (proactive, decides at request time)
  versus deadlock **detection** (reactive, periodically scans a wait-for
  graph for cycles after the fact and picks a victim to break one) covered
  conceptually in `17_DeadlockDetectionAvoidance/NOTES.md`.
