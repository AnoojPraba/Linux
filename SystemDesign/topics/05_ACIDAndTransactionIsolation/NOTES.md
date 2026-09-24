# ACID and Transaction Isolation

## ACID

- **Atomicity**: a transaction's operations either all commit or all roll
  back - no partial application.
- **Consistency**: a transaction takes the database from one valid state to
  another, respecting all constraints (foreign keys, uniqueness, application
  invariants).
- **Isolation**: concurrent transactions behave as if they ran one at a
  time (to whatever degree the chosen isolation level guarantees - see
  below), even though they're actually interleaved for performance.
- **Durability**: once a transaction commits, its effects survive a crash
  (typically via a write-ahead log flushed to disk before acknowledging the
  commit).

## Isolation level ladder and the anomalies each prevents

- **Read Uncommitted**: transactions can see other transactions' uncommitted
  writes. Allows dirty reads, non-repeatable reads, and phantom reads.
  Rarely used in practice.
- **Read Committed**: only ever reads committed data. Prevents dirty reads,
  but a row re-read later in the same transaction can have changed (if
  another transaction committed in between) - non-repeatable reads and
  phantom reads still possible.
- **Repeatable Read**: re-reading the same row within a transaction always
  returns the same value. Prevents dirty reads and non-repeatable reads,
  but phantom reads (a *new* row matching a previous query's `WHERE` clause
  can still appear) are possible in the classic definition (some engines,
  e.g. MySQL InnoDB, close this gap further via gap locking/MVCC snapshots).
- **Serializable**: transactions behave as if executed strictly one at a
  time. Prevents all three anomalies, at the cost of the most locking/
  conflict-checking overhead and lowest concurrency.
- Anomaly definitions: **dirty read** = reading another transaction's
  uncommitted (possibly to-be-rolled-back) write; **non-repeatable read** =
  re-reading the same row gives a different value within one transaction;
  **phantom read** = re-running the same range query returns a different
  set of rows within one transaction.

## Optimistic vs pessimistic locking

- **Pessimistic locking**: acquire a lock before touching a row, blocking
  other transactions from touching it until release. Good under high
  contention, but reduces concurrency and risks deadlocks.
- **Optimistic locking**: don't lock up front; instead check at commit time
  (e.g. a version number/timestamp column) whether the row changed since it
  was read, and abort/retry if so. Good under low contention (most
  transactions don't actually conflict), avoids holding locks, but wastes
  work on retries when contention is high.
