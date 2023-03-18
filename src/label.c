#include "label.h"

const char* label__get_name(struct label* label) {
    return label->name;
}

void label_list__create(struct label_list* self, label_list_error error_handler) {
    self->labels               = NULL;
    self->error_handler        = error_handler;
    self->unique_label_counter = 0;
}

void label_list__destroy(struct label_list* self) {
    while (self->labels) {
        struct label* next = self->labels->next;
        free(self->labels);
        self->labels = next;
    }
}

struct label* label_list__insert(struct label_list* self, uint32_t instruction_pointer, const char* label_name) {
    struct label** current_label_pointer = &self->labels;

    while (*current_label_pointer && instruction_pointer >= (*current_label_pointer)->instruction_pointer) {
        if ((*current_label_pointer)->instruction_pointer == instruction_pointer) {
            return *current_label_pointer;
        }
        current_label_pointer = &(*current_label_pointer)->next;
    }

    struct label* current_label = *current_label_pointer;
    (*current_label_pointer) = (struct label*) malloc(sizeof(struct label));
    // handle error
    (*current_label_pointer)->unique_label_id = self->unique_label_counter++;
    (*current_label_pointer)->next = current_label;
    (*current_label_pointer)->instruction_pointer = instruction_pointer;
    if ((int)sizeof((*current_label_pointer)->name) <= snprintf((*current_label_pointer)->name, sizeof((*current_label_pointer)->name), "%s%d", label_name, (*current_label_pointer)->unique_label_id)) {
        self->error_handler(LABEL_LIST_ERROR__LABEL_TRUNCATED, "label_list__insert: label name '%s' + unique id '%d' was truncated to '%s'", label_name, (*current_label_pointer)->unique_label_id, (*current_label_pointer)->name);
        --self->unique_label_counter;
        (*current_label_pointer) = current_label;
        return NULL;
    }

    return (*current_label_pointer);
}
