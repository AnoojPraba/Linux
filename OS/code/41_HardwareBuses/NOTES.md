# Hardware Buses: UART, I2C, SPI, PCIe

Notes-only: these are hardware-electrical-level protocols - a meaningful demo
needs real hardware (a UART/I2C/SPI peripheral, or a bus analyzer/logic
analyzer) to observe, not something a plain userspace C program on a dev box
can demonstrate. Contrast with `20_Atomics`/`21_AdvancedSyncPrimitives`,
which are OS/language-level concepts and so are fully runnable here.

## UART (Universal Asynchronous Receiver/Transmitter)

- 2-wire (TX/RX), point-to-point only - no addressing, so only 2 devices can
  share a single UART link.
- Asynchronous: no shared clock line. Both ends must agree in advance on a
  baud rate (bit rate); each side times its own sampling off that agreement,
  plus start/stop bits to frame each byte.
- Full-duplex (TX and RX are independent wires, so both directions can be
  active simultaneously).
- Used for: debug consoles/serial terminals, GPS modules, simple
  sensor/bootloader links - anywhere a cheap, simple point-to-point link is
  enough.

## I2C (Inter-Integrated Circuit)

- 2-wire (SDA = data, SCL = clock), synchronous - the clock line means both
  sides sample on the same timing reference, no baud-rate agreement needed.
- Multi-device bus: master-slave, with devices addressed by a 7-bit (or
  10-bit) address on the shared bus - many peripherals can share the same 2
  wires.
- Half-duplex (one direction at a time on SDA).
- Open-drain lines with external pull-up resistors - devices only pull the
  line low, never drive it high, which is what allows multiple devices to
  share the bus without contention.
- Supports clock stretching: a slow slave can hold SCL low to pause the
  master mid-transaction.
- Typical speed: 100 kHz (Standard Mode) to 400 kHz (Fast Mode) up to a few
  MHz (Fast Mode Plus/High Speed) - much slower than SPI.
- Used for: many low-speed peripherals sharing a bus - EEPROMs, sensors,
  RTCs (real-time clocks).

## SPI (Serial Peripheral Interface)

- 4-wire: MOSI (master-out-slave-in), MISO (master-in-slave-out), SCLK
  (clock), and CS/SS (chip select).
- Full-duplex (MOSI and MISO are independent, so data flows both ways at
  once) and generally faster than I2C.
- No shared addressing like I2C - each slave needs its own dedicated CS
  line from the master, so wiring grows with device count.

## PCIe (PCI Express)

- High-speed serial point-to-point interconnect - not a shared bus like
  legacy parallel PCI.
- Organized into lanes (x1/x4/x8/x16); each lane is an independent
  full-duplex pair of differential signal pairs (one pair per direction),
  and lane counts can be combined for more bandwidth.
- Packet-based, switched topology: a root complex connects to switches,
  which fan out to endpoints - traffic is routed/switched, not broadcast on
  a shared wire.
- Layered protocol stack: physical layer (electrical signaling) -> data
  link layer (error detection/retry) -> transaction layer (read/write
  requests, completions).
- Used for: GPUs, NVMe SSDs, NICs - anything needing high bandwidth and low
  latency to the CPU/memory subsystem.
- Supports hot-plug (adding/removing devices without a reboot) and
  MSI/MSI-X interrupts (message-signaled interrupts delivered as a memory
  write, instead of a dedicated interrupt pin).
- Devices are discovered/configured via config-space enumeration at boot
  (or hot-plug time), not fixed wiring.

## Comparison

| Bus   | Wires/Topology                  | Devices per link      | Addressing        | Speed              | Typical use |
|-------|----------------------------------|------------------------|--------------------|---------------------|-------------|
| UART  | 2 (TX/RX), point-to-point        | 2 only                 | none               | low (baud-rate based) | debug console, GPS, bootloader link |
| I2C   | 2 (SDA/SCL), shared bus          | many                   | 7/10-bit address   | low-medium (100kHz-few MHz) | sensors, EEPROMs, RTCs |
| SPI   | 4 (MOSI/MISO/SCLK/CS), star      | many (one CS per device) | none (CS line per device) | medium-high | sensors, flash, displays |
| PCIe  | serial lanes, switched fabric    | many (switched topology) | config-space enumeration | very high (GB/s per lane) | GPUs, NVMe SSDs, NICs |

- Rule of thumb for interviews: UART is the simplest (2 devices, no
  addressing at all); I2C/SPI are peripheral buses for low/medium-speed,
  multi-device, on-board communication; PCIe is a completely different
  scale - a switched, packet-based interconnect for high-bandwidth devices,
  closer in spirit to a small network than to a simple wire.
