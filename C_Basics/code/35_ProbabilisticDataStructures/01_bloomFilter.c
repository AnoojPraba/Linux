#include <stdio.h>

#define BIT_ARRAY_SIZE 64
#define HASH_COUNT 3
#define FNV_OFFSET_BASIS 2166136261u
#define FNV_PRIME 16777619u
#define DJB2_SEED 5381
#define DJB2_SHIFT 5
#define SDBM_SHIFT 6

typedef struct
{
    unsigned char bits[BIT_ARRAY_SIZE];
} BloomFilter;

/*****************************************************************************
 * Name: initBloomFilter
 *
 * Description:
 *         Clears every bit in the filter's bit array.
 *
 * Inputs:
 *         filter : filter to initialize.
 *
 * Returns:
 *         None
 *****************************************************************************/
void initBloomFilter(BloomFilter *filter)
{
    int i;

    for (i = 0; i < BIT_ARRAY_SIZE; i++)
    {
        filter->bits[i] = 0;
    }
}

/*****************************************************************************
 * Name: hashDjb2
 *
 * Description:
 *         Classic djb2 string hash, folded into the bit array's index range.
 *
 * Inputs:
 *         item : NUL-terminated string to hash.
 *
 * Returns:
 *         Index in [0, BIT_ARRAY_SIZE).
 *****************************************************************************/
unsigned int hashDjb2(const char *item)
{
    unsigned int hash = DJB2_SEED;
    int c;

    while ((c = *item++) != '\0')
    {
        hash = ((hash << DJB2_SHIFT) + hash) + (unsigned int) c;
    }
    return hash % BIT_ARRAY_SIZE;
}

/*****************************************************************************
 * Name: hashSdbm
 *
 * Description:
 *         Classic sdbm string hash (different multiplier/shift than djb2),
 *         folded into the bit array's index range.
 *
 * Inputs:
 *         item : NUL-terminated string to hash.
 *
 * Returns:
 *         Index in [0, BIT_ARRAY_SIZE).
 *****************************************************************************/
unsigned int hashSdbm(const char *item)
{
    unsigned int hash = 0;
    int c;

    while ((c = *item++) != '\0')
    {
        hash = (unsigned int) c + (hash << SDBM_SHIFT) + (hash << (SDBM_SHIFT * 2)) - hash;
    }
    return hash % BIT_ARRAY_SIZE;
}

/*****************************************************************************
 * Name: hashFnv1a
 *
 * Description:
 *         FNV-1a string hash (a third, independently-structured hash from
 *         djb2/sdbm), folded into the bit array's index range.
 *
 * Inputs:
 *         item : NUL-terminated string to hash.
 *
 * Returns:
 *         Index in [0, BIT_ARRAY_SIZE).
 *****************************************************************************/
unsigned int hashFnv1a(const char *item)
{
    unsigned int hash = FNV_OFFSET_BASIS;
    int c;

    while ((c = *item++) != '\0')
    {
        hash ^= (unsigned int) c;
        hash *= FNV_PRIME;
    }
    return hash % BIT_ARRAY_SIZE;
}

/*****************************************************************************
 * Name: setBit
 *
 * Description:
 *         Sets a single bit in the filter's bit array.
 *
 * Inputs:
 *         filter : filter to modify.
 *         index  : bit index to set.
 *
 * Returns:
 *         None
 *****************************************************************************/
void setBit(BloomFilter *filter, unsigned int index)
{
    filter->bits[index / 8] |= (unsigned char) (1u << (index % 8));
}

/*****************************************************************************
 * Name: testBit
 *
 * Description:
 *         Tests whether a single bit in the filter's bit array is set.
 *
 * Inputs:
 *         filter : filter to check.
 *         index  : bit index to test.
 *
 * Returns:
 *         Non-zero if the bit is set, 0 otherwise.
 *****************************************************************************/
int testBit(const BloomFilter *filter, unsigned int index)
{
    return filter->bits[index / 8] & (unsigned char) (1u << (index % 8));
}

/*****************************************************************************
 * Name: bloomAdd
 *
 * Description:
 *         Adds an item to the filter by setting the bit each of the K hash
 *         functions maps it to.
 *
 * Inputs:
 *         filter : filter to modify.
 *         item   : NUL-terminated string to add.
 *
 * Returns:
 *         None
 *****************************************************************************/
void bloomAdd(BloomFilter *filter, const char *item)
{
    setBit(filter, hashDjb2(item));
    setBit(filter, hashSdbm(item));
    setBit(filter, hashFnv1a(item));
}

/*****************************************************************************
 * Name: mightContain
 *
 * Description:
 *         Checks whether an item may have been added: true only if all K
 *         hash bits are set. False positives are possible (bits set by the
 *         union of other items happen to cover this item's bits too); false
 *         negatives are not (a bit this item needs is only ever cleared by
 *         re-initializing the whole filter).
 *
 * Inputs:
 *         filter : filter to check.
 *         item   : NUL-terminated string to check.
 *
 * Returns:
 *         Non-zero if the item might be present, 0 if it is definitely not.
 *****************************************************************************/
int mightContain(const BloomFilter *filter, const char *item)
{
    if (!testBit(filter, hashDjb2(item)))
    {
        return 0;
    }
    if (!testBit(filter, hashSdbm(item)))
    {
        return 0;
    }
    if (!testBit(filter, hashFnv1a(item)))
    {
        return 0;
    }
    return 1;
}

int main(void)
{
    BloomFilter filter;

    initBloomFilter(&filter);
    bloomAdd(&filter, "apple");
    bloomAdd(&filter, "banana");
    bloomAdd(&filter, "cherry");

    printf("mightContain(\"banana\") = %s (definitely absent if 0)\n",
           mightContain(&filter, "banana") ? "maybe present" : "definitely absent");
    printf("mightContain(\"durian\") = %s (definitely absent if 0)\n",
           mightContain(&filter, "durian") ? "maybe present" : "definitely absent");

    return 0;
}
