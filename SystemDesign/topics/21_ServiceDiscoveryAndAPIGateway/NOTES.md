# Service Discovery and API Gateway

## Service discovery

- Problem: in a dynamic environment (instances scaling up/down, getting
  rescheduled), how does a service instance find the current network
  address of another service?
- **Client-side discovery**: the client queries a registry (Consul, etcd,
  ZooKeeper) directly and picks an instance itself.
- **Server-side discovery**: a load balancer/router does the lookup; the
  client just calls a fixed endpoint and the routing layer handles the
  rest.

## API Gateway

- Single entry point for external clients, sitting in front of many
  backend services.
- Centralizes cross-cutting concerns so individual services don't each
  reimplement them:
  - Authentication / authorization.
  - Rate limiting (see `12_RateLimiting`).
  - Request routing and composition (fan-out to multiple services).
  - TLS termination (see `11_DNSAndTLSBasics`).
  - Response caching.
- Tradeoffs:
  - Can become a single point of failure / throughput bottleneck if not
    scaled and made highly available itself.
  - Can become a "god object" if too much business logic creeps into it
    instead of staying in the backend services.

## Interview framing

- Service discovery answers "how do services find each other internally";
  the API gateway answers "how do external clients find the system as a
  whole." Both typically sit alongside `20_ResiliencePatterns` (timeouts,
  circuit breakers) at the call boundary.
