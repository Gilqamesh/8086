#pragma once

#include <stdbool.h>
#include "defs.h"
#include <stdlib.h>

struct file_reader // circular buffer data structure for storing read bytes from a file
{
    // byte  buffer[1024];
    byte  buffer[1]; // for testing
    int   available;
    int   head;
    int   tail;
    int   fd;
    bool  eof;
};

enum file_reader_error {
    FILE_READER_ERROR_WARN,
    FILE_READER_ERROR_FATAL
};
typedef void (*file_reader_error)(const char* msg, enum file_reader_error level);

// @brief open a file for reading
void file_reader__create(struct file_reader* self, const char* filename, file_reader_error error_handler);
void file_reader__destroy(struct file_reader* self);

// @returns true if end of file has been reached
bool file_reader__eof_reached(struct file_reader* self);

// @brief reads and stores out a byte
void file_reader__read_byte(struct file_reader* self, byte* out, file_reader_error error_handler);
// @brief if there is a byte reads and stores out
// @returns true on read
bool file_reader__read_byte_opt(struct file_reader* self, byte* out, file_reader_error error_handler);
