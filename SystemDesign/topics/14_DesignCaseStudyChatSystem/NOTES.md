# Design Case Study: Chat System

## Real-time delivery

- **WebSockets**: persistent, full-duplex connection - server can push
  messages to the client the instant they arrive. The standard choice for
  real-time chat.
- **Long polling**: client makes a request that the server holds open until
  there's a message (or a timeout), then the client immediately
  re-requests. Works without WebSocket support, but higher overhead
  (repeated connection setup) than a persistent socket.
- **Plain polling**: client asks "anything new?" on a fixed interval -
  simplest to implement, but adds latency (up to the poll interval) and
  wastes requests when there's nothing new. Rarely acceptable for "real-
  time" chat today.

## Ordering, delivery guarantees, and presence

- Message ordering per conversation typically needs a sequence number or
  server-assigned timestamp, since client clocks aren't trustworthy and
  network delivery order isn't guaranteed.
- Delivery guarantees mirror the general messaging semantics in
  `../09_MessageQueuesAndEventDrivenArchitecture` (at-least-once is typical,
  with the client deduplicating by message ID).
- **Online-presence tracking**: typically a lightweight, short-TTL
  key-value entry per user (e.g. in Redis) updated by heartbeats from the
  active connection; other users query/subscribe to presence changes.
  Presence is inherently a best-effort/eventually-consistent signal, not a
  strongly consistent one.

## Message storage/history

- A wide-column store (e.g. Cassandra) is a good fit for chat history: data
  is naturally partitioned by conversation ID, written once and rarely
  updated, queried in time order (most-recent-first), and needs to scale to
  huge write volumes across many conversations - exactly wide-column
  stores' sweet spot (see `../06_SQLvsNoSQLTradeoffs`).

## Fan-out for group chats

- **Fan-out on write**: when a message is sent, immediately write a copy
  into every group member's inbox/timeline. Reads are then fast (just read
  your own inbox), but a very large group makes writes expensive (fan-out
  to potentially thousands of members per message).
- **Fan-out on read**: store the message once per conversation; each
  member's client queries the conversation directly when reading. Writes
  stay cheap regardless of group size, but reads must aggregate across
  conversations, which is more expensive per read.
- Real systems often mix both: fan-out on write for normal-sized groups/DMs,
  fan-out on read for very large groups/channels where write fan-out would
  be prohibitively expensive.

## Offline delivery

- Push notifications (APNs/FCM) wake a user's device when they're offline
  or the app is backgrounded, prompting them to open the app and fetch
  missed messages from history rather than relying on a live socket that
  isn't connected.
