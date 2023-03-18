#include "instruction.h"

void instruction__set_instruction_pointer(struct instruction* self, uint32_t instruction_pointer) {
    if (self->instruction_pointer != 0) {
        self->instruction_pointer = instruction_pointer;
    }
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

void instruction__set_segment_override_prefix(struct instruction* self, enum segment_registries segment_register) {
    strncpy(self->segment_override_prefix, segment_reg_to_word(segment_register), sizeof(self->segment_override_prefix));
}

bool instruction_list__create(struct instruction_list* self) {
    self->instructions_fill = 0;
    self->instructions_size = 8;
    self->instructions = (struct instruction*) calloc(sizeof(*self->instructions), self->instructions_size);

    return self->instructions != NULL;
}

void instruction_list__destroy(struct instruction_list* self) {
    if (self->instructions) {
        free(self->instructions);
    }
}

struct instruction* instruction_list__get(struct instruction_list* self) {
    if (self->instructions_fill >= self->instructions_size) {
        self->instructions_size <<= 1;
        self->instructions = (struct instruction*) realloc(self->instructions, sizeof(*self->instructions) * self->instructions_size);
        if (self->instructions == NULL) {
            return false;
        }
        memset(self->instructions + self->instructions_fill, 0, (self->instructions_size - self->instructions_fill) * sizeof(*self->instructions));
    }

    return &self->instructions[self->instructions_fill];
}

bool instruction_list__push(struct instruction_list* self) {
    if (self->instructions_fill >= self->instructions_size) {
        return false;
    }

    struct instruction* pushed_instruction = &self->instructions[self->instructions_fill];
    if (pushed_instruction->segment_override_prefix[0]) {
        char* left_bracket_p = strchr(pushed_instruction->buffer, '[');
        assert(left_bracket_p != NULL);
        unsigned int segment_override_prefix_len = strlen(pushed_instruction->segment_override_prefix) + 1; // +1 for ':'
        if (pushed_instruction->buffer_fill + segment_override_prefix_len > sizeof(pushed_instruction->buffer) - 1) {
            return false;
        }

        char* buffer_end_p = pushed_instruction->buffer + pushed_instruction->buffer_fill;
        assert(*buffer_end_p == '\0');
        memmove(left_bracket_p + segment_override_prefix_len, left_bracket_p, buffer_end_p - left_bracket_p + 1);
        memcpy(left_bracket_p, pushed_instruction->segment_override_prefix, segment_override_prefix_len - 1);
        memcpy(left_bracket_p + segment_override_prefix_len - 1, ":", 1);
    }

    ++self->instructions_fill;
    return true;
}

// bool instruction_list__push(struct instruction_list* self, struct instruction instruction) {
//     if (self->instructions_fill == self->instructions_size) {
//         self->instructions_size <<= 1;
//         self->instructions = (struct instruction*) realloc(self->instructions, sizeof(*self->instructions) * self->instructions_size);
//         if (self->instructions == NULL) {
//             return false;
//         }
//         memset(self->instructions + self->instructions_fill, 0, (self->instructions_size - self->instructions_fill) * sizeof(*self->instructions));
//     }

//     // NOTE(david): has segment_override_prefix
//     struct instruction* top_instruction = &self->instructions[self->instructions_fill];
//     if (top_instruction->segment_override_prefix[0]) {
//         char* left_bracket_p = strchr(instruction.buffer, '[');
//         assert(left_bracket_p != NULL);
//         unsigned int segment_override_prefix_len = strlen(top_instruction->segment_override_prefix) + 1;
//         if (instruction.buffer_fill + segment_override_prefix_len > sizeof(instruction.buffer) - 1) {
//             return false;
//         }

//         char* null_char_p = top_instruction->buffer + top_instruction->buffer_fill;
//         assert(*null_char_p == '\0');
//         memmove(left_bracket_p + segment_override_prefix_len, left_bracket_p, null_char_p - left_bracket_p + 1);
//         memcpy(left_bracket_p, top_instruction->segment_override_prefix, segment_override_prefix_len - 1);
//         memcpy(left_bracket_p + segment_override_prefix_len - 1, ":", 1);
//     }

//     // NOTE(david): already have a prefix
//     if (top_instruction->buffer_fill > 0) {
//         instruction__push(top_instruction, instruction.buffer);
//         ++self->instructions_fill;
//     } else {
//         strcpy(self->instructions[self->instructions_fill].buffer, instruction.buffer);
//         self->instructions[self->instructions_fill].buffer_fill = instruction.buffer_fill;
//         self->instructions[self->instructions_fill].instruction_pointer = instruction.instruction_pointer;
//         strcpy(self->instructions[self->instructions_fill].segment_override_prefix, instruction.segment_override_prefix);
//     }

//     return true;
// }

// bool instruction_list__push_prefix(struct instruction_list* self, struct instruction instruction) {
//     // NOTE(david): already have a prefix
//     if (instruction__push(&self->instructions[self->instructions_fill], instruction.buffer) == false) {
//         return false;
//     }
//     self->instructions[self->instructions_fill].instruction_pointer = instruction.instruction_pointer;
//     strcpy(self->instructions[self->instructions_fill].segment_override_prefix, instruction.segment_override_prefix);

//     return true;
// }
