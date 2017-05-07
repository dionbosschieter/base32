#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BUFFER_SIZE 5
#define BITS_5 0x1F
#define GROUPBITS 5

// divide round up macro
#define DIVIDE_CEIL(a,b) ((a / b) + (a % b > 0 ? 1 : 0))

void print40bits(unsigned long b) {
    int i;
    int s = 39;

    for (i = s; i >= 0; i--) {
        unsigned long mask = 1L << i;
        putchar(b & mask ? '1' : '0');
        if (i%5==0) putchar(' ');
    }
    putchar('\n');
}

void print_buffer(unsigned long buffer, size_t length) {
    char base32[33] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','2','3','4','5','6','7','='};

    for (int i=0; i<length; i++) {
        int amount_of_shifts = (7-i) * GROUPBITS;
        int temp = (buffer >> amount_of_shifts) & BITS_5;
        putchar(base32[temp]);
    }

    // needs padding
    if (length % 8 != 0) {
        for (int i = length % 8; i<8; i++) {
            putchar('=');
        }
    }
}

void encode_base32(unsigned const char *input, size_t length) {
    printf("Start:\n");

    unsigned long buffer = 0L;

    // buffer till we have 40 bits
    int buffer_index = 0;

    for (size_t i=0; i < length; i++) {
        // temp save to unsigned long because we can bitshift this > 32
        unsigned long to_mask_with = *input++;
        buffer |= to_mask_with << (32 - (buffer_index * 8));

        if (++buffer_index == BUFFER_SIZE) {
            print_buffer(buffer, 8);
            buffer_index = 0; // start over
            buffer = 0L;
        }
    }

    if (buffer) {
        print_buffer(buffer, DIVIDE_CEIL((length % 5) * 8, 5));
    }

    printf("\nEnd\n");
}

int main() {
    unsigned const char *data = (unsigned char*)"abcdeabc";

    printf("Converting %s\n", data);
    encode_base32(data, 8);
}