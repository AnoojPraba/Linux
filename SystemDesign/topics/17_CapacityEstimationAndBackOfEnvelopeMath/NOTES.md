# Capacity Estimation and Back-of-Envelope Math

## Why it matters

- Interviewers want to see the REASONING process, not an exact number -
  state assumptions out loud, round aggressively, and use the result to
  justify architecture decisions (e.g. "at this write volume we need
  sharding" or "this fits comfortably on one DB, no need to over-engineer").

## Handy reference numbers

- 1 day ~= 86,400s ~= 10^5 s for quick mental math.
- 1 million ~= 10^6, 1 billion ~= 10^9.
- Typical read:write ratios for read-heavy systems: 100:1 or even 1000:1
  (e.g. social feeds, URL shorteners).

## QPS estimation

- Average QPS = (daily active users x requests per user per day) / 86,400
  (use 10^5 for the quick version).
- Peak QPS = average QPS x 2-3 (daily traffic isn't uniform - there's a
  peak window, e.g. evenings).
- Split into read QPS and write QPS using the read:write ratio, since they
  often need different scaling strategies (e.g. read replicas / caching
  vs write sharding).

## Storage estimation

- Storage = per-record size x number of records x retention period.
- Multiply by replication factor (e.g. 3x for typical distributed storage)
  to get actual disk footprint.
- Round record size generously (e.g. "a tweet is ~roughly 300 bytes of
  text plus metadata, call it 1KB") - precision doesn't matter, order of
  magnitude does.

## Bandwidth estimation

- Bandwidth = QPS x average payload size (separately for read and write
  paths, and for ingress vs egress).
- Compare against typical NIC/link capacity to sanity check whether a
  single server or a fleet is needed.

## Interview framing

- Use the numbers to make a call, not just to produce a number: "at 50K
  writes/sec, a single-leader RDBMS won't keep up, so we need to shard by
  X" vs "at 10 writes/sec, a single Postgres instance is fine - don't
  over-engineer."
- These estimates feed directly into the case studies in
  `13_DesignCaseStudyURLShortener`, `14_DesignCaseStudyChatSystem`, and
  `15_DesignCaseStudyDistributedCache` - always do a quick capacity pass
  before drawing the architecture diagram.
