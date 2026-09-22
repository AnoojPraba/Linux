#ifndef MATHUTILS_H
#define MATHUTILS_H

// A C++ compiler mangles function names to encode argument types (so it
// can support overloading) - a plain C compiler never does this, so a
// name like "addTwo" in the resulting object file looks completely
// different depending on which compiler built it. extern "C" tells the
// C++ compiler "don't mangle this - use plain C linkage", which is what
// lets C++ code call into a library actually compiled as C (as
// mathutils.c is here, unchanged from 27_MultiFile/mathutils.c).
// __cplusplus is only defined when this header is included by a C++
// compiler, so a plain C translation unit including this same header
// sees none of this - it's a no-op for C, required only for C++ callers.
#ifdef __cplusplus
extern "C" {
#endif

int addTwo(int a, int b);
int squareOf(int a);

#ifdef __cplusplus
}
#endif

#endif
