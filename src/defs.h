#pragma once

#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>

typedef uint16_t  word;
typedef int16_t   signed_word;
typedef uint8_t   byte;
typedef int8_t    signed_byte;

enum word_registries {
    AX = 0b000,
    BX = 0b011,
    CX = 0b001,
    DX = 0b010,

    SP = 0b100, // stack pointer
    BP = 0b101, // base pointer

    SI = 0b110, // src index
    DI = 0b111  // dst index
};

enum byte_registries { // low-high part of AX, BX, CX and DX 
    AL = 0b000,
    AH = 0b100,

    BL = 0b011,
    BH = 0b111,

    CL = 0b001,
    CH = 0b101,
    
    DL = 0b010,
    DH = 0b110
};

enum segment_registries {
    ES = 0b00,  // extra segment
    CS = 0b01, // code segment
    SS = 0b10, // stack segment
    DS = 0b11  // data segment
};

enum instruction_registry {
    IP = 0b00 // instruction_pointer
};

enum control_flags {
    TF = 0b00, // trap
    DF = 0b01, // direction
    IF = 0b10  // interrupt-enable
};

enum status_flags {
    OF = 0b000, // overflow
    SF = 0b001, // sign
    ZF = 0b010, // zero
    AF = 0b011, // auxiliary carry
    PF = 0b100, // parity
    CF = 0b101  // carry
};

enum mod_field {
    MEMORY_MODE_NO_DISPLACEMENT     = 0b00,
    MEMORY_MODE_8_BIT_DISPLACEMENT  = 0b01,
    MEMORY_MODE_16_BIT_DISPLACEMENT = 0b10,
    REGISTER_MODE_NO_DISPLACEMENT   = 0b11
};

static inline const char* reg_to_word(unsigned int reg, int is_wide) {
    static const char* const registry_names[] = {
        "al", "cl", "dl", "bl", "ah", "ch", "dh", "bh",
        "ax", "cx", "dx", "bx", "sp", "bp", "si", "di"
    };

    return registry_names[reg + (is_wide << 3)];
}

static inline const char* effective_address_to_word(unsigned int r_m) {
    static const char* const formulas_for_ea_calculations[] = {
        "bx + si",
        "bx + di",
        "bp + si",
        "bp + di",
        "si",
        "di",
        "bp",
        "bx"
    };

    return formulas_for_ea_calculations[r_m];
}

static inline const char* segment_reg_to_word(enum segment_registries reg) {
    static const char* const segment_registry_names[] = {
        "es", "cs", "ss", "ds"
    };

    return segment_registry_names[reg];
}
