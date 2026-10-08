# 16_SecurityFundamentalsForInterviews

Interview-level security: OWASP Top 10 highlights, authentication vs authorization, session vs JWT auth, OAuth2 authorization code flow and least privilege.

## Files
- `NOTES.md` - (59 lines) sections: OWASP Top 10 (conceptual level - enough to speak to each); Authentication vs authorization; Session-based vs token-based (JWT) auth; OAuth2 (conceptual - authorization code grant); Principle of least privilege

## How to use this note
- Drill: explain injection, broken auth and sensitive data exposure with the fix for each in one sentence.
- Be ready to compare session cookies with JWTs, especially revocation.
- Case-study answer framework: requirements -> estimates -> API -> data model -> architecture -> deep dives -> trade-offs (question bank: `../28_CommonInterviewQuestionsCheatSheet`).

## Key trade-offs / interview angles
- Injection: use parameterised queries; broken authentication: MFA, rate-limited login; sensitive data: encrypt at rest and in transit, do not log secrets.
- Authn proves who; authz checks permission on the specific resource (a common bug is checking only authentication).
- Sessions: server-side state, instant revocation; JWT: stateless verification but hard to revoke (short expiry plus refresh tokens, blocklist).
- OAuth2 authorization code: redirect, code, server-to-server exchange for a token, keeping tokens off the browser URL.
- Least privilege limits blast radius for humans and services.

## Related
- `../11_DNSAndTLSBasics`
- `../12_RateLimiting`
- `../20_ServiceDiscoveryAndAPIGateway`
- `../01_ScalabilityBasics`
- `../../../C_Basics/code/65_SecurityDemos`

## Conventions when extending
- Keep the NOTES.md concise, bullet-style and interview-focused (no essay prose); new topic folders use the next two-digit prefix.
- Conceptual only: no code to build, so there are no binaries to commit.
