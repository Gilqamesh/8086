#pragma once

#include "../file_reader.h"
#include "mod_tables.h"

typedef void (*const mod_handler)(int w, int d, int reg, int r_m, struct opcode_context* context, struct instruction* instruction);

void mod__no_ea(int w, int d, int reg, int r_m, struct opcode_context* context, struct instruction* instruction);
void mod__8_bit_ea(int w, int d, int reg, int r_m, struct opcode_context* context, struct instruction* instruction);
void mod__16_bit_ea(int w, int d, int reg, int r_m, struct opcode_context* context, struct instruction* instruction);
void mod__register_mode_no_ea(int w, int d, int reg, int r_m, struct opcode_context* context, struct instruction* instruction);

typedef void (*const mod_immediate_handler)(int w, int r_m, struct opcode_context* context, struct instruction* instruction);

void mod__immediate_no_ea(int w, int r_m, struct opcode_context* context, struct instruction* instruction);
void mod__immediate_8_bit_ea(int w, int r_m, struct opcode_context* context, struct instruction* instruction);
void mod__immediate_16_bit_ea(int w, int r_m, struct opcode_context* context, struct instruction* instruction);
void mod__immediate_register_mode_no_ea(int w, int r_m, struct opcode_context* context, struct instruction* instruction);

typedef void (*const mod_immediate_const_handler)(int s, int w, int r_m, struct opcode_context* context, struct instruction* instruction);

void mod__immediate_signed_no_ea(int s, int w, int r_m, struct opcode_context* context, struct instruction* instruction);
void mod__immediate_signed_8_bit_ea(int s, int w, int r_m, struct opcode_context* context, struct instruction* instruction);
void mod__immediate_signed_16_bit_ea(int s, int w, int r_m, struct opcode_context* context, struct instruction* instruction);
void mod__immediate_signed_register_mode_no_ea(int s, int w, int r_m, struct opcode_context* context, struct instruction* instruction);

typedef void (*const mod_source_only_handler)(int w, int reg_or_r_m, struct opcode_context* context, struct instruction* instruction);

void mod__source_only___no_ea(int w, int reg_or_r_m, struct opcode_context* context, struct instruction* instruction);
void mod__source_only__8_bit_ea(int w, int reg_or_r_m, struct opcode_context* context, struct instruction* instruction);
void mod__source_only__16_bit_ea(int w, int reg_or_r_m, struct opcode_context* context, struct instruction* instruction);
void mod__source_only__register_mode_no_ea(int w, int reg_or_r_m, struct opcode_context* context, struct instruction* instruction);

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

static const mod_immediate_const_handler mod_immediate_signed_handlers[4] = {
    mod__immediate_signed_no_ea,
    mod__immediate_signed_8_bit_ea,
    mod__immediate_signed_16_bit_ea,
    mod__immediate_signed_register_mode_no_ea
};

static const mod_source_only_handler mod_source_only_handlers[4] = {
    mod__source_only___no_ea,
    mod__source_only__8_bit_ea,
    mod__source_only__16_bit_ea,
    mod__source_only__register_mode_no_ea
};
