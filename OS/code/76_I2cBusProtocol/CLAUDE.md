# 76_I2cBusProtocol

I2C protocol from the wire up: a bit-level open-drain bus simulation with an EEPROM slave, and a user-space i2c-dev reader for real hardware, with a senior Q&A.

## Files
- `01_i2cBusSimulation.c` - wired-AND SDA, master bit-bangs START/address/data/ACK/STOP, 24C02-style EEPROM slave
- `02_i2cDevUserspace.c` - open /dev/i2c-N, ioctl(I2C_SLAVE), read a register; usage [bus] [addr_hex] [reg_hex] (defaults bus 1, 0x68, 0x75)
- `NOTES.md` - electrical, protocol, Linux stack, debugging checklist, "Senior interviewer Q&A"

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_i2cBusSimulation.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/76_I2cBusProtocol/` (git-ignored).

## Key concepts / interview angles
- Open-drain lines with pull-ups: any device can pull low (wired-AND), which enables ACK, clock stretching and arbitration.
- Frame: START, 7-bit address + R/W, ACK, data bytes with ACK, STOP; repeated START for register reads.
- Linux: `i2c-dev` char devices, `i2cdetect`, kernel client drivers via device tree.
- Debug: missing/wrong pull-ups, address clashes, bus stuck low, logic analyser.

## Gotchas
- `01` is a pure simulation; `02` needs real I2C hardware, i2c-dev enabled (`raspi-config` or `modprobe i2c-dev`) and permissions, otherwise it reports open/ioctl failure.

## Related
- `../42_HardwareBuses`
- `../75_UartSerialProgramming`
- `../70_InterruptPathAndKernelModules`
- `../../../C_Basics/code/16_ConstVolatile/04_memoryMappedRegisterAccess.c`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
