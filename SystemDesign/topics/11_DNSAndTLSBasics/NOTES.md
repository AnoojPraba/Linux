# DNS and TLS Basics

## DNS resolution chain

- Client asks a **recursive resolver** (often the ISP's or a public one like
  8.8.8.8) to resolve a domain.
- Recursive resolver queries a **root** nameserver, which points to the
  right **TLD** (top-level domain, e.g. `.com`) nameserver, which points to
  the domain's **authoritative** nameserver, which finally returns the
  actual record (e.g. an A/AAAA record with the IP).
- **Caching at each level**: every response includes a TTL, and resolvers
  (and the client's own OS/browser) cache the answer for that TTL - this is
  why most lookups never walk the full chain and why DNS changes take time
  to propagate (until every cache holding the old TTL expires).

## TLS handshake basics

- Goal: establish a secure channel where the client is convinced it's
  talking to the real server, and both sides share a key that only they
  know.
- **Certificate validation**: server presents a certificate (its public
  key, signed by a trusted Certificate Authority); the client verifies the
  signature chain up to a CA it already trusts, confirming the server's
  identity.
- **Key exchange**: an asymmetric (public/private key) exchange is used to
  securely agree on a shared secret, which both sides then use to derive a
  symmetric session key for the actual data transfer.
- **Why not just use asymmetric crypto for everything**: asymmetric
  algorithms (RSA, ECC) are computationally far more expensive than
  symmetric ones (AES) - fine for the one-time handshake, but far too slow
  to encrypt/decrypt every byte of bulk application traffic. So TLS uses
  asymmetric crypto only to bootstrap a symmetric session key, then
  switches to fast symmetric encryption for the actual connection.
