#include "utils.h"
#define TOTAL_BITS 32
#define GROUP_SIZE 4
void printBinary(unsigned int num) {
    int bits = sizeof(num) * 8;  // Total bits in the number (usually 32)
    printf("Bin Val:");
    for (int i = bits - 1; i >= 0; i--) {
        // Print the bit
        printf("%d", (num >> i) & 1);

        // Format with space every 4 bits
        if (i % 4 == 0 && i != 0) {
            printf(" ");
        }
    }
    printf("\n");
    printf("Bit pos:");
    for (int i = TOTAL_BITS - 1; i >= 0; i--) {
        if (i % GROUP_SIZE == 0) {
            // Align position under the 4-bit group
            printf(" %3d ", i);
        }
    }
    printf("\n");
    linedisplay();
}

void bitSet(unsigned int num) {
    printf("Number : 0x%x or %u", num, num);
    printf("\n");

    // printf("        ");
    printf("Set bit:");
    int start = -1;
    int i;
    printf ("[ ");
    for (i = 0; i < 32; i++) {
        if (num & (1 << i)) { // Check if the i-th bit is set
            if (start == -1) {
                start = i; // Mark the start of a new group
            }
        } else {
            if (start != -1) {
                // End of a group
                if (i - 1 == start) {
                    printf("%d ", start);
                } else {
                    printf("%d-%d ", start, i - 1);
                }
                start = -1; // Reset start for the next group
            }
        }
    }

    // If the last set of bits was continuous and reached the end
    if (start != -1) {
        if (31 == start) {
            printf("%d ", start);
        } else {
            printf("%d-31 ", start);
        }
    }
    printf ("]\n");
    linedisplay();
    // old implementation
    // int count = 0;
    // printf("Bits that are set in 0x%xare: ", n);
    // while (n) {
    // if(n & 1)
    // printf("%d ",count);
    // count++;
    // n >>= 1;
    // }
    printBinary(num);
}

int main(int argc, char *argv[]) {
    // unsigned int number;
    const char *input;
    if (argc > 1) {
        input = argv[1];
    } else {
        // printf("Enter number: ");
        // scanf("%s", input);
        return -1;
    }
    if(is_hexadecimal(input))
        bitSet(strtoul(input, NULL, 16));
    else if(is_decimal(input))
        bitSet(strtoul(input, NULL, 10));
    else
        printf("Not an acceptable format");
    printf("\n");
    return 0;
}

