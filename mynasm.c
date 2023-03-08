#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "file_reader.h"

void error_handler(const char* msg, enum file_reader_error level) {
    switch (level) {
        case FILE_READER_ERROR_WARN: {
            fprintf(stdout, "%s\n", msg);
        } break ;
        case FILE_READER_ERROR_FATAL: {
            fprintf(stderr, "%s\n", msg);
            exit(1);
        } break ;
        default: {
            assert(false && "log_level is not handled");
        }
    }
}

void dispatch_by_first_byte(byte first_byte, struct file_reader* reader) {
    if ((first_byte >> 2) == 0b100010) {
        // Register/memory to/from register
        // mov instruction
        // 100010 d w    mod reg r/m    (DISP-LO)    (DISP-HI)
        printf("%s", "mov");
        printf(" ");
        int is_dest = (first_byte >> 1) & 1;
        int is_wide = first_byte & 1;

        byte second_byte;
        file_reader__read_byte(reader, &second_byte, error_handler);
        int mod = (second_byte >> 6) & 3;
        int reg = (second_byte >> 3) & 7;
        int r_m = second_byte & 7;

        switch (mod) {
            case MEMORY_MODE_NO_DISPLACEMENT: {
                if (r_m == BP) { // direct address, it has a 16-bit displacement, so check next 2 bytes
                    word data = 0;
                    file_reader__read_byte(reader, (byte*)&data, error_handler);
                    file_reader__read_byte(reader, (byte*)&data + 1, error_handler);

                    if (is_dest == 1) {
                        printf("%s, [%u]", reg_to_word(reg, is_wide), data);
                        printf(" ");
                    } else {
                        printf("[%u], %s", data, reg_to_word(reg, is_wide));
                        printf(" ");
                    }
                } else {
                    if (is_dest == 1) {
                        printf("%s", reg_to_word(reg, is_wide));
                        printf(",");
                        printf("[%s]", effective_address_to_word(r_m));
                        printf(" ");
                    } else {
                        printf("[%s]", effective_address_to_word(r_m));
                        printf(",");
                        printf("%s", reg_to_word(reg, is_wide));
                        printf(" ");
                    }
                }
            } break ;
            case MEMORY_MODE_8_BIT_DISPLACEMENT: {
                byte displacement;
                file_reader__read_byte(reader, &displacement, error_handler);
            } break ;
            case MEMORY_MODE_16_BIT_DISPLACEMENT: {
                word data = 0;
                file_reader__read_byte(reader, (byte*)&data, error_handler);
                file_reader__read_byte(reader, (byte*)&data + 1, error_handler);
            } break ;
            case REGISTER_MODE_NO_DISPLACEMENT: {
                const char* dst_registry;
                const char* src_registry;

                dst_registry = reg_to_word(reg, is_wide);
                // depends on reg which formula to choose and on mod the amount of displacement
                src_registry = reg_to_word(r_m, is_wide);

                if (is_dest == 0) {
                    const char* tmp = src_registry;
                    src_registry = dst_registry;
                    dst_registry = tmp;
                }

                printf("%s, %s", dst_registry, src_registry);
            } break ;
            default: assert(false);
        }
        printf("\n");
    } else if ((first_byte >> 1) == 0b1100011) {
        // mov instruction
        // Immediate to register/memory
        // 1100011 w    mod 0 0 0 r/m    (DISP-LO)    (DISP-HI)    data    data if w = 1
    } else if ((first_byte >> 4) == 0b1011) {
        // mov instruction
        // Immediate to register
        // 1011 w reg    data    data if w = 1

        int   is_wide        = (first_byte >> 3) & 1;
        int   reg            = first_byte & 7;
        const char *reg_word = reg_to_word(reg, is_wide);

        word data = 0;
        file_reader__read_byte(reader, (byte*)&data, error_handler);
        if (is_wide) {
            file_reader__read_byte(reader, (byte*)&data + 1, error_handler);
        }

        printf("%s %s, %u\n", "mov", reg_word, data);
    } else if ((first_byte >> 1) == 0b1010000) {
        // mov instruction
        // Memory to accumulator
        // 1010000 w    addr-lo    addr-hi
    } else if ((first_byte >> 1) == 0b1010001) {
        // mov instruction
        // Accumulator to memory
        // 1010001 w    addr-lo    addr-hi
    } else if (first_byte == 0b10001110) {
        // mov instruction
        // Register/memory to segment register
        // 10001110    mod 0 SR r/m    (DISP-LO)    (DISP-HI)
    } else if (first_byte == 0b10001100) {
        // mov instruction
        // Segment register to register/memory
        // 10001100    mod 0 SR r/m    (DISP-LO)    (DISP-HI)
    } else {
        fprintf(stderr, "first_byte: %d\n", first_byte);
        assert(false && "instruction not implemented");
    }
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("Usage: mynasm filename\n");
        exit(1);
    }

    struct file_reader  reader;

    file_reader__create(&reader, argv[1], error_handler);

    printf("; %s disassembly:\nbits 16\n", argv[1]);
    while (file_reader__eof_reached(&reader) == false) {
        byte first_byte;
        file_reader__read_byte(&reader, &first_byte, error_handler);
        dispatch_by_first_byte(first_byte, &reader);
    }

    file_reader__destroy(&reader);
}
