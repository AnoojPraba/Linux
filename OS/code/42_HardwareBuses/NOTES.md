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

## CAN (Controller Area Network)

- Multi-master, differential 2-wire bus (CAN_H/CAN_L) - differential signaling
  gives strong noise immunity, which is why it dominates automotive/industrial
  environments full of electrical noise.
- Message-based, not address-based: there is no destination address at all.
  Each message carries an identifier that other logic uses for two purposes -
  priority arbitration (see below) and content filtering (each node decides
  for itself whether a given identifier is relevant, rather than the sender
  naming a recipient).
- Non-destructive bitwise arbitration: a `0` bit is "dominant" and a `1` bit
  is "recessive" - a dominant bit always wins on the shared wire. If two
  nodes transmit simultaneously, each also reads back the bus while sending
  its identifier; the node with the lower identifier value hits a dominant
  bit first where the other sent a recessive one, notices the mismatch, and
  backs off - the higher-priority (lower-value) message continues
  uninterrupted. This is fundamentally different from Ethernet-style
  collision detection, where a collision corrupts both messages and both
  senders must back off and retry.
- Typical speeds: up to 1 Mbps for classic CAN; CAN FD (Flexible Data-rate)
  supports higher data-phase speeds and larger payloads per frame.
- Used heavily in automotive (ECUs, engine/body/infotainment control) and
  industrial embedded systems where robustness and deterministic priority
  matter more than raw throughput.

## Comparison

- **UART** - 2 wires (TX/RX), point-to-point topology, 2 devices only, no addressing,
  low speed (baud-rate based), typical use: debug console, GPS, bootloader link.
- **I2C** - 2 wires (SDA/SCL), shared bus topology, many devices, 7/10-bit address,
  low-medium speed (100kHz to a few MHz), typical use: sensors, EEPROMs, RTCs.
- **SPI** - 4 wires (MOSI/MISO/SCLK/CS), star topology, many devices (one chip-select
  line per device), no addressing (chip-select line per device instead), medium-high
  speed, typical use: sensors, flash memory, displays.
- **PCIe** - serial lanes, switched fabric topology, many devices (switched topology),
  addressed via config-space enumeration, very high speed (GB/s per lane),
  typical use: GPUs, NVMe SSDs, NICs.
- **CAN** - differential 2 wires (CAN_H/CAN_L), multi-master shared bus topology,
  many nodes, message identifier (no destination address), up to 1 Mbps (higher for
  CAN FD), typical use: automotive ECUs, industrial control.

- Rule of thumb for interviews: UART is the simplest (2 devices, no
  addressing at all); I2C/SPI are peripheral buses for low/medium-speed,
  multi-device, on-board communication; PCIe is a completely different
  scale - a switched, packet-based interconnect for high-bandwidth devices,
  closer in spirit to a small network than to a simple wire.
