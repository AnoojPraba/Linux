# 30_LSMTreesAndStorageEngines

Storage-engine internals: B-tree (update-in-place) vs LSM-tree (log-structured merge), write/read path, compaction and the amplification triangle, plus WAL, MVCC and recovery, with a senior Q&A.

## Files
- `NOTES.md` - (105 lines) sections: Two families; LSM write path; Read path; Compaction (the heart of the trade-offs); Other building blocks; Failure/recovery; Senior interviewer Q&A

## How to use this note
- Drill: draw the LSM write path (WAL, memtable, SSTable flush) and the read path (memtable, SSTables, Bloom filters), then explain leveled vs size-tiered compaction.
- Rehearse the "Senior interviewer Q&A": why LSM writes are fast, amplification, deletes/tombstones, B-tree vs LSM choice, write stalls, fsync.
- Case-study answer framework: requirements -> estimates -> API -> data model -> architecture -> deep dives -> trade-offs (question bank: `../28_CommonInterviewQuestionsCheatSheet`).

## Key trade-offs / interview angles
- B-tree: in-place updates, predictable reads, random I/O; LSM: sequential appends and flushes, read amplification mitigated by Bloom filters and caches, high compaction write amplification.
- Compaction: leveled (low read/space amp, high write amp), size-tiered (low write amp, higher read/space amp), FIFO/time-window for time series; you cannot minimise write, read and space amplification together.
- Deletes write tombstones removed only at the bottom of compaction; write stalls occur when L0 piles up.
- Durability: WAL plus fsync policy and group commit; crash recovery replays the WAL, immutable SSTables plus a manifest make snapshots and backups cheap.
- Variants: copy-on-write B-trees (LMDB), Bitcask, WiscKey, columnar formats; SSD wear makes write amplification matter.

## Related
- `../04_DatabaseIndexingAndQueryOptimization`
- `../06_SQLvsNoSQLTradeoffs`
- `../../../C_Basics/code/40_BTreeAndBPlusTree`
- `../../../C_Basics/code/51_SkipList`
- `../../../C_Basics/code/35_ProbabilisticDataStructures`

## Conventions when extending
- Keep the NOTES.md concise, bullet-style and interview-focused (no essay prose); new topic folders use the next two-digit prefix.
- Conceptual only: no code to build, so there are no binaries to commit.
