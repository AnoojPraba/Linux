#include <stdio.h>
#include <stdarg.h>

void miniPrintf(const char *format, ...)
{
    va_list args;
    const char *p;

    va_start(args, format);

    for (p = format; *p != '\0'; p++)
    {
        if (*p != '%')
        {
            putchar(*p);
            continue;
        }

        p++;
        switch (*p)
        {
            case 'd':
                printf("%d", va_arg(args, int));
                break;

            case 's':
                printf("%s", va_arg(args, char *));
                break;

            case 'f':
                printf("%f", va_arg(args, double));
                break;

            default:
                putchar(*p);
                break;
        }
    }

    va_end(args);
}

int main()
{
    miniPrintf("int=%d string=%s float=%f\n", 42, "hello", 3.14);
    return 0;
}
