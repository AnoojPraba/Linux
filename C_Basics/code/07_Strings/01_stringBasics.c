#include <stdio.h>
#include <string.h>

int main()
{
    char greeting[32] = "Hello";
    char name[] = "World";
    char combined[64];

    // A C string is just a char array terminated by '\0'; strlen counts bytes
    // before that terminator, not the array's declared size.
    printf("strlen(greeting) = %lu\n", (unsigned long)strlen(greeting));

    strcpy(combined, greeting);
    strcat(combined, ", ");
    strcat(combined, name);
    strcat(combined, "!");
    printf("combined = %s\n", combined);

    printf("strcmp(greeting, name) = %d\n", strcmp(greeting, name));
    printf("strcmp(greeting, \"Hello\") = %d\n\n", strcmp(greeting, "Hello"));

    char *found = strchr(combined, ',');
    if (found != NULL)
    {
        printf("Found ',' at offset %ld\n", (long)(found - combined));
    }

    return 0;
}
