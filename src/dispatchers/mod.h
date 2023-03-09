#pragma once

#include "../file_reader.h"

typedef void (*const mod_handler)(int w, int d, int reg, int r_m, struct file_reader* reader, file_reader_error error_handler);

void mod__no_displacement(int w, int d, int reg, int r_m, struct file_reader* reader, file_reader_error error_handler);
void mod__8_bit_displacement(int w, int d, int reg, int r_m, struct file_reader* reader, file_reader_error error_handler);
void mod__16_bit_displacement(int w, int d, int reg, int r_m, struct file_reader* reader, file_reader_error error_handler);
void mod__register_mode_no_displacement(int w, int d, int reg, int r_m, struct file_reader* reader, file_reader_error error_handler);

static const mod_handler mod_handlers[4] = {
    mod__no_displacement,
    mod__8_bit_displacement,
    mod__16_bit_displacement,
    mod__register_mode_no_displacement
};
