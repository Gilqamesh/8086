#include "mod_tables.h"

void mod__no_ea__reg(int w, int reg, int r_m) {
    print__reg_or_r_m(reg, w);

    print__seperator__dst_src();
    
    BRACKETED_EXPRESSION(print__ea(r_m));
}

void mod__no_ea__rm(int w, int reg, int r_m) {
    BRACKETED_EXPRESSION(print__ea(r_m));

    print__seperator__dst_src();

    print__reg_or_r_m(reg, w);
}

void mod__no_ea__reg__da(int w, int reg, int direct_address) {
    print__reg_or_r_m(reg, w);

    print__seperator__dst_src();

    BRACKETED_EXPRESSION(printf("%u", direct_address));
}

void mod__no_ea__rm__da(int w, int reg, int direct_address) {
    BRACKETED_EXPRESSION(printf("%u", direct_address));

    print__seperator__dst_src();

    print__reg_or_r_m(reg, w);
}

void mod__ea__reg(int w, int reg, int r_m, signed_word ea) {
    print__reg_or_r_m(reg, w);

    print__seperator__dst_src();

    mod__ea__print__zero_handler[ea == 0](r_m, ea);
}

void mod__ea__rm(int w, int reg, int r_m, signed_word ea) {
    mod__ea__print__zero_handler[ea == 0](r_m, ea);

    print__seperator__dst_src();

    print__reg_or_r_m(reg, w);
}

void mod__register_mode__reg(int w, int reg, int r_m) {
    mod__register_mode__rm(w, r_m, reg);
}

void mod__register_mode__rm(int w, int reg, int r_m) {
    print__reg_or_r_m(r_m, w);

    print__seperator__dst_src();

    print__reg_or_r_m(reg, w);
}

void mod__no_ea_immediate(int w, int r_m, word immediate) {
    BRACKETED_EXPRESSION(print__ea(r_m));
    
    print__seperator__dst_src();

    print_type[w]();
    printf(" ");
    printf("%u", immediate);
}

void mod__no_ea_immediate__da(int w, int r_m, word direct_address) {
    BRACKETED_EXPRESSION(print__ea(r_m));

    print__seperator__dst_src();

    BRACKETED_EXPRESSION(
        print_type[w]();
        printf(" ");
        printf("%u", direct_address);
    );
}

void mod__ea_immediate(int w, int r_m, word immediate, signed_word ea) {
    mod__ea__print__zero_handler[ea == 0](r_m, ea);

    print__seperator__dst_src();

    print_type[w]();
    printf(" ");
    printf("%u", immediate);

}

void mod__register_mode_immediate(int w, int r_m, word immediate) {
    print__reg_or_r_m(r_m, w);

    print__seperator__dst_src();

    printf("%u", immediate);
}
