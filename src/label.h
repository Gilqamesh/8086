#pragma once

#include "defs.h"

struct label
{
    struct label* next;
    uint32_t      instruction_pointer;
    int32_t       unique_label_id;
    char          name[48];
};

const char* label__get_name(struct label* label);

struct label_list
{
    struct label* labels;
    uint32_t      unique_label_counter;
};

void label_list__create(struct label_list* self);
void label_list__destroy(struct label_list* self);

// @returns the unique label id that was inserted (or if it exists already), -1 if the label with the name already exists with the same instruction_pointer
int32_t label_list__insert(struct label_list* self, uint32_t instruction_pointer, const char* label_name);
