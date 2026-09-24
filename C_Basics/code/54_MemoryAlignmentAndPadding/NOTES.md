# Memory Alignment and Padding

- Every type has an alignment requirement (`alignof`); the compiler inserts
  padding so each member sits at an address that is a multiple of its
  alignment.
- Struct size is always a multiple of its own alignment (usually the
  alignment of its largest member) so arrays of the struct stay aligned too.
- Reordering members largest-to-smallest usually minimizes padding - a classic
  "free" memory optimization with zero behavior change.
- `#pragma pack(1)` / `__attribute__((packed))` removes padding entirely:
  saves memory (useful for wire formats/on-disk structs) but risks unaligned
  access, which is slower on x86 and can fault (SIGBUS) on stricter ISAs
  (ARM, SPARC).
- `_Alignas` / `alignas()` (C11) request a *stronger* alignment than default -
  used to put hot data on its own cache line to avoid false sharing.
- `_Alignof` / `alignof()` queries a type's required alignment.
- Interview gotcha: "why is sizeof(struct) bigger than the sum of its
  members?" -> padding for alignment, and trailing padding to make the whole
  struct's size a multiple of its alignment.
- ABI relevance: struct layout (and thus padding) is part of a compiled
  interface - reordering members changes the ABI, breaking binary
  compatibility even though it's source-compatible.

(No separate `#pragma pack`-focused folder: `01_structPadding.c` above
already demonstrates `#pragma pack(push, 1)`/`pop` directly, so a dedicated
folder for it would just duplicate this one.)
