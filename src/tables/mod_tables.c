#include "mod_tables.h"

void mod__no_ea__reg(int w, int reg, int r_m, struct instruction* instruction) {
    instruction__push(
        instruction,
        "%s, [%s]",
        reg_to_word(reg, w),
        effective_address_to_word(r_m)
    );
}

void mod__no_ea__rm(int w, int reg, int r_m, struct instruction* instruction) {
    instruction__push(
        instruction,
        "[%s], %s",
        effective_address_to_word(r_m),
        reg_to_word(reg, w)
    );
}

void mod__no_ea__reg__da(int w, int reg, int direct_address, struct instruction* instruction) {
    instruction__push(
        instruction,
        "%s, [%u]",
        reg_to_word(reg, w),
        direct_address
    );
}

void mod__no_ea__rm__da(int w, int reg, int direct_address, struct instruction* instruction) {
    instruction__push(
        instruction,
        "[%u], %s",
        direct_address,
        reg_to_word(reg, w)
    );
}

void mod__ea__reg(int w, int reg, int r_m, signed_word ea, struct instruction* instruction) {
    instruction__push(
        instruction,
        "%s, ",
        reg_to_word(reg, w)
    );

    mod__ea__print__zero_handler[ea == 0](r_m, ea, instruction);
}

void mod__ea__rm(int w, int reg, int r_m, signed_word ea, struct instruction* instruction) {
    mod__ea__print__zero_handler[ea == 0](r_m, ea, instruction);

    instruction__push(
        instruction,
        ", %s",
        reg_to_word(reg, w)
    );
}

void mod__register_mode__reg(int w, int reg, int r_m, struct instruction* instruction) {
    mod__register_mode__rm(w, r_m, reg, instruction);
}

void mod__register_mode__rm(int w, int reg, int r_m, struct instruction* instruction) {
    instruction__push(
        instruction,
        "%s, %s",
        reg_to_word(r_m, w),
        reg_to_word(reg, w)
    );
}

void mod__no_ea_immediate(int w, int r_m, word immediate, struct instruction* instruction) {
    instruction__push(
        instruction,
        "[%s], %s %u",
        effective_address_to_word(r_m),
        type_to_word[w](),
        immediate
    );
}

void mod__no_ea_immediate__da(int w, word immediate, word direct_address, struct instruction* instruction) {
    instruction__push(
        instruction,
        "[%u], %s %u",
        direct_address,
        type_to_word[w](),
        immediate
    );
}

void mod__ea_immediate(int w, int r_m, word immediate, signed_word ea, struct instruction* instruction) {
    mod__ea__print__zero_handler[ea == 0](r_m, ea, instruction);
    instruction__push(
        instruction,
        ", %s %u",
        type_to_word[w](),
        immediate
    );
}

void mod__register_mode_immediate(int w, int r_m, word immediate, struct instruction* instruction) {
    instruction__push(
        instruction,
        "%s, %u",
        reg_to_word(r_m, w),
        immediate
    );
}
