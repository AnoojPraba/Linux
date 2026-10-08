# 06_EndiannessAndByteOrder

Runtime endianness detection and byte swapping (manual vs htonl vs __builtin_bswap32); a classic embedded/networking interview question.

## Files
- `01_endianness.c` - detects host endianness, hand-rolled 32-bit swap compared against htonl/__builtin_bswap32, prints raw bytes of an int
- `NOTES.md` - why endianness matters (network byte order, file formats, struct memcpy across machines) and cross-references to OS socket code

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_endianness.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/06_EndiannessAndByteOrder/` (git-ignored).

## Key concepts / interview angles
- Network byte order is big-endian; x86/ARM Linux hosts are little-endian, so use `htons/htonl/ntohs/ntohl`.
- Detect endianness by reading the first byte of a known multi-byte int through `unsigned char*` (legal aliasing).
- Binary formats carry no endianness tag; the spec must define it.
- Never `memcpy` structs between machines: endianness plus padding both break it.

## Gotchas
- Includes `<arpa/inet.h>` (POSIX), so Linux/macOS only. This Pi is little-endian, so the "big-endian" path is only simulated.

## Related
- `../../../OS/code/52_SocketProgramming` - htons on port numbers
- `../59_MemoryAlignmentAndPadding`
- `../05_BitManipulation`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
