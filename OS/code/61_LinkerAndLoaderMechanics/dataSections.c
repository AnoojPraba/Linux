#include <stdio.h>

// Initialized global data lives in the .data section - it has a nonzero
// initial value that must be stored in the executable file itself.
int initializedGlobal = 42;

// Uninitialized global data lives in .bss - the ELF file only records its
// size, not its bytes (they're implicitly zero), so .bss takes no space in
// the file on disk even though it occupies memory at runtime.
int uninitializedGlobal;

/*****************************************************************************
 * Name: printSections
 *
 * Description:
 *         Prints a reminder of which ELF section each global variable and
 *         this function itself land in, for cross-checking against
 *         "size"/"nm"/"objdump -h" output on the built binary.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void printSections(void)
{
    printf("this function's code lives in .text\n");
    printf("string literals like this one live in .rodata\n");
}
