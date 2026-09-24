#include <stdio.h>

#define FILE_PATH "/tmp/c_basics_binary_demo.bin"
#define ARRAY_SIZE 5

int main()
{
    int written[ARRAY_SIZE] = {10, 20, 30, 40, 50};
    int readBack[ARRAY_SIZE];
    FILE *fp = fopen(FILE_PATH, "wb");
    size_t itemsWritten;
    size_t itemsRead;
    int i;

    if (fp == NULL)
    {
        printf("Failed to open file for binary write\n");
        return 1;
    }
    itemsWritten = fwrite(written, sizeof(int), ARRAY_SIZE, fp);
    fclose(fp);
    printf("wrote %zu items\n", itemsWritten);

    fp = fopen(FILE_PATH, "rb");
    if (fp == NULL)
    {
        printf("Failed to open file for binary read\n");
        return 1;
    }
    itemsRead = fread(readBack, sizeof(int), ARRAY_SIZE, fp);
    fclose(fp);

    printf("read %zu items: ", itemsRead);
    for (i = 0; i < ARRAY_SIZE; i++)
    {
        printf("%d ", readBack[i]);
    }
    printf("\n");

    return 0;
}
