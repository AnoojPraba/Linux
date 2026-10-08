# 06_SQLvsNoSQLTradeoffs

When relational wins vs when NoSQL wins, the four NoSQL families with an example use each, and denormalisation trade-offs.

## Files
- `NOTES.md` - (44 lines) sections: When relational wins; When NoSQL wins; NoSQL categories, one example use case each; Denormalization tradeoffs

## How to use this note
- Drill: for a given feature, pick the store type and justify it with access pattern, consistency need and scale.
- Do not answer "NoSQL because scale"; name the access pattern and write volume.
- Case-study answer framework: requirements -> estimates -> API -> data model -> architecture -> deep dives -> trade-offs (question bank: `../28_CommonInterviewQuestionsCheatSheet`).

## Key trade-offs / interview angles
- Relational: strong consistency, joins, known schema, multi-row ACID (ledgers, inventory).
- NoSQL: very high write throughput, flexible schema, simple key access.
- Document (catalogues), key-value (sessions, cache), wide-column (chat history, time series), graph (friend-of-friend, recommendations).
- Denormalisation speeds reads but makes writes complex and copies can drift out of sync.

## Related
- `../04_DatabaseIndexingAndQueryOptimization`
- `../05_ACIDAndTransactionIsolation`
- `../07_DatabaseShardingAndReplication`
- `../14_DesignCaseStudyChatSystem`
- `../30_LSMTreesAndStorageEngines`

## Conventions when extending
- Keep the NOTES.md concise, bullet-style and interview-focused (no essay prose); new topic folders use the next two-digit prefix.
- Conceptual only: no code to build, so there are no binaries to commit.
