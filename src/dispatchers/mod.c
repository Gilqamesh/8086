#include "mod.h"
#include "../defs.h"

void mod__no_ea(int w, int d, int reg, int r_m, struct file_reader* reader, file_reader_error error_handler) {
    if (r_m == 0b110) {
        word direct_address;
        file_reader__read_word(reader, &direct_address, error_handler);

        mod__no_ea_da_handlers[d](w, reg, direct_address);
    } else {
        mod__no_ea_handlers[d](w, reg, r_m);
    }
}

void mod__8_bit_ea(int w, int d, int reg, int r_m, struct file_reader* reader, file_reader_error error_handler) {
    byte ea;
    file_reader__read_byte(reader, &ea, error_handler);

    mod__ea_handlers[d](w, reg, r_m, (char)ea);
}

void mod__16_bit_ea(int w, int d, int reg, int r_m, struct file_reader* reader, file_reader_error error_handler) {
    word ea;
    file_reader__read_word(reader, &ea, error_handler);

    mod__ea_handlers[d](w, reg, r_m, ea);
}

void mod__register_mode_no_ea(int w, int d, int reg, int r_m, struct file_reader* reader, file_reader_error error_handler) {
    (void)reader;
    (void)error_handler;

    mod__register_mode_handlers[d](w, reg, r_m);
}

void mod__immediate_no_ea(int w, int r_m, struct file_reader* reader, file_reader_error error_handler) {
    if (r_m == 0b110) {
        word direct_address;
        file_reader__read_word(reader, &direct_address, error_handler);

        mod__no_ea_immediate__da(w, r_m, direct_address);

        printf("\n"); // remove after test, here to flush
        assert(false && "todo: test to see if correct, can this case even happen?");
    } else {
        word immediate;
        file_reader__read_by_type[w](reader, &immediate, error_handler);
        mod__no_ea_immediate(w, r_m, immediate);
    }
}

void mod__immediate_8_bit_ea(int w, int r_m, struct file_reader* reader, file_reader_error error_handler) {
    byte ea;
    file_reader__read_byte(reader, &ea, error_handler);

    word immediate;
    file_reader__read_by_type[w](reader, &immediate, error_handler);

    mod__ea_immediate(w, r_m, immediate, ea);
}

void mod__immediate_16_bit_ea(int w, int r_m, struct file_reader* reader, file_reader_error error_handler) {
    word ea = 0;
    file_reader__read_word(reader, &ea, error_handler);

    word immediate;
    file_reader__read_by_type[w](reader, &immediate, error_handler);

    mod__ea_immediate(w, r_m, immediate, ea);
}

void mod__immediate_register_mode_no_ea(int w, int r_m, struct file_reader* reader, file_reader_error error_handler) {
    word immediate;
    file_reader__read_by_type[w](reader, &immediate, error_handler);

    mod__register_mode_immediate(w, r_m, immediate);
}
