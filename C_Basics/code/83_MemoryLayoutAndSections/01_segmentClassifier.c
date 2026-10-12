// Classify the address of a function, globals, statics, string literal, heap
// block and local into code/data/bss/heap-or-stack using the linker symbols
// etext, edata and end (GNU ld).

#include <stdio.h>
#include <stdlib.h>

extern char etext, edata, end;       /* boundary symbols supplied by the GNU linker */

int init_global = 42;                /* initialized data   */
int zero_global;                     /* bss                */
static int static_zero;              /* bss                */
static int static_init = 7;          /* initialized data   */
const char *literal = "hello";       /* pointer in data, characters in read-only data */

static const char *where(const void *p)
{
    const char *a = p;
    if (a < &etext) return "code";
    if (a < &edata) return "data";
    if (a < &end)   return "bss";
    return "heap, stack or mapped memory";
}

int main(void)
{
    int local = 1;
    int *heap = malloc(sizeof *heap);
    static int counter;                          /* static local: bss, not the stack */

    printf("main (function)     : %s\n", where((void *)main));
    printf("init_global         : %s\n", where(&init_global));
    printf("static_init         : %s\n", where(&static_init));
    printf("zero_global         : %s\n", where(&zero_global));
    printf("static_zero         : %s\n", where(&static_zero));
    printf("static local counter: %s\n", where(&counter));
    printf("string literal bytes: %s\n", where(literal));
    printf("malloc'd block      : %s\n", where(heap));
    printf("local variable      : %s\n", where(&local));
    printf("heap below stack    : %s\n", (void *)heap < (void *)&local ? "yes" : "no");
    free(heap);
    return 0;
}
