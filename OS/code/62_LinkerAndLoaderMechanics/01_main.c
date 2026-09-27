#include <stdio.h>

// Declared here, defined in dataSections.c - demonstrates cross-TU symbol
// resolution performed by the linker.
extern int initializedGlobal;
extern int uninitializedGlobal;

void printSections(void);

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Calls into dataSections.c to show global variables split across
 *         ELF sections. Run "size <binary>" or "nm <binary>" on the built
 *         executable to see .data/.bss/.text sizes and symbol placement.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    printf("initializedGlobal (in .data)  = %d\n", initializedGlobal);
    printf("uninitializedGlobal (in .bss) = %d\n", uninitializedGlobal);
    printSections();
    return 0;
}
