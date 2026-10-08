#include <stdint.h>
#include <stdio.h>
#include <string.h>

// Bit-level simulation of an I2C bus: an open-drain, wired-AND SDA line, a
// master that bit-bangs START / address / data / ACK / STOP exactly like a
// GPIO driver would, and a 24C02-style EEPROM slave (address 0x50) implemented
// as a state machine reacting to SDA/SCL edges - the way real silicon does.
//
// Open-drain: every device may only pull a line LOW or RELEASE it (pull-up
// resistor makes it HIGH). The bus level is the AND of all drivers, so two
// devices never fight: whoever pulls low wins. This is what makes sharing,
// ACK (slave pulls SDA low) and arbitration possible.
static int sda_master = 1, sda_slave = 1;   // 1 = released
static int scl = 1;                         // master drives the clock (no stretching here)
static int bus_sda(void) { return sda_master & sda_slave; }

// ---------------- EEPROM slave (address 0x50) ----------------
enum sstate { S_IDLE, S_ADDR, S_ADDR_ACK, S_WR_DATA, S_WR_ACK, S_RD_DATA, S_RD_ACK };
static struct
{
    enum sstate st;
    uint8_t shift, bits, rw, ptr, mem[256];
    int got_ptr, master_acked;
} dev = { .st = S_IDLE };
#define MY_ADDR 0x50

static void slave_on_event(int prev_sda, int prev_scl)
{
    int sda = bus_sda();
    // START: SDA falls while SCL high. STOP: SDA rises while SCL high.
    if (scl && prev_scl && prev_sda && !sda)
    {
        dev.st = S_ADDR; dev.bits = 0; dev.shift = 0;
        return;
    }
    if (scl && prev_scl && !prev_sda && sda)
    {
        dev.st = S_IDLE; sda_slave = 1;
        return;
    }
    if (!prev_scl && scl)                    // SCL rising edge: data is stable, sample it
    {
        if (dev.st == S_ADDR || dev.st == S_WR_DATA)
        {
            dev.shift = (uint8_t)((dev.shift << 1) | sda);
            dev.bits++;
        }
        else if (dev.st == S_RD_ACK)
            dev.master_acked = (sda == 0);
    }
    if (prev_scl && !scl)                    // SCL falling edge: safe to change SDA
    {
        switch (dev.st)
        {
        case S_ADDR:
            if (dev.bits == 8)
            {
                if ((dev.shift >> 1) == MY_ADDR) { dev.rw = dev.shift & 1; sda_slave = 0; dev.st = S_ADDR_ACK; }
                else dev.st = S_IDLE;        // not for us: stay silent (NACK by omission)
            }
            break;
        case S_ADDR_ACK:
            sda_slave = 1;                   // release after the ACK clock
            if (dev.rw)                      // master wants to READ: send mem[ptr++]
            {
                dev.shift = dev.mem[dev.ptr++]; dev.bits = 0; dev.st = S_RD_DATA;
                sda_slave = (dev.shift >> 7) & 1;
            }
            else { dev.bits = 0; dev.got_ptr = 0; dev.st = S_WR_DATA; }
            break;
        case S_WR_DATA:
            if (dev.bits == 8)
            {
                if (!dev.got_ptr) { dev.ptr = dev.shift; dev.got_ptr = 1; }   // 1st byte = word address
                else dev.mem[dev.ptr++] = dev.shift;                          // then data, auto-increment
                sda_slave = 0; dev.st = S_WR_ACK;
            }
            break;
        case S_WR_ACK:
            sda_slave = 1; dev.bits = 0; dev.shift = 0; dev.st = S_WR_DATA;
            break;
        case S_RD_DATA:
            dev.bits++;
            if (dev.bits < 8) sda_slave = (dev.shift >> (7 - dev.bits)) & 1;
            else { sda_slave = 1; dev.st = S_RD_ACK; }       // release SDA for master's ACK/NACK
            break;
        case S_RD_ACK:
            if (dev.master_acked)            // master ACKed: send the next byte
            {
                dev.shift = dev.mem[dev.ptr++]; dev.bits = 0; dev.st = S_RD_DATA;
                sda_slave = (dev.shift >> 7) & 1;
            }
            else { sda_slave = 1; dev.st = S_IDLE; }         // NACK: master is done
            break;
        default: break;
        }
    }
}

// Any line change goes through here so the slave "sees" each edge.
static void drive(int new_sda_master, int new_scl)
{
    int ps = bus_sda(), pc = scl;
    sda_master = new_sda_master;
    scl = new_scl;
    slave_on_event(ps, pc);
    // The slave may have changed SDA in response (only at SCL falling edges).
}

// ---------------- Master (the bit-banging driver) ----------------
static void m_start(void)  { drive(1, 1); drive(0, 1); drive(0, 0); }   // SDA falls while SCL high
static void m_stop(void)   { drive(0, 0); drive(0, 1); drive(1, 1); }   // SDA rises while SCL high

static int m_write_byte(uint8_t b)               // returns 1 if ACKed
{
    for (int i = 7; i >= 0; i--)
    {
        int bit = (b >> i) & 1;
        drive(bit, 0);                           // set data while SCL low
        drive(bit, 1);                           // clock high: slave samples
        drive(bit, 0);
    }
    drive(1, 0);                                 // release SDA for the ACK slot
    drive(1, 1);
    int ack = (bus_sda() == 0);                  // slave pulling low = ACK
    drive(1, 0);
    return ack;
}

static uint8_t m_read_byte(int send_ack)
{
    uint8_t v = 0;
    drive(1, 0);                                 // release SDA: slave drives it
    for (int i = 0; i < 8; i++)
    {
        drive(1, 1);
        v = (uint8_t)((v << 1) | bus_sda());
        drive(1, 0);
    }
    drive(send_ack ? 0 : 1, 0);                  // master ACK (low) = "send more", NACK = last byte
    drive(send_ack ? 0 : 1, 1);
    drive(send_ack ? 0 : 1, 0);
    drive(1, 0);
    return v;
}

int main(void)
{
    printf("--- probe addresses (like i2cdetect) ---\n");
    for (int a = 0x4F; a <= 0x51; a++)
    {
        m_start();
        int ack = m_write_byte((uint8_t)(a << 1));      // 7-bit address + R/W=0
        m_stop();
        printf("  0x%02X: %s\n", a, ack ? "ACK  (device present)" : "NACK (nobody home)");
    }

    printf("--- write 4 bytes to EEPROM at word address 0x10 ---\n");
    const uint8_t data[] = { 0xDE, 0xAD, 0xBE, 0xEF };
    m_start();
    int ok = m_write_byte((MY_ADDR << 1) | 0);          // S, addr+W
    ok &= m_write_byte(0x10);                           // word address
    for (size_t i = 0; i < sizeof data; i++)
        ok &= m_write_byte(data[i]);
    m_stop();
    printf("  all bytes ACKed: %s\n", ok ? "yes" : "NO");

    printf("--- random read: set pointer, REPEATED START, read 4 ---\n");
    m_start();
    m_write_byte((MY_ADDR << 1) | 0);
    m_write_byte(0x10);                                 // dummy write sets the address pointer
    m_start();                                          // repeated START (no STOP in between)
    m_write_byte((MY_ADDR << 1) | 1);                   // addr+R
    uint8_t rd[4];
    for (int i = 0; i < 4; i++)
        rd[i] = m_read_byte(i < 3);                     // ACK all but the last byte
    m_stop();
    printf("  read back: %02X %02X %02X %02X  (%s)\n", rd[0], rd[1], rd[2], rd[3],
           memcmp(rd, data, 4) == 0 ? "matches" : "MISMATCH");
    return 0;
}
