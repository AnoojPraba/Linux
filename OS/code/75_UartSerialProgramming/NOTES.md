# UART and Serial Programming

Electrical/overview notes: `../42_HardwareBuses/NOTES.md`. This folder adds
protocol detail and runnable C. `01_ptyUartLoopback.c` and `03_uartFraming.c`
run anywhere (pseudo-terminal / simulated ISR); `02_openRealSerialPort.c`
drives a real device (`/dev/ttyUSB0`, `/dev/ttyAMA0`, `/dev/serial0` on a Pi)
and exits politely if none is present.

## The wire
- Idle line is HIGH. A frame: **start bit** (low) + 5-9 **data bits**
  (LSB first, usually 8) + optional **parity** bit + **1 or 2 stop bits** (high).
  "8N1" = 8 data, no parity, 1 stop -> 10 bit-times per byte, so effective
  throughput = baud / 10 bytes/s (115200 baud ~ 11.5 KB/s).
- **Asynchronous:** no clock; both ends must use the same baud rate. The
  receiver detects the start edge and samples each bit at its middle, usually
  with 16x oversampling. Clock mismatch must stay under ~2-3% or later bits are
  mis-sampled (framing errors) - why cheap RC oscillators fail at high baud.
- **Errors detected by hardware:** framing (stop bit not high - wrong baud,
  break), parity, overrun (RX FIFO/register overwritten before software read it).
  A **break** = line held low longer than a frame (used for signalling).
- **Levels:** TTL/CMOS (3.3/5 V) between chips; RS-232 (+-3..15 V, inverted) for
  legacy connectors; RS-485 (differential, half-duplex, multi-drop up to 32+
  nodes, 1200 m) for industrial buses (Modbus RTU); RS-422 full-duplex differential.
  Never connect 3.3 V logic to RS-232 or a 5 V TX without a level shifter.
- **Flow control:** hardware RTS/CTS (separate wires; reliable at speed),
  software XON/XOFF (in-band bytes 0x11/0x13 - breaks binary data), or none
  (then buffers and protocol must prevent overrun).
- Hardware FIFOs (16550: 16 bytes) + interrupts or DMA keep CPUs from servicing
  every byte; at 115200 baud a byte arrives every ~87 us.

## Linux serial programming (`01`, `02`)
- Serial ports are tty devices; use the **termios** API: `tcgetattr` /
  `tcsetattr`, `cfmakeraw` (disable echo, canonical mode, CR/LF translation,
  signals - mandatory for binary protocols), `cfsetispeed/ospeed`, `c_cflag`
  (`CS8`, `PARENB`, `CSTOPB`, `CRTSCTS`, `CLOCAL`, `CREAD`), `c_iflag`
  (`IXON/IXOFF`), `tcflush`, `tcdrain` (wait until transmitted), `tcsendbreak`.
- `open(..., O_RDWR | O_NOCTTY | O_NONBLOCK)`; `CLOCAL` to ignore modem lines.
- **Canonical vs raw mode:** canonical (default) = line-buffered with editing
  (`read` returns per line). Raw = bytes immediately. `VMIN/VTIME` define read
  semantics: (VMIN=0,VTIME=n) timeout; (VMIN=k,VTIME=0) block for k bytes;
  (k,n) inter-byte timer; (0,0) pure non-blocking.
- Use `poll/select/epoll` on the fd for multiple devices. `read` may return
  fewer bytes than a frame - always reassemble (stream, like TCP).
- `ioctl(TIOCMGET/TIOCMSET)` for modem lines (DTR/RTS) - used to reset
  Arduino/ESP boards; `TIOCSSERIAL`/`termios2` (`BOTHER`) for non-standard bauds;
  `TIOCGICOUNT` for error counters.
- USB-serial adapters (FTDI, CP210x) appear as `/dev/ttyUSBn`; permissions via
  `dialout`; udev rules for stable names (`/dev/serial/by-id`).
- Pseudo-terminals (`posix_openpt`, `openpty`) give a fake serial pair for
  testing and are how `ssh`/terminal emulators/`socat PTY,link=...` work.
- Embedded side (bare metal/RTOS): ISR pushes bytes into a ring buffer; main loop
  parses (see `03`); DMA with idle-line interrupt for variable-length frames.
  ISR rules: `../61_IOManagementPollingInterruptsDMA`.

## Framing and robustness (`03_uartFraming.c`)
- A byte stream needs: **start marker** (SOF, resync), **length** (or
  delimiter + escaping like SLIP/COBS), **CRC** (CRC-8/16/32; parity alone is
  weak), optional sequence number/ACK, max-length checks, timeouts to abandon a
  half-received frame.
- Parser = state machine consuming one byte at a time (no blocking, interrupt
  friendly). On any error, drop to `WAIT_SOF` (resync) - never trust length
  fields blindly.
- Text protocols (AT commands, NMEA) use line delimiters; binary protocols
  (Modbus RTU uses a 3.5-char silence as frame gap + CRC16, MAVLink, SLIP/COBS).

## Senior interviewer Q&A
**Q: How does UART receive data with no clock?**
A: Both ends agree on a baud rate; the receiver sees the falling edge of the
start bit, waits half a bit time to land mid-bit, then samples every bit time
(with 16x oversampling and majority voting). The stop bit re-establishes idle so
timing re-syncs on every byte. Mismatch beyond a few percent causes framing errors.

**Q: UART vs SPI vs I2C - how do you choose?**
A: UART: simple, point-to-point, async, no addressing, long cables with RS-485.
I2C: 2 wires, many slaves by address, slow (100-400 kHz-1 MHz), needs pull-ups,
good for sensors/EEPROMs. SPI: fastest (tens of MHz), full-duplex, one CS per
slave, no addressing/ack, good for flash/displays/ADCs. (`../76_I2cBusProtocol`)

**Q: How do you avoid losing bytes at high baud on an MCU/Linux?**
A: Use hardware FIFO + interrupt/DMA with a ring buffer, keep ISR short, enable
flow control (RTS/CTS), size buffers for worst-case latency (bytes = baud/10 x
latency), and monitor overrun counters; on Linux use raw mode, a dedicated reader
thread, and a large enough kernel/userspace buffer.

**Q: Why does `read()` on a serial port block/behave oddly?**
A: Canonical mode returns only on newline; with raw mode `VMIN/VTIME` control
blocking. Also `O_NONBLOCK` vs blocking and `CLOCAL`/carrier-detect waits when
opening without `O_NONBLOCK`.

**Q: How would you detect and handle a corrupted frame?**
A: CRC check on the length-delimited frame, resync by scanning for SOF,
discard + NACK or let the sender time out and retransmit; bounded buffer to
prevent a bogus length from consuming memory.

**Q: How would you debug a flaky UART link?**
A: Verify baud/format on both ends, measure with a logic analyzer/scope (bit
width, idle level, voltage), check grounds and level shifting, inspect error
counters (`TIOCGICOUNT`, `/proc/tty/driver`), add flow control, shorten cables,
check clock accuracy at high baud, and capture raw bytes (`stty -F`, `hexdump`).
