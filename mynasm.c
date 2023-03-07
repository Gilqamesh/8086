#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "file_reader.h"

int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("Usage: mynasm filename\n");
        exit(1);
    }

    struct file_reader  reader;

    if (!file_reader__create(&reader, argv[1])) {
        fprintf(stderr, "Couldn't open file: %s\n", argv[1]);
        exit(1);
    }

    while (file_reader__read(&reader, file_reader__available(&reader)) > 0) {
        while (file_reader__size(&reader) > 0) {
            byte first_byte;
            if (!file_reader__advance(&reader, &first_byte)) {
                fprintf(stderr, "Something unexpected happened");
                exit(1);
            }

            int opcode  = first_byte >> 2;
            int d       = (first_byte >> 1) & 1;
            int w       = first_byte & 1;
            
            // process first byte of the 16-bit 8086 instruction

            byte second_byte;
            if (!file_reader__advance(&reader, &second_byte)) {
                fprintf(stderr, "Parse error, expected second byte");
                exit(1);
            }

            int mod  = (second_byte >> 6) & 3;
            int reg  = (second_byte >> 3) & 7;
            int rm   = second_byte & 7;

            // process second byte of the 16-bit 8086 instruction

            printf("opcode: %d, d: %d, w: %d, mod: %d, reg: %d, rm: %d\n", opcode, d, w, mod, reg, rm);
        }
    }

    printf("\n");

    file_reader__destroy(&reader);
}
