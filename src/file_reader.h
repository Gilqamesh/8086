#pragma once

#include <stdbool.h>
#include "defs.h"
#include <stdlib.h>

enum file_reader_error {
    FILE_READER_ERROR_WARN,
    FILE_READER_ERROR_FATAL
};
typedef void (*file_reader_error)(const char* msg, enum file_reader_error level);

struct file_reader // circular buffer data structure for storing read bytes from a file
{
    file_reader_error error_handler;
    // byte  buffer[1024];
    uint32_t  read_bytes_so_far;
    int       available;
    int       head;
    int       tail;
    int       fd;
    bool      eof;
    byte      buffer[1]; // for testing
};

// @brief open a file for reading
void file_reader__create(struct file_reader* self, const char* filename, file_reader_error error_handler);
void file_reader__destroy(struct file_reader* self);

// @returns true if end of file has been reached
bool file_reader__eof_reached(struct file_reader* self);

// @brief reads and stores out a byte
void file_reader__read_byte(struct file_reader* self, void* out);
// @brief reads and stores out a word
void file_reader__read_word(struct file_reader* self, void* out);

uint32_t file_reader__read_bytes_so_far(struct file_reader* self);

static void (*const file_reader__read_by_type[2])(struct file_reader* self, void* out) = {
    file_reader__read_byte,
    file_reader__read_word
};
