#pragma once

#include "../file_reader.h"
#include "mod_tables.h"

typedef void (*const mod_handler)(int w, int d, int reg, int r_m, struct file_reader* reader, file_reader_error error_handler);

void mod__no_ea(int w, int d, int reg, int r_m, struct file_reader* reader, file_reader_error error_handler);
void mod__8_bit_ea(int w, int d, int reg, int r_m, struct file_reader* reader, file_reader_error error_handler);
void mod__16_bit_ea(int w, int d, int reg, int r_m, struct file_reader* reader, file_reader_error error_handler);
void mod__register_mode_no_ea(int w, int d, int reg, int r_m, struct file_reader* reader, file_reader_error error_handler);

typedef void (*const mod_immediate_handler)(int w, int r_m, struct file_reader* reader, file_reader_error error_handler);

void mod__immediate_no_ea(int w, int r_m, struct file_reader* reader, file_reader_error error_handler);
void mod__immediate_8_bit_ea(int w, int r_m, struct file_reader* reader, file_reader_error error_handler);
void mod__immediate_16_bit_ea(int w, int r_m, struct file_reader* reader, file_reader_error error_handler);
void mod__immediate_register_mode_no_ea(int w, int r_m, struct file_reader* reader, file_reader_error error_handler);

static const mod_handler mod_handlers[4] = {
    mod__no_ea,
    mod__8_bit_ea,
    mod__16_bit_ea,
    mod__register_mode_no_ea
};

static const mod_immediate_handler mod_immediate_handlers[4] = {
    mod__immediate_no_ea,
    mod__immediate_8_bit_ea,
    mod__immediate_16_bit_ea,
    mod__immediate_register_mode_no_ea
};
