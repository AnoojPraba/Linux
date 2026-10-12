// C as a mid-level language: structured data and control flow next to bit
// masks, addresses and byte-order inspection.

#include <stdio.h>
#include <stdint.h>

#define LED_BIT 3
#define ENABLE_BIT 0

int main(void)
{
    /* High-level style: functions, structured control flow, typed data. */
    struct { const char *name; int score; } team[] = { {"ann", 7}, {"raj", 9}, {"li", 8} };
    int best = 0;
    for (int i = 1; i < 3; i++)
        if (team[i].score > team[best].score)
            best = i;
    printf("best: %s\n", team[best].name);

    /* Low-level style: bits, addresses and the exact layout of data. */
    uint8_t reg = 0;                              /* pretend this is a hardware register */
    reg |= (uint8_t)(1u << ENABLE_BIT);
    reg |= (uint8_t)(1u << LED_BIT);
    volatile uint8_t *p = &reg;                   /* access memory through an address */
    printf("register = 0x%02X, byte 0 of 0x01020304 is %d\n", *p,
           *(unsigned char *)&(uint32_t){ 0x01020304 });
    return 0;
}
