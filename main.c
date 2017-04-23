#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BUFFER_SIZE 5
#define BITS_5 0x1F
#define GROUPBITS 5

void print64bits(unsigned long b)
{
    int i;
    int s = 8 * (sizeof b) - 1;

    for (i = s; i >= 0; i--)
    {
        unsigned long mask = 1L << i;
        putchar(b & mask ? '1' : '0');
    }
    putchar('\n');
}

void encode_base32(unsigned const char *input, size_t length) {
    char base32[33] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','2','3','4','5','6','7','='};
    printf("Start:\n");
    //unsigned char buffer[BUFFER_SIZE];
    unsigned long buffer = 0L;

    // buffer till we have 40 bits
    int buffer_index = 0;
    for (size_t i=0; i < length; i++) {
        buffer |= (*input++ << buffer_index * 8);

        if (++buffer_index == BUFFER_SIZE) {
            print64bits(buffer);

            for (int j=0;j<8;j++) {
                int temp = (buffer >> ((7-j)*GROUPBITS)) & BITS_5;
                printf("%c", base32[temp]);
            }

            buffer_index = 0; // start over
            buffer = 0L;
        }
    }

    printf("\nEnd\n");
}

int main() {
    unsigned const char *data = (unsigned char*)"bbbbb";

    printf("Converting %s\n", data);
    encode_base32(data, 5);
}