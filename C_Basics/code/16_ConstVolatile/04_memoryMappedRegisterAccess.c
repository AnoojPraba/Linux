#include <stdio.h>
#include <stdint.h>

#define REG_SPACE_WORDS 4
#define STATUS_REG_INDEX 0
#define DATA_REG_INDEX 1
#define STATUS_READY_BIT 0x1u
#define SIMULATED_DEVICE_VALUE 0x1234u
#define UART_WRITE_VALUE_1 0x41u
#define UART_WRITE_VALUE_2 0x42u

/* Simulated device register space. In real embedded code this would be a
 * fixed physical address (e.g. 0x40000000) reinterpreted as a pointer;
 * here we stand in with a static array so the demo runs without hardware. */
static uint32_t simulatedRegisterSpace[REG_SPACE_WORDS];

/* A multi-register device: a status register followed by a data register,
 * mirroring how real peripherals expose consecutive registers at a base
 * address. Every field is volatile because the "hardware" (here, our
 * simulation) can change status/data independently of program flow, and
 * writes to dataReg have a side effect (e.g. transmitting a byte) that the
 * compiler must not optimize away. */
struct DeviceRegisters
{
    volatile uint32_t statusReg;
    volatile uint32_t dataReg;
};

/*****************************************************************************
 * Name: demonstrateSingleRegisterAccess
 *
 * Description:
 *         Demonstrates the canonical embedded idiom
 *         `volatile uint32_t *reg = (volatile uint32_t *)ADDRESS;` followed
 *         by reads/writes through it. Without `volatile`, the compiler
 *         could cache the first read in a CPU register and reuse that
 *         stale value for the second read, or could eliminate the second
 *         write below as a "redundant" store since it does not know the
 *         write has an observable hardware side effect (e.g. a UART data
 *         register where every write actually transmits a byte, so
 *         collapsing two writes into one would silently drop a
 *         transmission).
 *
 * Returns:
 *         None.
 *****************************************************************************/
void demonstrateSingleRegisterAccess()
{
    volatile uint32_t *reg = (volatile uint32_t *)&simulatedRegisterSpace[DATA_REG_INDEX];

    *reg = SIMULATED_DEVICE_VALUE;

    uint32_t readBack = *reg;

    printf("Single register: wrote 0x%x, read back 0x%x\n", SIMULATED_DEVICE_VALUE, readBack);

    // Without volatile the compiler could merge these two writes into one,
    // but each write here represents a distinct hardware side effect (e.g.
    // transmitting a byte over UART), so both must actually reach memory.
    *reg = UART_WRITE_VALUE_1;
    *reg = UART_WRITE_VALUE_2;
}

/*****************************************************************************
 * Name: demonstrateStructRegisterAccess
 *
 * Description:
 *         Demonstrates the driver idiom of casting a base address to a
 *         pointer to a struct of volatile registers, then accessing named
 *         fields (`dev->statusReg`, `dev->dataReg`) instead of raw offset
 *         arithmetic. This is how real embedded drivers describe a
 *         peripheral's register layout.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void demonstrateStructRegisterAccess()
{
    volatile struct DeviceRegisters *dev =
        (volatile struct DeviceRegisters *)&simulatedRegisterSpace[STATUS_REG_INDEX];

    dev->statusReg = STATUS_READY_BIT;
    dev->dataReg = SIMULATED_DEVICE_VALUE;

    if ((dev->statusReg & STATUS_READY_BIT) != 0u)
    {
        printf("Struct register: device ready, dataReg = 0x%x\n", dev->dataReg);
    }
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Runs both memory-mapped register access demonstrations. See
 *         ../../OS/code/11_VolatileVsAtomicEmbedded for volatile-vs-atomic
 *         under concurrent access; this file focuses specifically on the
 *         hardware-register-access idiom instead.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    demonstrateSingleRegisterAccess();
    demonstrateStructRegisterAccess();
    return 0;
}
