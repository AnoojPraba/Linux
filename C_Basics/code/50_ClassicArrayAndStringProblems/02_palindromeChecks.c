#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*****************************************************************************
 * Name: isPalindromeString
 *
 * Description:
 *         Checks whether a string is a palindrome, ignoring non-alphanumeric
 *         characters and case (the classic "valid palindrome" variant).
 *         Uses two pointers starting at each end, skipping over any
 *         non-alphanumeric character on either side before comparing.
 *
 * Inputs:
 *         str : null-terminated string to check.
 *
 * Returns:
 *         1 if str is a palindrome under the ignore-case/non-alnum rule,
 *         0 otherwise.
 *****************************************************************************/
int isPalindromeString(const char *str)
{
    int left;
    int right;

    left = 0;
    right = (int)strlen(str) - 1;

    while (left < right)
    {
        if (!isalnum((unsigned char)str[left]))
        {
            left++;
        }
        else if (!isalnum((unsigned char)str[right]))
        {
            right--;
        }
        else
        {
            if (tolower((unsigned char)str[left]) != tolower((unsigned char)str[right]))
            {
                return 0;
            }
            left++;
            right--;
        }
    }
    return 1;
}

#define TEN 10

/*****************************************************************************
 * Name: isPalindromeInt
 *
 * Description:
 *         Checks whether an integer is a palindrome without converting it
 *         to a string, by building the digit-reversed value arithmetically
 *         (repeatedly peeling off the last digit with %10 and appending it
 *         to a reversed accumulator) and comparing it to the original.
 *
 * Special Considerations:
 *         Negative numbers are never palindromes here (the '-' sign has no
 *         mirrored counterpart), so they short-circuit to 0.
 *
 * Inputs:
 *         number : integer to check.
 *
 * Returns:
 *         1 if number is a palindrome, 0 otherwise.
 *****************************************************************************/
int isPalindromeInt(int number)
{
    int original;
    int reversed;

    if (number < 0)
    {
        return 0;
    }

    original = number;
    reversed = 0;
    while (number > 0)
    {
        reversed = (reversed * TEN) + (number % TEN);
        number = number / TEN;
    }

    return reversed == original;
}

int main(void)
{
    const char *str = "A man, a plan, a canal: Panama";

    printf("\"%s\" isPalindromeString = %d\n", str, isPalindromeString(str));
    printf("isPalindromeInt(12321) = %d\n", isPalindromeInt(12321));
    printf("isPalindromeInt(12345) = %d\n", isPalindromeInt(12345));

    return 0;
}
