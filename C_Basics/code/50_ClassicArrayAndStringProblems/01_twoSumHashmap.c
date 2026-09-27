#include <stdio.h>

#define ARR_SIZE 5
#define TABLE_SIZE 101
#define NOT_FOUND (-1)

/* A simple hash table (see ../34_HashTable for a from-scratch, reusable
 * implementation) - here inlined since the point is the TWO SUM algorithm
 * pattern, not re-implementing a hash table. Maps value -> index seen so
 * far, using separate chaining. */
typedef struct Entry
{
    int value;
    int index;
    struct Entry *next;
} Entry;

static Entry *table[TABLE_SIZE];

unsigned int hashValue(int value)
{
    return ((unsigned int)(value < 0 ? -value : value)) % TABLE_SIZE;
}

int lookup(int value)
{
    Entry *entry = table[hashValue(value)];

    while (entry != NULL)
    {
        if (entry->value == value)
        {
            return entry->index;
        }
        entry = entry->next;
    }
    return NOT_FOUND;
}

void insertEntry(int value, int index, Entry storage[], int *storageUsed)
{
    unsigned int bucket = hashValue(value);
    Entry *entry = &storage[*storageUsed];

    *storageUsed = *storageUsed + 1;
    entry->value = value;
    entry->index = index;
    entry->next = table[bucket];
    table[bucket] = entry;
}

/*****************************************************************************
 * Name: twoSum
 *
 * Description:
 *         Finds indices of two numbers in an unsorted array that sum to a
 *         target, in O(n): for each element, check whether
 *         (target - element) has already been seen in the hash table; if
 *         so, that earlier index plus the current index is the answer,
 *         otherwise record the current element and keep scanning. This
 *         works on UNSORTED input without needing to sort first (which
 *         would cost O(n log n)), unlike the O(n) two-pointer version in
 *         49_SlidingWindowAndTwoPointer/01_twoPointerPairSum.c, which
 *         requires a SORTED array.
 *
 * Inputs:
 *         arr    : unsorted array of integers.
 *         n      : number of elements in arr.
 *         target : target sum to find.
 *         first  : out-param, index of the first number in the pair.
 *         second : out-param, index of the second number in the pair.
 *
 * Returns:
 *         1 if a pair was found, 0 otherwise.
 *****************************************************************************/
int twoSum(int arr[], int n, int target, int *first, int *second)
{
    Entry storage[ARR_SIZE];
    int storageUsed = 0;
    int i;
    int complementIndex;

    for (i = 0; i < TABLE_SIZE; i++)
    {
        table[i] = NULL;
    }

    for (i = 0; i < n; i++)
    {
        complementIndex = lookup(target - arr[i]);
        if (complementIndex != NOT_FOUND)
        {
            *first = complementIndex;
            *second = i;
            return 1;
        }
        insertEntry(arr[i], i, storage, &storageUsed);
    }
    return 0;
}

int main(void)
{
    int arr[ARR_SIZE] = {2, 7, 11, 15, 3};
    int first;
    int second;

    if (twoSum(arr, ARR_SIZE, 9, &first, &second))
    {
        printf("indices %d and %d sum to 9\n", first, second);
    }
    else
    {
        printf("no pair found\n");
    }

    return 0;
}
