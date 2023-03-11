#include "mod.h"
#include "../defs.h"

void mod__no_ea(int w, int d, int reg, int r_m, struct opcode_context* context, struct instruction* instruction) {
    if (r_m == 0b110) {
        word direct_address;
        file_reader__read_word(&context->file_reader, &direct_address, context->error_handler);

        mod__no_ea_da_handlers[d](w, reg, direct_address, instruction);
    } else {
        mod__no_ea_handlers[d](w, reg, r_m, instruction);
    }
}

void mod__8_bit_ea(int w, int d, int reg, int r_m, struct opcode_context* context, struct instruction* instruction) {
    byte ea;
    file_reader__read_byte(&context->file_reader, &ea, context->error_handler);

    mod__ea_handlers[d](w, reg, r_m, (signed_byte)ea, instruction);
}

void mod__16_bit_ea(int w, int d, int reg, int r_m, struct opcode_context* context, struct instruction* instruction) {
    word ea;
    file_reader__read_word(&context->file_reader, &ea, context->error_handler);

    mod__ea_handlers[d](w, reg, r_m, ea, instruction);
}

void mod__register_mode_no_ea(int w, int d, int reg, int r_m, struct opcode_context* context, struct instruction* instruction) {
    (void)context;

    mod__register_mode_handlers[d](w, reg, r_m, instruction);
}

void mod__immediate_no_ea(int w, int r_m, struct opcode_context* context, struct instruction* instruction) {
    if (r_m == 0b110) {
        word direct_address;
        file_reader__read_word(&context->file_reader, &direct_address, context->error_handler);

        mod__no_ea_immediate__da(w, r_m, direct_address, instruction);

        printf("\n"); // remove after test, here to flush
        assert(false && "todo: test to see if correct, can this case even happen?");
    } else {
        word immediate;
        file_reader__read_by_type[w](&context->file_reader, &immediate, context->error_handler);
        mod__no_ea_immediate(w, r_m, immediate, instruction);
    }
}

void mod__immediate_8_bit_ea(int w, int r_m, struct opcode_context* context, struct instruction* instruction) {
    byte ea;
    file_reader__read_byte(&context->file_reader, &ea, context->error_handler);

    word immediate;
    file_reader__read_by_type[w](&context->file_reader, &immediate, context->error_handler);

    mod__ea_immediate(w, r_m, immediate, ea, instruction);
}

void mod__immediate_16_bit_ea(int w, int r_m, struct opcode_context* context, struct instruction* instruction) {
    word ea = 0;
    file_reader__read_word(&context->file_reader, &ea, context->error_handler);

    word immediate;
    file_reader__read_by_type[w](&context->file_reader, &immediate, context->error_handler);

    mod__ea_immediate(w, r_m, immediate, ea, instruction);
}

void mod__immediate_register_mode_no_ea(int w, int r_m, struct opcode_context* context, struct instruction* instruction) {
    word immediate = 0;
    file_reader__read_by_type[w](&context->file_reader, &immediate, context->error_handler);

    mod__register_mode_immediate(w, r_m, immediate, instruction);
}

void mod__immediate_signed_no_ea(int s, int w, int r_m, struct opcode_context* context, struct instruction* instruction) {
    if (r_m == 0b110) {
        word direct_address;
        file_reader__read_word(&context->file_reader, &direct_address, context->error_handler);

        word immediate = 0;
        file_reader__read_by_type[w == 1 && s == 0](&context->file_reader, &immediate, context->error_handler);

        mod__no_ea_immediate__da(w, immediate, direct_address, instruction);
    } else {
        word immediate;
        file_reader__read_by_type[w == 1 && s == 0](&context->file_reader, &immediate, context->error_handler);
        mod__no_ea_immediate(w, r_m, immediate, instruction);
    }
}

void mod__immediate_signed_8_bit_ea(int s, int w, int r_m, struct opcode_context* context, struct instruction* instruction) {
    byte ea;
    file_reader__read_byte(&context->file_reader, &ea, context->error_handler);

    word immediate;
    file_reader__read_by_type[w == 1 && s == 0](&context->file_reader, &immediate, context->error_handler);

    mod__ea_immediate(w, r_m, immediate, ea, instruction);
}

void mod__immediate_signed_16_bit_ea(int s, int w, int r_m, struct opcode_context* context, struct instruction* instruction) {
    word ea = 0;
    file_reader__read_word(&context->file_reader, &ea, context->error_handler);

    word immediate;
    file_reader__read_by_type[w == 1 && s == 0](&context->file_reader, &immediate, context->error_handler);

    mod__ea_immediate(w, r_m, immediate, ea, instruction);
}

void mod__immediate_signed_register_mode_no_ea(int s, int w, int r_m, struct opcode_context* context, struct instruction* instruction) {
    word immediate;
    file_reader__read_by_type[w == 1 && s == 0](&context->file_reader, &immediate, context->error_handler);

    mod__register_mode_immediate(w, r_m, immediate, instruction);
}
