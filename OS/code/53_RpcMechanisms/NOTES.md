# RPC (Remote Procedure Call) Mechanisms

- Builds directly on `49_SocketProgramming` (TCP client/server basics) - see
  that folder for the socket API itself (`socket()`/`bind()`/`listen()`/
  `accept()`/`connect()`). This folder's demo (`01_rpcServer.c` +
  `02_rpcClient.c`) reuses that boilerplate unchanged and focuses comments
  only on the RPC-specific parts: marshalling, the request-dispatch table,
  and the client-side stub that hides the network call.

## What RPC is, conceptually

- The goal: make a call to a function running in another process (often on
  another machine) look and feel like an ordinary local function call to the
  calling code - `callRemoteProcedure()` in `02_rpcClient.c` is that
  illusion, wrapping "format request, send, block for reply, parse reply"
  behind one call.
- Requires: a wire protocol (how a call is represented as bytes),
  marshalling/serialization (converting in-memory arguments/results to and
  from that wire format), and a dispatch mechanism on the server (map an
  incoming request to the right function and invoke it).
- The demo's protocol is a hand-rolled line-based text format
  (`"procedureName arg1 arg2\n"` in, one line back) - deliberately minimal,
  just enough to show marshal -> send -> dispatch -> unmarshal -> reply. A
  real system would use a schema-driven binary format instead (see below).

## Marshalling / serialization problem

- In-memory representations (struct layout, endianness, pointers) aren't
  portable across processes/machines - marshalling converts arguments (and
  later, results) into a self-describing, portable byte sequence; the
  receiver unmarshals them back into native types.
- Gets harder with complex types: variable-length strings/arrays need
  explicit length prefixes or delimiters; pointers can't be sent at all
  (must serialize what they point to); versioning (adding a field later
  without breaking old clients) needs a schema with optional/tagged fields.

## Synchronous vs asynchronous RPC

- Synchronous (this demo): caller blocks until the response arrives - simple
  to reason about, but ties up the caller for the full round-trip latency.
- Asynchronous: caller gets a future/callback/promise and continues
  immediately; the response is delivered later. Needed for high-throughput
  or fire-and-forget calls, at the cost of more complex client-side
  bookkeeping (matching responses back to the right pending call).

## Delivery semantics

- **At-most-once**: call is sent once; if it's lost or the reply never
  arrives, the caller gives up rather than retry - safe for non-idempotent
  operations, but a genuine failure and a lost-reply-after-success look
  identical to the caller.
- **At-least-once**: caller retries on timeout until it gets a reply -
  simple, but a retried call can execute the procedure more than once if
  the original request actually succeeded and only the *reply* was lost
  (fine for idempotent operations like `add`, dangerous for something like
  "transfer $100").
- **Exactly-once**: the ideal, but genuinely hard to guarantee end-to-end -
  requires the server to deduplicate retried requests (e.g. via a unique
  request ID + a log of already-processed IDs) *and* atomically commit the
  side effect and the "I've done this" record together. Real systems
  approximate it (idempotency keys, dedup with a bounded window) rather
  than solving it perfectly, because true exactly-once requires distributed
  consensus in the general case.

## Real-world RPC frameworks (for context)

- **gRPC + Protocol Buffers**: schema (`.proto` file) defines message types
  and service methods; a compiler generates typed client stubs and server
  skeletons in many languages - marshalling code is generated, not hand
  written like this demo's `dispatchRequest()`/`callRemoteProcedure()`.
  Runs over HTTP/2, supports streaming RPCs (not just single request/reply).
  Not installed on this system, hence the hand-rolled protocol here - but
  the concept is identical, just with generated code instead of a hand-
  written dispatch table.
- **Apache Thrift**: similar schema-driven approach (IDL file -> generated
  stubs), predates gRPC, supports multiple wire protocols/transports.
- **JSON-RPC / XML-RPC**: text-based, human-readable wire formats over
  HTTP - simpler to debug than a binary protocol, less compact/faster than
  protobuf, no built-in schema/codegen (usually).
- **ONC/Sun RPC**: the classic Unix RPC mechanism, notably used by NFS
  (Network File System) - uses XDR (External Data Representation) for
  marshalling. Historically important; still relevant to know for
  interviews given NFS's ubiquity.

## RPC's cost vs a local call

- A local function call can't partially fail (either it runs or the process
  crashed); a remote call can fail *after* the server executed but before
  the reply arrives - the caller cannot always tell success from failure
  from a timeout alone. This is why delivery semantics (above) matter.
- Network latency is orders of magnitude higher than a local call - RPC
  APIs should batch/minimize round trips, not be sprinkled everywhere a
  local function call would be.
- Requires explicit timeouts and retry policies - a local call never
  "hangs" waiting on a dead peer; a network call can, indefinitely, without
  one.
- This partial-failure + latency + retry-semantics combination is a classic
  distributed-systems interview topic - "why is calling a function over the
  network different from calling it locally" is really asking about all
  three points above.
