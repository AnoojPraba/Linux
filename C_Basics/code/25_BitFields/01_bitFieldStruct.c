#include <stdio.h>

// A bit-field member claims exactly N bits instead of a whole byte/word -
// the compiler packs consecutive bit-fields together into as few bytes as
// possible. Useful for mirroring a hardware register or wire-format layout
// where individual flag bits matter, at the cost of losing a portable,
// well-defined memory layout (bit order and padding are implementation
// defined, unlike the explicit shifting/masking in BitManipulation/).
typedef struct
{
    unsigned int isReadable : 1;
    unsigned int isWritable : 1;
    unsigned int isExecutable : 1;
    unsigned int permissionLevel : 4;
} FilePermissions;

int main()
{
    FilePermissions perms;

    perms.isReadable = 1;
    perms.isWritable = 1;
    perms.isExecutable = 0;
    perms.permissionLevel = 7;

    printf("isReadable = %u\n", perms.isReadable);
    printf("isWritable = %u\n", perms.isWritable);
    printf("isExecutable = %u\n", perms.isExecutable);
    printf("permissionLevel = %u\n", perms.permissionLevel);

    // All four fields (1+1+1+4 = 7 bits) fit in far less than a full
    // unsigned int, but the struct is still at least 1 byte, and often
    // padded up to the compiler's usual alignment for this type.
    printf("sizeof(FilePermissions) = %lu bytes\n",
           (unsigned long)sizeof(FilePermissions));

    return 0;
}
