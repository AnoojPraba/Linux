A grab bag of smaller, mostly self-contained standard-library and language
features.

Time and math (`<time.h>`, `<math.h>`):

*   time() gives seconds since the Unix epoch; localtime() breaks that
    single number into a struct tm; strftime() formats a struct tm using
    printf-style specifiers, but for dates. clock() measures CPU time
    consumed, not wall-clock time - a sleeping process burns ~0 CPU time
    even as real time passes. See `code/35_TimeAndMath/01_timeBasics.c`.
*   Standard math functions (sqrt, pow, floor, ceil, fabs, trig) all work
    on doubles; trig functions take radians, not degrees. NAN/isnan()
    handle results with no real value (e.g. sqrt of a negative number) -
    a NAN is never `==` to anything, even itself, so isnan() is the only
    correct test. See `code/35_TimeAndMath/02_mathFunctions.c` (links
    against libm via `-lm`, harmless to add globally for files that don't
    need it).

Qualifiers and hints:

*   restrict - a promise to the compiler that two pointer parameters never
    point at overlapping memory during this call, letting it skip
    re-reading one after writing through the other. This is exactly what
    memcpy relies on, and exactly why memcpy's behavior is undefined if
    the buffers *do* overlap (that's what memmove is for). See
    `code/21_RestrictQualifier/01_restrict.c`.
*   inline - a hint (like `register`, see `15_StorageClasses/03_registerAuto.c`)
    that the compiler may substitute a function's body directly at each
    call site instead of emitting a real call; the compiler can ignore it.
    See `code/23_InlineFunctions/01_inline.c`.

Wide characters (`<wchar.h>`):

*   wchar_t stores one "wide" character instead of a single byte, so it
    can represent characters outside the 0-255 range a plain `char`
    covers - necessary for text that doesn't fit in one byte per
    character, unlike the `char[]`/`char *` strings in `06_Strings/`. See
    `code/22_WideChars/01_wcharBasics.c`.
