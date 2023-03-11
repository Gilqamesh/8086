#pragma once

#include "../defs.h"
#include "opcode.h"

void mod__no_ea__reg(int w, int reg, int r_m, struct instruction* instruction);
void mod__no_ea__rm(int w, int reg, int r_m, struct instruction* instruction);

void mod__no_ea__reg__da(int w, int reg, int direct_address, struct instruction* instruction);
void mod__no_ea__rm__da(int w, int reg, int direct_address, struct instruction* instruction);

void mod__ea__reg(int w, int reg, int r_m, signed_word ea, struct instruction* instruction);
void mod__ea__rm(int w, int reg, int r_m, signed_word ea, struct instruction* instruction);

void mod__register_mode__reg(int w, int reg, int r_m, struct instruction* instruction);
void mod__register_mode__rm(int w, int reg, int r_m, struct instruction* instruction);


void mod__no_ea_immediate(int w, int r_m, word immediate, struct instruction* instruction);
void mod__no_ea_immediate__da(int w, word immediate, word direct_address, struct instruction* instruction);
void mod__ea_immediate(int w, int r_m, word immediate, signed_word ea, struct instruction* instruction);
void mod__register_mode_immediate(int w, int r_m, word immediate, struct instruction* instruction);

static void (*const mod__no_ea_handlers[2])(int w, int reg, int r_m, struct instruction* instruction) = {
    mod__no_ea__rm,
    mod__no_ea__reg
};

static void (*const mod__no_ea_da_handlers[2])(int w, int reg, int direct_address, struct instruction* instruction) = {
    mod__no_ea__rm__da,
    mod__no_ea__reg__da
};

static void (*const mod__ea_handlers[2])(int w, int reg, int r_m, signed_word ea, struct instruction* instruction) = {
    mod__ea__rm,
    mod__ea__reg
};

static void (*const mod__register_mode_handlers[2])(int w, int reg, int r_m, struct instruction* instruction) = {
    mod__register_mode__rm,
    mod__register_mode__reg
};

static inline const char* type_to_word__word(void) {
    return "word";
}

static inline const char* type_to_word__byte(void) {
    return "byte";
}

static const char* (*const type_to_word[2])(void) = {
    type_to_word__byte,
    type_to_word__word
};

static void mod__ea__print__non_zero(int r_m, signed_word ea, struct instruction* instruction) {
    instruction__push(
        instruction,
        "[%s %s %u]",
        effective_address_to_word(r_m),
        ea < 0 ? "-" : "+",
        ea < 0 ? -ea : ea
    );
}

static void mod__ea__print__zero(int r_m, signed_word ea, struct instruction* instruction) {
    (void)ea;
    instruction__push(
        instruction,
        "[%s]",
        effective_address_to_word(r_m)
    );
}

static void (*const mod__ea__print__zero_handler[2])(int r_m, signed_word ea, struct instruction* instruction) = {
    mod__ea__print__non_zero,
    mod__ea__print__zero
};
