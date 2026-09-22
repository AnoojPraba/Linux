#include <stdio.h>
#include <string.h>

int main()
{
    char sentence[] = "the,quick,brown,fox";
    char *token;

    // strtok modifies its input buffer in place, replacing each delimiter
    // with '\0'; pass NULL on later calls to continue the same tokenization.
    token = strtok(sentence, ",");
    while (token != NULL)
    {
        printf("token: %s\n", token);
        token = strtok(NULL, ",");
    }

    return 0;
}
