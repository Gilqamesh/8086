#include "label.h"

const char* label__get_name(struct label* label) {
    return label->name;
}

void label_list__create(struct label_list* self) {
    self->labels               = NULL;
    self->unique_label_counter = 0;
}

void label_list__destroy(struct label_list* self) {
    while (self->labels) {
        struct label* next = self->labels->next;
        free(self->labels);
        self->labels = next;
    }
}

int32_t label_list__insert(struct label_list* self, uint32_t instruction_pointer, const char* label_name) {
    struct label** current_label_pointer = &self->labels;

    while (*current_label_pointer && instruction_pointer >= (*current_label_pointer)->instruction_pointer) {
        if ((*current_label_pointer)->instruction_pointer == instruction_pointer) {
            if (strcmp(label_name, (*current_label_pointer)->name) == 0) {
                return (*current_label_pointer)->unique_label_id;
            }
            return -1;
        }
        current_label_pointer = &(*current_label_pointer)->next;
    }

    struct label* current_label = *current_label_pointer;
    (*current_label_pointer) = (struct label*) malloc(sizeof(struct label));
    // handle error
    (*current_label_pointer)->unique_label_id = self->unique_label_counter++;
    (*current_label_pointer)->next = current_label;
    (*current_label_pointer)->instruction_pointer = instruction_pointer;
    uint32_t name_index = 0;
    while (name_index < sizeof((*current_label_pointer)->name) - 1 && label_name[name_index] != '\0') {
        ++name_index;
    }
    (*current_label_pointer)->name[name_index] = '\0';

    return (*current_label_pointer)->unique_label_id;
}
