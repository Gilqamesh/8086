#include "file_reader.h"

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

static inline void print_byte(byte b) {
    for (int i = 0; i < 8; ++i) {
        printf("%s", (b & 0b1) ? "1" : "0");
        if (i == 3) {
            printf(" ");
        }
        b >>= 1;
    }
    printf("   ");
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: bprinter filename\n");
        exit(1);
    }

    struct file_reader reader;

    file_reader__create(&reader, argv[1], error_handler);

    unsigned int byte_index = 0;
    printf("; %s bytes:\n", argv[1]);
    while(file_reader__eof_reached(&reader) == false) {
        byte b;
        file_reader__read_byte(&reader, &b, error_handler);

        printf("%d: ", byte_index++);
        for (int i = 0; i < 8; ++i) {
            printf("%s", ((b >> 7) & 0b1) ? "1" : "0");
            if (i == 3) {
                printf(" ");
            }
            b <<= 1;
        }
        printf("\n");
    }

    file_reader__destroy(&reader);
}
