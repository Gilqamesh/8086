#pragma once

#include <stdint.h>
#include <assert.h>

typedef uint16_t  word;
typedef uint8_t   byte;

enum word_registries {
    AX = 0b000,
    CX = 0b001,
    DX = 0b010,
    BX = 0b011,
    SP = 0b100,
    BP = 0b101,
    SI = 0b110,
    DI = 0b111
};

enum byte_registries {
    AL = 0b000,
    CL = 0b001,
    DL = 0b010,
    BL = 0b011,
    AH = 0b100,
    CH = 0b101,
    DH = 0b110,
    BH = 0b111,
};

enum mod_field {
    MEMORY_MODE_NO_DISPLACEMENT     = 0b00,
    MEMORY_MODE_8_BIT_DISPLACEMENT  = 0b01,
    MEMORY_MODE_16_BIT_DISPLACEMENT = 0b10,
    REGISTER_MODE_NO_DISPLACEMENT   = 0b11
};

static inline const char* reg_to_word_registry(enum byte_registries reg) {
    static const char* registry_names[] = {
        "ax", "cx", "dx", "bx", "sp", "bp", "si", "di"
    };

    assert(reg < (sizeof(registry_names)/sizeof(registry_names[0])));
    return registry_names[reg];
}

static inline const char* reg_to_byte_registry(enum word_registries reg) {
    static const char* registry_names[] = {
        "al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"
    };

    assert(reg < (sizeof(registry_names)/sizeof(registry_names[0])));
    return registry_names[reg];
}

static inline const char* opcode_to_instruction(int opcode) {
    switch (opcode) {
        case 0b100010: {
            return "mov";
        } break ;
        default: {
            assert(false && "not implemented opcode");
        }
    }

    return "";
}
