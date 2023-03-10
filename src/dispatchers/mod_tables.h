#pragma once

#include "../defs.h"

void mod__no_ea__reg(int w, int reg, int r_m);
void mod__no_ea__rm(int w, int reg, int r_m);

void mod__no_ea__reg__da(int w, int reg, int direct_address);
void mod__no_ea__rm__da(int w, int reg, int direct_address);

void mod__ea__reg(int w, int reg, int r_m, signed_word ea);
void mod__ea__rm(int w, int reg, int r_m, signed_word ea);

void mod__register_mode__reg(int w, int reg, int r_m);
void mod__register_mode__rm(int w, int reg, int r_m);


void mod__no_ea_immediate(int w, int r_m, word immediate);
void mod__no_ea_immediate__da(int w, int r_m, word direct_address);
void mod__ea_immediate(int w, int r_m, word immediate, signed_word ea);
void mod__register_mode_immediate(int w, int r_m, word immediate);

static void (*const mod__no_ea_handlers[2])(int w, int reg, int r_m) = {
    mod__no_ea__rm,
    mod__no_ea__reg
};

static void (*const mod__no_ea_da_handlers[2])(int w, int reg, int direct_address) = {
    mod__no_ea__rm__da,
    mod__no_ea__reg__da
};

static void (*const mod__ea_handlers[2])(int w, int reg, int r_m, signed_word ea) = {
    mod__ea__rm,
    mod__ea__reg
};

static void (*const mod__register_mode_handlers[2])(int w, int reg, int r_m) = {
    mod__register_mode__rm,
    mod__register_mode__reg
};

static inline void print_word() {
    printf("word");
}

static inline void print_byte() {
    printf("byte");
}

static void (*const print_type[2])(void) = {
    print_byte,
    print_word
};

static inline void print__reg_or_r_m(int reg__r_m, int w) {
    printf("%s", reg_to_word(reg__r_m, w));
}

static inline void print__ea(int r_m) {
    printf("%s", effective_address_to_word(r_m));
}

static inline void print__seperator__dst_src(void) {
    printf(", ");
}

static inline void print__seperator__add(void) {
    printf(" + ");
}

static inline void print__seperator__sub(void) {
    printf(" - ");
}

static void (*const print_seperator__sign[2])(void) = {
    print__seperator__add,
    print__seperator__sub
};

#define BRACKETED_EXPRESSION(expr)  \
    printf("[");                    \
    expr;                           \
    printf("]")

static inline signed_word signed_word__identity(signed_word w) {
    return w;
}

static inline signed_word signed_word__negate(signed_word w) {
    return -w;
}

static signed_word (*const signed_word__abs[2])(signed_word w) = {
    signed_word__identity,
    signed_word__negate
};

static void mod__ea__print__non_zero(int r_m, signed_word ea) {
    BRACKETED_EXPRESSION(
        print__ea(r_m);
        print_seperator__sign[(ea >> 15) & 0b1]();
        printf("%u", signed_word__abs[(ea >> 15) & 0b1](ea));
    );
}

static void mod__ea__print__zero(int r_m, signed_word ea) {
    (void)ea;
    BRACKETED_EXPRESSION(
        print__ea(r_m);
    );
}

static void (*const mod__ea__print__zero_handler[2])(int r_m, signed_word ea) = {
    mod__ea__print__non_zero,
    mod__ea__print__zero
};
