# Load Balancing

Builds on `../01_ScalabilityBasics` - load balancing is what makes horizontal
scaling actually work by spreading requests across instances.

## L4 vs L7 load balancing

- **L4 (transport layer)**: balances based on IP/port/TCP-connection info
  only, without looking at the actual application content. Fast and
  low-overhead (no need to parse HTTP), but can't make routing decisions
  based on URL path, headers, or cookies.
- **L7 (application layer)**: understands the application protocol (e.g.
  HTTP), so it can route based on URL path, headers, hostname, or cookies
  (e.g. `/api/*` to one service, `/static/*` to another). Also commonly
  terminates TLS at the load balancer, so backend instances don't each need
  to handle certificate management. More CPU-intensive than L4 due to
  parsing/decryption, but far more flexible.
- Real systems often layer both: an L4 balancer/router at the edge for raw
  throughput, with L7 balancers behind it for content-aware routing.

## Load balancing algorithms

- **Round robin**: cycles through servers in order. Simple, works well when
  requests are roughly uniform cost and servers are roughly equal capacity.
- **Least connections**: routes to the server with the fewest active
  connections - better than round robin when request cost varies a lot.
- **Consistent hashing**: maps both servers and request keys onto a hash
  ring; a request goes to the next server clockwise from its key's hash
  position.
  - **Why it matters**: with a naive `hash(key) % N` scheme, adding or
    removing a single node changes N and remaps almost every key to a
    different node - catastrophic for caches (mass cache misses) or shards
    (mass data movement). Consistent hashing bounds the remapping to
    roughly `1/N` of the keys when a node joins or leaves, since only the
    keys between the changed node and its neighbor on the ring move.
  - Used for cache node assignment (e.g. Memcached client-side hashing) and
    database/shard key routing.

## Health checks

- Load balancers periodically probe backend instances (TCP connect, HTTP
  endpoint like `/health`) and stop routing to instances that fail checks.
- Distinguish liveness (is the process up) from readiness (is it ready to
  serve traffic, e.g. warmed caches/DB connections) - routing traffic to a
  live-but-not-ready instance causes errors even though the health check
  technically "passed" a liveness-only check.
