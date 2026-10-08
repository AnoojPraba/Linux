# 42_HardwareBuses

Conceptual comparison of UART, I2C, SPI, PCIe and CAN buses plus HAL/BSP (NOTES-only; embedded interview material).

## Files
- `NOTES.md` - UART, I2C, SPI, PCIe, CAN, comparison table, HAL and BSP

## Build and run
- Notes-only folder: nothing to compile.

## Key concepts / interview angles
- UART: asynchronous, 2 wires, baud-rate agreement, framing (start/data/parity/stop).
- I2C: 2-wire multi-master/multi-slave, addressed, open-drain with pull-ups, ACK/NACK, clock stretching.
- SPI: 4-wire full duplex, chip select per slave, fast, no addressing.
- PCIe: high-speed serial point-to-point lanes, packet based, enumeration.
- CAN: differential multi-master bus with arbitration by message ID (automotive).
- HAL vs BSP layering.

## Related
- `../75_UartSerialProgramming`
- `../76_I2cBusProtocol`
- `../61_IOManagementPollingInterruptsDMA`
- `../43_EmbeddedReliabilityAndPowerManagement`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
