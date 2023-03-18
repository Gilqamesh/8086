#pragma once

#include "defs.h"

struct instruction
{
    uint32_t  instruction_pointer;
    byte      buffer_fill;
    char      segment_override_prefix[3];
    char      buffer[56];
};
static_assert(sizeof(struct instruction) == 64, "");

bool instruction__push(struct instruction* self, const char* msg, ...);
void instruction__set_instruction_pointer(struct instruction* self, uint32_t instruction_pointer);
void instruction__set_segment_override_prefix(struct instruction* self, enum segment_registries segment_register);

struct instruction_list
{
    struct instruction*  instructions;
    uint32_t             instructions_fill;
    uint32_t             instructions_size;
};

bool instruction_list__create(struct instruction_list* self);
void instruction_list__destroy(struct instruction_list* self);

struct instruction* instruction_list__get(struct instruction_list* self);

bool instruction_list__push(struct instruction_list* self);
