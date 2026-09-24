# Security Fundamentals for Interviews

## OWASP Top 10 (conceptual level - enough to speak to each)

- **Injection** (SQL, command, etc.): untrusted input is concatenated into
  a query/command instead of being parameterized - fix with parameterized
  queries/prepared statements, never string-building queries from user
  input.
- **Broken authentication**: weak session management, credential stuffing,
  missing MFA - fix with proper session/token handling and rate-limited
  login attempts (see `../12_RateLimiting`).
- **Sensitive data exposure**: secrets/PII stored or transmitted
  unencrypted - fix with encryption at rest and in transit (TLS, see
  `../11_DNSAndTLSBasics`) and not logging sensitive fields.
- Other categories worth a one-line mention if asked: broken access
  control, security misconfiguration, cross-site scripting (XSS),
  insecure deserialization, using components with known vulnerabilities,
  insufficient logging/monitoring, server-side request forgery (SSRF).

## Authentication vs authorization

- **Authentication**: verifying who you are (login, credentials, MFA).
- **Authorization**: verifying what you're allowed to do once
  authenticated (permissions, roles, resource ownership checks). A common
  bug class is confusing the two - checking that a request is
  *authenticated* but forgetting to check it's *authorized* for the
  specific resource being accessed.

## Session-based vs token-based (JWT) auth

- **Session-based**: server stores session state (typically in a shared
  store like Redis) keyed by a session ID the client holds in a cookie -
  server can revoke a session instantly by deleting it, but requires
  server-side state (ties into `../01_ScalabilityBasics`'s statelessness
  discussion).
- **Token-based (JWT)**: a signed token (`header.payload.signature`)
  encodes claims (user ID, roles, expiry) that the server can verify
  without a lookup, enabling stateless auth. Tradeoff: since there's no
  server-side record, a JWT can't be revoked before its expiry without
  extra machinery (a blocklist, short expiries plus refresh tokens) -
  statelessness is gained at the cost of losing easy revocation.

## OAuth2 (conceptual - authorization code grant)

- Client (e.g. a web app) redirects the user to the authorization server
  (e.g. Google) to log in and consent.
- Authorization server redirects back to the client with a short-lived
  authorization code.
- Client exchanges that code (server-to-server, using a client secret) for
  an access token - keeping the actual token exchange off the browser/URL
  bar, which is why this flow is preferred over exposing tokens directly to
  the front end.

## Principle of least privilege

- Grant every user, service, and credential only the minimum access needed
  to do its job, nothing more - limits blast radius if that account/service
  is ever compromised. Applies equally to human permissions (IAM roles) and
  service-to-service credentials.
