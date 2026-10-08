# 34_DesignCaseStudySearchAndInvertedIndex

Case study: full-text search built on an inverted index (analysis, postings, ranking with BM25, Elasticsearch-style sharding and Lucene segments, autocomplete, pitfalls), with a senior Q&A.

## Files
- `NOTES.md` - (107 lines) sections: 1. Requirements; 2. Core data structure: the inverted index; 3. Ranking; 4. Architecture (Elasticsearch/Solr/OpenSearch style); 5. Features; 6. Trade-offs and pitfalls; Senior interviewer Q&A

## How to use this note
- Whiteboard: inverted index with postings intersection, analysis pipeline, scatter-gather architecture, then ranking and autocomplete.
- Rehearse the "Senior interviewer Q&A": why not SQL LIKE, global top-K across shards, freshness, autocomplete at 100k QPS, tail latency, ES vs DB full-text.
- Case-study answer framework: requirements -> estimates -> API -> data model -> architecture -> deep dives -> trade-offs (question bank: `../28_CommonInterviewQuestionsCheatSheet`).

## Key trade-offs / interview angles
- Inverted index maps term -> sorted postings (doc ids, tf, positions); AND = intersection (shortest first, skip pointers), OR = union; compress with delta encoding, PForDelta, Roaring.
- Same analyzer at index and query time (tokenise, lowercase, stop words, stemming, synonyms, n-grams).
- Ranking: TF-IDF, BM25 (Lucene default), then learning-to-rank over top-K; hybrid lexical plus vector (ANN) search.
- Document-partitioned shards give scatter-gather: latency is the slowest shard (hedged requests, timeouts, replicas); coordinator merges local top-K, then a fetch phase.
- Lucene segments are immutable, refresh (~1 s) makes docs searchable, background merges are LSM-like; the database stays the source of truth, fed by CDC/outbox with alias-swap reindexing.
- Features and pitfalls: edge n-grams/FST for autocomplete, edit-distance typo tolerance, `search_after` instead of deep paging, too many shards, mapping explosions; search is eventually consistent.

## Related
- `../04_DatabaseIndexingAndQueryOptimization`
- `../30_LSMTreesAndStorageEngines`
- `../07_DatabaseShardingAndReplication`
- `../29_DistributedTransactions2PCSagaOutbox`
- `../../../C_Basics/code/44_Trie`
- `../../../C_Basics/code/51_SkipList`

## Conventions when extending
- Keep the NOTES.md concise, bullet-style and interview-focused (no essay prose); new topic folders use the next two-digit prefix.
- Conceptual only: no code to build, so there are no binaries to commit.
