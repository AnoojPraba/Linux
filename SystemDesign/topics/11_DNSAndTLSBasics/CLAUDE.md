# 11_DNSAndTLSBasics

DNS resolution chain with caching and TLS handshake basics (certificates, key exchange, why symmetric crypto for bulk data).

## Files
- `NOTES.md` - (33 lines) sections: DNS resolution chain; TLS handshake basics

## How to use this note
- Drill: narrate what happens from typing a URL to the first byte, covering DNS and the TLS handshake.
- Be able to explain why DNS changes propagate slowly (TTL caches).
- Case-study answer framework: requirements -> estimates -> API -> data model -> architecture -> deep dives -> trade-offs (question bank: `../28_CommonInterviewQuestionsCheatSheet`).

## Key trade-offs / interview angles
- Resolution: recursive resolver -> root -> TLD -> authoritative; every layer caches by TTL.
- TLS: certificate chain validated up to a trusted CA, asymmetric key exchange agrees a shared secret, then fast symmetric session encryption.
- Asymmetric crypto is too slow for bulk data, so it only bootstraps the session key.
- Follow-ups: SNI, certificate pinning, TLS 1.3 round trips, DNS-based load balancing.

## Related
- `../02_LoadBalancing`
- `../16_SecurityFundamentalsForInterviews`
- `../20_ServiceDiscoveryAndAPIGateway`
- `../../../OS/code/51_NetworkStackBasics`
- `../../../OS/code/69_TcpDeepDive`

## Conventions when extending
- Keep the NOTES.md concise, bullet-style and interview-focused (no essay prose); new topic folders use the next two-digit prefix.
- Conceptual only: no code to build, so there are no binaries to commit.
