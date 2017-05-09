#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define BUFFER_SIZE 5
#define BITS_5 0x1F
#define GROUPBITS 5

// divide round up macro
#define DIVIDE_CEIL(a,b) ((a / b) + (a % b > 0 ? 1 : 0))

char base32[32] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','2','3','4','5','6','7'};

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

void encode_base32(const char *input, size_t length) {
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

    if (buffer_index > 0) {
        print_buffer(buffer, DIVIDE_CEIL((length % 5) * 8, 5));
    }
    putchar('\n');
}


void print_buffer_as_char(unsigned long buffer, size_t length) {
    for (int i=0;i<length;i++) {
        putchar(buffer >> (32 - (i*8)));
    }
}

void decode_base32(const char *input, size_t length) {
    unsigned long buffer = 0L;
    int buffer_index = 0;

    for (int i=0;i<length;i++) {
        if (input[i] == '=') {
            break;
        }
        unsigned long matched;

        // match char to the base32 array
        for (int j=0;j<32;j++) {
            if (base32[j] == input[i]) {
                matched = j;
            }
        }
        buffer |= matched << (35 - (buffer_index * 5));

        if (++buffer_index == 8) {
            print_buffer_as_char(buffer, 5);
            buffer = 0L;
            buffer_index = 0;
        }
    }

    if (buffer_index > 0) {
        print_buffer_as_char(buffer, DIVIDE_CEIL(buffer_index * 5, 8));
    }
    putchar('\n');
}

int main() {
    const char *to_encode = (char*)"abcdeabc";
    const char *to_decode = (char*)"MFRGGZDFMFRGG===";

    printf("Encoding %s\n", to_encode);
    encode_base32(to_encode, strlen(to_encode));
    printf("Decoding %s\n", to_decode);
    decode_base32(to_decode, strlen(to_decode));
}