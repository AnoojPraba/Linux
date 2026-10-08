# LSM Trees and Storage Engines

B-tree indexing: `04_DatabaseIndexingAndQueryOptimization` and
`../../../C_Basics/code/40_BTreeAndBPlusTree`. SQL vs NoSQL: `06_...`.

## Two families
| | B-tree / B+tree (update-in-place) | LSM-tree (log-structured merge) |
|---|---|---|
| Write path | find page, modify in place (+ WAL) -> random I/O | append to memtable + WAL, flush sequentially -> sequential I/O |
| Read path | O(log n) page reads, predictable | check memtable + several SSTables (use Bloom filters, index) -> read amplification |
| Space | some fragmentation (~70% page fill) | compaction temporarily doubles; can pack tightly |
| Write amplification | moderate (page rewrites + WAL) | high from compaction (10-30x) but sequential |
| Used by | InnoDB, PostgreSQL, SQLite, LMDB | RocksDB/LevelDB, Cassandra, HBase, ScyllaDB, Bigtable, InfluxDB |
| Good for | read-heavy, range scans, in-place updates | write-heavy, ingest, time-series, SSD-friendly |

## LSM write path
1. Write to the **WAL** (sequential append, fsync policy = durability) so the
   memtable survives crashes.
2. Insert into the **memtable** (in-memory sorted structure: skip list/red-black
   tree; see `../../../C_Basics/code/51_SkipList`).
3. When full, freeze it and **flush** to an immutable sorted **SSTable** file on
   disk (sorted key/value blocks + sparse index + Bloom filter + footer).
4. Deletes write a **tombstone** marker; updates write a newer version.

## Read path
Check memtable(s) -> then SSTables newest to oldest; stop at the first hit.
**Bloom filters** (`../../../C_Basics/code/35_ProbabilisticDataStructures`) skip files that
definitely don't contain the key; block cache and index blocks avoid disk reads.
Range scans merge iterators over all sorted runs.

## Compaction (the heart of the trade-offs)
- Merges SSTables, drops overwritten values and tombstones (once they cannot hide
  older data below), reducing read amplification and space.
- **Leveled (LevelDB/RocksDB default):** levels L0..Ln, each ~10x larger, files in
  L1+ have non-overlapping key ranges; low read & space amplification, high write
  amplification.
- **Size-tiered (Cassandra default, HBase):** merge similar-sized tables; low write
  amplification, higher read/space amplification (overlapping ranges).
- **FIFO/time-window:** drop whole expired files - time-series data.
- **Amplification triangle:** write vs read vs space amplification - you can't
  minimize all three. Tuning = picking the corner for your workload.
- Compaction competes with foreground I/O: **write stalls** when L0 piles up;
  rate-limit, separate compaction threads, `bytes_per_sync`.

## Other building blocks
- **WAL (write-ahead log) and checkpoints:** durability for both engine types;
  group commit batches fsyncs. fsync cost drives commit latency.
- **MVCC:** snapshot reads via versions (sequence numbers in LSM; undo/old tuples
  in Postgres/InnoDB), vacuum/purge to reclaim.
- **Page cache vs direct I/O:** DBs (InnoDB, RocksDB w/ direct I/O) often manage
  their own buffer pool to control eviction and avoid double caching.
- **Copy-on-write B-trees** (LMDB, BoltDB, btrfs/ZFS): never overwrite; root pointer swap
  is the commit; no WAL needed, great read concurrency, write amplification.
- **Fractal/Bε-trees**, **Bitcask** (log + in-memory hash index; all keys fit in
  RAM), **WiscKey** (store large values in a separate log, keys in the LSM to cut
  compaction I/O).
- **Columnar formats** (Parquet/ORC, ClickHouse MergeTree - an LSM variant): column
  compression + vectorized scans for analytics.
- **Hardware:** SSDs hate small random writes and wear out (write amplification
  matters for endurance); NVMe changes the cost of read amplification.

## Failure/recovery
Crash: replay WAL into a fresh memtable; SSTables are immutable so they can't be
half-written (manifest file tracks the live set; atomic rename to publish).
Replication/backups: ship the WAL/snapshots; immutable files make consistent
snapshots/hard-link backups cheap.

## Senior interviewer Q&A
**Q: Why are LSM trees fast at writes?**
A: Writes are an in-memory insert + sequential WAL append; disk gets large
sequential flushes and background merges instead of random in-place page updates.

**Q: What are read/write/space amplification?**
A: Ratios of physical to logical work: bytes read per query, bytes written per
user byte (WAL + flush + each compaction rewrite), bytes on disk per logical byte.
Leveled compaction minimizes space/read amp at the cost of write amp; tiered the
opposite.

**Q: How do deletes work in an LSM?**
A: A tombstone shadows older versions; it is removed only when compaction reaches
the bottom level (or all overlapping files) - until then deleted data and
tombstones consume space and slow scans ("tombstone problem" in Cassandra; use
TTL/time windows).

**Q: When would you pick a B-tree database over an LSM?**
A: Read-heavy/mixed OLTP, many point reads and range scans needing predictable
latency, strong transactional features, or small datasets where write
amplification doesn't matter; LSM for ingest-heavy, time-series, big-data
stores, and SSD-optimized write throughput.

**Q: How do you reduce read latency on an LSM?**
A: Bloom filters, block cache, partitioned index/filter blocks, fewer levels /
more aggressive compaction, key prefix filters, larger memtables, and
pinning hot data.

**Q: What does fsync do to performance and durability?**
A: It forces data to stable storage; without it a crash can lose acknowledged
writes. Group commit amortizes it. Tradeoff knobs: `fsync` per commit vs
interval vs OS-managed. Lying disks/controllers and missing directory fsync after
rename are classic durability bugs.

**Q: Explain a write stall.**
A: Flush/compaction can't keep up, so L0 files accumulate; the engine slows or
blocks writers to bound read amplification and memory. Fix by more compaction
threads, faster disks, larger memtables, or tuning triggers.
