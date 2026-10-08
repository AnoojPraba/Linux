# 12_StreamsAndIO

iostream file and string streams: ofstream, ifstream, stringstream parsing.

## Files
- `01_streams.cpp` - writes /tmp/streams_example.txt, reads it back, parses fields from a stringstream

## Build and run
- Single file: `g++ -std=c++17 -Wall -Wextra -pthread 01_streams.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `Cpp/code/bin/12_StreamsAndIO/` (git-ignored).

## Key concepts / interview angles
- Streams are RAII: files close at scope exit; check state with `if (!stream)` / `fail()`.
- `stringstream` is the idiomatic in-memory parser/formatter.
- `std::endl` flushes, `\n` does not; `sync_with_stdio(false)` speeds up large I/O.
- Stream state flags (`eofbit`, `failbit`, `badbit`) and clearing them.

## Gotchas
- Writes a scratch file under `/tmp/`; safe to rerun.

## Related
- `../14_RAII`
- `../../../C_Basics/code/28_FileIO`
- `../../../OS/code/48_IPC`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.cpp`); keep each `.cpp` self-contained and independently compilable.
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`Cpp/code/bin/` is git-ignored).
