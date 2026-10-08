# Distributed Transactions: 2PC, Saga, Outbox

Single-node ACID is in `05_ACIDAndTransactionIsolation`; consensus in
`22_ConsensusAndCoordination`; retries/dedup in `24_IdempotencyInDistributedSystems`.

## The problem
An operation spans several services/databases (order + payment + inventory). No
shared transaction manager, networks fail, nodes crash mid-operation. You need
**atomicity across services** or a way to live without it.

## Two-Phase Commit (2PC)
- **Phase 1 (prepare/vote):** coordinator asks all participants "can you commit?";
  each durably logs a PREPARED record (holding locks) and votes yes/no.
- **Phase 2 (commit/abort):** if all yes -> coordinator logs COMMIT and tells
  everyone; any no/timeout -> ABORT.
- Guarantees atomic outcome (all commit or all abort) - XA/JTA, database
  distributed transactions, Spanner (2PC across Paxos groups).
- **Problems:** blocking - if the coordinator dies after participants voted yes,
  they hold locks and can't decide alone ("in-doubt" transactions) until it
  recovers; latency (2 round trips + forced log writes); reduced availability
  (all participants must be up); doesn't scale to many/slow/third-party services.
- **3PC** adds a pre-commit phase to avoid blocking but still fails under network
  partitions; rarely used. Real fix: make the coordinator highly available
  (replicate its log with Raft/Paxos - Spanner does this).

## Sagas (eventual consistency with compensation)
- Break the business transaction into a sequence of LOCAL transactions
  T1..Tn, each with a **compensating action** Ci that semantically undoes it
  (refund, release stock, cancel booking). If Tk fails, run C(k-1)..C1.
- No distributed locks, each step commits locally (so intermediate states are
  VISIBLE: "order pending, payment taken" - no isolation, i.e. ACD not ACID).
- **Orchestration:** a central orchestrator (workflow engine - Temporal, Cadence,
  AWS Step Functions, Camunda) tells services what to do next and tracks state.
  Easier to reason about and monitor; the orchestrator is a (replicated) component.
- **Choreography:** services react to each other's events on a bus; no central
  controller. Loose coupling but flow is implicit, harder to debug, risk of
  cyclic dependencies.
- **Requirements:** every step and compensation must be **idempotent** and
  retryable; compensations must always be able to succeed (or alert a human);
  design for **semantic locks / pending states** and ordering ("reserve" then
  "confirm", also called TCC - Try/Confirm/Cancel); handle timeouts and
  out-of-order/duplicate events; persist saga state.
- Non-compensatable steps (email sent, money wired) go LAST or use a "pending"
  step before the pivot.

## The dual-write problem and the Transactional Outbox
- "Update my DB **and** publish an event" are two systems: crash in between and you
  have a DB change with no event (or the reverse). Retries can't fix atomicity.
- **Outbox pattern:** in ONE local DB transaction write the business row AND an
  `outbox` row (the event). A separate **relay** reads the outbox and publishes to
  the broker, marking sent; delivery is **at-least-once**, so consumers dedupe
  (idempotency key / inbox table).
- Relay options: polling publisher, or **CDC** (change data capture: Debezium
  tailing the WAL/binlog) - lower latency, no polling load, ordering preserved.
- Mirror image **inbox** pattern: consumer records processed message ids in the
  same transaction as its side effects -> effectively-once processing.
- Also: **event sourcing** (the log of events IS the state; publish from the
  log) and **listen to yourself** (publish first, derive DB state from the event).

## Related guarantees
- Exactly-once is really **at-least-once delivery + idempotent processing** (or
  Kafka transactions/idempotent producer within Kafka, `31_...`).
- **Distributed locks** (Redis Redlock, ZooKeeper, etcd leases) are not
  transactions: use fencing tokens (see `22_ConsensusAndCoordination`).
- Alternatives: avoid cross-service transactions by merging boundaries (put
  data that must be atomic in one service/DB), versioned optimistic concurrency,
  or use a database that offers global transactions (Spanner, CockroachDB, TiDB,
  FoundationDB) at some latency/cost.

## Decision guide
| Need | Choice |
|---|---|
| Strong atomicity, few homogeneous DBs, can accept blocking | 2PC/XA |
| Microservices, long-running business flow, availability first | Saga (orchestrated if complex) |
| Reliable event publication after a DB change | Transactional outbox + CDC |
| Global consistent transactions as a service | Spanner/Cockroach-style (consensus + 2PC inside) |

## Senior interviewer Q&A
**Q: Why not just use 2PC across microservices?**
A: Blocking and availability: a coordinator failure leaves participants holding
locks; all services must be up and fast; many modern stores/third-party APIs don't
support it; tight coupling and latency. Sagas trade isolation for availability.

**Q: How do you handle a saga step failing after the payment was charged?**
A: Run compensations in reverse (refund), each idempotent with retries/backoff;
persist saga state so a crashed orchestrator resumes; if a compensation keeps
failing, alert/escalate (dead-letter + human workflow). Design the order so the
hardest-to-undo step is last.

**Q: What is the outbox pattern and what problem does it solve?**
A: It makes "write DB + publish message" atomic by storing the message in the same
local transaction and publishing it asynchronously via a relay/CDC, giving
at-least-once delivery without 2PC. Consumers must be idempotent.

**Q: How do sagas deal with lack of isolation?**
A: Countermeasures: semantic locks (status = PENDING), commutative updates,
re-reading and version checks, reservations with expiry, ordering steps so
dirty reads are harmless, and showing "pending" states to users.

**Q: Orchestration vs choreography?**
A: Orchestration centralizes the flow (clear, observable, easier changes, but a
coordination service to run). Choreography is decentralized events (loose
coupling but hidden flow, harder to evolve and debug). Use orchestration for
multi-step business processes with branching/compensation.

**Q: How does Spanner do cross-shard transactions?**
A: Each shard is a Paxos group; a transaction spanning groups uses 2PC with the
coordinator and participants being replicated groups (so the coordinator can't
get "stuck"), plus TrueTime commit-wait for external consistency.

**Q: Can you get exactly-once?**
A: Not over an unreliable network in the strict sense; achieve effectively-once
via idempotent consumers/dedup keys (+ transactional outbox/inbox), or
broker-internal transactions (Kafka read-process-write).
