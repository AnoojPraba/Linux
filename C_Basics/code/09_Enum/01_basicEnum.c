#include <stdio.h>

// Without explicit values, members count up from 0
enum Color
{
    RED,
    GREEN,
    BLUE
};

// Explicit values are useful for flags, protocol constants, etc.
enum HttpStatus
{
    HTTP_OK = 200,
    HTTP_NOT_FOUND = 404,
    HTTP_SERVER_ERROR = 500
};

int main()
{
    enum Color c = GREEN;

    printf("RED = %d, GREEN = %d, BLUE = %d\n", RED, GREEN, BLUE);
    printf("current color = %d\n\n", c);

    printf("HTTP_OK = %d\n", HTTP_OK);
    printf("HTTP_NOT_FOUND = %d\n", HTTP_NOT_FOUND);
    printf("HTTP_SERVER_ERROR = %d\n", HTTP_SERVER_ERROR);

    return 0;
}
