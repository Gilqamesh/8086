#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "file_reader.h"
#include "dispatchers/opcode.h"

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

void dispatch_instruction(struct file_reader* reader) {
    byte first_byte;
    byte second_byte;
    file_reader__read_byte(reader, &first_byte, error_handler);
    file_reader__read_byte_opt(reader, &second_byte, error_handler);

    opcode_handlers[first_byte](first_byte, second_byte, reader, error_handler);
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("Usage: mynasm filename\n");
        exit(1);
    }

    struct file_reader  reader;

    file_reader__create(&reader, argv[1], error_handler);

    printf("; %s disassembly:\nbits 16\n", argv[1]);
    while (file_reader__eof_reached(&reader) == false) {
        dispatch_instruction(&reader);
    }

    file_reader__destroy(&reader);
}
