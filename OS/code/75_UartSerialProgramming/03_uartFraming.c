#include <stdint.h>
#include <stdio.h>
#include <string.h>

// UART is just a raw byte stream: no addressing, no framing, no error
// correction (at most a parity bit). Real firmware layers a protocol on top:
//   [SOF 0x7E][LEN][PAYLOAD x LEN][CRC8]
// and parses it with a byte-at-a-time state machine fed from an RX ring buffer
// that the ISR fills. This demo feeds a noisy stream (garbage, a corrupted
// frame, a frame split across chunks) and recovers only the valid frames.
#define SOF 0x7E
#define MAX_PAYLOAD 16

static uint8_t crc8(const uint8_t *d, size_t n)     // poly 0x07, init 0
{
    uint8_t c = 0;
    while (n--)
    {
        c ^= *d++;
        for (int i = 0; i < 8; i++)
            c = (c & 0x80) ? (uint8_t)((c << 1) ^ 0x07) : (uint8_t)(c << 1);
    }
    return c;
}

// ---- RX ring buffer: single producer (ISR) / single consumer (main loop) ----
#define RB 64
static volatile uint8_t rb[RB];
static volatile unsigned rb_head, rb_tail;           // head: ISR writes, tail: main reads
static void isr_rx_byte(uint8_t b)                   // would be the UART RX interrupt
{
    unsigned next = (rb_head + 1) % RB;
    if (next != rb_tail)                             // drop on overflow (count it in real code)
    {
        rb[rb_head] = b;
        rb_head = next;
    }
}
static int rb_pop(uint8_t *b)
{
    if (rb_tail == rb_head)
        return 0;
    *b = rb[rb_tail];
    rb_tail = (rb_tail + 1) % RB;
    return 1;
}

// ---- parser ----
enum st { WAIT_SOF, WAIT_LEN, PAYLOAD, WAIT_CRC };
static struct { enum st s; uint8_t len, n, buf[MAX_PAYLOAD + 1]; } p;
static int good, bad;

static void parse_byte(uint8_t b)
{
    switch (p.s)
    {
    case WAIT_SOF:
        if (b == SOF)
            p.s = WAIT_LEN;
        break;                                       // anything else = noise, skip
    case WAIT_LEN:
        if (b == 0 || b > MAX_PAYLOAD)               // implausible length: resync
            p.s = (b == SOF) ? WAIT_LEN : WAIT_SOF;
        else
        {
            p.len = b; p.n = 0; p.buf[0] = b;
            p.s = PAYLOAD;
        }
        break;
    case PAYLOAD:
        p.buf[1 + p.n++] = b;
        if (p.n == p.len)
            p.s = WAIT_CRC;
        break;
    case WAIT_CRC:
        if (crc8(p.buf, 1 + p.len) == b)
        {
            printf("  frame OK  (%u bytes): \"%.*s\"\n", p.len, p.len, (char *)&p.buf[1]);
            good++;
        }
        else
        {
            printf("  frame BAD CRC - dropped, resync\n");
            bad++;
        }
        p.s = WAIT_SOF;
        break;
    }
}

static size_t make_frame(uint8_t *out, const char *payload)
{
    uint8_t len = (uint8_t)strlen(payload);
    out[0] = SOF; out[1] = len;
    memcpy(out + 2, payload, len);
    out[2 + len] = crc8(out + 1, 1 + len);
    return 3 + len;
}

int main(void)
{
    uint8_t stream[128]; size_t n = 0;
    stream[n++] = 0x00; stream[n++] = 0xFF; stream[n++] = 0x55;   // line noise
    n += make_frame(stream + n, "temp=21.5");
    size_t bad_at = n;
    n += make_frame(stream + n, "hum=40");
    stream[bad_at + 3] ^= 0x04;                                   // flip one payload bit
    n += make_frame(stream + n, "ok");

    // Deliver in uneven chunks as an ISR would (frames split across reads).
    size_t chunks[] = { 5, 7, 3, 11, 100 }, pos = 0;
    for (size_t c = 0; c < sizeof chunks / sizeof *chunks && pos < n; c++)
    {
        size_t take = chunks[c] < n - pos ? chunks[c] : n - pos;
        for (size_t i = 0; i < take; i++)
            isr_rx_byte(stream[pos + i]);
        pos += take;
        printf("chunk of %zu bytes delivered\n", take);
        uint8_t b;
        while (rb_pop(&b))
            parse_byte(b);
    }
    printf("good=%d bad=%d\n", good, bad);
    return 0;
}
