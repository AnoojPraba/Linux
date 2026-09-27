# const / volatile notes

## Deep vs shallow const pointer declarations (`03_deepVsShallowConstCheatSheet.c`)

Read declarations right-to-left starting from the variable name:

| Declaration | Read as | Reassign ptr? | Modify pointee? |
|---|---|:---:|:---:|
| `int *p` | p is a pointer to an int | yes | yes |
| `const int *p` | p is a pointer to a const int | yes | no |
| `int * const p` | p is a const pointer to an int | no | yes |
| `const int * const p` | p is a const pointer to a const int | no | no |

`const int *p` and `int const *p` are equivalent - the `const` binds to `int`
either way. It's only once `const` appears on the right side of the `*`
(`int * const p`) that it applies to the pointer itself.

## Memory-mapped hardware register access (`04_memoryMappedRegisterAccess.c`)

The canonical embedded idiom:

```c
volatile uint32_t *reg = (volatile uint32_t *)REGISTER_ADDRESS;
*reg = value;          // write
uint32_t x = *reg;     // read
```

`volatile` tells the compiler the value at that address can change outside
of the program's control (hardware updates it independently), and that
reads/writes through it have observable side effects it must not optimize
away. Without `volatile` the compiler could:
- Cache a read value in a CPU register and reuse it across multiple
  accesses, missing hardware-side changes (e.g. a status register flipping
  from busy to ready).
- Eliminate a "duplicate" write, e.g. writing to a UART data register twice
  in a row - each write actually transmits a byte, so collapsing them
  would silently drop a transmission.

Multi-register devices are typically described with a struct of volatile
fields, cast from a base address:

```c
volatile struct DeviceRegisters *dev = (volatile struct DeviceRegisters *)BASE_ADDRESS;
dev->statusReg = READY_BIT;
uint32_t data = dev->dataReg;
```

This file's focus is specifically the hardware-register-access idiom. For
`volatile` vs `atomic` under concurrent access (multiple threads, not a
single-threaded program racing with hardware), see
`../../OS/code/11_VolatileVsAtomicEmbedded` - a related but distinct use
of `volatile`.
