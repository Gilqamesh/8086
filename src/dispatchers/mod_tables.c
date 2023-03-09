#include "mod_tables.h"

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

void mod__register_mode_no_ea__reg(int w, int reg, int r_m) {
    printf("%s", reg_to_word(reg, w));
    printf(",");
    printf(" ");
    printf("%s", reg_to_word(r_m, w));
}

void mod__register_mode_no_ea__rm(int w, int reg, int r_m) {
    printf("%s", reg_to_word(r_m, w));
    printf(",");
    printf(" ");
    printf("%s", reg_to_word(reg, w));
}

void mod__ea__reg(int w, int reg, int r_m, int ea) {
    printf("%s", reg_to_word(reg, w));
    printf(",");
    printf(" ");
    printf("[");
    printf("%s", effective_address_to_word(r_m));
    printf(" ");
    printf("+");
    printf(" ");
    printf("%u", ea);
    printf("]");
}

void mod__ea__rm(int w, int reg, int r_m, int ea) {
    printf("[");
    printf("%s", effective_address_to_word(r_m));
    printf(" ");
    printf("+");
    printf(" ");
    printf("%u", ea);
    printf("]");
    printf(",");
    printf(" ");
    printf("%s", reg_to_word(reg, w));
}

void mod__no_ea__reg(int w, int reg, int r_m) {
    printf("%s", reg_to_word(reg, w));
    printf(",");
    printf(" ");
    printf("%s", effective_address_to_word(r_m));
}

void mod__no_ea__rm(int w, int reg, int r_m) {
    printf("%s", effective_address_to_word(r_m));
    printf(",");
    printf(" ");
    printf("%s", reg_to_word(reg, w));
}

void mod__register_mode_no_ea_immediate__reg(int w, int reg, int r_m, word immediate) {
    printf("%s", reg_to_word(reg, w));
    printf(",");
    printf(" ");
    printf("[");
    printf("%s", reg_to_word(r_m, w));
    printf(" ");
    printf("+");
    printf(" ");
    print_type[w]();
    printf(" ");
    printf("%u", immediate);
    printf("]");
}

void mod__register_mode_no_ea_immediate__rm(int w, int reg, int r_m, word immediate) {
    printf("%s", reg_to_word(r_m, w));
    printf(",");
    printf(" ");
    printf("[");
    printf("%s", reg_to_word(reg, w));
    printf(" ");
    printf("+");
    printf(" ");
    print_type[w]();
    printf(" ");
    printf(" ");
    printf("%u", immediate);
    printf("]");
}

void mod__no_ea_immediate__reg(int w, int reg, int r_m, word immediate) {
    printf("%s", reg_to_word(reg, w));
    printf(",");
    printf(" ");
    printf("[");
    printf("%s", effective_address_to_word(r_m));
    printf(" ");
    printf("+");
    printf(" ");
    print_type[w]();
    printf(" ");
    printf("%u", immediate);
    printf("]");
}

void mod__no_ea_immediate__rm(int w, int reg, int r_m, word immediate) {
    printf("%s", effective_address_to_word(r_m));
    printf(",");
    printf(" ");
    printf("[");
    printf("%s", reg_to_word(reg, w));
    printf(" ");
    printf("+");
    printf(" ");
    print_type[w]();
    printf(" ");
    printf("%u", immediate);
    printf("]");
}

void mod__ea_immediate__reg(int w, int reg, int r_m, word immediate, int ea) {
    printf("[");
    printf("%s", reg_to_word(reg, w));
    printf(" ");
    printf("+");
    printf(" ");
    printf("%u", ea);
    printf("]");
    printf(",");
    printf(" ");
    printf("[");
    printf("%s", effective_address_to_word(r_m));
    printf(" ");
    printf("+");
    printf(" ");
    print_type[w]();
    printf(" ");
    printf("%u", immediate);
    printf("]");
}

void mod__ea_immediate__rm(int w, int reg, int r_m, word immediate, int ea) {
    printf("[");
    printf("%s", effective_address_to_word(r_m));
    printf(" ");
    printf("+");
    printf(" ");
    printf("%u", ea);
    printf("]");
    printf(",");
    printf(" ");
    printf("[");
    printf("%s", reg_to_word(reg, w));
    printf(" ");
    printf("+");
    printf(" ");
    print_type[w]();
    printf(" ");
    printf("%u", immediate);
    printf("]");
}
