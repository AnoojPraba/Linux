# 56_RpcMechanisms

Hand-rolled line-based RPC over TCP loopback (marshalling, dispatch, client stub), plus RPC theory notes.

## Files
- `01_rpcServer.c` - server on port 8090 parsing request lines like "add 2 3" or "reverseString hello", dispatching to procedures
- `02_rpcClient.c` - callRemoteProcedure marshals a request line, sends it, blocks for the response
- `NOTES.md` - what RPC is, marshalling, sync vs async, delivery semantics, real frameworks, RPC cost vs local call

## Build and run
- Two terminals, server first: `gcc -Wall -Wextra -std=gnu11 01_rpcServer.c -o /tmp/srv && /tmp/srv`, then `gcc -Wall -Wextra -std=gnu11 02_rpcClient.c -o /tmp/cli && /tmp/cli`.
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_rpcServer.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/56_RpcMechanisms/` (git-ignored).

## Key concepts / interview angles
- RPC hides the network behind a function call but cannot hide latency, partial failure or timeouts.
- Marshalling: text vs binary (protobuf), byte order, versioning.
- Delivery semantics: at-most-once, at-least-once (needs idempotency), exactly-once is effectively at-least-once plus dedup.
- Sync vs async stubs; deadlines and retries.

## Gotchas
- Uses fixed TCP port 8090 on 127.0.0.1 (not ephemeral); the server accepts a connection and blocks until the client connects.

## Related
- `../../../Cpp/code/32_RpcMechanismsCpp`
- `../52_SocketProgramming`
- `../../../SystemDesign/topics/10_APIProtocolsRESTvsGRPCvsGraphQL`
- `../../../SystemDesign/topics/24_IdempotencyInDistributedSystems`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
