# 05_ACIDAndTransactionIsolation

ACID properties, the four SQL isolation levels with the anomalies each prevents, and optimistic vs pessimistic locking.

## Files
- `NOTES.md` - (49 lines) sections: ACID; Isolation level ladder and the anomalies each prevents; Optimistic vs pessimistic locking

## How to use this note
- Drill: fill in the isolation-level vs anomaly table from memory (dirty read, non-repeatable read, phantom).
- Be ready to say how MVCC engines (e.g. InnoDB) differ from the textbook definitions.
- Case-study answer framework: requirements -> estimates -> API -> data model -> architecture -> deep dives -> trade-offs (question bank: `../28_CommonInterviewQuestionsCheatSheet`).

## Key trade-offs / interview angles
- Durability usually means a write-ahead log flushed before acknowledging commit.
- Read Uncommitted allows all anomalies; Read Committed stops dirty reads; Repeatable Read stops non-repeatable reads; Serializable stops phantoms too at the cost of concurrency.
- Some engines (InnoDB) close the phantom gap at Repeatable Read via gap locks/MVCC snapshots.
- Pessimistic locking suits high contention but risks deadlocks; optimistic (version column, retry on conflict) suits low contention.

## Related
- `../06_SQLvsNoSQLTradeoffs`
- `../08_CAPTheoremAndConsistencyModels`
- `../29_DistributedTransactions2PCSagaOutbox`
- `../24_IdempotencyInDistributedSystems`
- `../../../OS/code/17_DeadlockDetectionAvoidance`

## Conventions when extending
- Keep the NOTES.md concise, bullet-style and interview-focused (no essay prose); new topic folders use the next two-digit prefix.
- Conceptual only: no code to build, so there are no binaries to commit.
