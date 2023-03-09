#include "mod.h"
#include "../defs.h"

void mod__no_displacement(int w, int d, int reg, int r_m, struct file_reader* reader, file_reader_error error_handler) {
    if (r_m == BP) { // direct address, it has a 16-bit displacement, so check next 2 bytes
        word data = 0;
        file_reader__read_byte(reader, (byte*)&data, error_handler);
        file_reader__read_byte(reader, (byte*)&data + 1, error_handler);

        if (d == 1) {
            printf("%s, [%u]", reg_to_word(reg, w), data);
            printf(" ");
        } else {
            printf("[%u], %s", data, reg_to_word(reg, w));
            printf(" ");
        }
    } else {
        if (d == 1) {
            printf("%s", reg_to_word(reg, w));
            printf(",");
            printf("[%s]", effective_address_to_word(r_m));
            printf(" ");
        } else {
            printf("[%s]", effective_address_to_word(r_m));
            printf(",");
            printf("%s", reg_to_word(reg, w));
            printf(" ");
        }
    }
}

void mod__8_bit_displacement(int w, int d, int reg, int r_m, struct file_reader* reader, file_reader_error error_handler) {
    (void)w;
    (void)d;
    (void)reg;
    (void)r_m;
    byte displacement;
    file_reader__read_byte(reader, &displacement, error_handler);
}

void mod__16_bit_displacement(int w, int d, int reg, int r_m, struct file_reader* reader, file_reader_error error_handler) {
    (void)w;
    (void)d;
    (void)reg;
    (void)r_m;
    word data = 0;
    file_reader__read_byte(reader, (byte*)&data, error_handler);
    file_reader__read_byte(reader, (byte*)&data + 1, error_handler);
}

void mod__register_mode_no_displacement(int w, int d, int reg, int r_m, struct file_reader* reader, file_reader_error error_handler) {
    (void)reader;
    (void)error_handler;
    const char* dst_registry;
    const char* src_registry;

    dst_registry = reg_to_word(reg, w);
    // depends on reg which formula to choose and on mod the amount of displacement
    src_registry = reg_to_word(r_m, w);

    if (d == 0) {
        const char* tmp = src_registry;
        src_registry = dst_registry;
        dst_registry = tmp;
    }

    printf("%s, %s", dst_registry, src_registry);
}
