#include <stdio.h>

#define MAX_STR_LEN 64

int myStrlen(const char *s)
{
    int len = 0;

    while (s[len] != '\0')
    {
        len++;
    }
    return len;
}

void myStrcpy(char *dst, const char *src)
{
    int i = 0;

    while (src[i] != '\0')
    {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
}

void myStrrev(char *s)
{
    int left = 0;
    int right = myStrlen(s) - 1;

    while (left < right)
    {
        char tmp = s[left];

        s[left] = s[right];
        s[right] = tmp;
        left++;
        right--;
    }
}

int main()
{
    char src[MAX_STR_LEN] = "reverse me";
    char dst[MAX_STR_LEN];

    printf("myStrlen(src) = %d\n", myStrlen(src));

    myStrcpy(dst, src);
    printf("myStrcpy result: %s\n", dst);

    myStrrev(dst);
    printf("myStrrev result: %s\n", dst);

    return 0;
}
