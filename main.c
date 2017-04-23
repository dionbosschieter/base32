#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BUFFER_SIZE 5

void encode_base32(unsigned const char *input, size_t length) {
    printf("Start:\n");
    unsigned char buffer[BUFFER_SIZE];

    // buffer till we have 40 bits
    int buffer_index = 0;
    for (int i=0; i < length; i++) {
        buffer[buffer_index] = *input++;

        if (++buffer_index == BUFFER_SIZE) {
            buffer_index = 0; // start over
        }
    }
    printf("%s", buffer);

    printf("\nEnd\n");
}

int main() {
    unsigned const char *data = (unsigned char*)"aaaaa";

    printf("Converting %s\n", data);
    encode_base32(data, 5);
}