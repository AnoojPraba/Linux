# RPC Mechanisms in Modern C++

- Same conceptual territory as `OS/code/53_RpcMechanisms` (dispatch table +
  client stub), but shows the *type-safe* / idiomatic-C++ side of implementing an RPC
  framework's internals instead of a hand-parsed text protocol over raw sockets. See
  that folder's `NOTES.md` for the wire-protocol, marshalling, and delivery-semantics
  material (at-most-once/at-least-once/exactly-once, gRPC/Thrift/JSON-RPC comparison)
  -- not duplicated here.

## Type-safe dispatch table (`01_typeSafeDispatchTable.cpp`)

- `std::unordered_map<std::string, std::function<...>>` mapping procedure name ->
  handler is the same idea as the C demo's `if/else` chain on the parsed procedure
  name, just generalized: any callable can be registered, not just one hardcoded set.
- `std::any` gives type-erased arguments/results so the table can hold handlers with
  different signatures without templating the table itself on a fixed argument list.
  The lambda registered for each procedure is where the type-erasure boundary is
  crossed back to concrete types (`std::any_cast`).
- `std::bad_any_cast` (thrown by a failed `std::any_cast`) is caught and rethrown as a
  custom `ArgumentTypeError` -- mirrors how a real server would reject a malformed
  request instead of crashing.
- Unknown procedure name -> custom `UnknownProcedureError`, same role as an RPC server
  returning a "method not found" status (e.g. gRPC's `UNIMPLEMENTED`).
- **Real frameworks do this via codegen, not by hand**: gRPC/Thrift generate this exact
  dispatch table (server-side "service skeleton") from a `.proto`/`.thrift` schema file
  at build time. The registry concept is identical; only who writes the registration
  code differs (a compiler vs. this file's `registerProcedure()` calls).

## Future-based async client (`02_futureBasedAsyncRpcClient.cpp`)

- Cross-reference: `24_Concurrency/05_futureAsyncPromise.cpp` covers `std::async`/
  `std::future` basics -- this file applies that mechanism to an RPC-shaped problem
  rather than re-teaching futures.
- Why real RPC client libraries (gRPC C++ async API, Thrift async clients) return a
  future/callback instead of blocking: network latency is orders of magnitude larger
  than a local call, and a synchronous call ties up the calling thread for the entire
  round trip. An async API lets the caller issue the call, do other useful work, and
  only pay the waiting cost when it actually needs the result (`.get()`) or poll for
  it opportunistically (`.wait_for()`).
- The tradeoff: async APIs push complexity onto the caller (matching results back up,
  handling cancellation/timeouts, possibly chaining continuations) in exchange for not
  stalling. This is the same sync-vs-async tradeoff the C demo's NOTES.md describes at
  the protocol level; here it shows up as a client-side API design choice.
