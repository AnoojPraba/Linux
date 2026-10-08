# I2C Bus Protocol

Overview/comparison with UART/SPI/PCIe: `../42_HardwareBuses/NOTES.md`.
`01_i2cBusSimulation.c` runs anywhere: a bit-level simulation of the bus with a
bit-banging master and an EEPROM slave state machine. `02_i2cDevUserspace.c` is
the real Linux `/dev/i2c-N` interface; on this Pi no I2C adapter is enabled so it
prints an explanation and exits (the code is for hardware with I2C turned on).

## Electrical
- Two wires: **SDA** (data) and **SCL** (clock), both **open-drain** with
  pull-up resistors (typ. 2.2-10 kOhm; value trades rise time vs power).
  Devices only pull low or release; bus = wired-AND. That allows multiple
  masters, clock stretching and ACKs without bus contention.
- Speeds: 100 kHz (standard), 400 kHz (fast), 1 MHz (fast-mode plus), 3.4 MHz
  (high-speed), 5 MHz (ultra-fast, write-only). Bus capacitance (<= 400 pF)
  limits speed/length - long wires or many devices need lower speed/stronger
  pull-ups/buffers.
- 7-bit addressing (112 usable; 0x00-0x07 and 0x78-0x7F reserved) or 10-bit.
  Address collisions between identical chips -> use address pins, an I2C
  multiplexer (TCA9548A) or separate buses.

## Protocol (what the simulation implements)
1. **START:** SDA falls while SCL is high. **STOP:** SDA rises while SCL high.
   Everywhere else SDA only changes while SCL is LOW (so data is stable
   while SCL is high). A **repeated START** (a START without a STOP) keeps the
   bus owned across a write-then-read.
2. Master sends 7-bit address + R/W bit (0 = write, 1 = read), MSB first.
3. **ACK bit:** after every 8 bits the RECEIVER pulls SDA low for the 9th clock
   (ACK) or leaves it high (NACK). NACK on the address = no such device (basis of
   `i2cdetect`). On reads the MASTER NACKs the last byte to tell the slave to stop.
4. Typical register access: `S addr+W, reg, Sr addr+R, data..., NACK, P`.
- **Clock stretching:** a slave holds SCL low to ask the master to wait (needs
  master support - some SoC masters, including old Raspberry Pi hardware, have
  bugs here).
- **Arbitration (multi-master):** each master watches SDA while sending; a master
  that sends 1 but reads 0 lost and backs off - lossless, because wired-AND
  makes the low win.
- **SMBus** is a stricter subset (timeouts 25-35 ms, PEC checksum, defined
  command formats; used for battery/temperature chips); **PMBus** builds on it.

## Linux stack
- Kernel: I2C adapter drivers (SoC controller) + client drivers (sensors, RTCs,
  PMICs) bound by device tree; `i2c-dev` exposes `/dev/i2c-N` to user space.
- User space: `ioctl(I2C_SLAVE)` then `read/write`; `I2C_RDWR` for combined
  messages with repeated START (atomic w.r.t. other masters); `i2c-tools`
  (`i2cdetect -y N`, `i2cget`, `i2cset`, `i2cdump`); libgpiod/bit-bang adapter
  `i2c-gpio` for arbitrary pins. Prefer a kernel driver (IIO/hwmon/rtc) over
  user-space poking for production; user space is great for bring-up.
- Don't `i2cdetect` blindly on a live system: some devices misinterpret reads
  (EEPROM pointer changes, PMICs).

## Debugging checklist
No ACK: check address (7-bit vs shifted 8-bit - the classic 0x50 vs 0xA0
confusion), pull-ups present, voltage levels (3.3 V vs 5 V - use level shifters),
power/ground, SDA/SCL swapped, device in reset, bus stuck low (a slave mid-byte
after a reset: clock out 9 pulses and send STOP to recover), logic analyzer
decode of START/ACK/STOP, speed too high for capacitance.

## Senior interviewer Q&A
**Q: Why does I2C need pull-up resistors?**
A: Lines are open-drain: devices can only pull low. Pull-ups return the line to
high when released. That wired-AND behavior enables sharing, ACK, arbitration and
clock stretching. Too weak = slow rise times at speed; too strong = excess current
and devices can't pull low enough.

**Q: Walk me through reading a register from a sensor.**
A: START, address+W (device ACKs), register number (ACK), REPEATED START,
address+R (ACK), device clocks out data byte(s), master ACKs all but the last
(NACK), STOP. On Linux: one `I2C_RDWR` ioctl with two messages
(`02_i2cDevUserspace.c`).

**Q: What is the difference between an ACK and a NACK, and what does a NACK mean?**
A: ACK = SDA low on the 9th clock by the receiver. NACK = high: on an address
no one claimed it; on a write the slave can't accept more; on a master read it's
the signal that the last byte was received.

**Q: How do you handle two identical sensors with the same fixed address?**
A: Use their address-select pins, an I2C mux/switch, a second bus, or an
address-translating bridge.

**Q: I2C vs SPI?**
A: I2C: 2 wires, addressing, ACK feedback, multi-master, slower, half-duplex,
pull-ups. SPI: 4+ wires (CS per slave), no addressing/ACK, full duplex, much
faster, simple push-pull drivers. I2C wins on pin count, SPI on throughput.

**Q: What happens if the master crashes mid-transaction?**
A: A slave may still be driving SDA low waiting for clocks, hanging the bus.
Recover by toggling SCL up to 9 times until SDA releases, then issue STOP;
hardware watchdogs/SMBus timeouts help.

**Q: How does a bit-banged I2C master differ from a hardware controller?**
A: Bit-bang: GPIO toggling with delays in software (easy, flexible, CPU-bound,
timing jitter, no clock stretching unless polled, can't be preempted safely).
Hardware: dedicated block with FIFOs/DMA/interrupts, precise timing, lower CPU.
