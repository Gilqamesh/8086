#include "defs.h"
#include "file_reader.h"
#include "tables/opcode.h"
#include "label.h"
#include "instruction.h"

void error_handler(const char* msg, enum file_reader_error level) {
    switch (level) {
        case FILE_READER_ERROR_WARN: {
            fprintf(stdout, "%s\n", msg);
        } break ;
        case FILE_READER_ERROR_FATAL: {
            fprintf(stderr, "%s\n", msg);
            exit(1);
        } break ;
        default: {
            assert(false && "log_level is not handled");
        }
    }
}

void label_list__print(struct label_list* self) {
    struct label* cur_label = self->labels;

    printf("  ----==Labels==----  \n");
    while (cur_label) {
        printf(
            "    ip:  %d\n"
            "    uid: %d\n\n",
            cur_label->instruction_pointer,
            cur_label->unique_label_id);
        cur_label = cur_label->next;
    }
    printf("\n");
}

void instruction_list__print(struct instruction_list* self) {
    printf("  ----==Instructions==----  \n");
    for (uint32_t instruction_index = 0; instruction_index < self->instructions_fill; ++instruction_index) {
        printf(
            "    instruction: %s\n",
            self->instructions[instruction_index].buffer
        );
    }
}

void print_disassemble(struct opcode_context* context) {
    struct label* cur_label = context->label_list.labels;

    for (uint32_t instruction_index = 0; instruction_index < context->instruction_list.instructions_fill; ++instruction_index) {
        uint32_t cur_instruction_pointer = context->instruction_list.instructions[instruction_index].instruction_pointer;
        if (cur_label && cur_label->instruction_pointer == cur_instruction_pointer) {
            printf("%s_%d:", "label", cur_label->unique_label_id);
            cur_label = cur_label->next;
        }
        printf("%s\n", context->instruction_list.instructions[instruction_index].buffer);
    }
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("Usage: mynasm filename\n");
        exit(1);
    }

    struct opcode_context  opcode_context;
    opcode_context.error_handler = &error_handler;

    file_reader__create(&opcode_context.file_reader, argv[1], error_handler);
    label_list__create(&opcode_context.label_list);
    instruction_list__create(&opcode_context.instruction_list);

    printf("; %s disassembly:\nbits 16\n", argv[1]);
    while (file_reader__eof_reached(&opcode_context.file_reader) == false) {
        byte first_byte;
        file_reader__read_byte(&opcode_context.file_reader, &first_byte, error_handler);
        opcode_handlers[first_byte](first_byte, &opcode_context);
    }

    print_disassemble(&opcode_context);

    file_reader__destroy(&opcode_context.file_reader);
    label_list__destroy(&opcode_context.label_list);
    instruction_list__destroy(&opcode_context.instruction_list);
}
