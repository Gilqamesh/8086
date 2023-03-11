#pragma once

#include "defs.h"

struct label
{
    uint32_t      instruction_pointer;
    uint32_t      unique_label_id;
    struct label* next;
};

void label__print_name(struct label* label);

struct label_list
{
    struct label* labels;
    uint32_t      unique_label_counter;
};

void label_list__create(struct label_list* self);
void label_list__destroy(struct label_list* self);

// @brief adds a new unique label at the specified index, if it doesn't exist already
bool label_list__insert(struct label_list* self, uint32_t instruction_pointer);

bool label_list__get_unique_id(struct label_list* self, uint32_t instruction_pointer, uint32_t* out);
