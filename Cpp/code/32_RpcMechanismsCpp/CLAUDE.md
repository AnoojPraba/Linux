# 32_RpcMechanismsCpp

RPC mechanics in modern C++: a type-erased dispatch table and a future-based async client (simulated, no real network).

## Files
- `01_typeSafeDispatchTable.cpp` - registry mapping method names to type-erased handlers; arguments and results as type-erased values
- `02_futureBasedAsyncRpcClient.cpp` - simulatedRemoteAdd sleeps to model latency; std::async/future returns the result
- `NOTES.md` - type-safe dispatch table and future-based async client

## Build and run
- Single file: `g++ -std=c++17 -Wall -Wextra -pthread 01_typeSafeDispatchTable.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `Cpp/code/bin/32_RpcMechanismsCpp/` (git-ignored).

## Key concepts / interview angles
- An RPC stack = serialisation/marshalling + transport + dispatch + stubs; here only dispatch and async are shown.
- Type erasure (`std::any`/function wrappers) lets one table hold heterogeneous signatures; check types at registration.
- Futures model asynchronous replies; add timeouts and cancellation in real systems.
- Versioning, idempotency and retries matter more than the call syntax.

## Gotchas
- Simulated latency uses `sleep_for`, so run time is a fraction of a second.

## Related
- `../../../OS/code/56_RpcMechanisms`
- `../../../SystemDesign/topics/10_APIProtocolsRESTvsGRPCvsGraphQL`
- `../24_Concurrency/05_futureAsyncPromise.cpp`
- `../26_TemplateMetaprogramming/06_customAnyTypeErasure.cpp`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.cpp`); keep each `.cpp` self-contained and independently compilable.
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`Cpp/code/bin/` is git-ignored).
