#include <stdio.h>
#include <stdlib.h>

// A flexible array member must be the struct's last member, and declared
// with no size - it isn't counted in sizeof(struct Buffer) at all. This
// lets one malloc allocate both the struct's fixed fields and its
// variable-length payload as a single contiguous block, instead of a
// struct with a separate malloc'd pointer member (better cache locality,
// one free() instead of two).
typedef struct
{
    int length;
    char data[];
} Buffer;

Buffer *createBuffer(int length)
{
    Buffer *buf = malloc(sizeof(Buffer) + length);

    buf->length = length;
    return buf;
}

int main()
{
    int length = 5;
    Buffer *buf = createBuffer(length);
    int i;

    for (i = 0; i < length; i++)
    {
        buf->data[i] = (char)('a' + i);
    }

    printf("buf->length = %d\n", buf->length);
    printf("buf->data = ");
    for (i = 0; i < length; i++)
    {
        printf("%c", buf->data[i]);
    }
    printf("\n");

    printf("sizeof(Buffer) = %lu (data[] contributes 0)\n",
           (unsigned long)sizeof(Buffer));

    free(buf);
    return 0;
}
