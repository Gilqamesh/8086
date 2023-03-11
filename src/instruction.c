#include "instruction.h"

void instruction__create(struct instruction* self, uint32_t instruction_pointer) {
    self->instruction_pointer = instruction_pointer;
    self->buffer_fill         = 0;
    self->buffer[0]           = '\0';
}

static inline int instruction__available_bytes_left(struct instruction* self) {
    return sizeof(self->buffer) - self->buffer_fill - 1;
}

bool instruction__push(struct instruction* self, const char* msg, ...) {
    int      written_bytes;
    va_list  ap;

    if (instruction__available_bytes_left(self) == 0) {
        return false;
    }

    va_start(ap, msg);
    written_bytes = vsnprintf(self->buffer + self->buffer_fill, instruction__available_bytes_left(self), msg, ap);
    va_end(ap);

    if (written_bytes < 0) {
        // TODO(david): error handling
    }

    if (written_bytes > instruction__available_bytes_left(self)) {
        return false;
    }
    self->buffer_fill += written_bytes;

    return true;
}

bool instruction_list__create(struct instruction_list* self) {
    self->instructions_fill = 0;
    self->instructions_size = 8;
    self->instructions = (struct instruction*) malloc(sizeof(*self->instructions) * self->instructions_size);

    return self->instructions != NULL;
}

void instruction_list__destroy(struct instruction_list* self) {
    if (self->instructions) {
        free(self->instructions);
    }
}

bool instruction_list__push(struct instruction_list* self, struct instruction instruction) {
    if (self->instructions_fill == self->instructions_size) {
        self->instructions_size <<= 1;
        self->instructions = (struct instruction*) realloc(self->instructions, sizeof(*self->instructions) * self->instructions_size);
        if (self->instructions == NULL) {
            return false;
        }
    }

    self->instructions[self->instructions_fill++] = instruction;

    return true;
}
