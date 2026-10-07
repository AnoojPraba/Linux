#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

struct config
{
    int port;
    const char *host;
    uint8_t flags;
    double timeout;
};

// Compile-time checks: fail the BUILD, not the run. Use for struct layout,
// wire-format sizes, array-length assumptions.
_Static_assert(sizeof(int) >= 4, "int must be at least 32 bits");
_Static_assert(offsetof(struct config, port) == 0, "port must be first");

static void show(const struct config *c)
{
    printf("host=%s port=%d flags=0x%02X timeout=%.1f\n", c->host, c->port,
           c->flags, c->timeout);
}

int main(void)
{
    // Designated initializers: order-independent, unnamed fields zeroed.
    struct config a = { .host = "localhost", .port = 8080 };
    show(&a);

    // Array designators: sparse tables without counting.
    int lut[8] = { [2] = 20, [5] = 50 };
    for (int i = 0; i < 8; i++)
        printf("%d ", lut[i]);
    printf("\n");

    // Compound literal: anonymous object with automatic storage, lifetime of
    // the enclosing block. Handy for passing a temporary struct by pointer.
    show(&(struct config){ .host = "example.org", .port = 443, .flags = 0x3 });

    // Zero-initialize anything: = {0} (or `= {}` in C23).
    struct config zero = {0};
    printf("zero.port=%d\n", zero.port);

    // Variable-length array: runtime size on the stack. Optional since C11;
    // no failure signal if too large - prefer malloc/alloca with a check.
    size_t n = 4;
    int vla[n];
    memset(vla, 0, sizeof vla);          // sizeof is evaluated at run time
    printf("sizeof vla = %zu\n", sizeof vla);

    // restrict is covered in 23_RestrictQualifier; _Generic in 31/73.

    // Anonymous struct/union members (C11) and mixed declarations: both used
    // routinely in modern code.
    struct { int type; union { int i; float f; }; } v = { .type = 1, .i = 9 };
    printf("anon union i=%d\n", v.i);
    return 0;
}
