#include <stdio.h>

#define FILE_PATH "/tmp/c_basics_fileio_demo.txt"

int main()
{
    FILE *fp = fopen(FILE_PATH, "w");
    char line[128];

    if (fp == NULL)
    {
        printf("Failed to open file for writing\n");
        return 1;
    }

    fprintf(fp, "line one\n");
    fprintf(fp, "line two: %d\n", 42);
    fclose(fp);

    fp = fopen(FILE_PATH, "r");
    if (fp == NULL)
    {
        printf("Failed to open file for reading\n");
        return 1;
    }

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        printf("read: %s", line);
    }
    fclose(fp);

    return 0;
}
