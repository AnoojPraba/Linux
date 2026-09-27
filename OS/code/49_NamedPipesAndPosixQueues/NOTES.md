# Named Pipes (FIFOs) and POSIX Message Queues

## Named pipe / FIFO (`01_namedPipeFifo.c`)

- `mkfifo()` creates a filesystem entry (visible via `ls`, has a path) of
  type FIFO. Any process that knows the path can `open()` it - contrast with
  the anonymous pipe in `47_IPC/01_pipeCommunication.c`, which only exists
  as a pair of file descriptors and requires a common ancestor process
  (typically via `fork()`) to share those descriptors.
- Still a byte stream, one-way by default (like an anonymous pipe) - open
  two FIFOs for bidirectional communication, or use `O_RDWR` carefully.
- Opening a FIFO for read (or write) blocks until a writer (or reader) also
  opens it - both ends rendezvous at open time.
- Persists in the filesystem until `unlink()`ed - it is not automatically
  cleaned up when a program using it exits, unlike an anonymous pipe's fds.

## POSIX message queue (`02_posixMessageQueue.c`)

- Named like a file (leading `/`, e.g. `/85_demo_mq`), not keyed by an
  integer like the SysV queue in `48_SystemVIPC/02_messageQueue.c`.
- Real message priority: `mq_send()` takes a priority, and higher-priority
  messages are delivered before lower-priority ones already queued. SysV's
  message "type" (`48_SystemVIPC/02_messageQueue.c`) only supports selective
  receive by type, not true priority ordering.
- Has a pollable file descriptor via `mq_notify()` (can be registered for
  asynchronous notification, or used with `select()`/`epoll()` in some
  implementations) - SysV message queues have no equivalent, they can only
  be polled by attempting a non-blocking `msgrcv()`.
- Simpler, more modern API overall - but SysV IPC is older and more likely
  to be present on ancient/embedded Unix variants, so it remains more
  portable in that sense.
- Linking: on this system, `mq_open()`/`mq_send()`/`mq_receive()` require
  `-lrt` (verified by a link failure without it) - see `Makefile`.

## IPC mechanism comparison table

- **Anonymous pipe** (`47_IPC`) - scope: related processes only - structure: byte
  stream - needs a common ancestor process (via fork()).
- **Named pipe / FIFO** (this folder) - scope: any processes, same host - structure:
  byte stream - filesystem path, open() rendezvous.
- **SysV message queue** (`48_SystemVIPC`) - scope: any processes, same host -
  structure: discrete typed messages - integer key, type-based selective receive.
- **POSIX message queue** (this folder) - scope: any processes, same host -
  structure: discrete priority-ordered messages - name-based, `mq_notify()` for
  async events.
- **Shared memory** (`47_IPC`, `48_SystemVIPC`) - scope: any processes, same host -
  structure: raw shared bytes - fastest, but needs external synchronization
  (semaphore/mutex).
- **Sockets (TCP/UDP)** (`51_SocketProgramming`, `52_UDPSockets`) - scope: any
  processes, any host (network) - structure: byte stream or datagrams - only
  mechanism here that crosses machines.

- Rule of thumb: same-host + fastest -> shared memory (+ a semaphore for
  sync); same-host + simple message passing -> a pipe/FIFO or message queue;
  need to cross machines, or want a well-understood client/server model ->
  sockets (see also `55_RpcMechanisms` for building a request/response
  protocol on top of sockets).
