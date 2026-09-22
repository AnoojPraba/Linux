#include <stdio.h>
#include <stdalign.h>

// Poor ordering: compiler must insert padding after 'flag' and 'small' to
// keep 'value' and 'big' aligned to their natural boundaries.
typedef struct
{
    char flag;
    int value;
    char small;
    double big;
} PoorlyOrdered;

// Good ordering: largest members first, so members are naturally aligned
// back-to-back with minimal (or no) padding between them.
typedef struct
{
    double big;
    int value;
    char flag;
    char small;
} WellOrdered;

#pragma pack(push, 1)
// Packed: forces byte alignment, eliminating all padding. Saves memory but
// can cause unaligned access, which is slower (or a bus fault) on some
// architectures - never pack structs shared with hardware/DMA without care.
typedef struct
{
    char flag;
    int value;
    char small;
    double big;
} PackedStruct;
#pragma pack(pop)

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Prints sizeof() for three struct layouts to demonstrate how
 *         member ordering and #pragma pack affect padding.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    printf("sizeof(PoorlyOrdered) = %zu\n", sizeof(PoorlyOrdered));
    printf("sizeof(WellOrdered)   = %zu\n", sizeof(WellOrdered));
    printf("sizeof(PackedStruct)  = %zu (no padding, may be slower to access)\n",
           sizeof(PackedStruct));

    printf("alignof(double) = %zu\n", alignof(double));
    printf("alignof(int)    = %zu\n", alignof(int));

    return 0;
}
