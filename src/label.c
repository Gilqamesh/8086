#include "label.h"

void label__print_name(struct label* label) {
    printf("%s_%d", "label", label->unique_label_id);
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

bool label_list__insert(struct label_list* self, uint32_t instruction_pointer) {
    struct label** current_label_pointer = &self->labels;

    while (*current_label_pointer && instruction_pointer >= (*current_label_pointer)->instruction_pointer) {
        if ((*current_label_pointer)->instruction_pointer == instruction_pointer) {
            return false;
        }
        current_label_pointer = &(*current_label_pointer)->next;
    }

    struct label* current_label = *current_label_pointer;
    (*current_label_pointer) = (struct label*) malloc(sizeof(struct label));
    // handle error
    (*current_label_pointer)->unique_label_id = self->unique_label_counter++;
    (*current_label_pointer)->next = current_label;
    (*current_label_pointer)->instruction_pointer = instruction_pointer;

    return true;
}

bool label_list__get_unique_id(struct label_list* self, uint32_t instruction_pointer, uint32_t *out) {
    struct label* cur_label = self->labels;
    while (cur_label) {
        if (cur_label->instruction_pointer == instruction_pointer) {
            *out = cur_label->unique_label_id;
            return true;
        }
        cur_label = cur_label->next;
    }

    return false;
}
