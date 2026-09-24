# SQL vs NoSQL Tradeoffs

## When relational wins

- Strong consistency requirements and complex, multi-table relationships
  that need real joins.
- A schema that's known upfront and doesn't change constantly - the
  schema-on-write discipline catches bad data early.
- Transactions spanning multiple rows/tables need real ACID guarantees
  (e.g. financial ledgers, inventory with strict invariants).

## When NoSQL wins

- Very high write throughput or massive horizontal scale requirements that
  outgrow a single relational primary's write capacity.
- Flexible/evolving schema, or genuinely schemaless data (varying
  attributes per record).
- Access patterns are simple key-based lookups rather than ad-hoc joins
  across many tables.

## NoSQL categories, one example use case each

- **Document store** (MongoDB, Couchbase): storing nested, semi-structured
  JSON-like documents - e.g. a product catalog where different product
  types have different attribute sets.
- **Key-value store** (Redis, DynamoDB): simple get/put by key at very high
  throughput/low latency - e.g. session storage, a cache layer.
- **Wide-column store** (Cassandra, HBase): huge volumes of write-heavy,
  time-ordered or append-style data queried by row key - e.g. chat message
  history, time-series/event logging (see `../14_DesignCaseStudyChatSystem`).
- **Graph database** (Neo4j): data that's fundamentally about relationships/
  traversals - e.g. a social network's friend-of-friend queries or a
  recommendation engine walking a graph.

## Denormalization tradeoffs

- Relational design normalizes to avoid redundancy; NoSQL/high-scale
  systems often deliberately denormalize (duplicate data across documents/
  rows) to avoid needing joins at read time, since joins are expensive or
  unsupported at that scale.
- Tradeoff: faster, simpler reads (no join fan-out) in exchange for more
  complex writes (every place the duplicated data lives must be updated
  together) and a real risk of the duplicated copies drifting out of sync
  if that update isn't handled carefully.
