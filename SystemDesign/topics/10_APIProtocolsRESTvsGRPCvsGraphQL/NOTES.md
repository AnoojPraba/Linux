# API Protocols: REST vs gRPC vs GraphQL

## REST

- Resource-oriented: URLs identify resources (`/users/123`), HTTP verbs
  (`GET`/`POST`/`PUT`/`DELETE`) express the operation.
- Stateless (each request is self-contained) and widely cacheable, since
  HTTP caching semantics (ETags, `Cache-Control`) map naturally onto
  `GET` requests for resources.
- Downsides: over-fetching (endpoint returns more fields than the client
  needs) or under-fetching (client needs multiple round trips to assemble
  what it needs) are common, since the response shape is fixed by the
  server per endpoint.

## gRPC

- Uses Protocol Buffers (protobuf) for a compact binary wire format and
  strongly-typed, schema-driven service contracts (`.proto` files) generate
  client/server code in multiple languages.
- Built on HTTP/2, which enables multiplexed streaming (client-streaming,
  server-streaming, bidirectional streaming), not just simple request/
  response.
- Great fit for internal service-to-service calls within a system: low
  latency, strong typing catches contract mismatches at compile time,
  efficient binary encoding. Less ideal for public-facing browser clients
  historically (needs gRPC-Web/a proxy) and is harder to casually inspect/
  debug than plain JSON over REST.
- Same RPC-over-the-wire fundamentals (marshalling, dispatch, sync vs async
  calls) as the hand-rolled demo in `../../OS/code/51_RpcMechanisms` and the
  C++ walkthrough in `../../Cpp/code/31_RpcMechanismsCpp` - gRPC is a
  production-grade, schema-driven realization of the same ideas.

## GraphQL

- Client specifies exactly the fields it needs in the query itself, across
  potentially nested/related resources, resolved by the server in one
  round trip - directly solves REST's over-fetching/under-fetching problem.
- Single endpoint (typically `/graphql`) instead of many resource URLs;
  the schema (types + resolvers) defines what's queryable.
- Tradeoffs: HTTP-level caching is much harder (everything is typically a
  `POST` to one URL, so there's no natural per-resource cache key the way
  REST's `GET /users/123` has), and naive resolver implementations can
  reintroduce the N+1 query problem server-side when resolving nested
  fields (see `../04_DatabaseIndexingAndQueryOptimization`) unless batching
  (e.g. DataLoader-style) is used.
