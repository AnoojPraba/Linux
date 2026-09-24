#include <stdio.h>
#include <wchar.h>
#include <locale.h>

int main()
{
    // wchar_t stores one "wide" character instead of a single byte, so it
    // can represent characters outside the 0-255 range a plain char covers
    // - necessary for scripts/symbols that don't fit in one byte, which a
    // char[]/char* string (see 07_Strings/) cannot hold directly.
    wchar_t greeting[] = L"Hello, éèê!";

    // The locale controls how wide characters are converted for output -
    // without setting one, wprintf may not print non-ASCII characters
    // correctly.
    setlocale(LC_ALL, "");

    wprintf(L"%ls\n", greeting);
    printf("wcslen = %zu wide characters\n", wcslen(greeting));

    return 0;
}
