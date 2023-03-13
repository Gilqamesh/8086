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
            fprintf(stderr, "level = %d\n", level);
            assert(false && "level handle isn't implemented");
        }
    }
}

void label_list_error_handler(enum label_list_error error, const char* msg, ...) {
    va_list ap;

    va_start(ap, msg);
    switch (error) {
        case LABEL_LIST_ERROR__LABEL_TRUNCATED: {
            fprintf(stdout, "[Warn] ");
            vfprintf(stdout, msg, ap);
            fprintf(stdout, "\n");
        } break ;
        case LABEL_LIST_ERROR__LABEL_ALREADY_EXISTS: {
            fprintf(stdout, "[Warn] ");
            vfprintf(stdout, msg, ap);
            fprintf(stdout, "\n");
        } break ;
        default: {
            fprintf(stderr, "error = %d\n", error);
            assert(false && "error handle isn't implemented");
        }
    }
    va_end(ap);
}

void print_disassemble(struct opcode_context* context) {
    struct label* cur_label = context->label_list.labels;

    for (uint32_t instruction_index = 0; instruction_index < context->instruction_list.instructions_fill; ++instruction_index) {
        uint32_t cur_instruction_pointer = context->instruction_list.instructions[instruction_index].instruction_pointer;
        if (cur_label && cur_label->instruction_pointer == cur_instruction_pointer) {
            printf("%s:\n", cur_label->name);
            cur_label = cur_label->next;
        }
        printf("%s\n", context->instruction_list.instructions[instruction_index].buffer);
    }

    while (cur_label) {
        printf("%s:\n", cur_label->name);
        cur_label = cur_label->next;
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
    label_list__create(&opcode_context.label_list, label_list_error_handler);
    instruction_list__create(&opcode_context.instruction_list);

    printf("; %s disassembly:\nbits 16\n", argv[1]);
    while (file_reader__eof_reached(&opcode_context.file_reader) == false) {
        byte first_byte;
        file_reader__read_byte(&opcode_context.file_reader, &first_byte);
        opcode_handlers[first_byte](first_byte, &opcode_context);
    }

    print_disassemble(&opcode_context);

    file_reader__destroy(&opcode_context.file_reader);
    label_list__destroy(&opcode_context.label_list);
    instruction_list__destroy(&opcode_context.instruction_list);
}
