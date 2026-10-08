# 14_DesignCaseStudyChatSystem

Case study: chat system (WebSockets vs polling, ordering and presence, history storage, group fan-out, offline delivery).

## Files
- `NOTES.md` - (58 lines) sections: Real-time delivery; Ordering, delivery guarantees, and presence; Message storage/history; Fan-out for group chats; Offline delivery

## How to use this note
- Whiteboard: connection layer, message service, storage, fan-out and push; then discuss a 100k-member channel.
- State the ordering key (conversation sequence number) and the delivery semantics up front.
- Case-study answer framework: requirements -> estimates -> API -> data model -> architecture -> deep dives -> trade-offs (question bank: `../28_CommonInterviewQuestionsCheatSheet`).

## Key trade-offs / interview angles
- WebSockets are the real-time default; long polling is a fallback; plain polling adds latency and wasted requests.
- Order per conversation with server-assigned sequence numbers (client clocks are untrustworthy); at-least-once delivery with client dedup by message ID.
- Presence: short-TTL key (Redis) refreshed by heartbeats; best-effort and eventually consistent.
- History in a wide-column store partitioned by conversation ID, time-ordered.
- Fan-out on write (fast reads, expensive for huge groups) vs on read (cheap writes, costly reads); mix by group size.
- Offline users are reached by push notifications (APNs/FCM), then fetch history.

## Related
- `../09_MessageQueuesAndEventDrivenArchitecture`
- `../06_SQLvsNoSQLTradeoffs`
- `../33_DesignCaseStudyNewsFeed`
- `../24_IdempotencyInDistributedSystems`
- `../../../OS/code/67_EpollInDepth`

## Conventions when extending
- Keep the NOTES.md concise, bullet-style and interview-focused (no essay prose); new topic folders use the next two-digit prefix.
- Conceptual only: no code to build, so there are no binaries to commit.
