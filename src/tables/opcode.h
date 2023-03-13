#pragma once

#include "../defs.h"
#include "../file_reader.h"
#include "../label.h"
#include "../instruction.h"

typedef void (*opcode_error)(const char* msg, ...);

struct opcode_context
{
    struct file_reader       file_reader;
    struct label_list        label_list;
    struct instruction_list  instruction_list;

    opcode_error             opcode_error_handler;
};

typedef void (*opcode_fn)(byte first_byte, struct opcode_context *context);
typedef void (*opcode_cont_fn)(byte first_byte, byte optional_second_byte, struct opcode_context *context);

#define opcode_type(fn) void fn(byte first_byte, struct opcode_context *context)
#define opcode_type_cont(fn) void fn(byte first_byte, byte optional_second_byte, struct opcode_context *context)
#define OPCODE(name) opcode__##name
#define OPCODE_CONT(name) opcode__cont_##name
#define opcode(name) opcode_type(OPCODE(name))
#define opcode_cont(name) opcode_type_cont(OPCODE_CONT(name))

// @brief move, register/memory to/from register
opcode(mov_1000_10xx);
// @brief move, immediate to register/memory
opcode(mov_1100_011x);
// @brief move, immediate to register
opcode(mov_1011_xxxx);
// @brief move, memory to accumulator
opcode(mov_1010_000x);
// @brief move, accumulator to memory
opcode(mov_1010_001x);
// @brief move, register/memory to segment register
opcode(mov_1000_1110);
// @brief move, segment register to register/memory
opcode(mov_1000_1100);

// @brief push, register/memory
opcode_cont(push_1111_1111);
// @brief push, register
opcode(push_0101_0xxx);
// @brief push, segment register
opcode(push_000x_x110);

// @brief pop, register/memory
opcode(pop_1000_1111);
// @brief pop, register
opcode(pop_0101_1xxx);
// @brief pop, segment register
opcode(pop_000x_x111);

// @brief exchange, register/memory with register
opcode(xchg_1000_011x);
// @brief exchange, register with accumulator
opcode(xchg_1001_0xxx);

// @brief input from, fixed port
opcode(in_1110_010x);
// @brief input from, variable port
opcode(in_1110_110x);

// @brief output to, fixed port
opcode(out_1110_011x);
// @brief output to, variable port
opcode(out_1110_111x);

// @brief translate byte to AL
opcode(xlat);

// @brief load EA to register
opcode(lea);

// @brief load pointer to DS
opcode(lds);

// @brief load pointer to ES
opcode(les);

// @brief load AH with flags
opcode(lahf);

// @brief store AH into flags
opcode(sahf);

// @brief push flags
opcode(pushf);

// @brief pop flags
opcode(popf);

// @brief add, reg/memory with register to either
opcode(add_0000_00xx);
// @brief add, immediate to register/memory
opcode_cont(add_1000_00xx);
// @brief add, immediate to accumulator
opcode(add_0000_010x);

// @brief add with carry, reg/memory with register to either
opcode(adc_0001_00xx);
// @brief add with carry, immediate to register/memory
opcode_cont(adc_1000_00xx);
// @brief add with carry, immediate to accumulator
opcode(adc_0001_010x);

// @brief increment, register/memory
opcode_cont(inc_1111_111x);
// @brief increment, register
opcode(inc_0100_0xxx);

// @brief ASCII adjust for add
opcode(aaa);

// @brief decimal adjust for add
opcode(daa);

// @brief subtract, reg/memory and register to either
opcode(sub_0010_10xx);
// @brief subtract, immediate from register/memory
opcode_cont(sub_1000_00xx);
// @brief subtract, immediate from accumulator
opcode(sub_0010_110x);

// @brief subtract with borrow, reg/memory and register to either
opcode(sbb_0001_10xx);
// @brief subtract with borrow, immediate from register/memory
opcode_cont(sbb_1000_00xx);
// @brief subtract with borrow, immediate from accumulator
opcode(sbb_0001_110x);

// @brief decrement, register/memory
opcode_cont(dec_1111_111x);
// @brief decrement, register
opcode(dec_0100_1xxx);

// @brief change sign
opcode_cont(neg);

// @brief compare, register/memory and register
opcode(cmp_0011_10xx);
// @brief compare, immediate with register/memory
opcode_cont(cmp_1000_00xx);
// @brief compare, immediate with accumulator
opcode(cmp_0011_110x);

// @brief ASCII adjust for subtract
opcode(aas);

// @brief decimal adjust for subtract
opcode(das);

// @brief multiply (unsigned)
opcode_cont(mul);

// @brief integer multiply (signed)
opcode_cont(imul);

// @brief ASCII adjust for multiply
opcode(aam);

// @brief divide (unsigned)
opcode_cont(div);

// @brief integer divide (signed)
opcode_cont(idiv);

// @brief ASCII adjust for divide
opcode(aad);

// @brief convert byte to word
opcode(cbw);

// @brief convert word to double word
opcode(cwd);

// @brief invert
opcode_cont(not);

// @brief shift logical/arithmetic left
opcode_cont(shl_sal);

// @brief shift logical right
opcode_cont(shr);

// @brief shift arithmetic right
opcode_cont(sar);

// @brief rotate left
opcode_cont(rol);

// @brief rotate right
opcode_cont(ror);

// @brief rotate through carry flag left
opcode_cont(rcl);

// @brief rotate through carry right
opcode_cont(rcr);

// @brief and, reg/memory with register to either
opcode(and_0010_00xx);
// @brief and, immediate to register/memory
opcode_cont(and_1000_000x);
// @brief and, immediate to accumulator
opcode(and_0010_010x);

// @brief and function to flags no result, register/memory and register
opcode(test_0001_00xx);
// @brief and function to flags no result, undocumented
opcode(test_1000_010x);
// @brief and function to flags no result, immediate data and register/memory
opcode_cont(test_1111_011x);
// @brief and function to flags no result, immediate data and accumulator
opcode(test_1010_100x);

// @brief or, reg/memory and register to either
opcode(or_0000_10xx);
// @brief or, immediate to register/memory
opcode_cont(or_1000_000x);
// @brief or, immediate to accumulator
opcode(or_0000_110x);

// @brief exclusive or, reg/memory and register to either
opcode(xor_0011_00xx);
// @brief exclusive or, immediate to register/memory
opcode_cont(xor_1000_000x);
// @brief exclusive or, immediate to accumulator
opcode(xor_0011_010x);

// @brief repeat
opcode(rep);

// @brief move byte/word
opcode(movs);

// @brief compare byte/word
opcode(cmps);

// @brief scan byte/word
opcode(scas);

// @brief load byte/wd to AL/AX
opcode(lods);

// @brief store byte/wd from AL/AX
opcode(stos);

// @brief call, direct_within segment
opcode(call_1110_1000);
// @brief call, (indirect within segment) / (indirect intersegment)
opcode_cont(call_1111_1111);
// @brief call, direct intersegment
opcode(call_1001_1010);

// @brief unconditional jump, direct within segment
opcode(jmp_1110_1001);
// @brief unconditional jump, direct within segment-short
opcode(jmp_1110_1011);
// @brief unconditional jump, (indirect within segment) / (indirect intersegment)
opcode_cont(jmp_1111_1111);
// @brief unconditional jump, direct intersegment
opcode(jmp_1110_1010);

// @brief return from call, within segment
opcode(ret_1100_0011);
// @brief return from call, within seg adding immed to SP
opcode(ret_1100_0010);
// @brief return from call, intersegment
opcode(ret_1100_1011);
// @brief return from call, intersegment adding immediate to SP
opcode(ret_1100_1010);

// @brief jump on equal/zero
opcode(je_jz);

// @brief jump on less/not greater or equal
opcode(jl_jnge);

// @brief jump on less or equal/not greater
opcode(jle_jng);

// @brief jump on below/not above or equal
opcode(jb_jnae);

// @brief jump on below or equal/not above
opcode(jbe_jna);

// @brief jump on parity/parity even
opcode(jp_jpe);

// @brief jump on overflow
opcode(jo);

// @brief jump on sign
opcode(js);

// @brief jump on not equal/not zero
opcode(jne_jnz);

// @brief jump on not less/greater or equal
opcode(jnl_jge);

// @brief jump on not less or equal/greater
opcode(jnle_jg);

// @brief jump on not below/above or equal
opcode(jnb_jae);

// @brief jump on not below or equal/above
opcode(jnbe_ja);

// @brief jump on not par/par odd
opcode(jnp_jpo);

// @brief jump on not overflow
opcode(jno);

// @brief jump on not sign
opcode(jns);

// @brief loop CX times
opcode(loop);

// @brief loop while zero/equal
opcode(loopz_loope);

// @brief loop while not zero/equal
opcode(loopnz_loopne);

// @brief jump on CX zero
opcode(jcxz);

// @brief interrupt, type specified
opcode(int_1100_1101);
// @brief interrupt, type 3
opcode(int_1100_1100);

// @brief interrupt on overflow
opcode(into);

// @brief interrupt return
opcode(iret);

// @brief clear carry
opcode(clc);

// @brief complement carry
opcode(cmc);

// @brief set carry
opcode(stc);

// @brief clear direction
opcode(cld);

// @brief set direction
opcode(std);

// @brief clear interrupt
opcode(cli);

// @brief set interrupt
opcode(sti);

// @brief halt
opcode(hlt);

// @brief wait
opcode(wait);

// @brief escape (to external device)
opcode(esc);

// @brief bus lock prefix
opcode(lock);

// @brief override prefix
opcode(segment);

// @brief not used
opcode(null);
opcode_cont(null);
// @brief segment override prefix (stack segment register)
opcode(ss);
// @brief segment override prefix (code segment register)
opcode(cs);
// @brief segment override prefix (data segment register)
opcode(ds);
// @brief segment override prefix (extra segment register)
opcode(es);
// @brief nop instruction
opcode(nop);
// @brief dispatches the 1000 000x bytes to the right opcode
opcode(1000_000x);
// @brief dispatches the 1000 001x bytes to the right opcode
opcode(1000_001x);
// @brief dispatches the 1101 00xx bytes to the right opcode
opcode(1101_00xx);
// @brief dispatches the 1111 011x bytes to the right opcode
opcode(1111_011x);
// @brief dispatches the 1111 1110 byte to the right opcode
opcode(1111_1110);
// @brief dispatches the 1111 1111 byte to the right opcode
opcode(1111_1111);
// @brief repeat (string instruction) if not equal/zero (as long as the compared bytes or words are equal (ZF = 1) and CX is not yet counted down to zero)
opcode(repne_repnz);
// @brief repeat (string instruction) if equal/zero (until the compared bytes or words are equal (ZF = 1) or until CX = 0 (end of string))
opcode(rep_repe_repz);

static const opcode_fn opcode_handlers[256] = {
    // 0000 0000 - 0000 0111
    OPCODE(add_0000_00xx), OPCODE(add_0000_00xx), OPCODE(add_0000_00xx), OPCODE(add_0000_00xx), OPCODE(add_0000_010x), OPCODE(add_0000_010x), OPCODE(push_000x_x110), OPCODE(pop_000x_x111),
    // 0000 1000 - 0000 1111
    OPCODE(or_0000_10xx),  OPCODE(or_0000_10xx),  OPCODE(or_0000_10xx),  OPCODE(or_0000_10xx),  OPCODE(or_0000_110x),  OPCODE(or_0000_110x),  OPCODE(push_000x_x110), OPCODE(pop_000x_x111),

    // 0001 0000 - 0001 0111
    OPCODE(adc_0001_00xx), OPCODE(adc_0001_00xx), OPCODE(adc_0001_00xx), OPCODE(adc_0001_00xx), OPCODE(adc_0001_010x), OPCODE(adc_0001_010x), OPCODE(push_000x_x110), OPCODE(pop_000x_x111),
    // 0001 1000 - 0001 1111
    OPCODE(sbb_0001_10xx), OPCODE(sbb_0001_10xx), OPCODE(sbb_0001_10xx), OPCODE(sbb_0001_10xx), OPCODE(sbb_0001_110x), OPCODE(sbb_0001_110x), OPCODE(push_000x_x110), OPCODE(pop_000x_x111),

    // 0010 0000 - 0010 0111
    OPCODE(and_0010_00xx), OPCODE(and_0010_00xx), OPCODE(and_0010_00xx), OPCODE(and_0010_00xx), OPCODE(and_0010_010x), OPCODE(and_0010_010x), OPCODE(es), OPCODE(daa),
    // 0010 1000 - 0010 1111
    OPCODE(sub_0010_10xx), OPCODE(sub_0010_10xx), OPCODE(sub_0010_10xx), OPCODE(sub_0010_10xx), OPCODE(sub_0010_110x), OPCODE(sub_0010_110x), OPCODE(cs), OPCODE(das),

    // 0011 0000 - 0011 0111
    OPCODE(xor_0011_00xx), OPCODE(xor_0011_00xx), OPCODE(xor_0011_00xx), OPCODE(xor_0011_00xx), OPCODE(xor_0011_010x), OPCODE(xor_0011_010x), OPCODE(ss), OPCODE(aaa),
    // 0011 1000 - 0011 1111
    OPCODE(cmp_0011_10xx), OPCODE(cmp_0011_10xx), OPCODE(cmp_0011_10xx), OPCODE(cmp_0011_10xx), OPCODE(cmp_0011_110x), OPCODE(cmp_0011_110x), OPCODE(ds), OPCODE(aas),

    // 0100 0000 - 0100 0111
    OPCODE(inc_0100_0xxx), OPCODE(inc_0100_0xxx), OPCODE(inc_0100_0xxx), OPCODE(inc_0100_0xxx), OPCODE(inc_0100_0xxx), OPCODE(inc_0100_0xxx), OPCODE(inc_0100_0xxx), OPCODE(inc_0100_0xxx),
    // 0100 1000 - 0100 1111
    OPCODE(dec_0100_1xxx), OPCODE(dec_0100_1xxx), OPCODE(dec_0100_1xxx), OPCODE(dec_0100_1xxx), OPCODE(dec_0100_1xxx), OPCODE(dec_0100_1xxx), OPCODE(dec_0100_1xxx), OPCODE(dec_0100_1xxx),

    // 0101 0000 - 0101 0111
    OPCODE(push_0101_0xxx), OPCODE(push_0101_0xxx), OPCODE(push_0101_0xxx), OPCODE(push_0101_0xxx), OPCODE(push_0101_0xxx), OPCODE(push_0101_0xxx), OPCODE(push_0101_0xxx), OPCODE(push_0101_0xxx),
    // 0101 1000 - 0101 1111
    OPCODE(pop_0101_1xxx),  OPCODE(pop_0101_1xxx),  OPCODE(pop_0101_1xxx),  OPCODE(pop_0101_1xxx),  OPCODE(pop_0101_1xxx),  OPCODE(pop_0101_1xxx),  OPCODE(pop_0101_1xxx),  OPCODE(pop_0101_1xxx),

    // 0110 0000 - 0110 0111
    OPCODE(null), OPCODE(null), OPCODE(null), OPCODE(null), OPCODE(null), OPCODE(null), OPCODE(null), OPCODE(null),
    // 0110 1000 - 0110 1111
    OPCODE(null), OPCODE(null), OPCODE(null), OPCODE(null), OPCODE(null), OPCODE(null), OPCODE(null), OPCODE(null),

    // 0111 0000 - 0111 0111
    OPCODE(jo), OPCODE(jno), OPCODE(jb_jnae), OPCODE(jnb_jae), OPCODE(je_jz),   OPCODE(jne_jnz), OPCODE(jbe_jna), OPCODE(jnbe_ja),
    // 0111 1000 - 0111 1111
    OPCODE(js), OPCODE(jns), OPCODE(jp_jpe),  OPCODE(jnp_jpo), OPCODE(jl_jnge), OPCODE(jnl_jge), OPCODE(jle_jng), OPCODE(jnle_jg),

    // 1000 0000 - 1000 0111
    OPCODE(1000_000x),     OPCODE(1000_000x),     OPCODE(1000_001x),     OPCODE(1000_001x),     OPCODE(test_1000_010x), OPCODE(test_1000_010x), OPCODE(xchg_1000_011x), OPCODE(xchg_1000_011x),
    // 1000 1000 - 1000 1111
    OPCODE(mov_1000_10xx), OPCODE(mov_1000_10xx), OPCODE(mov_1000_10xx), OPCODE(mov_1000_10xx), OPCODE(mov_1000_1100),  OPCODE(lea),            OPCODE(mov_1000_1110),  OPCODE(pop_1000_1111),

    // 1001 0000 - 1001 0111
    OPCODE(nop), OPCODE(xchg_1001_0xxx), OPCODE(xchg_1001_0xxx), OPCODE(xchg_1001_0xxx), OPCODE(xchg_1001_0xxx),  OPCODE(xchg_1001_0xxx), OPCODE(xchg_1001_0xxx), OPCODE(xchg_1001_0xxx),
    // 1001 1000 - 1001 1111
    OPCODE(cbw), OPCODE(cwd),            OPCODE(call_1001_1010), OPCODE(wait),           OPCODE(pushf),           OPCODE(popf),           OPCODE(sahf),           OPCODE(lahf),

    // 1010 0000 - 1010 0111
    OPCODE(mov_1010_000x),  OPCODE(mov_1010_000x),  OPCODE(mov_1010_001x), OPCODE(mov_1010_001x), OPCODE(movs), OPCODE(movs), OPCODE(cmps), OPCODE(cmps),
    // 1010 1000 - 1010 1111
    OPCODE(test_1010_100x), OPCODE(test_1010_100x), OPCODE(stos),          OPCODE(stos),          OPCODE(lods), OPCODE(lods), OPCODE(scas), OPCODE(scas),

    // 1011 0000 - 1011 0111
    OPCODE(mov_1011_xxxx), OPCODE(mov_1011_xxxx), OPCODE(mov_1011_xxxx), OPCODE(mov_1011_xxxx), OPCODE(mov_1011_xxxx), OPCODE(mov_1011_xxxx), OPCODE(mov_1011_xxxx), OPCODE(mov_1011_xxxx),
    // 1011 1000 - 1011 1111
    OPCODE(mov_1011_xxxx), OPCODE(mov_1011_xxxx), OPCODE(mov_1011_xxxx), OPCODE(mov_1011_xxxx), OPCODE(mov_1011_xxxx), OPCODE(mov_1011_xxxx), OPCODE(mov_1011_xxxx), OPCODE(mov_1011_xxxx),

    // 1100 0000 - 1100 0111
    OPCODE(null), OPCODE(null), OPCODE(ret_1100_0010), OPCODE(ret_1100_0011), OPCODE(les),           OPCODE(lds),           OPCODE(mov_1100_011x), OPCODE(mov_1100_011x),
    // 1100 1000 - 1100 1111
    OPCODE(null), OPCODE(null), OPCODE(ret_1100_1010), OPCODE(ret_1100_1011), OPCODE(int_1100_1100), OPCODE(int_1100_1101), OPCODE(into),          OPCODE(iret),

    // 1101 0000 - 1101 0111
    OPCODE(1101_00xx), OPCODE(1101_00xx), OPCODE(1101_00xx), OPCODE(1101_00xx), OPCODE(aam), OPCODE(aad), OPCODE(null), OPCODE(xlat),
    // 1101 1000 - 1101 1111
    OPCODE(esc),       OPCODE(esc),       OPCODE(esc),       OPCODE(esc),       OPCODE(esc), OPCODE(esc), OPCODE(esc),  OPCODE(esc),

    // 1110 0000 - 1110 0111
    OPCODE(loopnz_loopne),  OPCODE(loopz_loope),   OPCODE(loop),          OPCODE(jcxz),          OPCODE(in_1110_010x), OPCODE(in_1110_010x), OPCODE(out_1110_011x), OPCODE(out_1110_011x),
    // 1110 1000 - 1110 1111
    OPCODE(call_1110_1000), OPCODE(jmp_1110_1001), OPCODE(jmp_1110_1010), OPCODE(jmp_1110_1011), OPCODE(in_1110_110x), OPCODE(in_1110_110x), OPCODE(out_1110_111x), OPCODE(out_1110_111x),

    // 1111 0000 - 1111 0111
    OPCODE(lock), OPCODE(null), OPCODE(repne_repnz), OPCODE(rep_repe_repz), OPCODE(hlt), OPCODE(cmc), OPCODE(1111_011x), OPCODE(1111_011x),
    // 1111 1000 - 1111 1111
    OPCODE(clc),  OPCODE(stc),  OPCODE(cli),         OPCODE(sti),           OPCODE(cld), OPCODE(std), OPCODE(1111_1110), OPCODE(1111_1111)
};
