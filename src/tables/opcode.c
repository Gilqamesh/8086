#include "opcode.h"
#include "mod.h"

void opcode__mov_1000_10xx(byte first_byte, struct opcode_context* context) {

    // Move
    // Register/memory to/from register
    // 100010 d w    mod reg r/m    (DISP-LO)    (DISP-HI)

    /*
        MOV transfers a byte or a word from the source
        operand to the destination operand.
    */

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

    // Move
    // Immediate to register/memory
    // 1100011 w    mod 000 r/m    (DISP-LO)    (DISP-HI)    data    data if w = 1

    /*
        MOV transfers a byte or a word from the source
        operand to the destination operand.
    */

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

    // Move
    // Immediate to register
    // 1011 w reg    data    data if w = 1

    /*
        MOV transfers a byte or a word from the source
        operand to the destination operand.
    */

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

    // Move
    // Memory to accumulator
    // 1010000 w    addr-lo    addr-hi

    /*
        MOV transfers a byte or a word from the source
        operand to the destination operand.
    */

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

    // Move
    // Accumulator to memory
    // 1010001 w    addr-lo    addr-hi

    /*
        MOV transfers a byte or a word from the source
        operand to the destination operand.
    */

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

    // Move
    // Register/memory to segment register
    // 10001110    mod 0 SR r/m    (DISP-LO)    (DISP-HI)

    /*
        MOV transfers a byte or a word from the source
        operand to the destination operand.
    */

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "mov ");

    (void)first_byte;
    (void)context;
    assert(false && "TODO(david): implement how to dispatch 0SR");

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__mov_1000_1100(byte first_byte, struct opcode_context* context) {

    // Move
    // Segment register to register/memory
    // 10001100    mod 0 SR r/m    (DISP-LO)    (DISP-HI)

    /*
        MOV transfers a byte or a word from the source
        operand to the destination operand.
    */

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

    /*
        PUSH decrements SP (the stack pointer) by two
        and then transfers a word from the source
        operand to the top of stack now pointed to by SP.
        PUSH often is used to place parameters on the
        stack before calling a procedure; more generally,
        it is the basic means of storing temporary data on
        the stack. 
    */

    (void)first_byte;

    struct instruction instruction;
    
    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "push ");

    int mod = (second_byte >> 6) & 0b11;
    int r_m = second_byte & 0b111;

    mod_source_only_handlers[mod](1, r_m, context, &instruction);

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__push_0101_0xxx(byte first_byte, struct opcode_context* context) {

    // Register
    // 0101 0reg

    /*
        PUSH decrements SP (the stack pointer) by two
        and then transfers a word from the source
        operand to the top of stack now pointed to by SP.
        PUSH often is used to place parameters on the
        stack before calling a procedure; more generally,
        it is the basic means of storing temporary data on
        the stack. 
    */

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

    /*
        PUSH decrements SP (the stack pointer) by two
        and then transfers a word from the source
        operand to the top of stack now pointed to by SP.
        PUSH often is used to place parameters on the
        stack before calling a procedure; more generally,
        it is the basic means of storing temporary data on
        the stack. 
    */

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

    /*
        POP transfers the word at the current top of stack
        (pointed to by SP) to the destination operand,
        and then increments SP by two to point to the
        new top of stack. POP can be used to move temporary variables from the stack to registers or
        memory. 
    */

    (void)first_byte;

    struct instruction instruction;
    
    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "pop ");

    byte second_byte;
    file_reader__read_byte(&context->file_reader, &second_byte, context->error_handler);

    int mod = (second_byte >> 6) & 0b11;
    int r_m = second_byte & 0b111;

    mod_source_only_handlers[mod](1, r_m, context, &instruction);

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__pop_0101_1xxx(byte first_byte, struct opcode_context* context) {

    // Register
    // 0101 1reg

    /*
        POP transfers the word at the current top of stack
        (pointed to by SP) to the destination operand,
        and then increments SP by two to point to the
        new top of stack. POP can be used to move temporary variables from the stack to registers or
        memory. 
    */

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

    /*
        POP transfers the word at the current top of stack
        (pointed to by SP) to the destination operand,
        and then increments SP by two to point to the
        new top of stack. POP can be used to move temporary variables from the stack to registers or
        memory. 
    */

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
    
    // Reg/memory with register to either
    // 0000 00dw    mod reg r/m    (DISP-LO)    (DISP_HI)

    /*
        The sum of the two operands, which may be bytes
        or words, replaces the destination operand. Both
        operands may be signed or unsigned binary
        numbers (see AAA and DAA). ADD updates AF,
        CF, OF, PF, SF and ZF. 
    */

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
    
    // Immediate to register/memory
    // 1000 00sw    mod 000 r_m    (DISP-LO)    (DISP-HI)    data    data if sw = 01

    /*
        The sum of the two operands, which may be bytes
        or words, replaces the destination operand. Both
        operands may be signed or unsigned binary
        numbers (see AAA and DAA). ADD updates AF,
        CF, OF, PF, SF and ZF. 
    */

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

    // Immediate to accumulator
    // 0000 010w    data    data if w = 1

    /*
        The sum of the two operands, which may be bytes
        or words, replaces the destination operand. Both
        operands may be signed or unsigned binary
        numbers (see AAA and DAA). ADD updates AF,
        CF, OF, PF, SF and ZF. 
    */

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

    // Add with carry
    // Reg/memory with register to either
    // 0001 00dw    mod reg r/m    (DISP-LO)    (DISP-HI)

    /*
        ADC (Add with Carry) sums the operands, which
        may be bytes or words, adds one if CF is set and
        replaces the destination operand with the result.
        Both operands may be signed or unsigned binary
        numbers (see AAA and DAA). ADC updates AF,
        CF, OF, PF, SF and ZF. Since ADC incorporates
        a carry from a previous operation, it can be used
        to write routines to add numbers longer than 16
        bits. 
    */

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "adc ");

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

void opcode__cont_adc_1000_00xx(byte first_byte, byte second_byte, struct opcode_context* context) {

    // Add with carry
    // Immediate to register/memory
    // 1000 00sw    mod 010 r/m    (DISP-LO)    (DISP-HI)    data    data if sw = 01

    /*
        ADC (Add with Carry) sums the operands, which
        may be bytes or words, adds one if CF is set and
        replaces the destination operand with the result.
        Both operands may be signed or unsigned binary
        numbers (see AAA and DAA). ADC updates AF,
        CF, OF, PF, SF and ZF. Since ADC incorporates
        a carry from a previous operation, it can be used
        to write routines to add numbers longer than 16
        bits. 
    */

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "adc ");

    int s = (first_byte >> 1) & 0b1;
    int w = first_byte & 0b1;
    int mod = (second_byte >> 6) & 0b11;
    int r_m = second_byte & 0b111;

    mod_immediate_signed_handlers[mod](s, w, r_m, context, &instruction);

    instruction_list__push(&context->instruction_list, instruction);
}

void opcode__adc_0001_010x(byte first_byte, struct opcode_context* context) {

    // Add with carry
    // Immediate to accumulator
    // 0001 010w    data    data if w = 1

    /*
        ADC (Add with Carry) sums the operands, which
        may be bytes or words, adds one if CF is set and
        replaces the destination operand with the result.
        Both operands may be signed or unsigned binary
        numbers (see AAA and DAA). ADC updates AF,
        CF, OF, PF, SF and ZF. Since ADC incorporates
        a carry from a previous operation, it can be used
        to write routines to add numbers longer than 16
        bits. 
    */

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "adc ");

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

void opcode__cont_inc_1111_111x(byte first_byte, byte second_byte, struct opcode_context* context) {

    // Increment
    // Register/memory
    // 1111 111w    mod 000 r/m    (DISP-LO)    (DISP-HI)

    /*
        INC (Increment) adds one to the destination
        operand. The operand may be a byte or a word
        and is treated as an unsigned binary number (see
        AAA and DAA). INC updates AF, OF, PF, SF
        and ZF; it does not affect CF. 
    */

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "inc ");

    int w   = first_byte & 0b1;
    int mod = (second_byte >> 6) & 0b11;
    int r_m = second_byte & 0b111;

    mod_source_only_handlers[mod](w, r_m, context, &instruction);

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__inc_0100_0xxx(byte first_byte, struct opcode_context* context) {

    // Increment
    // Register
    // 0100 0reg

    /*
        INC (Increment) adds one to the destination
        operand. The operand may be a byte or a word
        and is treated as an unsigned binary number (see
        AAA and DAA). INC updates AF, OF, PF, SF
        and ZF; it does not affect CF. 
    */

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "inc ");

    int reg = first_byte & 0b111;

    instruction__push(
        &instruction,
        "%s",
        reg_to_word(reg, 1)
    );

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__aaa(byte first_byte, struct opcode_context* context) {

    // ASCII adjust for add
    // 0011 0111

    /*
        AAA (ASCII Adjust for Addition) changes the
        contents of register AL to a valid unpacked
        decimal number; the high-order half-byte is
        zeroed. AAA updates AF and CF; the content of
        OF, PF, SF and ZF is undefined following execution of AAA. 
    */

    (void)first_byte;

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "aaa");

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__daa(byte first_byte, struct opcode_context* context) {

    // Decimal adjust for add
    // 0010 0111

    /*
        DAA (Decimal Adjust for Addition) corrects the
        result of previously adding two valid packed
        decimal operands (the destination operand must
        have been register AL). DAA changes the content
        of AL to a pair of valid packed decimal digits. It
        updates AF, CF, PF, SF and ZF; the content of
        OF is undefined following execution of DAA. 
    */

    (void)first_byte;

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "daa");

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__sub_0010_10xx(byte first_byte, struct opcode_context* context) {

    // Reg/Memory and register to either
    // 0010 10dw    mod reg r/m    (DISP-LO)    (DISP-HI)

    /*
        The source operand is subtracted from the
        destination operand, and the result replaces the
        destination operand. The operands may be bytes
        or words. Both operands may be signed or
        unsigned binary numbers (see AAS and DAS).
        SUB updates AF, CF, OF, PF, SF and ZF. 
    */

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

    /*
        The source operand is subtracted from the
        destination operand, and the result replaces the
        destination operand. The operands may be bytes
        or words. Both operands may be signed or
        unsigned binary numbers (see AAS and DAS).
        SUB updates AF, CF, OF, PF, SF and ZF. 
    */

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

    /*
        The source operand is subtracted from the
        destination operand, and the result replaces the
        destination operand. The operands may be bytes
        or words. Both operands may be signed or
        unsigned binary numbers (see AAS and DAS).
        SUB updates AF, CF, OF, PF, SF and ZF. 
    */

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

    // Subtract with borrow
    // Reg/memory and register to either
    // 0001 10dw    mod reg r/m    (DISP-LO)    (DISP-HI)

    /*
        SBB (Subtract with Borrow) subtracts the source
        from the destination, subtracts one if CF is set,
        and returns the result to the destination operand.
        Both operands may be bytes or words. Both
        operands may be signed or unsigned binary
        numbers (see AAS and DAS). SBB updates AF,
        CF, OF, PF, SF and ZF. Since it incorporates a
        borrow from a previous operation, SBB may be
        used to write routines that subtract numbers
        longer than 16 bits. 
    */

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "sbb ");

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

void opcode__cont_sbb_1000_00xx(byte first_byte, byte second_byte, struct opcode_context* context) {

    // Immediate from register/memory
    // 1000 00sw    mod 011 r/m    (DISP-LO)    (DISP-HI)    data    data if sw = 01

    /*
        SBB (Subtract with Borrow) subtracts the source
        from the destination, subtracts one if CF is set,
        and returns the result to the destination operand.
        Both operands may be bytes or words. Both
        operands may be signed or unsigned binary
        numbers (see AAS and DAS). SBB updates AF,
        CF, OF, PF, SF and ZF. Since it incorporates a
        borrow from a previous operation, SBB may be
        used to write routines that subtract numbers
        longer than 16 bits. 
    */

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "sbb ");

    int s   = (first_byte >> 1) & 0b1;
    int w   = first_byte & 0b1;
    int mod = (second_byte >> 6) & 0b11;
    int r_m = second_byte & 0b111;

    mod_immediate_signed_handlers[mod](s, w, r_m, context, &instruction);

    instruction_list__push(&context->instruction_list, instruction);
}

void opcode__sbb_0001_110x(byte first_byte, struct opcode_context* context) {

    // Immediate from accumulator
    // 0011 110w    data    data if w = 1

    /*
        SBB (Subtract with Borrow) subtracts the source
        from the destination, subtracts one if CF is set,
        and returns the result to the destination operand.
        Both operands may be bytes or words. Both
        operands may be signed or unsigned binary
        numbers (see AAS and DAS). SBB updates AF,
        CF, OF, PF, SF and ZF. Since it incorporates a
        borrow from a previous operation, SBB may be
        used to write routines that subtract numbers
        longer than 16 bits. 
    */

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "sbb ");

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

void opcode__cont_dec_1111_111x(byte first_byte, byte second_byte, struct opcode_context* context) {

    // Decrement
    // Register/memory
    // 1111 111w    mod 001 r/m    (DISP-LO)    (DISP-HI)

    /*
        DEC (Decrement) subtracts one from the destination, which may be a byte or a word. DEC
        updates AF, OF, PF, SF, and ZF; it does not affect CF. 
    */

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "dec ");

    int w   = first_byte & 0b1;
    int mod = (second_byte >> 6) & 0b11;
    int r_m = second_byte & 0b111;

    mod_source_only_handlers[mod](w, r_m, context, &instruction);

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__dec_0100_1xxx(byte first_byte, struct opcode_context* context) {

    // Increment
    // Register
    // 0100 1reg

    /*
        DEC (Decrement) subtracts one from the destination, which may be a byte or a word. DEC
        updates AF, OF, PF, SF, and ZF; it does not affect CF. 
    */

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "dec ");

    int reg = first_byte & 0b111;

    instruction__push(
        &instruction,
        "%s",
        reg_to_word(reg, 1)
    );

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__cont_neg(byte first_byte, byte second_byte, struct opcode_context* context) {

    // Change sign
    // 1111 011w    mod 011 r/m    (DISP-LO)    (DISP-HI)

    /*
        NEG (Negate) subtracts the destination operand,
        which may be a byte or a word, from 0 and
        returns the result to the destination. This forms
        the two's complement of the number, effectively
        reversing the sign of an integer. If the operand is
        zero, its sign is not changed. Attempting to negate
        a byte containing -128 or a word containing
        -32,768 causes no change to the operand and sets
        OF. NEG updates AF, CF, OF, PF, SF and ZF.
        CF is always set except when the operand is zero,
        in which case it is cleared. 
    */

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "neg ");

    int w   = first_byte & 0b1;
    int mod = (second_byte >> 6) & 0b11;
    int r_m = second_byte & 0b111;

    mod_source_only_handlers[mod](w, r_m, context, &instruction);

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__cmp_0011_10xx(byte first_byte, struct opcode_context* context) {

    // Register/memory and register
    // 0011 10dw    mod reg r/m    (DISP-LO)    (DISP-HI)

    /*
        CMP (Compare) subtracts the source from the
        destination, which may be bytes or words, but
        does not return the result. The operands are
        unchanged, but the flags are updated and can be
        tested by a subsequent conditional jump instruction. CMP updates AF, CF, OF, PF, SF and ZF.
        The comparison reflected in the flags is that of the
        destination to the source. If a CMP instruction is
        followed by a 1G (jump if greater) instruction, for
        example, the jump is taken if the destination
        operand is greater than the source operand. 
    */

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

    /*
        CMP (Compare) subtracts the source from the
        destination, which may be bytes or words, but
        does not return the result. The operands are
        unchanged, but the flags are updated and can be
        tested by a subsequent conditional jump instruction. CMP updates AF, CF, OF, PF, SF and ZF.
        The comparison reflected in the flags is that of the
        destination to the source. If a CMP instruction is
        followed by a 1G (jump if greater) instruction, for
        example, the jump is taken if the destination
        operand is greater than the source operand. 
    */

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

    /*
        CMP (Compare) subtracts the source from the
        destination, which may be bytes or words, but
        does not return the result. The operands are
        unchanged, but the flags are updated and can be
        tested by a subsequent conditional jump instruction. CMP updates AF, CF, OF, PF, SF and ZF.
        The comparison reflected in the flags is that of the
        destination to the source. If a CMP instruction is
        followed by a 1G (jump if greater) instruction, for
        example, the jump is taken if the destination
        operand is greater than the source operand. 
    */

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

    // ASCII adjust for subtract
    // 0011 1111

    /*
        AAS (ASCII Adjust for Subtraction) corrects the
        result of a previous subtraction of two valid
        unpacked decimal operands (the destination
        operand must have been specified as register AL).
        AAS changes the content of AL to a valid
        unpacked decimal number; the high-order halfbyte is zeroed.
        AAS updates AF and CF; the content of OF, PF, SF and ZF is undefined following
        execution of AAS. 
    */

    (void)first_byte;

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "aas");

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__das(byte first_byte, struct opcode_context* context) {

    // Decimal adjust for subtract
    // 0010 1111

    /*
        DAS (Decimal Adjust for Subtraction) corrects
        the result of a previous subtraction of two valid
        packed decimal operands (the destination
        operand must have been specified as register AL).
        DAS changes the content of AL to a pair of valid
        packed decimal digits. DAS updates AF, CF, PF,
        SF and ZF; the content of OF is undefined
        following execution of DAS. 
    */

    (void)first_byte;

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "das");

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__cont_mul(byte first_byte, byte second_byte, struct opcode_context* context) {

    // Multiply (unsigned)
    // 1111 011w    mod 100 r/m    (DISP-LO)    (DISP-HI)

    /*
        MUL (Multiply) performs an unsigned multiplication of the source operand and the
        accumulator. If the source is a byte, then it is
        multiplied by register AL, and the double-length
        result is returned in AH and AL. If the source
        operand is a word, then it is multiplied by register
        AX, and the double-length result is returned in
        registers DX and AX. The operands are treated as
        unsigned binary numbers (see AAM). If the upper
        half of the result (AH for byte source, DX for
        word source) is nonzero, CF and OF are set;
        otherwise they are cleared. When CF and OF are
        set, they indicate that AH or DX contains significant digits of the result. The content of AF, PF,
        SF and ZF is undefined following execution of
        MUL. 
    */

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "mul ");

    int w   = first_byte & 0b1;
    int mod = (second_byte >> 6) & 0b11;
    int r_m = second_byte & 0b111;

    mod_source_only_handlers[mod](w, r_m, context, &instruction);

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__cont_imul(byte first_byte, byte second_byte, struct opcode_context* context) {

    // Integer multiply (signed)
    // 1111 011w    mod 101 r/m    (DISP-LO)    (DISP-HI)

    /*
        IMUL (Integer Multiply) performs a signed
        mUltiplication of the source operand and the
        accumulator. If the source is a byte, then it is
        mUltiplied by register AL, and the double-length
        result is returned in AH and AL. If the source is a
        word, then it is multiplied by register AX, and the
        double-length result is returned in registers DX
        and AX. If the upper half of the result (AH for
        byte source, DX for word source) is not the sign
        extension of the lower half of the result, CF and
        OF are set; otherwise they are cleared. When CF
        and OF are set, they indicate that AH or DX contains significant digits of the result. The content
        of AF, PF, SF and ZF is undefined following
        execution of IMUL. 
    */

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "imul ");

    int w   = first_byte & 0b1;
    int mod = (second_byte >> 6) & 0b11;
    int r_m = second_byte & 0b111;

    mod_source_only_handlers[mod](w, r_m, context, &instruction);

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__aam(byte first_byte, struct opcode_context* context) {

    // ASCII adjust for multiply
    // 1101 0100    0000 1010    (DISP-LO)    (DISP-HI)

    /*
        AAM (ASCII Adjust for Multiply) corrects the
        result of a previous multiplication of two valid
        unpacked decimal operands. A valid 2-digit
        unpacked decimal number is derived from the
        content of AH and AL and is returned to AH and
        AL. The high-order half-bytes of the multiplied
        operands must have been OH for AAM to produce a correct result. AAM updates PF, SF and
        ZF; the content of AF, CF and OF is undefined
        following execution of AAM. 
    */

    (void)first_byte;

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "aam ");

    byte second_byte;
    file_reader__read_byte(&context->file_reader, &second_byte, context->error_handler);

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__cont_div(byte first_byte, byte second_byte, struct opcode_context* context) {

    // Divide (unsigned)
    // 1111 011w    mod 110 r/m    (DISP-LO)    (DISP-HI)

    /*
        DIV (divide) performs an unsigned division of the
        accumulator (and its extension) by the source
        operand. If the source operand is a byte, it is
        divided into the double-length dividend assumed
        to be in registers AL and AH. The single-length
        quotient is returned in AL, and the single-length
        remainder is returned in AH. If the source
        operand is a word, it is divided into the doublelength dividend in registers AX and DX. The
        single-length quotient is returned in AX, and the
        single-length remainder is returned in DX. If the
        quotient exceeds the capacity of its destination
        register (FFH for byte source, FFFFFH for word
        source), as when division by zero is attempted, a
        type 0 interrupt is generated, and the quotient and
        remainder are undefined. Nonintegral quotients
        are truncated to integers. The content of AF, CF,
        OF, PF, SF and ZF is undefined following execution of DIV. 
    */

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "div ");

    int w   = first_byte & 0b1;
    int mod = (second_byte >> 6) & 0b11;
    int r_m = second_byte & 0b111;

    mod_source_only_handlers[mod](w, r_m, context, &instruction);

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__cont_idiv(byte first_byte, byte second_byte, struct opcode_context* context) {

    // Integer divide (signed)
    // 1111 011w    mod 111 r/m    (DISP-LO)    (DISP-HI)

    /*
        IDIV (Integer Divide) performs a signed division
        of the accumulator (and its extension) by the
        source operand. If the source operand is a byte, it
        is divided into the double-length dividend
        assumed to be in registers AL and AH; the singlelength quotient is returned in AL, and the singlelength remainder is returned in AH. For byte integer division, the maximum positive quotient is
        +127 (7FH) and the minimum negative quotient is
        -127 (SIH). If the source operand is a word, it is
        divided into the double-length dividend in
        registers AX and DX; the single-length quotient is
        returned in AX, and the single-length remainder
        is returned in DX. For word integer division, the
        maximum positive quotient is +32,767 (7FFFH)
        and the minimum negative quotient is -32,767
        (SOOIH). If the quotient is positive and exceeds
        the maximum, or is negative and is less than the
        minimum, the quotient and remainder are
        undefined, and a type 0 interrupt is generated. In
        particular, this occurs if division by 0 is
        attempted. Nonintegral quotients are truncated
        (toward 0) to integers, and the remainder has the
        same sign as the dividend. The content of AF,
        CF, OF, PF, SF and ZF is undefined following
        IDIV. 
    */

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "idiv ");

    int w   = first_byte & 0b1;
    int mod = (second_byte >> 6) & 0b11;
    int r_m = second_byte & 0b111;

    mod_source_only_handlers[mod](w, r_m, context, &instruction);

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__aad(byte first_byte, struct opcode_context* context) {

    // ASCII adjust for divide
    // 1101 0101    0000 1010    (DISP-LO)    (DISP-HI)

    /*
        AAD (ASCII Adjust for Division) modifies the
        numerator in AL before dividing two valid
        unpacked decimal operands so that the quotient
        produced by the division will be a valid unpacked
        decimal number. AH must be zero for the subsequent
        DIV to produce the correct result. The quotient is returned in AL, and the remainder is
        returned in AH; both high-order half-bytes are
        zeroed. AAD updates PF, SF and ZF; the content
        of AF, CF and OF is undefined following execution of AAD. 
    */

    (void)first_byte;

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "aad");

    byte second_byte;
    file_reader__read_byte(&context->file_reader, &second_byte, context->error_handler);

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__cbw(byte first_byte, struct opcode_context* context) {

    // Convert byte to word
    // 1001 1000

    /*
        CBW (Convert Byte to Word) extends the sign of
        the byte in register AL throughout register AH.
        CBW does not affect any flags. CBW can be used
        to produce a double-length (word) dividend from
        a byte prior to performing byte division. 
    */

    (void)first_byte;

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "cbw");

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__cwd(byte first_byte, struct opcode_context* context) {

    // Convert word to double word
    // 1001 1001

    /*
        CWD (Convert Word to Doubleword) extends the
        sign of the word in register AX throughout
        register DX. CWD does not affect any flags.
        CWD can be used to produce a double-length
        (doubleword) dividend from a word prior to performing word division. 
    */

    (void)first_byte;

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "cwd");

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__cont_not(byte first_byte, byte second_byte, struct opcode_context* context) {

    // Invert
    // 1111 011w    mod 010 r/m    (DISP-LO)    (DISP-HI)

    /*
        NOT inverts the bits (forms the one's complement) of the byte or word operand. 
    */

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "not ");

    int w   = first_byte & 0b1;
    int mod = (second_byte >> 6) & 0b11;
    int r_m = second_byte & 0b111;

    mod_source_only_handlers[mod](w, r_m, context, &instruction);

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__cont_shl_sal(byte first_byte, byte second_byte, struct opcode_context* context) {

    // Shift logical/airthmetic left
    // 1101 00vw    mod 100 r/m    (DISP-LO)    (DISP-HI)

    /*
        SHL and SAL (Shift Logical Left and Shift
        Arithmetic Left) perform the same operation and
        are physically the same instruction. The destination byte or word is shifted left by the number of
        bits specified in the count operand. Zeros are
        shifted in on the right. If the sign bit retains its
        original value, then OF is cleared. 
    */

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "shl ");

    int v   = (first_byte >> 1) & 0b1;
    int w   = first_byte & 0b1;
    int mod = (second_byte >> 6) & 0b11;
    int r_m = second_byte & 0b111;

    mod_source_only_handlers[mod](w, r_m, context, &instruction);

    instruction__push(
        &instruction,
        ", %s",
        v ? "cl" : "1"
    );

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__cont_shr(byte first_byte, byte second_byte, struct opcode_context* context) {

    // Shift logical right
    // 1101 00vw    mod 101 r/m    (DISP-LO)    (DISP-HI)

    /*
        SHR (Shift Logical Right) shifts the bits in the
        destination operand (byte or word) to the right by
        the number of bits specified in the count operand.
        Zeros are shifted in on the left. If the sign bit
        retains its original value, then OF is cleared.
    */

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "shr ");

    int v   = (first_byte >> 1) & 0b1;
    int w   = first_byte & 0b1;
    int mod = (second_byte >> 6) & 0b11;
    int r_m = second_byte & 0b111;

    mod_source_only_handlers[mod](w, r_m, context, &instruction);

    instruction__push(
        &instruction,
        ", %s",
        v ? "cl" : "1"
    );

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__cont_sar(byte first_byte, byte second_byte, struct opcode_context* context) {

    // Shift arithmetic right
    // 1101 00vw    mod 111 r/m    (DISP-LO)    (DISP-HI)

    /*
        SAR (Shift Arithmetic Right) shifts the bits in the
        destination operand (byte or word) to the right by
        the number of bits specified in the count operand.
        Bits equal to the original high-order (sign) bit are
        shifted in on the left, preserving the sign of the
        original value. Note that SAR does not produce
        the same result as the dividend of an
        "equivalent" IDIV instruction if the destination
        operand is negative and I-bits are shifted out. For
        example, shifting -5 right by one bit yields -3,
        while integer division of -5 by 2 yields -2. The
        difference in the instructions is that IDIV truncates all
        numbers toward zero, while SAR truncates positive numbers toward zero and negative
        numbers toward negative infinity. 
    */

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "sar ");

    int v   = (first_byte >> 1) & 0b1;
    int w   = first_byte & 0b1;
    int mod = (second_byte >> 6) & 0b11;
    int r_m = second_byte & 0b111;

    mod_source_only_handlers[mod](w, r_m, context, &instruction);

    instruction__push(
        &instruction,
        ", %s",
        v ? "cl" : "1"
    );

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__cont_rol(byte first_byte, byte second_byte, struct opcode_context* context) {

    // Rotate left
    // 1101 00vw    mod 000 r/m    (DISP-LO)    (DISP-HI)

    /*
        ROL (Rotate Left) rotates the destination byte or
        word left by the number of bits specified in the
        count operand. 
    */

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "rol ");

    int v   = (first_byte >> 1) & 0b1;
    int w   = first_byte & 0b1;
    int mod = (second_byte >> 6) & 0b11;
    int r_m = second_byte & 0b111;

    mod_source_only_handlers[mod](w, r_m, context, &instruction);

    instruction__push(
        &instruction,
        ", %s",
        v ? "cl" : "1"
    );

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__cont_ror(byte first_byte, byte second_byte, struct opcode_context* context) {

    // Rotate right
    // 1101 00vw    mod 001 r/m    (DISP-LO)    (DISP-HI)

    /*
        ROR (Rotate Right) operates similar to ROL
        except that the bits in the destination byte or word
        are rotated right instead of left. 
    */

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "ror ");

    int v   = (first_byte >> 1) & 0b1;
    int w   = first_byte & 0b1;
    int mod = (second_byte >> 6) & 0b11;
    int r_m = second_byte & 0b111;

    mod_source_only_handlers[mod](w, r_m, context, &instruction);

    instruction__push(
        &instruction,
        ", %s",
        v ? "cl" : "1"
    );

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__cont_rcl(byte first_byte, byte second_byte, struct opcode_context* context) {

    // Rotate through carry flag left
    // 1101 00vw    mod 010 r/m    (DISP-LO)    (DISP-HI)

    /*
        RCL (Rotate through Carry Left) rotates the bits
        in the byte or word destination operand to the left
        by the number of bits specified in the count
        operand. The carry flag (CF) is treated as "part
        of" the destination operand; that is, its value is
        rotated into the low-order bit of the destination,
        and itself is replaced by the high-order bit of the
        destination.
    */

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "rcl ");

    int v   = (first_byte >> 1) & 0b1;
    int w   = first_byte & 0b1;
    int mod = (second_byte >> 6) & 0b11;
    int r_m = second_byte & 0b111;

    mod_source_only_handlers[mod](w, r_m, context, &instruction);

    instruction__push(
        &instruction,
        ", %s",
        v ? "cl" : "1"
    );

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__cont_rcr(byte first_byte, byte second_byte, struct opcode_context* context) {

    // Rotate through carry flag right
    // 1101 00vw    mod 011 r/m    (DISP-LO)    (DISP-HI)

    /*
        RCR (Rotate through Carry Right) operates
        exactly like RCL except that the bits are rotated
        right instead of left. 
    */

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "rcr ");

    int v   = (first_byte >> 1) & 0b1;
    int w   = first_byte & 0b1;
    int mod = (second_byte >> 6) & 0b11;
    int r_m = second_byte & 0b111;

    mod_source_only_handlers[mod](w, r_m, context, &instruction);

    instruction__push(
        &instruction,
        ", %s",
        v ? "cl" : "1"
    );

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__and_0010_00xx(byte first_byte, struct opcode_context* context) {

    // Register/memory with register to either
    // 001000dw    mod reg r/m    (DISP-LO)    (DISP-HI)

    /*
        AND performs the logical "and" of the two
        operands (byte or word) and returns the result to
        the destination operand. A bit in the result is set if
        both corresponding bits of the original operands
        are set; otherwise the bit is cleared.
    */

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "and ");

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

void opcode__cont_and_1000_000x(byte first_byte, byte second_byte, struct opcode_context* context) {

    // Immediate to register/memory
    // 1000 000w    mod 100 r/m    (DISP-LO)    (DISP-HI)    data    data if w = 1

    /*
        AND performs the logical "and" of the two
        operands (byte or word) and returns the result to
        the destination operand. A bit in the result is set if
        both corresponding bits of the original operands
        are set; otherwise the bit is cleared.
    */

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "and ");

    int w   = first_byte & 0b1;
    int mod = (second_byte >> 6) & 0b11;
    int r_m = second_byte & 0b111;

    mod_immediate_handlers[mod](w, r_m, context, &instruction);

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__and_0010_010x(byte first_byte, struct opcode_context* context) {

    // Immediate data and accumulator
    // 0010 010w    data    data if w = 1

    /*
        AND performs the logical "and" of the two
        operands (byte or word) and returns the result to
        the destination operand. A bit in the result is set if
        both corresponding bits of the original operands
        are set; otherwise the bit is cleared.
    */

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "and ");

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

void opcode__test_0001_00xx(byte first_byte, struct opcode_context* context) {
    (void)first_byte;
    (void)context;
    assert(false && "// note(david): this case isn't in the corresponding table of the 8086 manual, so just ignoring it");
}

void opcode__test_1000_010x(byte first_byte, struct opcode_context* context) {

    // And function to flags no result
    // 1000 01dw    mod reg r/m    (DISP-LO)    (DISP-HI)

    /*
        TEST performs the logical "and" of the two
        operands (byte or word), updates the flags, but
        does not return the result, i.e., neither operand is
        changed. If a TEST instruction is followed by a
        JNZ (jump if not zero) instruction, the jump will
        be taken if there are any corresponding I-bits in
        both operands. 
    */
    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "test ");

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

void opcode__cont_test_1111_011x(byte first_byte, byte second_byte, struct opcode_context* context) {

    // And function to flags no result
    // Immediate data and register/memory
    // 1111 011w    mod 000 r/m    (DISP-LO)    (DISP-HI)    data    data if w = 1

    /*
        TEST performs the logical "and" of the two
        operands (byte or word), updates the flags, but
        does not return the result, i.e., neither operand is
        changed. If a TEST instruction is followed by a
        JNZ (jump if not zero) instruction, the jump will
        be taken if there are any corresponding I-bits in
        both operands. 
    */

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "test ");

    int w   = first_byte & 0b1;
    int mod = (second_byte >> 6) & 0b11;
    int r_m = second_byte & 0b111;

    mod_immediate_handlers[mod](w, r_m, context, &instruction);

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__test_1010_100x(byte first_byte, struct opcode_context* context) {

    // And function to flags no result
    // Immediate to accumulator
    // 1010 100w    data    data if w = 1

    /*
        TEST performs the logical "and" of the two
        operands (byte or word), updates the flags, but
        does not return the result, i.e., neither operand is
        changed. If a TEST instruction is followed by a
        JNZ (jump if not zero) instruction, the jump will
        be taken if there are any corresponding I-bits in
        both operands. 
    */

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "test ");

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

void opcode__or_0000_10xx(byte first_byte, struct opcode_context* context) {

    // Or
    // Reg/memory and register to either
    // 0000 10dw    mod reg r/m    (DISP-LO)    (DISP-HI)

    /*
        OR performs the logical "inclusive or" of the two
        operands (byte or word) and returns the result to
        the destination operand. A bit in the result is set if
        either or both corresponding bits in the original
        operands are set; otherwise the result bit is
        cleared. 
    */

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "or ");

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

void opcode__cont_or_1000_000x(byte first_byte, byte second_byte, struct opcode_context* context) {

    // Or
    // Immediate to register/memory
    // 1000 000w    mod 001 r/m    (DISP-LO)    (DISP-HI)    data    data if w = 1

    /*
        OR performs the logical "inclusive or" of the two
        operands (byte or word) and returns the result to
        the destination operand. A bit in the result is set if
        either or both corresponding bits in the original
        operands are set; otherwise the result bit is
        cleared. 
    */

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "or ");

    int w   = first_byte & 0b1;
    int mod = (second_byte >> 6) & 0b11;
    int r_m = second_byte & 0b111;

    mod_immediate_handlers[mod](w, r_m, context, &instruction);

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__or_0000_110x(byte first_byte, struct opcode_context* context) {

    // Or
    // Immediate to accumulator
    // 0000 110w    data    data if w = 1

    /*
        OR performs the logical "inclusive or" of the two
        operands (byte or word) and returns the result to
        the destination operand. A bit in the result is set if
        either or both corresponding bits in the original
        operands are set; otherwise the result bit is
        cleared. 
    */

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "or ");

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

void opcode__xor_0011_00xx(byte first_byte, struct opcode_context* context) {

    // Exclusive or
    // Reg/memory and register to either
    // 0011 00dw    mod reg r/m    (DISP-LO)    (DISP-HI)

    /*
        XOR (Exclusive Or) performs the logical "exclusive or" of the two operands and returns the
        result to the destination operand. A bit in the result is set if the corresponding bits of the
        original operands contain opposite values (one is set, the other is cleared);
        otherwise the result bit is cleared. 
    */

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "xor ");

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

void opcode__cont_xor_1000_000x(byte first_byte, byte second_byte, struct opcode_context* context) {

    // Or
    // Immediate to register/memory
    // 1000 000w    mod 110 r/m    (DISP-LO)    (DISP-HI)    data    data if w = 1

    /*
        XOR (Exclusive Or) performs the logical "exclusive or" of the two operands and returns the
        result to the destination operand. A bit in the result is set if the corresponding bits of the
        original operands contain opposite values (one is set, the other is cleared);
        otherwise the result bit is cleared. 
    */

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "xor ");

    int w   = first_byte & 0b1;
    int mod = (second_byte >> 6) & 0b11;
    int r_m = second_byte & 0b111;

    mod_immediate_handlers[mod](w, r_m, context, &instruction);

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__xor_0011_010x(byte first_byte, struct opcode_context* context) {

    // Immediate to accumulator
    // 0011 010w    data    data if w = 1

    /*
        XOR (Exclusive Or) performs the logical "exclusive or" of the two operands and returns the
        result to the destination operand. A bit in the result is set if the corresponding bits of the
        original operands contain opposite values (one is set, the other is cleared);
        otherwise the result bit is cleared. 
    */

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "xor ");

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

void opcode__rep(byte first_byte, struct opcode_context* context) {

    // Repeat, repeat while equal, repeat while zero, repeat while not equal and repeat while not zero
    // 1111 001z

    /*
        Repeat, Repeat While Equal, Repeat While Zero,
        Repeat While Not Equal and Repeat While Not
        Zero are five mnemonics for two forms of the
        prefix byte that controls repetition of a subsequent string instruction. The different mnemonics
        are provided to improve program clarity. The
        repeat prefixes do not affect the flags.
        REP is used in conjunction with the MOYS
        (Move String) and STOS (Store String) instructions and is interpreted as "repeat while not endof-string" (CX not 0). REPE and REPZ operate
        identically and are physically the same prefix byte
        as REP. These instructions are used with the
        CMPS (Compare String) and SCAS (Scan String)
        instructions and require ZF (posted by these
        instructions) to be set before initiating the next
        repetition. REPNE and REPNZ are two
        mnemonics for the same prefix byte. These
        instructions function the same as REPE and
        REPZ except that the zero flag must be cleared or
        the repetition is terminated. Note that ZF does
        not need to be initialized before executing the
        repeated string instruction.
        Repeated string sequences are interruptable; the
        processor will recognize the interrupt before processing the next string element. System interrupt
        processing is not affected in any way. Upon
        return from the interrupt, the repeated operation
        is resumed from the point of interruption. Note,
        however, that execution does not resume properly
        if a second or third prefix (i.e., segrnent override
        or LOCK) has been specified in addition to any of
        the repeat prefixes. The processor "remembers"
        only one prefix in effect at the time of the interrupt, the prefix that immediately precedes the
        string instruction. After returning from the interrupt, processing resumes at this point, but any
        additional prefixes specified are not in effect. If
        more than one prefix must be used with a string
        instruction, interrupts may be disabled for the
        duration of the repeated execution. However, this
        will not prevent a non-maskable interrupt from
        being recognized. Also, the time that the system is
        unable to respond to interrupts may be unacceptable if long strings are being processed. 
    */

    struct instruction instruction;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(&instruction, "rep");

    int z = first_byte & 0b1;
    (void)z;

    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__movs(byte first_byte, struct opcode_context* context) {

    // Move byte/word
    // 1010 010w

    /*
        MOVS (Move String) transfers a byte or a word
        from the source string (addressed by SI) to the
        destination string (addressed by DI) and updates
        SI and DI to point to the next string element.
        When used in conjunction with REP, MOYS performs a memory-to-memory block transfer.

        MOVSB/MOVSW
        These are alternate mnemonics for the move
        string instruction. These mnemonics are coded
        without operands; they explicitly tell the
        assembler that a byte string (MOYSB) or a word
        string. (MOYSW) is to be moved (when MOYS is
        coded, the assembler determines the string type
        from the attributes of the operands). These
        mnemonics are useful when the assembler cannot
        determine the attributes of a string, e.g., a section
        of code is being moved. 
    */

    struct instruction instruction;

    int w = first_byte & 0b1;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(
        &instruction,
        "movs%c",
        w ? 'w' : 'b'
    );


    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__cmps(byte first_byte, struct opcode_context* context) {

    // Compare byte/word
    // 1010 011w

    /*
        CMPS(Compare String) subtracts the destination
        byte or word (addressed by DI) from the source
        byte or word (addressed by SI). CMPS affects the
        flags but does not alter either operand, updates SI
        and DI to point to the next string element and
        updates AF, CF, OF, PF, SF and ZF to reflect the
        relationship of the destination element to the
        source element. For example, if a JG (Jump if
        Greater) instruction follows CMPS, the jump is
        taken if the destination element is greater than the
        source element. If CMPS is prefixed with REPE 
        or REPZ, the operation is interpreted as "compare while not end-of-string (CX not zero) and
        strings are equal (ZF = 1)." If CMPS is preceded
        by REPNE or REPNZ, the operation is interpreted as "compare while not end-of-string (CX
        not zero) and strings are not equal (ZF = 0)."
        Thus, CMPS can be used to find matching or differing string elements.
    */

    struct instruction instruction;

    int w = first_byte & 0b1;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(
        &instruction,
        "cmps%c",
        w ? 'w' : 'b'
    );


    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__scas(byte first_byte, struct opcode_context* context) {

    // Scan byte/word
    // 1010 111w

    /*
        SCAS (Scan String) subtracts the destination
        string element (byte or word) addressed by DI
        from the content of AL (byte string) or AX (word
        string) and updates the flags, but does not alter
        the destination string or the accumulator. SCAS
        also updates DI to point to the next string element
        and AF, CF, OF, PF, SF and ZF to reflect the
        relationship of the scan value in ALI AX to the
        string element. If SCAS is prefixed with REPE or
        REPZ, the operation is interpreted as "scan while
        not end-of-string (CX not 0) and string-element =
        scan-value (ZF = 1)." This form may be used to
        scan for departure from a given value. If SCAS is
        prefixed with REPNE or REPNZ, the operation
        is interpreted as "scan while not end-of-string
        (CX not 0) and string-element is not equal to
        scan-value (ZF = 0)." This form may be used to
        locate a value in a string. 
    */

    struct instruction instruction;

    int w = first_byte & 0b1;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(
        &instruction,
        "scas%c",
        w ? 'w' : 'b'
    );


    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__lods(byte first_byte, struct opcode_context* context) {

    // Load byte/wd to AL/AX
    // 1010 110w

    /*
        LODS (Load String) transfers the byte or word
        string element addressed by SI to register AL or
        AX, and updates SI to point to the next element
        in the string. This instruction is not ordinarily
        repeated since the accumulator would be overwritten by each repetition, and only the last element would be retained. However, LODS is very
        useful in software loops as part of a more complex string function built up from string
        primitives and other instructions.
    */

    struct instruction instruction;

    int w = first_byte & 0b1;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(
        &instruction,
        "lods%c",
        w ? 'w' : 'b'
    );


    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__stos(byte first_byte, struct opcode_context* context) {

    // Stor byte/wd from AL/AX
    // 1010 101w

    /*
        STOS (Store String) transfers a byte or word from
        register AL or AX to the string element addressed
        by DI and updates DI to point to the next location
        in the string. As a repeated operation, STOS provides a convenient way to initialize a string to a
        constant value (e.g., to blank out a print line).
    */

    struct instruction instruction;

    int w = first_byte & 0b1;

    instruction__create(&instruction, file_reader__read_bytes_so_far(&context->file_reader) - 1);
    instruction__push(
        &instruction,
        "stos%c",
        w ? 'w' : 'b'
    );


    instruction_list__push(&context->instruction_list, instruction);

}

void opcode__call_1110_1000(byte first_byte, struct opcode_context* context) {

    // Direct within segment
    // 1110 1000    IP-INC-LO    IP-INC-HI

    /*
        CALL activates an out-of-line procedure, saving
        information on the stack to permit a RET (return)
        instruction in the procedure to transfer control
        back to the instruction following the CALL. The 
        assembler generates a different type of CALL
        instruction depending on whether the programmer has defined the procedure name as NEAR or
        FAR. For control to return properly, the type of
        CALL instruction must match the type of RET
        instruction that exits from the procedure. (The
        potential for a mismatch exists if the procedure
        and the CALL are contained in separately
        assembled programs.) Different forms of the
        CALL instruction allow the address of the target
        procedure to be obtained from the instruction
        itself (direct CALL) or from a memory location
        or register referenced by the instruction (indirect
        CALL). In the following descriptions, bear in
        mind that the processor automatically adjusts IP
        to point to the next instruction to be executed,
        before saving it on the stack.
        For an intrasegment direct CALL, SP (the stack
        pointer) is decremented by two and IP is pushed
        onto the stack. The relative displacement (up to
        ±32k) of the target procedure from the CALL
        instruction is then added to the instruction
        pointer. This form of the CALL instruction is
        "self-relative" and is appropriate for position- independent (dynamically relocatable) routines in
        which the CALL and its target are in the same
        segment and are moved together.
        An intrasegment indirect CALL may be made
        through memory or through a register. SP is
        decremented by two and IP is pushed onto the
        stack. The offset of the target procedure is
        obtained from the memory word or 16-bit general
        register referenced in the instruction and replaces
        IP.
        For an intersegment direct CALL, SP is
        decremented by two, and CS is pushed onto the
        stack. CS is replaced by the segment word contained in the instruction. SP again is decremented
        by two. IP is pushed onto the stack and is
        replaced by the offset word contained in the
        instruction.
        For an intersegment indirect CALL (which only
        may be made through memory), SP is
        decremented by two, and CS is pushed onto the
        stack. CS is then replaced by the content of the
        second word oithe doubleword memory pointer
        referenced by the instruction. SP again is
        decremented by two, and IP is pushed onto the
        stack and is replaced by the content of the first
        word of the doubleword pointer referenced by the
        instruction. 
    */


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
    opcode__rep(first_byte, context);
}

void opcode__rep_repe_repz(byte first_byte, struct opcode_context* context) {
    opcode__rep(first_byte, context);
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
