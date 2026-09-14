#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
int is_hexadecimal(const char *input) {
    // Check if the string starts with '0x' or '0X'
    return (strlen(input) > 2 && input[0] == '0' && (input[1] == 'x' || input[1] == 'X'));
}

int is_decimal(const char *input) {
    // Check if all characters are digits
    int i;
    for (i = 0; i < strlen(input); i++) {
        if (!isdigit(input[i])) {
            return 0;
        }
    }
    return 1;
}

void linedisplay(){
    printf("***********************************************\n");
}

