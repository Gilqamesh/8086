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

    printf("; %s disassembly:\nbits 16\n", argv[1]);
    while (file_reader__read(&reader, file_reader__available(&reader)) > 0) {
        while (file_reader__size(&reader) >= 2) {
            byte first_byte;
            byte second_byte;
            if (!file_reader__eat_byte(&reader, &first_byte)) {
                fprintf(stderr, "Something unexpected happened");
                exit(1);
            }
            if (!file_reader__eat_byte(&reader, &second_byte)) {
                fprintf(stderr, "Parse error, expected second byte");
                exit(1);
            }

            int opcode  = first_byte >> 2;
            int is_dest = (first_byte >> 1) & 1;
            int is_wide = first_byte & 1;
            int mod = (second_byte >> 6) & 3;
            int reg = (second_byte >> 3) & 7;
            int r_m = second_byte & 7;

            const char* instruction = opcode_to_instruction(opcode);
            const char* dst_registry;
            const char* src_registry;
            if (is_wide) {
                dst_registry = reg_to_word_registry(reg);
                src_registry = reg_to_word_registry(r_m);

            } else {
                dst_registry = reg_to_byte_registry(reg);
                src_registry = reg_to_byte_registry(r_m);
            }

            if (is_dest == 0) {
                const char* tmp = src_registry;
                src_registry = dst_registry;
                dst_registry = tmp;
            }

            if (mod == REGISTER_MODE_NO_DISPLACEMENT) {
            } else {
                assert(false && "not implemented mod field");
            }

            printf("%s %s, %s\n", instruction, dst_registry, src_registry);
        }
    }

    file_reader__destroy(&reader);
}
