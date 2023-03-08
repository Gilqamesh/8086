#pragma once

#include <stdbool.h>
#include "defs.h"

struct file_reader // circular buffer data structure for storing read bytes from a file
{
    byte  buffer[16];
    int   available;
    int   head;
    int   tail;
    int   fd;
};

enum file_reader_error {
    FILE_READER_ERROR_WARN,
    FILE_READER_ERROR_FATAL
};
typedef void (*file_reader_error)(const char* msg, enum file_reader_error level);

// @brief open a file for reading
bool file_reader__create(struct file_reader* self, const char* filename);
void file_reader__destroy(struct file_reader* self);

// @returns size currently read bytes in the buffer
int file_reader__size(struct file_reader* self);

// @returns size left in bytes available for reading
int file_reader__available(struct file_reader *self);

// @brief read size bytes from the configured file
// @returns the amount of bytes read or -1 on error
int file_reader__read(struct file_reader *self, int size, file_reader_error error_handler);

// @returns the byte in the stream and advances the file pointer by one
bool file_reader__eat_byte(struct file_reader* self, byte* out);

// @brief reads 1 byte if size is 0
// @returns true on success
bool file_reader__ensure_byte(struct file_reader* self, byte* out, file_reader_error error_handler);
