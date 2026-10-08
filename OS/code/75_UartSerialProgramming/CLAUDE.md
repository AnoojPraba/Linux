# 75_UartSerialProgramming

UART/serial programming on Linux with termios: a pty-based loopback demo, a generic real-port terminal and a framing protocol with CRC, plus a senior Q&A.

## Files
- `01_ptyUartLoopback.c` - pseudo-terminal pair acts as a serial port while a thread plays the remote device (uppercases input)
- `02_openRealSerialPort.c` - open a real device (default /dev/serial0, 115200 baud, optional text), configure 8N1 raw, print what arrives
- `03_uartFraming.c` - frame [SOF 0x7E][LEN][PAYLOAD][CRC8] with a byte-wise parser state machine
- `NOTES.md` - the wire, Linux serial programming, framing and robustness, "Senior interviewer Q&A"

## Build and run
- `02_openRealSerialPort.c [device] [baud] [text]` needs real hardware; without a device it just fails to open (fine for a build check).
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_ptyUartLoopback.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/75_UartSerialProgramming/` (git-ignored).

## Key concepts / interview angles
- Linux serial ports are tty devices configured via `termios`: raw mode (`cfmakeraw`), 8N1, VMIN/VTIME read semantics, flow control.
- UART is an unframed byte stream: add SOF, length, CRC and a resynchronising parser; handle partial reads.
- A pty has no baud/parity/electrical layer, so settings are accepted but not enforced.
- Typical errors: framing, overrun, parity, baud mismatch.

## Gotchas
- `01` and `03` run without hardware; `02` requires a serial device (e.g. /dev/serial0 on a Raspberry Pi) and permission (dialout group).

## Related
- `../42_HardwareBuses`
- `../76_I2cBusProtocol`
- `../61_IOManagementPollingInterruptsDMA`
- `../../../C_Basics/code/32_StackAndQueue/08_uartRingBuffer.c`
- `../../../C_Basics/code/05_BitManipulation/13_crc8Checksum.c`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
