#pragma once

#include "defs.h"
#include "file_reader.h"

struct file_reader;

// @brief move
void opcode__mov(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief push
void opcode__push(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief pop
void opcode__pop(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief exchange
void opcode__xchg(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief input from
void opcode__in(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief output to
void opcode__out(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief translate byte to AL
void opcode__xlat(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief load EA to register
void opcode__lea(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief load pointer to DS
void opcode__lds(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief load pointer to ES
void opcode__les(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief load AH with flags
void opcode__lahf(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief store AH into flags
void opcode__sahf(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief push flags
void opcode__pushf(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief pop flags
void opcode__popf(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief add
void opcode__add(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief add with carry
void opcode__adc(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief increment
void opcode__inc(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief ASCII adjust for add
void opcode__aaa(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief decimal adjust for add
void opcode__daa(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief subtract
void opcode__sub(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief subtract with borrow
void opcode__sbb(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief decrement
void opcode__dec(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief change sign
void opcode__neg(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief compare
void opcode__cmp(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief ASCII adjust for subtract
void opcode__aas(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief decimal adjust for subtract
void opcode__das(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief multiply (unsigned)
void opcode__mul(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief integer multiply (signed)
void opcode__imul(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief ASCII adjust for multiply
void opcode__aam(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief divide (unsigned)
void opcode__div(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief integer divide (signed)
void opcode__idiv(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief ASCII adjust for divide
void opcode__aad(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief convert byte to word
void opcode__cbw(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief convert word to double word
void opcode__cwd(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief invert
void opcode__not(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief shift logical/arithmetic left
void opcode__shl_sal(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief shift logical right
void opcode__shr(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief shift arithmetic right
void opcode__sar(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief rotate left
void opcode__rol(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief rotate right
void opcode__ror(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief rotate through carry flag left
void opcode__rcl(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief rotate through carry right
void opcode__rcr(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief and
void opcode__and(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief and function to flags no result
void opcode__test(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief or
void opcode__or(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief exclusive or
void opcode__xor(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief repeat
void opcode__rep(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief move byte/word
void opcode__movs(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief compare byte/word
void opcode__cmps(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief scan byte/word
void opcode__scas(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief load byte/wd to AL/AX
void opcode__lods(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief store byte/wd from AL/AX
void opcode__stds(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief call
void opcode__call(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief unconditional jump
void opcode__jmp(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief return from call
void opcode__ret(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief jump on equal/zero
void opcode__je_jz(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief jump on less/not greater or equal
void opcode__jl_jnge(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief jump on less or equal/not greater
void opcode__jle_jng(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief jump on below/not above or equal
void opcode__jb_jnae(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief jump on below or equal/not above
void opcode__jbe_jna(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief jump on parity/parity even
void opcode__jp_jpe(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief jump on overflow
void opcode__jo(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief jump on sign
void opcode__js(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief jump on not equal/not zero
void opcode__jne_jnz(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief jump on not less/greater or equal
void opcode__jnl_jge(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief jump on not less or equal/greater
void opcode__jnle_jg(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief jump on not below/above or equal
void opcode__jnb_jae(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief jump on not below or equal/above
void opcode__jnbe_ja(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief jump on not par/par odd
void opcode__jnp_jpo(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief jump on not overflow
void opcode__jno(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief jump on not sign
void opcode__jns(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief loop CX times
void opcode__loop(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief loop while zero/equal
void opcode__loopz_loope(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief loop while not zero/equal
void opcode__loopnz_loopne(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief jump on CX zero
void opcode__jcxz(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief interrupt
void opcode__int(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief interrupt on overflow
void opcode__into(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief interrupt return
void opcode__iret(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief clear carry
void opcode__clc(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief complement carry
void opcode__cmc(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief set carry
void opcode__stc(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief clear direction
void opcode__cld(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief set direction
void opcode__std(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief clear interrupt
void opcode__cli(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief set interrupt
void opcode__sti(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief halt
void opcode__hlt(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief wait
void opcode__wait(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief escape (to external device)
void opcode__esc(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief bus lock prefix
void opcode__lock(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief override prefix
void opcode__segment(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief not used
void opcode__null(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief segment override prefix (stack segment register)
void opcode__ss(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief segment override prefix (code segment register)
void opcode__cs(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief segment override prefix (data segment register)
void opcode__ds(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief segment override prefix (extra segment register)
void opcode__es(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief nop instruction
void opcode__nop(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief dispatches the 1000 000x bytes to the right opcode
void opcode__1000_000x(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief dispatches the 1000 001x bytes to the right opcode
void opcode__1000_001x(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief dispatches the 1000 1100 byte to the right opcode
void opcode__1000_1100(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief dispatches the 1000 1110 byte to the right opcode
void opcode__1000_1110(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief dispatches the 1000 1111 byte to the right opcode
void opcode__1000_1111(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief dispatches the 1100 0110 byte to the right opcode
void opcode__1100_0110(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief dispatches the 1100 0111 byte to the right opcode
void opcode__1100_0111(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief dispatches the 1101 00xx bytes to the right opcode
void opcode__1101_00xx(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief dispatches the 1111 011x bytes to the right opcode
void opcode__1111_011x(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief dispatches the 1111 1110 byte to the right opcode
void opcode__1111_1110(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief dispatches the 1111 1111 byte to the right opcode
void opcode__1111_1111(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief repeat (string instruction) if not equal/zero (as long as the compared bytes or words are equal (ZF = 1) and CX is not yet counted down to zero)
void opcode__repne_repnz(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
// @brief repeat (string instruction) if equal/zero (until the compared bytes or words are equal (ZF = 1) or until CX = 0 (end of string))
void opcode__rep_repe_repz(byte first_byte, byte optional_second_byte, struct file_reader* reader, file_reader_error error_handler);
