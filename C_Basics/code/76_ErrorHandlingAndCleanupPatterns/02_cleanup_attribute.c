#include <stdio.h>
#include <stdlib.h>

// GCC/Clang extension: __attribute__((cleanup(fn))) runs fn(&var) when the
// variable goes out of scope - C's closest thing to RAII. Not standard C
// (C23 has no equivalent; C2y proposes `defer`), but systemd, GLib and the
// kernel's cleanup.h use it.
static void free_ptr(void *p)
{
    void **pp = p;
    printf("  [cleanup] free(%p)\n", *pp);
    free(*pp);
}

static void close_file(FILE **f)
{
    if (*f)
    {
        printf("  [cleanup] fclose\n");
        fclose(*f);
    }
}

#define AUTO_FREE __attribute__((cleanup(free_ptr)))
#define AUTO_FILE __attribute__((cleanup(close_file)))

static int work(int fail_early)
{
    AUTO_FREE char *a = malloc(16);
    if (fail_early)
    {
        printf("  early return\n");
        return -1;          // 'a' still freed
    }
    AUTO_FILE FILE *f = fopen("/etc/hostname", "r");
    AUTO_FREE char *b = malloc(32);
    printf("  normal return\n");
    (void)a; (void)f; (void)b;
    return 0;               // destroyed in REVERSE declaration order: b, f, a
}

int main(void)
{
    printf("early:\n");
    work(1);
    printf("normal:\n");
    work(0);
    return 0;
}
