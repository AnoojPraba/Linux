# Fixed-Point Arithmetic

- **Q-format notation.** `Qm.n` means m integer bits and n fractional bits
  (plus an implicit sign bit for signed formats). `Q16.16` in `01_qFormatBasics.c`
  is a 32-bit signed integer with 16 integer bits and 16 fractional bits, scaled
  by 2^16 - so the integer value stored is `round(realValue * 2^16)`.
- **The multiply/divide rescaling gotcha.** This is the single most common
  fixed-point bug. Adding or subtracting two Qm.n values needs no rescaling
  (both operands share the same scale factor, so plain integer `+`/`-` works).
  But multiplying two Qm.n values produces a result scaled by `2^(2n)`, not
  `2^n` - so the raw product must be widened (e.g. to `int64_t`) to avoid
  overflow, then shifted right by `n` bits to rescale back down to Qm.n.
  Dividing has the opposite problem: dividing two raw Qm.n integers directly
  throws away the fractional scale entirely, so the dividend must be widened
  and shifted LEFT by `n` bits before the division, not after - shifting
  after division just discards precision that's already been lost.
- **Where fixed-point is used today.**
  - Audio/DSP pipelines (many CODECs and codecs still define Q15/Q31 formats).
  - Sensor fusion and control loops (motor control, IMUs) where deterministic,
    bounded-time arithmetic matters more than dynamic range.
  - Cheap microcontrollers with no hardware FPU, where floating point would
    otherwise require slow software emulation.
