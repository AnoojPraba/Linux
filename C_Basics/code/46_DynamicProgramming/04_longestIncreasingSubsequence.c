#include <stdio.h>

#define ARR_SIZE 8

/*****************************************************************************
 * Name: lisDpQuadratic
 *
 * Description:
 *         Classic O(n^2) DP for the length of the longest strictly
 *         increasing subsequence. lengthAt[i] = length of the longest
 *         increasing subsequence ending exactly at index i. For each i, scan
 *         every earlier j and extend the best qualifying lengthAt[j].
 *
 * Inputs:
 *         arr : the input array.
 *         n   : number of elements in arr.
 *
 * Returns:
 *         The length of the longest strictly increasing subsequence.
 *****************************************************************************/
int lisDpQuadratic(int arr[], int n)
{
    int lengthAt[ARR_SIZE];
    int best;
    int i;
    int j;

    best = 0;

    for (i = 0; i < n; i++)
    {
        lengthAt[i] = 1;

        for (j = 0; j < i; j++)
        {
            if ((arr[j] < arr[i]) && (lengthAt[j] + 1 > lengthAt[i]))
            {
                lengthAt[i] = lengthAt[j] + 1;
            }
        }

        if (lengthAt[i] > best)
        {
            best = lengthAt[i];
        }
    }

    return best;
}

/*****************************************************************************
 * Name: lisPatienceSorting
 *
 * Description:
 *         O(n log n) LIS length via the patience-sorting/binary-search
 *         approach. tails[len - 1] holds the smallest possible tail value of
 *         any increasing subsequence of length len seen so far. Each new
 *         element either extends tails (appended) or replaces the first
 *         tail that is >= it (binary search), keeping tails sorted and as
 *         small as possible so future elements have the best chance to
 *         extend it. tails itself is not a valid subsequence, only its
 *         length matches the true LIS length.
 *
 * Inputs:
 *         arr : the input array.
 *         n   : number of elements in arr.
 *
 * Returns:
 *         The length of the longest strictly increasing subsequence.
 *****************************************************************************/
int lisPatienceSorting(int arr[], int n)
{
    int tails[ARR_SIZE];
    int tailsLen;
    int i;
    int lo;
    int hi;

    tailsLen = 0;

    for (i = 0; i < n; i++)
    {
        lo = 0;
        hi = tailsLen;

        while (lo < hi)
        {
            int mid = (lo + hi) / 2;

            if (tails[mid] < arr[i])
            {
                lo = mid + 1;
            }
            else
            {
                hi = mid;
            }
        }

        tails[lo] = arr[i];

        if (lo == tailsLen)
        {
            tailsLen++;
        }
    }

    return tailsLen;
}

int main()
{
    int arr[ARR_SIZE] = {10, 9, 2, 5, 3, 7, 101, 18};

    printf("LIS length (O(n^2) DP)          = %d\n", lisDpQuadratic(arr, ARR_SIZE));
    printf("LIS length (O(n log n) patience) = %d\n", lisPatienceSorting(arr, ARR_SIZE));

    return 0;
}
