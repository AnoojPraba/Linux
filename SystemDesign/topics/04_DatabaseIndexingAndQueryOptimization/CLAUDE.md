# 04_DatabaseIndexingAndQueryOptimization

Database indexing and query tuning: B-tree vs hash indexes, composite index column order, N+1 queries, reading EXPLAIN plans and the write cost of indexes.

## Files
- `NOTES.md` - (50 lines) sections: B-tree vs hash indexes; Composite indexes and column order; N+1 query problem; EXPLAIN plans (conceptually); Costs of indexing

## How to use this note
- Drill: given a slow query, state which index you would add, why column order matters, and what it costs on writes.
- Read EXPLAIN output to spot full scans where an index should be used.
- Case-study answer framework: requirements -> estimates -> API -> data model -> architecture -> deep dives -> trade-offs (question bank: `../28_CommonInterviewQuestionsCheatSheet`).

## Key trade-offs / interview angles
- B-tree supports ranges, ordering and prefixes; hash index only equality.
- Composite index `(a,b,c)` follows the leftmost-prefix rule; equality columns before range columns.
- N+1: fix with eager loading, JOIN or batched `IN (...)`.
- Every index slows INSERT/UPDATE/DELETE and uses storage; index for real query patterns.

## Related
- `../05_ACIDAndTransactionIsolation`
- `../06_SQLvsNoSQLTradeoffs`
- `../30_LSMTreesAndStorageEngines`
- `../../../C_Basics/code/40_BTreeAndBPlusTree`
- `../../../OS/code/57_FileSystemStructuresAndAllocation`

## Conventions when extending
- Keep the NOTES.md concise, bullet-style and interview-focused (no essay prose); new topic folders use the next two-digit prefix.
- Conceptual only: no code to build, so there are no binaries to commit.
