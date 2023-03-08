#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "file_reader.h"
#include "opcode.h"

void error_handler(const char* msg, enum file_reader_error level) {
    switch (level) {
        case FILE_READER_ERROR_WARN: {
            fprintf(stdout, "%s\n", msg);
        } break ;
        case FILE_READER_ERROR_FATAL: {
            fprintf(stderr, "%s\n", msg);
            exit(1);
        } break ;
        default: {
            assert(false && "log_level is not handled");
        }
    }
}

void dispatch_instruction(struct file_reader* reader) {
    typedef void (*opcode_handler)(byte, byte, struct file_reader*, file_reader_error);
    typedef opcode_handler opcode_handlers16[16];
    static const opcode_handlers16 opcode_handlers[16] = {
        {
            opcode__add, opcode__add, opcode__add, opcode__add, opcode__add, opcode__add, opcode__push, opcode__pop,
            opcode__or,  opcode__or,  opcode__or,  opcode__or,  opcode__or,  opcode__or,  opcode__push, opcode__null
        },
        {
            opcode__adc, opcode__adc, opcode__adc, opcode__adc, opcode__adc, opcode__adc, opcode__push, opcode__pop,
            opcode__sbb, opcode__sbb, opcode__sbb, opcode__sbb, opcode__sbb, opcode__sbb, opcode__push, opcode__pop
        },
        {
            opcode__and, opcode__and, opcode__and, opcode__and, opcode__and, opcode__and, opcode__es, opcode__daa,
            opcode__sub, opcode__sub, opcode__sub, opcode__sub, opcode__sub, opcode__sub, opcode__cs, opcode__das
        },
        {
            opcode__xor, opcode__xor, opcode__xor, opcode__xor, opcode__xor, opcode__xor, opcode__ss, opcode__aaa,
            opcode__cmp, opcode__cmp, opcode__cmp, opcode__cmp, opcode__cmp, opcode__cmp, opcode__ds, opcode__aas
        },
        {
            opcode__inc, opcode__inc, opcode__inc, opcode__inc, opcode__inc, opcode__inc, opcode__inc, opcode__inc,
            opcode__dec, opcode__dec, opcode__dec, opcode__dec, opcode__dec, opcode__dec, opcode__dec, opcode__dec
        },
        {
            opcode__push, opcode__push, opcode__push, opcode__push, opcode__push, opcode__push, opcode__push, opcode__push,
            opcode__pop,  opcode__pop,  opcode__pop,  opcode__pop,  opcode__pop,  opcode__pop,  opcode__pop,  opcode__pop
        },
        {
            opcode__null, opcode__null, opcode__null, opcode__null, opcode__null, opcode__null, opcode__null, opcode__null,
            opcode__null, opcode__null, opcode__null, opcode__null, opcode__null, opcode__null, opcode__null, opcode__null
        },
        {
            opcode__jo, opcode__jno, opcode__jb_jnae, opcode__jnb_jae, opcode__je_jz,   opcode__jne_jnz, opcode__jbe_jna, opcode__jnbe_ja,
            opcode__js, opcode__jns, opcode__jp_jpe,  opcode__jnp_jpo, opcode__jl_jnge, opcode__jnl_jge, opcode__jle_jng, opcode__jnle_jg
        },
        {
            opcode__1000_000x, opcode__1000_000x, opcode__1000_001x, opcode__1000_001x, opcode__test,      opcode__test, opcode__xchg,      opcode__xchg,
            opcode__mov,       opcode__mov,       opcode__mov,       opcode__mov,       opcode__1000_1100, opcode__lea,  opcode__1000_1110, opcode__1000_1111
        },
        {
            opcode__nop, opcode__xchg, opcode__xchg, opcode__xchg, opcode__xchg,  opcode__xchg, opcode__xchg, opcode__xchg,
            opcode__cbw, opcode__cwd,  opcode__call, opcode__wait, opcode__pushf, opcode__popf, opcode__sahf, opcode__lahf
        },
        {
            opcode__mov,  opcode__mov,  opcode__mov,  opcode__mov,  opcode__movs, opcode__movs, opcode__cmps, opcode__cmps,
            opcode__test, opcode__test, opcode__stds, opcode__stds, opcode__lods, opcode__lods, opcode__scas, opcode__scas
        },
        {
            opcode__mov, opcode__mov, opcode__mov, opcode__mov, opcode__mov, opcode__mov, opcode__mov, opcode__mov,
            opcode__mov, opcode__mov, opcode__mov, opcode__mov, opcode__mov, opcode__mov, opcode__mov, opcode__mov
        },
        {
            opcode__null, opcode__null, opcode__ret, opcode__ret, opcode__les, opcode__lds, opcode__1100_0110, opcode__1100_0111,
            opcode__null, opcode__null, opcode__ret, opcode__ret, opcode__int, opcode__int, opcode__into,      opcode__iret
        },
        {
            opcode__1101_00xx, opcode__1101_00xx, opcode__1101_00xx, opcode__1101_00xx, opcode__aam, opcode__aad, opcode__null, opcode__xlat,
            opcode__esc,       opcode__esc,       opcode__esc,       opcode__esc,       opcode__esc, opcode__esc, opcode__esc,  opcode__esc
        },
        {
            opcode__loopnz_loopne, opcode__loopz_loope, opcode__loop, opcode__jcxz, opcode__in, opcode__in, opcode__out, opcode__out,
            opcode__call,          opcode__jmp,         opcode__jmp,  opcode__jmp,  opcode__in, opcode__in, opcode__out, opcode__out
        },
        {
            opcode__lock, opcode__null, opcode__repne_repnz, opcode__rep_repe_repz, opcode__hlt, opcode__cmc, opcode__1111_011x, opcode__1111_011x,
            opcode__clc,  opcode__stc,  opcode__cli,         opcode__sti,           opcode__cld, opcode__std, opcode__1111_1110, opcode__1111_1111
        }
    };

    byte first_byte;
    byte second_byte;
    file_reader__read_byte(reader, &first_byte, error_handler);
    file_reader__read_byte_opt(reader, &second_byte, error_handler);

    opcode_handlers[first_byte >> 4][first_byte & 0b00001111](first_byte, second_byte, reader, error_handler);
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("Usage: mynasm filename\n");
        exit(1);
    }

    struct file_reader  reader;

    file_reader__create(&reader, argv[1], error_handler);

    printf("; %s disassembly:\nbits 16\n", argv[1]);
    while (file_reader__eof_reached(&reader) == false) {
        dispatch_instruction(&reader);
    }

    file_reader__destroy(&reader);
}
