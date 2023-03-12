#include "opcode.h"
#include "mod.h"

void opcode__mov_1000_10xx(byte first_byte, struct opcode_context* context) {

    // Register/memory to/from register
    // mov instruction
    // 100010 d w    mod reg r/m    (DISP-LO)    (DISP-HI)

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "mov ");

    int d = (first_byte >> 1) & 0b1;
    int w = first_byte & 0b1;

    byte second_byte;
    file_reader__read_byte(&context->file_reader, &second_byte, context->error_handler);

    int mod = (second_byte >> 6) & 0b11;
    int reg = (second_byte >> 3) & 0b111;
    int r_m = second_byte & 0b111;

    mod_handlers[mod](w, d, reg, r_m, context, &instruction);

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__mov_1100_011x(byte first_byte, struct opcode_context* context) {
    // mov instruction
    // Immediate to register/memory
    // 1100011 w    mod 0 0 0 r/m    (DISP-LO)    (DISP-HI)    data    data if w = 1

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "mov ");

    byte second_byte;
    file_reader__read_byte(&context->file_reader, &second_byte, context->error_handler);

    int w   = first_byte & 0b1;
    int mod = (second_byte >> 6) & 0b11;
    int r_m = second_byte & 0b111;

    mod_immediate_handlers[mod](w, r_m, context, &instruction);

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__mov_1011_xxxx(byte first_byte, struct opcode_context* context) {
    // mov instruction
    // Immediate to register
    // 1011 w reg    data    data if w = 1

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "mov ");

    int w   = (first_byte >> 3) & 0b1;
    int reg = first_byte & 0b111;

    word data = 0;
    file_reader__read_by_type[w](&context->file_reader, &data, context->error_handler);

    instruction__push(
        &instruction,
        "%s, %u",
        reg_to_word(reg, w),
        data
    );

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__mov_1010_000x(byte first_byte, struct opcode_context* context) {
    // mov instruction
    // Memory to accumulator
    // 1010000 w    addr-lo    addr-hi

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "mov ");

    int w = first_byte & 0b1;

    word addr = 0;
    file_reader__read_word(&context->file_reader, &addr, context->error_handler);

    // NOTE(david): register ax for word transfers, al for bytes
    instruction__push(
        &instruction,
        "%s, [%u]",
        reg_to_word(0, w),
        addr
    );

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__mov_1010_001x(byte first_byte, struct opcode_context* context) {
    // mov instruction
    // Accumulator to memory
    // 1010001 w    addr-lo    addr-hi

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "mov ");

    int w = first_byte & 0b1;

    word addr = 0;
    file_reader__read_word(&context->file_reader, &addr, context->error_handler);

    // NOTE(david): register ax for word transfers, al for bytes
    instruction__push(
        &instruction,
        "[%u], %s",
        addr,
        reg_to_word(0, w)
    );

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__mov_1000_1110(byte first_byte, struct opcode_context* context) {
    // mov instruction
    // Register/memory to segment register
    // 10001110    mod 0 SR r/m    (DISP-LO)    (DISP-HI)

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "mov ");

    (void)first_byte;
    (void)context;
    assert(false && "TODO(david): implement how to dispatch 0SR");

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__mov_1000_1100(byte first_byte, struct opcode_context* context) {
    // mov instruction
    // Segment register to register/memory
    // 10001100    mod 0 SR r/m    (DISP-LO)    (DISP-HI)

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "mov ");

    (void)first_byte;
    (void)context;
    assert(false && "TODO(david): implement how to dispatch 0SR");

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__cont_push_1111_1111(byte first_byte, byte second_byte, struct opcode_context* context) {

    // Register/memory
    // 1111 1111    mod 110 r/m    (DISP-LO)    (DISP-HI)

    (void)first_byte;

    struct instruction instruction;
    
    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "push ");

    int mod = (second_byte >> 6) & 0b11;
    int r_m = second_byte & 0b111;

    mod_source_only_handlers[mod](r_m, context, &instruction);

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__push_0101_0xxx(byte first_byte, struct opcode_context* context) {

    // Register
    // 0101 0reg

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "push ");

    int reg = first_byte & 0b111;

    instruction__push(
        &instruction,
        "%s",
        reg_to_word(reg, 1)
    );

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__push_000x_x110(byte first_byte, struct opcode_context* context) {

    // Segment register
    // 000r eg110

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "push ");

    int reg = (first_byte >> 3) & 0b11;

    instruction__push(
        &instruction,
        "%s",
        segment_reg_to_word(reg)
    );

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__pop_1000_1111(byte first_byte, struct opcode_context* context) {

    // Register/memory
    // 1000 1111    mod 000 r/m    (DISP-LO)    (DISP-HI)

    (void)first_byte;

    struct instruction instruction;
    
    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "pop ");

    byte second_byte;
    file_reader__read_byte(&context->file_reader, &second_byte, context->error_handler);

    int mod = (second_byte >> 6) & 0b11;
    int r_m = second_byte & 0b111;

    mod_source_only_handlers[mod](r_m, context, &instruction);

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__pop_0101_1xxx(byte first_byte, struct opcode_context* context) {

    // Register
    // 0101 1reg

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "pop ");

    int reg = first_byte & 0b111;

    instruction__push(
        &instruction,
        "%s",
        reg_to_word(reg, 1)
    );

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__pop_000x_x111(byte first_byte, struct opcode_context* context) {

    // Segment register
    // 000r eg111

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "pop ");

    int reg = (first_byte >> 3) & 0b11;

    instruction__push(
        &instruction,
        "%s",
        segment_reg_to_word(reg)
    );

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__xchg_1000_011x(byte first_byte, struct opcode_context* context) {

    // Register/memory with register
    // 1000 011w    mod reg r/m    (DISP-LO)    (DISP-HI)

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "xchg ");

    int w = first_byte & 0b1;

    byte second_byte;
    file_reader__read_byte(&context->file_reader, &second_byte, context->error_handler);

    int mod = (second_byte >> 6) & 0b11;
    int reg = (second_byte >> 3) & 0b111;
    int r_m = second_byte & 0b111;

    mod_handlers[mod](w, 1, reg, r_m, context, &instruction);

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__xchg_1001_0xxx(byte first_byte, struct opcode_context* context) {

    // Register with accumulator
    // 1001 0reg

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "xchg ");

    int reg = first_byte & 0b111;

    mod__register_mode_no_ea(1, 0, reg, AX, context, &instruction);

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__in_1110_010x(byte first_byte, struct opcode_context* context) {

    // Fixed port
    // 1110 010w    DATA-8

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "in ");

    int w = first_byte & 0b1;

    byte data;
    file_reader__read_byte(&context->file_reader, &data, context->error_handler);

    instruction__push(
        &instruction,
        "%s, %u",
        w ? reg_to_word(AX, 1) : reg_to_word(AL, 0),
        data
    );

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__in_1110_110x(byte first_byte, struct opcode_context* context) {

    // Variable port
    // 1110 110w

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "in ");

    int w = first_byte & 0b1;

    instruction__push(
        &instruction,
        "%s, %s",
        w ? reg_to_word(AX, 1) : reg_to_word(AL, 0),
        "dx"
    );

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__out_1110_011x(byte first_byte, struct opcode_context* context) {

    // Fixed port
    // 1110 011w    DATA-8

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "out ");

    int w = first_byte & 0b1;

    byte data;
    file_reader__read_byte(&context->file_reader, &data, context->error_handler);

    instruction__push(
        &instruction,
        "%u, %s",
        data,
        w ? reg_to_word(AX, 1) : reg_to_word(AL, 0)
    );

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__out_1110_111x(byte first_byte, struct opcode_context* context) {

    // Variable port
    // 1110 111w

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "out ");

    int w = first_byte & 0b1;

    instruction__push(
        &instruction,
        "%s, %s",
        "dx",
        w ? reg_to_word(AX, 1) : reg_to_word(AL, 0)
    );

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__xlat(byte first_byte, struct opcode_context* context) {

    // Translate byte to AL
    // 1101 0111

    /*
        XLAT (translate) replaces a byte in the AL
        register with a byte from a 256-byte, user-coded
        translation table. Register BX is assumed to point
        to the beginning of the table. The byte in AL is
        used as an index into the table and is replaced by
        the byte at the offset in the table corresponding to
        AL's binary value. The first byte in the table has
        an offset of O. For example, if AL contains 5H,
        and the sixth element of the translation table contains 33H, then AL will contain 33H following
        the instruction. XLAT is useful for translating
        characters from one code to another, the classic
        example being ASCII to EBCDIC (extended binary-coded decimal interchange code) or the reverse. 
    */

    (void)first_byte;

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "xlat");

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__lea(byte first_byte, struct opcode_context* context) {

    // Load EA to register
    // 1000 1101    mod reg r/m    (DISP-LO)    (DISP-HI)

    /*
        LEA (load effective address) transfers the offset
        of the source operand (rather than its value) to the
        destination operand. The source operand must be
        a memory operand, and the destination operand
        must be a 16-bit general register. LEA does not
        affect any flags. The XLA T and string instructions assume that certain registers point to
        operands; LEA can be used to load these registers
        (e.g., 10'lding BX with the address of the translate
        table used by the XLA T instruction). 
    */

    (void)first_byte;

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "lea ");

    byte second_byte;
    file_reader__read_byte(&context->file_reader, &second_byte, context->error_handler);

    int mod = (second_byte >> 6) & 0b11;
    int reg = (second_byte >> 3) & 0b111;
    int r_m = second_byte & 0b111;

    mod_handlers[mod](1, 1, reg, r_m, context, &instruction);

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__lds(byte first_byte, struct opcode_context* context) {

    // Load pointer to DS
    // 1100 0101    mod reg r/m    (DISP-LO)    (DISP-HI)

    /*
        LDS (load pointer using DS) transfers a 32-bit
        pointer variable from the source operand, which
        must be a memory operand, to the destination
        operand and register DS. The offset word of the
        pointer is transferred to the destination operand,
        which may be any 16-bit general register. The segment word of the pointer is transferred to register
        DS. Specifying SI as the destination operand is a
        convenient way to prepare to process a source
        string that is not in the current data segment
        (string instructions assume that the source string
        is located in the current data segment and that SI
        contains the offset of the string). 
    */

    (void)first_byte;

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "lds ");

    byte second_byte;
    file_reader__read_byte(&context->file_reader, &second_byte, context->error_handler);

    int mod = (second_byte >> 6) & 0b11;
    int reg = (second_byte >> 3) & 0b111;
    int r_m = second_byte & 0b111;

    mod_handlers[mod](1, 1, reg, r_m, context, &instruction);

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__les(byte first_byte, struct opcode_context* context) {

    // Load pointer to ES
    // 1100 0100    mod reg r/m    (DISP-LO)    (DISP-HI)

    /*
        LES (load pointer using ES) transfers a 32-bit
        pointer variable from the source operand, which
        must be a memory operand, to the destination
        operand and register ES. The offset word of the
        pointer is transferred to the destination operand,
        which may be any 16-bit general register. The segment word of the pointer is transferred to register
        ES. Specifying DI as the destination operand is a
        convenient way to prepare to process a destination string that is not in the current extra segment.
        (The destination string must be located in the
        extra segment, and DI must contain the offset of
        the string.) 
    */

    (void)first_byte;

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "les ");

    byte second_byte;
    file_reader__read_byte(&context->file_reader, &second_byte, context->error_handler);

    int mod = (second_byte >> 6) & 0b11;
    int reg = (second_byte >> 3) & 0b111;
    int r_m = second_byte & 0b111;

    mod_handlers[mod](1, 1, reg, r_m, context, &instruction);

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__lahf(byte first_byte, struct opcode_context* context) {

    // Load AH with flags
    // 1001 1111

    /*
        LAHF (load register AH from flags) copies SF,
        ZF, AF, PF and CF (the 8080/8085 flags) into
        bits 7, 6, 4, 2 and 0, respectively, of register AH 
        (see figure 2-32). The content of bits 5, 3 and 1 is
        undefined; the flags themselves are not affected.
        LAHF is provided primarily for converting
        8080/8085 assembly language programs to run on
        an 8086 or 8088. 
    */

    (void)first_byte;

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "lahf");

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__sahf(byte first_byte, struct opcode_context* context) {

    // Store AH into flags
    // 1001 1110

    /*
        SAHF (store register AH into flags) transfers bits
        7,6,4,2 and 0 from register AH into SF, ZF, AF,
        PF and CF, respectively, replacing whatever
        values these flags previously had. OF, DF, IF and
        TF are not affected. This instruction is provided
        for 8080/8085 compatibility. 
    */

    (void)first_byte;

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "sahf");

    instruction_list__push(&context->instruction_list, instruction);
}

void opcode__pushf(byte first_byte, struct opcode_context* context) {

    // Push flags
    // 1001 1100

    /*
        PUSHF decrements SP (the stack pointer) by two
        and then transfers all flags to the word at the top
        of stack pointed to by SP (see figure 2-32). The
        flags themselves are not affected. 
    */

    (void)first_byte;

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "pushf");

    instruction_list__push(&context->instruction_list, instruction);
}

void opcode__popf(byte first_byte, struct opcode_context* context) {

    // Pop flags
    // 1001 1101

    /*
        POPF transfers specific bits from the word at the
        current top of stack (pointed to by register SP)
        into the 8086/8088 flags, replacing whatever
        values the flags previously contained (see figure
        2-32). SP is then incremented by two to point to
        the new top of stack. PUSHF and POPF allow a
        procedure to save and restore a calling program's
        flags. They also allow a program to change the 
        setting of TF (there is no instruction for updating
        this flag directly). The change is accomplished by
        pushing the flags, altering bit 8 of the memoryimage and then popping the flags. 
    */

    (void)first_byte;

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "popf");

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__add_0000_00xx(byte first_byte, struct opcode_context* context) {
    
    // 0000 00dw    mod reg r/m    (DISP-LO)    (DISP_HI)

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "add ");

    int d   = (first_byte >> 1) & 0b1;
    int w   = first_byte & 0b1;

    byte second_byte;
    file_reader__read_byte(&context->file_reader, &second_byte, context->error_handler);

    int mod = (second_byte >> 6) & 0b11;
    int reg = (second_byte >> 3) & 0b111;
    int r_m = second_byte & 0b111;

    mod_handlers[mod](w, d, reg, r_m, context, &instruction);

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__cont_add_1000_00xx(byte first_byte, byte second_byte, struct opcode_context* context) {
    
    // 1000 00sw    mod 000 r_m    (DISP-LO)    (DISP-HI)    data    data if sw = 01

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "add ");

    int s   = (first_byte >> 1) & 0b1;
    int w   = first_byte & 0b1;
    int mod = (second_byte >> 6) & 0b11;
    int r_m = second_byte & 0b111;

    mod_immediate_signed_handlers[mod](s, w, r_m, context, &instruction);

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__add_0000_010x(byte first_byte, struct opcode_context* context) {

    // 0000 010w    data    data if w = 1

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "add ");

    int w = first_byte & 0b1;

    word data = 0;
    file_reader__read_by_type[w](&context->file_reader, &data, context->error_handler);

    // NOTE(david): register ax for word transfers, al for bytes
    instruction__push(
        &instruction,
        "%s, %u",
        reg_to_word(0, w),
        data
    );

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__adc_0001_00xx(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__cont_adc_1000_00xx(byte first_byte, byte second_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)second_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__adc_0001_010x(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__cont_inc_1111_111x(byte first_byte, byte second_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)second_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__inc_0100_0xxx(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__aaa(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__daa(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__sub_0010_10xx(byte first_byte, struct opcode_context* context) {

    // Reg/Memory and register to either
    // 0010 10dw    mod reg r/m    (DISP-LO)    (DISP-HI)

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "sub ");

    int d = (first_byte >> 1) & 0b1;
    int w = first_byte & 0b1;

    byte second_byte;
    file_reader__read_byte(&context->file_reader, &second_byte, context->error_handler);

    int mod = (second_byte >> 6) & 0b11;
    int reg = (second_byte >> 3) & 0b111;
    int r_m = second_byte & 0b111;

    mod_handlers[mod](w, d, reg, r_m, context, &instruction);

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__cont_sub_1000_00xx(byte first_byte, byte second_byte, struct opcode_context* context) {

    // Immediate from register/memory
    // 1000 00sw    mod 101 r/m    (DISP-LO)    (DISP-HI)    data    data if sw = 01

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "sub ");

    int s   = (first_byte >> 1) & 0b1;
    int w   = first_byte & 0b1;
    int mod = (second_byte >> 6) & 0b11;
    int r_m = second_byte & 0b111;

    mod_immediate_signed_handlers[mod](s, w, r_m, context, &instruction);

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__sub_0010_110x(byte first_byte, struct opcode_context* context) {

    // Immediate from accumulator
    // 0010 110w    data    data if w = 1

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "sub ");

    int w = first_byte & 0b1;

    word data = 0;
    file_reader__read_by_type[w](&context->file_reader, &data, context->error_handler);

    // NOTE(david): register ax for word transfers, al for bytes
    instruction__push(
        &instruction,
        "%s, %u",
        reg_to_word(0, w),
        data
    );

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__sbb_0001_10xx(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__cont_sbb_1000_00xx(byte first_byte, byte second_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)second_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__sbb_0001_110x(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__cont_dec_1111_111x(byte first_byte, byte second_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)second_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__dec_0100_1xxx(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__cont_neg(byte first_byte, byte second_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)second_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__cmp_0011_10xx(byte first_byte, struct opcode_context* context) {

    // Register/memory and register
    // 0011 10dw    mod reg r/m    (DISP-LO)    (DISP-HI)

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "cmp ");

    int d = (first_byte >> 1) & 0b1;
    int w = first_byte & 0b1;

    byte second_byte;
    file_reader__read_byte(&context->file_reader, &second_byte, context->error_handler);

    int mod = (second_byte >> 6) & 0b11;
    int reg = (second_byte >> 3) & 0b111;
    int r_m = second_byte & 0b111;

    mod_handlers[mod](w, d, reg, r_m, context, &instruction);

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__cont_cmp_1000_00xx(byte first_byte, byte second_byte, struct opcode_context* context) {

    // Immediate with register/memory
    // 1000 00sw    mod 111 r/m    (DISP-LO)    (DISP-HI)    data    data if w = 1

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "cmp ");

    int s   = (first_byte >> 1) & 0b1;
    int w   = first_byte & 0b1;
    int mod = (second_byte >> 6) & 0b11;
    int r_m = second_byte & 0b111;

    mod_immediate_signed_handlers[mod](s, w, r_m, context, &instruction);

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__cmp_0011_110x(byte first_byte, struct opcode_context* context) {

    // Immediate with accumulator
    // 0011 110w    data

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "cmp ");

    int w = first_byte & 0b1;

    word data = 0;
    file_reader__read_by_type[w](&context->file_reader, &data, context->error_handler);

    // NOTE(david): register ax for word transfers, al for bytes
    instruction__push(
        &instruction,
        "%s, %u",
        reg_to_word(0, w),
        data
    );

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__aas(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__das(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__cont_mul(byte first_byte, byte second_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)second_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__cont_imul(byte first_byte, byte second_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)second_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__aam(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__cont_div(byte first_byte, byte second_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)second_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__cont_idiv(byte first_byte, byte second_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)second_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__aad(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__cbw(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__cwd(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__cont_not(byte first_byte, byte second_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)second_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__cont_shl_sal(byte first_byte, byte second_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)second_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__cont_shr(byte first_byte, byte second_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)second_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__cont_sar(byte first_byte, byte second_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)second_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__cont_rol(byte first_byte, byte second_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)second_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__cont_ror(byte first_byte, byte second_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)second_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__cont_rcl(byte first_byte, byte second_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)second_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__cont_rcr(byte first_byte, byte second_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)second_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__and_0010_00xx(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__cont_and_1000_000x(byte first_byte, byte second_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)second_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__and_0010_010x(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__test_0001_00xx(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "// note(david): this case isn't in the corresponding table of the 8086 manual, so just ignoring it");
}

void opcode__test_1000_010x(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__cont_test_1111_011x(byte first_byte, byte second_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)second_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__test_1010_100x(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__or_0000_10xx(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__cont_or_1000_000x(byte first_byte, byte second_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)second_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__or_0000_110x(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__xor_0011_00xx(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__cont_xor_1000_000x(byte first_byte, byte second_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)second_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__xor_0011_010x(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__rep(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__movs(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__cmps(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__scas(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__lods(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__stos(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__call_1110_1000(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__cont_call_1111_1111(byte first_byte, byte second_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)second_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__call_1001_1010(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__jmp_1110_1001(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__jmp_1110_1011(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__cont_jmp_1111_1111(byte first_byte, byte second_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)second_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__jmp_1110_1010(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__ret_1100_0011(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__ret_1100_0010(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__ret_1100_1011(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__ret_1100_1010(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__je_jz(byte first_byte, struct opcode_context* context) {

    // Jump on equal/zero
    // 0111 0100    (IP-INC8)

    (void)first_byte;

    struct instruction  instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "je ");

    signed_byte  signed_increment_to_instruction_pointer = 0;
    file_reader__read_byte(&context->file_reader, &signed_increment_to_instruction_pointer, context->error_handler);

    uint32_t label_instruction_pointer = file_reader__read_bytes_so_far(&context->file_reader) + signed_increment_to_instruction_pointer;
    label_list__insert(&context->label_list, label_instruction_pointer);

    uint32_t label_unique_id;
    if (!label_list__get_unique_id(&context->label_list, label_instruction_pointer, &label_unique_id)) {
        context->error_handler("Couldn't find just inserted label by its instruction pointer", FILE_READER_ERROR_FATAL);
    }

    instruction__push(
        &instruction,
        "%s_%u",
        "label",
        label_unique_id
    );

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__jl_jnge(byte first_byte, struct opcode_context* context) {

    // Jump on less/not greater or equal
    // 0111 1100    (IP-INC8)

    (void)first_byte;

    struct instruction  instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "jl ");

    signed_byte  signed_increment_to_instruction_pointer = 0;
    file_reader__read_byte(&context->file_reader, &signed_increment_to_instruction_pointer, context->error_handler);

    uint32_t label_instruction_pointer = file_reader__read_bytes_so_far(&context->file_reader) + signed_increment_to_instruction_pointer;
    label_list__insert(&context->label_list, label_instruction_pointer);
    
    uint32_t label_unique_id;
    if (!label_list__get_unique_id(&context->label_list, label_instruction_pointer, &label_unique_id)) {
        context->error_handler("Couldn't find just inserted label by its instruction pointer", FILE_READER_ERROR_FATAL);
    }

    instruction__push(
        &instruction,
        "%s_%u",
        "label",
        label_unique_id
    );

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__jle_jng(byte first_byte, struct opcode_context* context) {

    // Jump on less or equal/not greater
    // 0111 1110    (IP-INC8)

    (void)first_byte;

    struct instruction  instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "jle ");

    signed_byte  signed_increment_to_instruction_pointer = 0;
    file_reader__read_byte(&context->file_reader, &signed_increment_to_instruction_pointer, context->error_handler);

    uint32_t label_instruction_pointer = file_reader__read_bytes_so_far(&context->file_reader) + signed_increment_to_instruction_pointer;
    label_list__insert(&context->label_list, label_instruction_pointer);
    
    uint32_t label_unique_id;
    if (!label_list__get_unique_id(&context->label_list, label_instruction_pointer, &label_unique_id)) {
        context->error_handler("Couldn't find just inserted label by its instruction pointer", FILE_READER_ERROR_FATAL);
    }

    instruction__push(
        &instruction,
        "%s_%u",
        "label",
        label_unique_id
    );

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__jb_jnae(byte first_byte, struct opcode_context* context) {

    // Jump on below/not above or equal
    // 0111 0010    (IP-INC8)

    (void)first_byte;

    struct instruction  instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "jb ");

    signed_byte  signed_increment_to_instruction_pointer = 0;
    file_reader__read_byte(&context->file_reader, &signed_increment_to_instruction_pointer, context->error_handler);

    uint32_t label_instruction_pointer = file_reader__read_bytes_so_far(&context->file_reader) + signed_increment_to_instruction_pointer;
    label_list__insert(&context->label_list, label_instruction_pointer);
    
    uint32_t label_unique_id;
    if (!label_list__get_unique_id(&context->label_list, label_instruction_pointer, &label_unique_id)) {
        context->error_handler("Couldn't find just inserted label by its instruction pointer", FILE_READER_ERROR_FATAL);
    }

    instruction__push(
        &instruction,
        "%s_%u",
        "label",
        label_unique_id
    );

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__jbe_jna(byte first_byte, struct opcode_context* context) {

    // Jump on below or equal/not above
    // 0111 0110    (IP-INC8)

    (void)first_byte;

    struct instruction  instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "jbe ");

    signed_byte  signed_increment_to_instruction_pointer = 0;
    file_reader__read_byte(&context->file_reader, &signed_increment_to_instruction_pointer, context->error_handler);

    uint32_t label_instruction_pointer = file_reader__read_bytes_so_far(&context->file_reader) + signed_increment_to_instruction_pointer;
    label_list__insert(&context->label_list, label_instruction_pointer);
    
    uint32_t label_unique_id;
    if (!label_list__get_unique_id(&context->label_list, label_instruction_pointer, &label_unique_id)) {
        context->error_handler("Couldn't find just inserted label by its instruction pointer", FILE_READER_ERROR_FATAL);
    }

    instruction__push(
        &instruction,
        "%s_%u",
        "label",
        label_unique_id
    );

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__jp_jpe(byte first_byte, struct opcode_context* context) {

    // Jump on parity/parity even
    // 0111 1010    (IP-INC8)

    (void)first_byte;

    struct instruction  instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "jp ");

    signed_byte  signed_increment_to_instruction_pointer = 0;
    file_reader__read_byte(&context->file_reader, &signed_increment_to_instruction_pointer, context->error_handler);

    uint32_t label_instruction_pointer = file_reader__read_bytes_so_far(&context->file_reader) + signed_increment_to_instruction_pointer;
    label_list__insert(&context->label_list, label_instruction_pointer);
    
    uint32_t label_unique_id;
    if (!label_list__get_unique_id(&context->label_list, label_instruction_pointer, &label_unique_id)) {
        context->error_handler("Couldn't find just inserted label by its instruction pointer", FILE_READER_ERROR_FATAL);
    }

    instruction__push(
        &instruction,
        "%s_%u",
        "label",
        label_unique_id
    );

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__jo(byte first_byte, struct opcode_context* context) {

    // Jump on overflow
    // 0111 0000    (IP-INC8)

    (void)first_byte;

    struct instruction  instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "jo ");

    signed_byte  signed_increment_to_instruction_pointer = 0;
    file_reader__read_byte(&context->file_reader, &signed_increment_to_instruction_pointer, context->error_handler);

    uint32_t label_instruction_pointer = file_reader__read_bytes_so_far(&context->file_reader) + signed_increment_to_instruction_pointer;
    label_list__insert(&context->label_list, label_instruction_pointer);
    
    uint32_t label_unique_id;
    if (!label_list__get_unique_id(&context->label_list, label_instruction_pointer, &label_unique_id)) {
        context->error_handler("Couldn't find just inserted label by its instruction pointer", FILE_READER_ERROR_FATAL);
    }

    instruction__push(
        &instruction,
        "%s_%u",
        "label",
        label_unique_id
    );

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__js(byte first_byte, struct opcode_context* context) {

    // Jump on sign
    // 0111 1000    (IP-INC8)

    (void)first_byte;

    struct instruction  instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "js ");

    signed_byte  signed_increment_to_instruction_pointer = 0;
    file_reader__read_byte(&context->file_reader, &signed_increment_to_instruction_pointer, context->error_handler);

    uint32_t label_instruction_pointer = file_reader__read_bytes_so_far(&context->file_reader) + signed_increment_to_instruction_pointer;
    label_list__insert(&context->label_list, label_instruction_pointer);
    
    uint32_t label_unique_id;
    if (!label_list__get_unique_id(&context->label_list, label_instruction_pointer, &label_unique_id)) {
        context->error_handler("Couldn't find just inserted label by its instruction pointer", FILE_READER_ERROR_FATAL);
    }

    instruction__push(
        &instruction,
        "%s_%u",
        "label",
        label_unique_id
    );

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__jne_jnz(byte first_byte, struct opcode_context* context) {

    // Jump on not equal/not zero
    // 0111 0101    (IP-INC8)

    (void)first_byte;

    struct instruction  instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "jnz ");

    signed_byte  signed_increment_to_instruction_pointer = 0;
    file_reader__read_byte(&context->file_reader, &signed_increment_to_instruction_pointer, context->error_handler);

    uint32_t label_instruction_pointer = file_reader__read_bytes_so_far(&context->file_reader) + signed_increment_to_instruction_pointer;
    label_list__insert(&context->label_list, label_instruction_pointer);
    
    uint32_t label_unique_id;
    if (!label_list__get_unique_id(&context->label_list, label_instruction_pointer, &label_unique_id)) {
        context->error_handler("Couldn't find just inserted label by its instruction pointer", FILE_READER_ERROR_FATAL);
    }

    instruction__push(
        &instruction,
        "%s_%u",
        "label",
        label_unique_id
    );

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__jnl_jge(byte first_byte, struct opcode_context* context) {

    // Jump on not less/greater or equal
    // 0111 1101    (IP-INC8)

    (void)first_byte;

    struct instruction  instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "jnl ");

    signed_byte  signed_increment_to_instruction_pointer = 0;
    file_reader__read_byte(&context->file_reader, &signed_increment_to_instruction_pointer, context->error_handler);

    uint32_t label_instruction_pointer = file_reader__read_bytes_so_far(&context->file_reader) + signed_increment_to_instruction_pointer;
    label_list__insert(&context->label_list, label_instruction_pointer);
    
    uint32_t label_unique_id;
    if (!label_list__get_unique_id(&context->label_list, label_instruction_pointer, &label_unique_id)) {
        context->error_handler("Couldn't find just inserted label by its instruction pointer", FILE_READER_ERROR_FATAL);
    }

    instruction__push(
        &instruction,
        "%s_%u",
        "label",
        label_unique_id
    );

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__jnle_jg(byte first_byte, struct opcode_context* context) {

    // Jump on not less or equal/greater
    // 0111 1111    (IP-INC8)

    (void)first_byte;

    struct instruction  instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "jg ");

    signed_byte  signed_increment_to_instruction_pointer = 0;
    file_reader__read_byte(&context->file_reader, &signed_increment_to_instruction_pointer, context->error_handler);

    uint32_t label_instruction_pointer = file_reader__read_bytes_so_far(&context->file_reader) + signed_increment_to_instruction_pointer;
    label_list__insert(&context->label_list, label_instruction_pointer);
    
    uint32_t label_unique_id;
    if (!label_list__get_unique_id(&context->label_list, label_instruction_pointer, &label_unique_id)) {
        context->error_handler("Couldn't find just inserted label by its instruction pointer", FILE_READER_ERROR_FATAL);
    }

    instruction__push(
        &instruction,
        "%s_%u",
        "label",
        label_unique_id
    );

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__jnb_jae(byte first_byte, struct opcode_context* context) {

    // Jump on not below/above or equal
    // 0111 0011    (IP-INC8)

    (void)first_byte;

    struct instruction  instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "jnb ");

    signed_byte  signed_increment_to_instruction_pointer = 0;
    file_reader__read_byte(&context->file_reader, &signed_increment_to_instruction_pointer, context->error_handler);

    uint32_t label_instruction_pointer = file_reader__read_bytes_so_far(&context->file_reader) + signed_increment_to_instruction_pointer;
    label_list__insert(&context->label_list, label_instruction_pointer);
    
    uint32_t label_unique_id;
    if (!label_list__get_unique_id(&context->label_list, label_instruction_pointer, &label_unique_id)) {
        context->error_handler("Couldn't find just inserted label by its instruction pointer", FILE_READER_ERROR_FATAL);
    }

    instruction__push(
        &instruction,
        "%s_%u",
        "label",
        label_unique_id
    );

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__jnbe_ja(byte first_byte, struct opcode_context* context) {

    // Jump on not below or equal/above
    // 0111 0111    (IP-INC8)

    (void)first_byte;

    struct instruction  instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "ja ");

    signed_byte  signed_increment_to_instruction_pointer = 0;
    file_reader__read_byte(&context->file_reader, &signed_increment_to_instruction_pointer, context->error_handler);

    uint32_t label_instruction_pointer = file_reader__read_bytes_so_far(&context->file_reader) + signed_increment_to_instruction_pointer;
    label_list__insert(&context->label_list, label_instruction_pointer);
    
    uint32_t label_unique_id;
    if (!label_list__get_unique_id(&context->label_list, label_instruction_pointer, &label_unique_id)) {
        context->error_handler("Couldn't find just inserted label by its instruction pointer", FILE_READER_ERROR_FATAL);
    }

    instruction__push(
        &instruction,
        "%s_%u",
        "label",
        label_unique_id
    );

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__jnp_jpo(byte first_byte, struct opcode_context* context) {

    // Jump on not par/par odd
    // 0111 1011    (IP-INC8)

    (void)first_byte;

    struct instruction  instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "jnp ");

    signed_byte  signed_increment_to_instruction_pointer = 0;
    file_reader__read_byte(&context->file_reader, &signed_increment_to_instruction_pointer, context->error_handler);

    uint32_t label_instruction_pointer = file_reader__read_bytes_so_far(&context->file_reader) + signed_increment_to_instruction_pointer;
    label_list__insert(&context->label_list, label_instruction_pointer);
    
    uint32_t label_unique_id;
    if (!label_list__get_unique_id(&context->label_list, label_instruction_pointer, &label_unique_id)) {
        context->error_handler("Couldn't find just inserted label by its instruction pointer", FILE_READER_ERROR_FATAL);
    }

    instruction__push(
        &instruction,
        "%s_%u",
        "label",
        label_unique_id
    );

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__jno(byte first_byte, struct opcode_context* context) {

    // Jump on not overflow
    // 0111 0001    (IP-INC8)

    (void)first_byte;

    struct instruction  instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "jno ");

    signed_byte  signed_increment_to_instruction_pointer = 0;
    file_reader__read_byte(&context->file_reader, &signed_increment_to_instruction_pointer, context->error_handler);

    uint32_t label_instruction_pointer = file_reader__read_bytes_so_far(&context->file_reader) + signed_increment_to_instruction_pointer;
    label_list__insert(&context->label_list, label_instruction_pointer);
    
    uint32_t label_unique_id;
    if (!label_list__get_unique_id(&context->label_list, label_instruction_pointer, &label_unique_id)) {
        context->error_handler("Couldn't find just inserted label by its instruction pointer", FILE_READER_ERROR_FATAL);
    }

    instruction__push(
        &instruction,
        "%s_%u",
        "label",
        label_unique_id
    );

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__jns(byte first_byte, struct opcode_context* context) {

    // Jump on not sign
    // 0111 1001    (IP-INC8)

    (void)first_byte;

    struct instruction  instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "jns ");

    signed_byte  signed_increment_to_instruction_pointer = 0;
    file_reader__read_byte(&context->file_reader, &signed_increment_to_instruction_pointer, context->error_handler);

    uint32_t label_instruction_pointer = file_reader__read_bytes_so_far(&context->file_reader) + signed_increment_to_instruction_pointer;
    label_list__insert(&context->label_list, label_instruction_pointer);
    
    uint32_t label_unique_id;
    if (!label_list__get_unique_id(&context->label_list, label_instruction_pointer, &label_unique_id)) {
        context->error_handler("Couldn't find just inserted label by its instruction pointer", FILE_READER_ERROR_FATAL);
    }

    instruction__push(
        &instruction,
        "%s_%u",
        "label",
        label_unique_id
    );

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__loop(byte first_byte, struct opcode_context* context) {

    // Loop CX times
    // 1110 0010    (IP-INC8)

    (void)first_byte;

    struct instruction  instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "loop ");

    signed_byte  signed_increment_to_instruction_pointer = 0;
    file_reader__read_byte(&context->file_reader, &signed_increment_to_instruction_pointer, context->error_handler);

    uint32_t label_instruction_pointer = file_reader__read_bytes_so_far(&context->file_reader) + signed_increment_to_instruction_pointer;
    label_list__insert(&context->label_list, label_instruction_pointer);
    
    uint32_t label_unique_id;
    if (!label_list__get_unique_id(&context->label_list, label_instruction_pointer, &label_unique_id)) {
        context->error_handler("Couldn't find just inserted label by its instruction pointer", FILE_READER_ERROR_FATAL);
    }

    instruction__push(
        &instruction,
        "%s_%u",
        "label",
        label_unique_id
    );

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__loopz_loope(byte first_byte, struct opcode_context* context) {

    // Loop while zero/equal
    // 1110 0001    (IP-INC8)

    (void)first_byte;

    struct instruction  instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "loopz ");

    signed_byte  signed_increment_to_instruction_pointer = 0;
    file_reader__read_byte(&context->file_reader, &signed_increment_to_instruction_pointer, context->error_handler);

    uint32_t label_instruction_pointer = file_reader__read_bytes_so_far(&context->file_reader) + signed_increment_to_instruction_pointer;
    label_list__insert(&context->label_list, label_instruction_pointer);
    
    uint32_t label_unique_id;
    if (!label_list__get_unique_id(&context->label_list, label_instruction_pointer, &label_unique_id)) {
        context->error_handler("Couldn't find just inserted label by its instruction pointer", FILE_READER_ERROR_FATAL);
    }

    instruction__push(
        &instruction,
        "%s_%u",
        "label",
        label_unique_id
    );

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__loopnz_loopne(byte first_byte, struct opcode_context* context) {

    // Loop while not zero/equal
    // 1110 0000    (IP-INC8)

    (void)first_byte;

    struct instruction  instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "loopnz ");

    signed_byte  signed_increment_to_instruction_pointer = 0;
    file_reader__read_byte(&context->file_reader, &signed_increment_to_instruction_pointer, context->error_handler);

    uint32_t label_instruction_pointer = file_reader__read_bytes_so_far(&context->file_reader) + signed_increment_to_instruction_pointer;
    label_list__insert(&context->label_list, label_instruction_pointer);
    
    uint32_t label_unique_id;
    if (!label_list__get_unique_id(&context->label_list, label_instruction_pointer, &label_unique_id)) {
        context->error_handler("Couldn't find just inserted label by its instruction pointer", FILE_READER_ERROR_FATAL);
    }

    instruction__push(
        &instruction,
        "%s_%u",
        "label",
        label_unique_id
    );

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__jcxz(byte first_byte, struct opcode_context* context) {

    // Jump on CX zero
    // 1110 0011    (IP-INC8)

    (void)first_byte;

    struct instruction  instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "jcxz ");

    signed_byte  signed_increment_to_instruction_pointer = 0;
    file_reader__read_byte(&context->file_reader, &signed_increment_to_instruction_pointer, context->error_handler);

    uint32_t label_instruction_pointer = file_reader__read_bytes_so_far(&context->file_reader) + signed_increment_to_instruction_pointer;
    label_list__insert(&context->label_list, label_instruction_pointer);
    
    uint32_t label_unique_id;
    if (!label_list__get_unique_id(&context->label_list, label_instruction_pointer, &label_unique_id)) {
        context->error_handler("Couldn't find just inserted label by its instruction pointer", FILE_READER_ERROR_FATAL);
    }

    instruction__push(
        &instruction,
        "%s_%u",
        "label",
        label_unique_id
    );

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__int_1100_1101(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__int_1100_1100(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__into(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__iret(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__clc(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__cmc(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__stc(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__cld(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__std(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__cli(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__sti(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__hlt(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__wait(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__esc(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__lock(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__segment(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__null(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    context->error_handler("in 'opcode__null': not used instruction", FILE_READER_ERROR_FATAL);
}

void opcode__cont_null(byte first_byte, byte second_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)second_byte;
    (void)context;
    context->error_handler("in 'opcode__null': not used instruction", FILE_READER_ERROR_FATAL);
}

void opcode__ss(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__cs(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__ds(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__es(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__nop(byte first_byte, struct opcode_context* context) {

    // does nothing, for example: xchg ax, ax

    (void)first_byte;

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "nop");

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__1000_000x(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
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
    file_reader__read_byte(&context->file_reader, &second_byte, context->error_handler);

    byte reg = (second_byte >> 3) & 0b111;
    opcode_handlers[reg](first_byte, second_byte, context);
}

void opcode__1000_001x(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
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
    file_reader__read_byte(&context->file_reader, &second_byte, context->error_handler);

    byte reg = (second_byte >> 3) & 0b111;
    opcode_handlers[reg](first_byte, second_byte, context);
}

void opcode__1101_00xx(byte first_byte, struct opcode_context* context) {
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
    file_reader__read_byte(&context->file_reader, &second_byte, context->error_handler);

    byte reg = (second_byte >> 3) & 0b111;
    opcode_handler[reg](first_byte, second_byte, context);
}

void opcode__repne_repnz(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__rep_repe_repz(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "todo: implement");
}

void opcode__1111_011x(byte first_byte, struct opcode_context* context) {
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
    file_reader__read_byte(&context->file_reader, &second_byte, context->error_handler);

    byte reg = (second_byte >> 3) & 0b111;
    opcode_handler[reg](first_byte, second_byte, context);
}

void opcode__1111_1110(byte first_byte, struct opcode_context* context) {
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
    file_reader__read_byte(&context->file_reader, &second_byte, context->error_handler);

    byte reg = (second_byte >> 3) & 0b111;
    opcode_handler[reg](first_byte, second_byte, context);
}

void opcode__1111_1111(byte first_byte, struct opcode_context* context) {
    
    // 1111 1111    mod reg r_m

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
    file_reader__read_byte(&context->file_reader, &second_byte, context->error_handler);

    byte reg = (second_byte >> 3) & 0b111;
    opcode_handler[reg](first_byte, second_byte, context);
}
