#include "mod.h"
#include "../defs.h"

void mod__no_displacement(int w, int d, int reg, int r_m, struct file_reader* reader, file_reader_error error_handler) {
    if (r_m == 0b110) { // direct address, it has a 16-bit displacement, so check next 2 bytes
        mod__16_bit_displacement(w, d, reg, r_m, reader, error_handler);
    } else {
        mod__no_ea_handlers[d](w, reg, r_m);
    }
}

void mod__8_bit_displacement(int w, int d, int reg, int r_m, struct file_reader* reader, file_reader_error error_handler) {
    byte ea;
    file_reader__read_byte(reader, &ea, error_handler);

    mod__ea_handlers[d](w, reg, r_m, ea);
}

void mod__16_bit_displacement(int w, int d, int reg, int r_m, struct file_reader* reader, file_reader_error error_handler) {
    word ea = 0;
    file_reader__read_byte(reader, (byte*)&ea, error_handler);
    file_reader__read_byte(reader, (byte*)&ea + 1, error_handler);

    mod__ea_handlers[d](w, reg, r_m, ea);
}

void mod__register_mode_no_displacement(int w, int d, int reg, int r_m, struct file_reader* reader, file_reader_error error_handler) {
    (void)reader;
    (void)error_handler;

    mod__register_mode_no_ea_handlers[d](w, reg, r_m);
}

void mod__immediate_no_displacement(int w, int d, int reg, int r_m, struct file_reader* reader, file_reader_error error_handler) {
    if (r_m == 0b110) { // direct address, it has a 16-bit displacement, so check next 2 bytes
        mod__immediate_16_bit_displacement(w, d, reg, r_m, reader, error_handler);
    } else {
        word immediate;
        file_reader__read_byte(reader, (byte*)&immediate, error_handler);
        if (w) {
            file_reader__read_byte(reader, (byte*)&immediate + 1, error_handler);
        }
        mod__no_ea_immediate_handlers[d](w, reg, r_m, immediate);
    }
}

void mod__immediate_8_bit_displacement(int w, int d, int reg, int r_m, struct file_reader* reader, file_reader_error error_handler) {
    byte ea;
    file_reader__read_byte(reader, &ea, error_handler);

    word immediate;
    file_reader__read_byte(reader, (byte*)&immediate, error_handler);
    if (w) {
        file_reader__read_byte(reader, (byte*)&immediate + 1, error_handler);
    }

    mod__ea_immediate_handlers[d](w, reg, r_m, immediate, ea);
}

void mod__immediate_16_bit_displacement(int w, int d, int reg, int r_m, struct file_reader* reader, file_reader_error error_handler) {
    word ea = 0;
    file_reader__read_byte(reader, (byte*)&ea, error_handler);
    file_reader__read_byte(reader, (byte*)&ea + 1, error_handler);

    word immediate;
    file_reader__read_byte(reader, (byte*)&immediate, error_handler);
    if (w) {
        file_reader__read_byte(reader, (byte*)&immediate + 1, error_handler);
    }

    mod__ea_immediate_handlers[d](w, reg, r_m, immediate, ea);
}

void mod__immediate_register_mode_no_displacement(int w, int d, int reg, int r_m, struct file_reader* reader, file_reader_error error_handler) {
    (void)reader;
    (void)error_handler;

    word immediate;
    file_reader__read_byte(reader, (byte*)&immediate, error_handler);
    if (w) {
        file_reader__read_byte(reader, (byte*)&immediate + 1, error_handler);
    }

    mod__register_mode_no_ea_immediate_handlers[d](w, reg, r_m, immediate);
}
