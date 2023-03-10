#include "opcode.h"
#include "mod.h"

void opcode__mov_1000_10xx(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    // Register/memory to/from register
    // mov instruction
    // 100010 d w    mod reg r/m    (DISP-LO)    (DISP-HI)

    printf("%s", "mov");
    printf(" ");

    int d = (first_byte >> 1) & 0b1;
    int w = first_byte & 0b1;

    byte second_byte;
    file_reader__read_byte(reader, &second_byte, error_handler);

    int mod = (second_byte >> 6) & 0b11;
    int reg = (second_byte >> 3) & 0b111;
    int r_m = second_byte & 0b111;

    mod_handlers[mod](w, d, reg, r_m, reader, error_handler);

    printf("\n");
}

void opcode__mov_1100_011x(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    // mov instruction
    // Immediate to register/memory
    // 1100011 w    mod 0 0 0 r/m    (DISP-LO)    (DISP-HI)    data    data if w = 1

    printf("%s", "mov");
    printf(" ");

    byte second_byte;
    file_reader__read_byte(reader, &second_byte, error_handler);

    int w   = first_byte & 0b1;
    int mod = (second_byte >> 6) & 0b11;
    int r_m = second_byte & 0b111;

    mod_immediate_handlers[mod](w, r_m, reader, error_handler);

    printf("\n");
}

void opcode__mov_1011_xxxx(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    // mov instruction
    // Immediate to register
    // 1011 w reg    data    data if w = 1

    printf("%s", "mov");
    printf(" ");

    int w   = (first_byte >> 3) & 0b1;
    int reg = first_byte & 0b111;

    word data;
    file_reader__read_by_type[w](reader, &data, error_handler);

    print__reg_or_r_m(reg, w);
    print__seperator__dst_src();
    printf("%u", data);

    printf("\n");
}

void opcode__mov_1010_000x(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    // mov instruction
    // Memory to accumulator
    // 1010000 w    addr-lo    addr-hi

    printf("%s", "mov");
    printf(" ");

    int w = first_byte & 0b1;

    word addr;
    file_reader__read_word(reader, &addr, error_handler);

    // NOTE(david): register ax for word transfers, al for bytes
    printf("%s", reg_to_word(0, w));
    print__seperator__dst_src();
    BRACKETED_EXPRESSION(printf("%u", addr));

    printf("\n");
}

void opcode__mov_1010_001x(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    // mov instruction
    // Accumulator to memory
    // 1010001 w    addr-lo    addr-hi

    printf("%s", "mov");
    printf(" ");

    int w = first_byte & 0b1;

    word addr;
    file_reader__read_word(reader, &addr, error_handler);

    // NOTE(david): register ax for word transfers, al for bytes
    BRACKETED_EXPRESSION(printf("%u", addr));
    print__seperator__dst_src();
    printf("%s", reg_to_word(0, w));

    printf("\n");
}

void opcode__mov_1000_1110(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    // mov instruction
    // Register/memory to segment register
    // 10001110    mod 0 SR r/m    (DISP-LO)    (DISP-HI)

    printf("%s", "mov");
    printf(" ");

    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "TODO(david): implement how to dispatch 0SR");

    printf("\n");
}

void opcode__mov_1000_1100(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    // mov instruction
    // Segment register to register/memory
    // 10001100    mod 0 SR r/m    (DISP-LO)    (DISP-HI)

    printf("%s", "mov");
    printf(" ");

    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "TODO(david): implement how to dispatch 0SR");

    printf("\n");
}

void opcode__cont_push_1111_1111(byte first_byte, byte second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__push_0101_0xxx(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__push_000x_x110(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__pop_1000_1111(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;

    byte second_byte;
    file_reader__read_byte(reader, &second_byte, error_handler);

    byte reg = (second_byte >> 3) & 0b111;
    if (reg != 0) {
        error_handler("in 'opcode__pop_1000_1111': reg can only be 000, the rest is (not used)", FILE_READER_ERROR_FATAL);
    }

    assert(false && "todo: implement");
}

void opcode__pop_0101_1xxx(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__pop_000x_x111(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__xchg_1000_011x(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__xchg_1001_0xxx(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__in_1110_010x(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__in_1110_110x(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__out_1110_011x(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__out_1110_111x(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__xlat(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__lea(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__lds(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__les(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__lahf(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__sahf(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__pushf(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__popf(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__add_0000_00xx(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__cont_add_1000_00xx(byte first_byte, byte second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)second_byte;
    (void)reader;
    (void)error_handler;

    assert(false && "todo: implement");
}

void opcode__add_0000_010x(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__adc_0001_00xx(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__cont_adc_1000_00xx(byte first_byte, byte second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__adc_0001_010x(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__cont_inc_1111_111x(byte first_byte, byte second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__inc_0100_0xxx(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__aaa(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__daa(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__sub_0010_10xx(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__cont_sub_1000_00xx(byte first_byte, byte second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__sub_0010_110x(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__sbb_0001_10xx(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__cont_sbb_1000_00xx(byte first_byte, byte second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__sbb_0001_110x(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__cont_dec_1111_111x(byte first_byte, byte second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__dec_0100_1xxx(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__cont_neg(byte first_byte, byte second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__cmp_0011_10xx(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__cont_cmp_1000_00xx(byte first_byte, byte second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__cmp_0011_110x(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__aas(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__das(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__cont_mul(byte first_byte, byte second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__cont_imul(byte first_byte, byte second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__aam(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__cont_div(byte first_byte, byte second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__cont_idiv(byte first_byte, byte second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__aad(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__cbw(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__cwd(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__cont_not(byte first_byte, byte second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__cont_shl_sal(byte first_byte, byte second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__cont_shr(byte first_byte, byte second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__cont_sar(byte first_byte, byte second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__cont_rol(byte first_byte, byte second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__cont_ror(byte first_byte, byte second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__cont_rcl(byte first_byte, byte second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__cont_rcr(byte first_byte, byte second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__and_0010_00xx(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__cont_and_1000_000x(byte first_byte, byte second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__and_0010_010x(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__test_0001_00xx(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "// note(david): this case isn't in the corresponding table of the 8086 manual, so just ignoring it");
}

void opcode__test_1000_010x(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__cont_test_1111_011x(byte first_byte, byte second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__test_1010_100x(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__or_0000_10xx(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__cont_or_1000_000x(byte first_byte, byte second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__or_0000_110x(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__xor_0011_00xx(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__cont_xor_1000_000x(byte first_byte, byte second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__xor_0011_010x(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__rep(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__movs(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__cmps(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__scas(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__lods(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__stds(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__call_1110_1000(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__cont_call_1111_1111(byte first_byte, byte second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__call_1001_1010(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__jmp_1110_1001(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__jmp_1110_1011(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__cont_jmp_1111_1111(byte first_byte, byte second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__jmp_1110_1010(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__ret_1100_0011(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__ret_1100_0010(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__ret_1100_1011(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__ret_1100_1010(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__je_jz(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__jl_jnge(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__jle_jng(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__jb_jnae(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__jbe_jna(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__jp_jpe(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__jo(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__js(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__jne_jnz(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__jnl_jge(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__jnle_jg(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__jnb_jae(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__jnbe_ja(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__jnp_jpo(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__jno(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__jns(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__loop(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__loopz_loope(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__loopnz_loopne(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__jcxz(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__int_1100_1101(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__int_1100_1100(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__into(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__iret(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__clc(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__cmc(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__stc(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__cld(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__std(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__cli(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__sti(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__hlt(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__wait(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__esc(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__lock(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__segment(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__null(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    error_handler("in 'opcode__null': not used instruction", FILE_READER_ERROR_FATAL);
}

void opcode__cont_null(byte first_byte, byte second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)second_byte;
    (void)reader;
    error_handler("in 'opcode__null': not used instruction", FILE_READER_ERROR_FATAL);
}

void opcode__ss(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__cs(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__ds(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__es(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__nop(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__1000_000x(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    static const opcode_cont_fn opcode_handlers[8] = {
        opcode__cont_add_1000_00xx,
        opcode__cont_or_1000_000x,
        opcode__cont_adc_1000_00xx,
        opcode__cont_sbb_1000_00xx,
        opcode__cont_and_1000_000x,
        opcode__cont_sub_1000_00xx,
        opcode__cont_xor_1000_000x,
        opcode__cont_cmp_1000_00xx
    };

    byte second_byte;
    file_reader__read_byte(reader, &second_byte, error_handler);

    byte reg = (second_byte >> 3) & 0b111;
    opcode_handlers[reg](first_byte, second_byte, reader, error_handler);
}

void opcode__1000_001x(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    static const opcode_cont_fn opcode_handlers[8] = {
        opcode__cont_add_1000_00xx,
        opcode__cont_null,
        opcode__cont_adc_1000_00xx,
        opcode__cont_sbb_1000_00xx,
        opcode__cont_null,
        opcode__cont_sub_1000_00xx,
        opcode__cont_null,
        opcode__cont_cmp_1000_00xx
    };

    byte second_byte;
    file_reader__read_byte(reader, &second_byte, error_handler);

    byte reg = (second_byte >> 3) & 0b111;
    opcode_handlers[reg](first_byte, second_byte, reader, error_handler);
}

void opcode__1101_00xx(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    static const opcode_cont_fn opcode_handler[8] = {
        opcode__cont_rol,
        opcode__cont_ror,
        opcode__cont_rcl,
        opcode__cont_rcr,
        opcode__cont_shl_sal,
        opcode__cont_shr,
        opcode__cont_null,
        opcode__cont_sar
    };

    byte second_byte;
    file_reader__read_byte(reader, &second_byte, error_handler);

    byte reg = (second_byte >> 3) & 0b111;
    opcode_handler[reg](first_byte, second_byte, reader, error_handler);
}

void opcode__repne_repnz(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__rep_repe_repz(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__1111_011x(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    static const opcode_cont_fn opcode_handler[8] = {
        opcode__cont_test_1111_011x,
        opcode__cont_null,
        opcode__cont_not,
        opcode__cont_neg,
        opcode__cont_mul,
        opcode__cont_imul,
        opcode__cont_div,
        opcode__cont_idiv
    };

    byte second_byte;
    file_reader__read_byte(reader, &second_byte, error_handler);

    byte reg = (second_byte >> 3) & 0b111;
    opcode_handler[reg](first_byte, second_byte, reader, error_handler);
}

void opcode__1111_1110(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    static const opcode_cont_fn opcode_handler[8] = {
        opcode__cont_inc_1111_111x,
        opcode__cont_dec_1111_111x,
        opcode__cont_null,
        opcode__cont_null,
        opcode__cont_null,
        opcode__cont_null,
        opcode__cont_null,
        opcode__cont_null
    };

    byte second_byte;
    file_reader__read_byte(reader, &second_byte, error_handler);

    byte reg = (second_byte >> 3) & 0b111;
    opcode_handler[reg](first_byte, second_byte, reader, error_handler);
}

void opcode__1111_1111(byte first_byte, struct file_reader* reader, file_reader_error error_handler) {
    static const opcode_cont_fn opcode_handler[8] = {
        opcode__cont_inc_1111_111x,
        opcode__cont_dec_1111_111x,
        opcode__cont_call_1111_1111,
        opcode__cont_call_1111_1111,
        opcode__cont_jmp_1111_1111,
        opcode__cont_jmp_1111_1111,
        opcode__cont_push_1111_1111,
        opcode__cont_null
    };

    byte second_byte;
    file_reader__read_byte(reader, &second_byte, error_handler);

    byte reg = (second_byte >> 3) & 0b111;
    opcode_handler[reg](first_byte, second_byte, reader, error_handler);
}
