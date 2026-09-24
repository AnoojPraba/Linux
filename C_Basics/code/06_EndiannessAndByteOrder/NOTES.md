# Endianness and Byte Order

Companion demo: `01_endianness.c` - detects host endianness at runtime,
manually byte-swaps a 32-bit value and compares the result against
`htonl`/`__builtin_bswap32`, and prints the raw in-memory bytes of an int.

## Why endianness matters

- Network protocols define a "network byte order" (big-endian), so a
  little-endian host like x86 must convert multi-byte fields with
  `htons`/`htonl` before sending them and `ntohs`/`ntohl` after receiving
  them. Skipping the conversion on a little-endian machine silently sends
  byte-swapped values that only happen to work when talking to another
  little-endian machine.
- Binary file formats and serialization protocols must document which byte
  order they use, since the bytes on disk or wire carry no endianness tag
  of their own - the reader has to already know.
- Reading a struct via a raw byte pointer, or `memcpy`-ing one between
  machines of different endianness (e.g. writing a struct on x86 and
  reading it back on a big-endian machine), silently corrupts multi-byte
  fields instead of failing loudly - each field comes back byte-reversed.

## Cross-reference

The socket code that used to live in this repo now lives in `../OS/code/`
after an earlier split (see `../OS/code/49_SocketProgramming`,
`../OS/code/50_UDPSockets`, and `../OS/code/53_RpcMechanisms`). The
`htons(SERVER_PORT)` calls in those files are exactly the endianness
conversion this folder explains - the port number is converted from host
byte order to network (big-endian) byte order before being stored in the
`sockaddr_in` struct.
