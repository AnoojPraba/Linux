# Firmware Update and Flash Storage

Notes-only: a real OTA update / wear-leveling implementation needs real flash
hardware to demonstrate meaningfully - a plain userspace C program on a dev box can't
exercise sector-erase semantics or a bootloader's bank-switch logic, similar to
`42_HardwareBuses`/`43_EmbeddedReliabilityAndPowerManagement`.

## Dual-bank / A-B firmware OTA updates

- Classic safe OTA (over-the-air) pattern: the device has two firmware storage slots,
  bank A and bank B. While running from bank A, a new firmware image downloads into
  the inactive bank B.
- Only after the ENTIRE new image is written AND its checksum/signature is verified
  does the bootloader switch to booting from bank B. The currently-running bank is
  never touched during the download.
- If the new firmware fails to check in as healthy within a watchdog-monitored window
  after booting (see `43_EmbeddedReliabilityAndPowerManagement`'s watchdog coverage),
  the bootloader automatically rolls back to the known-good bank A.
- Why this survives a power loss AT ANY POINT: the device is never overwriting the
  currently-running/only copy of firmware. A power loss during the download just means
  the download was incomplete - on next boot the bootloader still has an intact,
  known-good bank A to boot from, and the update simply retries into bank B next time.
  There is no window in which the device ends up with no bootable firmware at all.

## Firmware signing and verification

- The bootloader should cryptographically verify a new firmware image's signature
  before switching to boot it, to prevent installing corrupted or maliciously modified
  firmware.
- This is the same chain-of-trust idea as `01_BootProcess`'s Secure Boot coverage,
  applied to field firmware updates instead of just the initial boot: a trusted key
  (or key hierarchy) baked into the bootloader/hardware validates a signature over the
  new image before it is ever allowed to run.

## Flash memory constraints

- Unlike RAM, flash memory can only be ERASED in large blocks/sectors, not
  byte-by-byte. A write can only flip bits from 1 to 0; flipping a bit back to 1
  requires erasing the whole containing sector first ("erase before write").
- Implication 1 - small updates are expensive: updating one small piece of data (a
  config value, a log entry) often forces an erase-and-rewrite of an entire sector,
  which is slow compared to a simple in-place RAM write.
- Implication 2 - wear leveling is required: flash sectors have a limited number of
  erase cycles before they wear out (typically tens of thousands to low hundreds of
  thousands, depending on flash type). Frequently-rewritten data needs WEAR LEVELING -
  spreading writes across many different physical sectors over time instead of always
  rewriting the same one, so no single sector wears out early while others sit unused.
- This is why a naive "just write logs/config directly to a fixed flash address"
  design fails in the field over time - that one sector wears out long before the rest
  of the flash. It's why wear-leveling flash translation layers and log-structured
  filesystems designed for flash (e.g. JFFS2, YAFFS, UBIFS) exist: they spread writes
  across sectors and track which physical sector currently holds which logical block.
