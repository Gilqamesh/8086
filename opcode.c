#include "opcode.h"

void opcode__mov(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    if ((first_byte >> 2) == 0b100010) {
        // Register/memory to/from register
        // mov instruction
        // 100010 d w    mod reg r/m    (DISP-LO)    (DISP-HI)
        printf("%s", "mov");
        printf(" ");
        int is_dest = (first_byte >> 1) & 1;
        int is_wide = first_byte & 1;

        int mod = (optional_second_byte >> 6) & 3;
        int reg = (optional_second_byte >> 3) & 7;
        int r_m = optional_second_byte & 7;

        switch (mod) {
            case MEMORY_MODE_NO_DISPLACEMENT: {
                if (r_m == BP) { // direct address, it has a 16-bit displacement, so check next 2 bytes
                    word data = 0;
                    file_reader__read_byte(reader, (byte*)&data, error_handler);
                    file_reader__read_byte(reader, (byte*)&data + 1, error_handler);

                    if (is_dest == 1) {
                        printf("%s, [%u]", reg_to_word(reg, is_wide), data);
                        printf(" ");
                    } else {
                        printf("[%u], %s", data, reg_to_word(reg, is_wide));
                        printf(" ");
                    }
                } else {
                    if (is_dest == 1) {
                        printf("%s", reg_to_word(reg, is_wide));
                        printf(",");
                        printf("[%s]", effective_address_to_word(r_m));
                        printf(" ");
                    } else {
                        printf("[%s]", effective_address_to_word(r_m));
                        printf(",");
                        printf("%s", reg_to_word(reg, is_wide));
                        printf(" ");
                    }
                }
            } break ;
            case MEMORY_MODE_8_BIT_DISPLACEMENT: {
                byte displacement;
                file_reader__read_byte(reader, &displacement, error_handler);
            } break ;
            case MEMORY_MODE_16_BIT_DISPLACEMENT: {
                word data = 0;
                file_reader__read_byte(reader, (byte*)&data, error_handler);
                file_reader__read_byte(reader, (byte*)&data + 1, error_handler);
            } break ;
            case REGISTER_MODE_NO_DISPLACEMENT: {
                const char* dst_registry;
                const char* src_registry;

                dst_registry = reg_to_word(reg, is_wide);
                // depends on reg which formula to choose and on mod the amount of displacement
                src_registry = reg_to_word(r_m, is_wide);

                if (is_dest == 0) {
                    const char* tmp = src_registry;
                    src_registry = dst_registry;
                    dst_registry = tmp;
                }

                printf("%s, %s", dst_registry, src_registry);
            } break ;
            default: assert(false);
        }
        printf("\n");
    } else if ((first_byte >> 1) == 0b1100011) {
        // mov instruction
        // Immediate to register/memory
        // 1100011 w    mod 0 0 0 r/m    (DISP-LO)    (DISP-HI)    data    data if w = 1
    } else if ((first_byte >> 4) == 0b1011) {
        // mov instruction
        // Immediate to register
        // 1011 w reg    data    data if w = 1

        int   is_wide        = (first_byte >> 3) & 1;
        int   reg            = first_byte & 7;
        const char *reg_word = reg_to_word(reg, is_wide);

        word data = optional_second_byte;
        if (is_wide) {
            file_reader__read_byte(reader, (byte*)&data + 1, error_handler);
        }

        printf("%s %s, %u\n", "mov", reg_word, data);
    } else if ((first_byte >> 1) == 0b1010000) {
        // mov instruction
        // Memory to accumulator
        // 1010000 w    addr-lo    addr-hi
    } else if ((first_byte >> 1) == 0b1010001) {
        // mov instruction
        // Accumulator to memory
        // 1010001 w    addr-lo    addr-hi
    } else if (first_byte == 0b10001110) {
        // mov instruction
        // Register/memory to segment register
        // 10001110    mod 0 SR r/m    (DISP-LO)    (DISP-HI)
    } else if (first_byte == 0b10001100) {
        // mov instruction
        // Segment register to register/memory
        // 10001100    mod 0 SR r/m    (DISP-LO)    (DISP-HI)
    } else {
        fprintf(stderr, "first_byte: %d\n", first_byte);
        assert(false && "instruction not implemented");
    }
}

void opcode__push(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__pop(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__xchg(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__in(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__out(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__xlat(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__lea(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__lds(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__les(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__lahf(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__sahf(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__pushf(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__popf(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__add(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__adc(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__inc(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__aaa(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__daa(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__sub(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__sbb(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__dec(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__neg(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__cmp(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__aas(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__das(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__mul(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__imul(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__aam(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__div(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__idiv(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__aad(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__cbw(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__cwd(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__not(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__shl_sal(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__shr(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__sar(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__rol(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__ror(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__rcl(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__rcr(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__and(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__test(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__or(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__xor(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__rep(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__movs(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__cmps(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__scas(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__lods(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__stds(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__call(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__jmp(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__ret(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__je_jz(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__jl_jnge(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__jle_jng(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__jb_jnae(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__jbe_jna(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__jp_jpe(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__jo(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__js(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__jne_jnz(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__jnl_jge(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__jnle_jg(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__jnb_jae(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__jnbe_ja(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__jnp_jpo(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__jno(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__jns(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__loop(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__loopz_loope(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__loopnz_loopne(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__jcxz(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__int(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__into(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__iret(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__clc(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__cmc(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__stc(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__cld(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__std(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__cli(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__sti(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__hlt(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__wait(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__esc(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__lock(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__segment(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__null(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    error_handler("in 'opcode__null': not used instruction", FILE_READER_ERROR_FATAL);
}

void opcode__ss(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__cs(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__ds(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__es(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__nop(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__1000_000x(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    typedef void (*opcode_handler_handler)(byte, byte, struct file_reader*, file_reader_error);
    static const opcode_handler_handler opcode_handlers[8] = {
        opcode__add, opcode__or, opcode__adc, opcode__sbb, opcode__and, opcode__sub, opcode__xor, opcode__cmp
    };

    byte reg = (optional_second_byte >> 3) & 0b111;
    opcode_handlers[reg](first_byte, optional_second_byte, reader, error_handler);
}

void opcode__1000_001x(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)reader;
    typedef void (*opcode_handler_handler)(byte, byte, struct file_reader*, file_reader_error);
    static const opcode_handler_handler opcode_handlers[8] = {
        opcode__add, opcode__null, opcode__adc, opcode__sbb, opcode__null, opcode__sub, opcode__null, opcode__cmp
    };

    byte reg = (optional_second_byte >> 3) & 0b111;
    opcode_handlers[reg](first_byte, optional_second_byte, reader, error_handler);
}

void opcode__1000_1100(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    typedef void (*opcode_handler_handler)(byte, byte, struct file_reader*, file_reader_error);
    static const opcode_handler_handler opcode_handlers[2] = {
        opcode__mov, opcode__null
    };

    (void)opcode_handlers;
    assert(false && "TODO(david): implement how to dispatch 0SR");
}

void opcode__1000_1110(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    typedef void (*opcode_handler_handler)(byte, byte, struct file_reader*, file_reader_error);
    static const opcode_handler_handler opcode_handlers[2] = {
        opcode__mov, opcode__null
    };

    (void)opcode_handlers;
    assert(false && "TODO(david): implement how to dispatch 0SR");
}

void opcode__1000_1111(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    typedef void (*opcode_handler_handler)(byte, byte, struct file_reader*, file_reader_error);
    static const opcode_handler_handler opcode_handler = opcode__pop;

    byte reg = (optional_second_byte >> 3) & 0b111;
    if (reg != 0) {
        error_handler("in 'opcode__1000_1111': reg can only be 000, the rest is (not used)", FILE_READER_ERROR_FATAL);
    }
    opcode_handler(first_byte, optional_second_byte, reader, error_handler);
}

void opcode__1100_0110(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    typedef void (*opcode_handler_handler)(byte, byte, struct file_reader*, file_reader_error);
    static const opcode_handler_handler opcode_handler = opcode__mov;

    byte reg = (optional_second_byte >> 3) & 0b111;
    if (reg != 0) {
        error_handler("in 'opcode__1100_0110': reg can only be 000, the rest is (not used)", FILE_READER_ERROR_FATAL);
    }
    opcode_handler(first_byte, optional_second_byte, reader, error_handler);
}

void opcode__1100_0111(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    typedef void (*opcode_handler_handler)(byte, byte, struct file_reader*, file_reader_error);
    static const opcode_handler_handler opcode_handler = opcode__mov;

    byte reg = (optional_second_byte >> 3) & 0b111;
    if (reg != 0) {
        error_handler("in 'opcode__1100_0111': reg can only be 000, the rest is (not used)", FILE_READER_ERROR_FATAL);
    }
    opcode_handler(first_byte, optional_second_byte, reader, error_handler);
}

void opcode__1101_00xx(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    typedef void (*opcode_handler_handler)(byte, byte, struct file_reader*, file_reader_error);
    static const opcode_handler_handler opcode_handler[8] = {
        opcode__rol, opcode__ror, opcode__rcl, opcode__rcr, opcode__shl_sal, opcode__shr, opcode__null, opcode__sar
    };

    byte reg = (optional_second_byte >> 3) & 0b111;
    opcode_handler[reg](first_byte, optional_second_byte, reader, error_handler);
}

void opcode__repne_repnz(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__rep_repe_repz(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    (void)first_byte;
    (void)optional_second_byte;
    (void)reader;
    (void)error_handler;
    assert(false && "todo: implement");
}

void opcode__1111_011x(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    typedef void (*opcode_handler_handler)(byte, byte, struct file_reader*, file_reader_error);
    static const opcode_handler_handler opcode_handler[8] = {
        opcode__test, opcode__null, opcode__not, opcode__neg, opcode__mul, opcode__imul, opcode__div, opcode__idiv
    };

    byte reg = (optional_second_byte >> 3) & 0b111;
    opcode_handler[reg](first_byte, optional_second_byte, reader, error_handler);
}

void opcode__1111_1110(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    typedef void (*opcode_handler_handler)(byte, byte, struct file_reader*, file_reader_error);
    static const opcode_handler_handler opcode_handler[8] = {
        opcode__inc, opcode__dec, opcode__null, opcode__null, opcode__null, opcode__null, opcode__null, opcode__null
    };

    byte reg = (optional_second_byte >> 3) & 0b111;
    opcode_handler[reg](first_byte, optional_second_byte, reader, error_handler);
}

void opcode__1111_1111(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler) {
    typedef void (*opcode_handler_handler)(byte, byte, struct file_reader*, file_reader_error);
    static const opcode_handler_handler opcode_handler[8] = {
        opcode__inc, opcode__dec, opcode__call, opcode__call, opcode__jmp, opcode__jmp, opcode__push, opcode__null
    };

    byte reg = (optional_second_byte >> 3) & 0b111;
    opcode_handler[reg](first_byte, optional_second_byte, reader, error_handler);
}
