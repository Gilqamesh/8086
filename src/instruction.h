#pragma once

#include "defs.h"

struct instruction
{
    uint32_t  instruction_pointer;
    byte      buffer_fill;
    char      buffer[59];
};
static_assert(sizeof(struct instruction) == 64, "");

void instruction__create(struct instruction* self, uint32_t instruction_pointer);
bool instruction__push(struct instruction* self, const char* msg, ...);

struct instruction_list
{
    struct instruction*  instructions;
    uint32_t             instructions_fill;
    uint32_t             instructions_size;
};

bool instruction_list__create(struct instruction_list* self);
void instruction_list__destroy(struct instruction_list* self);
bool instruction_list__push(struct instruction_list* self, struct instruction instruction);
