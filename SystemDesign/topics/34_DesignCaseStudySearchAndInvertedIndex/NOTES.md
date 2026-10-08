# Design Case Study: Search and the Inverted Index

Related: `04_DatabaseIndexingAndQueryOptimization`, `30_LSMTreesAndStorageEngines`,
`07_DatabaseShardingAndReplication`, `03_CachingStrategies`, `09_...`.

## 1. Requirements
Full-text search over documents (products, logs, pages): keyword and phrase
queries, ranking by relevance, filters/facets, typo tolerance, autocomplete,
near-real-time indexing (seconds), p99 < 200 ms, billions of docs, high availability.

## 2. Core data structure: the inverted index
- Map **term -> postings list** (sorted doc ids, optionally term frequency and
  positions). Query = look up terms, **intersect** (AND) / **union** (OR) postings.
```
doc1: "the quick brown fox"     quick -> [1, 3]
doc2: "brown dog"               brown -> [1, 2]
doc3: "quick dog"               dog   -> [2, 3]
Query "quick AND dog" -> [1,3] n [2,3] = [3]
```
- **Analysis pipeline** (index AND query time, same analyzer): tokenize ->
  lowercase -> stop words -> stemming/lemmatization -> synonyms -> n-grams.
- Compression is crucial: delta-encode doc ids + variable-byte/PForDelta/Roaring
  bitmaps; skip lists/skip pointers speed intersections of long lists
  (`../../../C_Basics/code/51_SkipList`); intersect shortest-first, galloping search.
- Also: **positions** for phrase queries ("new york"), **forward index** (doc ->
  terms), **doc values/columnar** storage for sorting/aggregations/facets.

## 3. Ranking
- **TF-IDF:** term frequency x inverse document frequency (rare terms matter more).
- **BM25** (Lucene default): TF saturation + document length normalization;
  strong baseline. Then **learning-to-rank** / business signals (popularity,
  freshness, personalization) in a second stage re-ranking top-K.
- Retrieval vs ranking split: cheap inverted-index retrieval of ~1000 candidates,
  expensive models rerank. **Vector/semantic search:** embeddings + ANN indexes
  (HNSW, IVF-PQ) alongside lexical search (hybrid).

## 4. Architecture (Elasticsearch/Solr/OpenSearch style)
```
Writers -> ingest (Kafka) -> indexers -> shards (Lucene segments)
Query -> coordinator -> scatter to one replica of EVERY shard -> each returns top-K
      -> coordinator merges (global top-K) -> fetch phase (load docs) -> response
```
- **Sharding** by doc id hash (document partitioning): every query hits all shards
  (scatter-gather; latency = slowest shard => tail latency problem, use hedged requests,
  timeouts, replicas). Alternative term partitioning rarely used (hot terms, multi-term joins).
- **Replicas** for availability and read throughput; primary-backup replication.
- **Lucene segments:** immutable mini-indexes; new docs go to a memory buffer ->
  flushed as a new segment on **refresh** (~1 s, "near real time"); background
  **merges** combine segments (an LSM-like design - `30_...`); deletes are marked
  in bitsets and removed on merge; translog (WAL) for durability.
- Mapping/schema, index templates, ILM (hot/warm/cold tiers), time-based indices
  for logs (drop old indices cheaply).
- **Source of truth stays in a primary DB;** search is a derived index fed by
  CDC/outbox (`29_...`) and rebuilt/reindexed from scratch when mappings change
  (alias swap for zero-downtime reindex).

## 5. Features
- **Autocomplete:** edge n-grams or a **trie/FST** of prefixes with scores
  (`../../../C_Basics/code/44_Trie`), completion suggesters; cache top prefixes; rank by
  popularity.
- **Typo tolerance:** edit distance (Levenshtein automaton), phonetic analyzers,
  n-gram overlap, "did you mean".
- **Facets/aggregations:** doc values + bitmaps; approximate counts at scale.
- **Filters** (`price < 50`, `in stock`) as cached bitsets, applied before scoring.
- **Pagination:** deep `from+size` is O(from+size) per shard - use `search_after`
  cursors or scroll/point-in-time.

## 6. Trade-offs and pitfalls
- Near-real-time vs indexing throughput (refresh interval, bulk requests).
- Too many shards/segments -> overhead; hot shards from skewed routing keys;
  mapping explosions (dynamic fields); heavy aggregations/wildcards leading to OOM;
  split-brain avoided with proper quorum/master election.
- Relevance is a product feature: measure with click-through, NDCG, A/B tests;
  keep query logs; synonyms/boosts managed as data.
- Consistency: search is **eventually consistent** with the DB; handle deleted/
  stale hits at hydrate time; permission filtering must be applied at query time.

## Senior interviewer Q&A
**Q: Why not just use `LIKE '%term%'` in SQL?**
A: It scans every row (a leading wildcard can't use a B-tree index), has no
relevance ranking, tokenization or stemming. An inverted index looks up terms
directly and intersects sorted postings lists.

**Q: How does a multi-shard query return the global top 10?**
A: Each shard returns its local top-K (K >= from+size) with scores; the coordinator
merges them with a priority queue; then fetches full documents for the winners.
Scores must be comparable (global IDF stats, `dfs_query_then_fetch`).

**Q: How do you keep the index fresh without rebuilding?**
A: Append new segments (immutable), mark deletes in bitsets, merge in the
background; stream changes via CDC/Kafka; refresh interval controls visibility.

**Q: How would you design autocomplete for 100k QPS?**
A: Precomputed prefix -> top-N suggestions in an FST/trie or Redis sorted sets,
aggressively cached and CDN-friendly, debounced on the client, personalized by a
small re-rank, updated from aggregated query logs offline.

**Q: What causes tail latency in search and how do you cut it?**
A: Waiting for the slowest shard (and GC pauses, cold caches, big aggregations).
Use replicas with adaptive selection, hedged requests, per-shard timeouts with
partial results, caching, smaller shards, and query limits.

**Q: How do you choose between Elasticsearch and a database full-text index?**
A: DB FTS (Postgres tsvector/GIN) is fine for modest scale and transactional
consistency; Elasticsearch/OpenSearch for large-scale relevance, aggregations,
analyzers, horizontal scaling, and logs/observability use cases at the cost of
another system to run and sync.
