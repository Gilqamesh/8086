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

enum label_list_error {
    LABEL_LIST_ERROR__LABEL_TRUNCATED,
    LABEL_LIST_ERROR__LABEL_ALREADY_EXISTS
};
typedef void (*label_list_error)(enum label_list_error error, const char* msg, ...);

struct label_list
{
    struct label*    labels;
    label_list_error error_handler;
    uint32_t         unique_label_counter;
};

void label_list__create(struct label_list* self, label_list_error error_handler);
void label_list__destroy(struct label_list* self);

// @returns the label that was inserted, null in case of failure
struct label* label_list__insert(struct label_list* self, uint32_t instruction_pointer, const char* label_name);
