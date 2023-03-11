#include "file_reader.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

void file_reader__create(struct file_reader* self, const char* filename, file_reader_error error_handler) {
    self->available          = sizeof(self->buffer);
    self->head               = 0;
    self->tail               = 0;
    self->fd                 = open(filename, O_RDONLY);
    self->eof                = false;
    self->read_bytes_so_far  = 0;

    if (self->fd == -1) {
        perror("open");
        error_handler("in 'file_reader__create'", FILE_READER_ERROR_FATAL);
    }
}

void file_reader__destroy(struct file_reader* self) {
    assert(self->fd != -1 && "in 'file_reader__destroy': first call 'file_reader__create'");
    close(self->fd);
}

bool file_reader__eof_reached(struct file_reader* self) {
    return self->eof;
}

static int file_reader__size(struct file_reader* self) {
    return sizeof(self->buffer) - self->available;
}

static inline int file_reader__available(struct file_reader *self) {
    return self->available;
}

static void file_reader_store_byte(struct file_reader* self, byte byte) {
    self->buffer[self->head++] = byte;
    --self->available;

    if (self->head == sizeof(self->buffer)) {
        self->head = 0;
    }
}

static byte file_reader_read_byte(struct file_reader* self) {
    byte byte = self->buffer[self->tail++];
    ++self->available;

    if (self->tail == sizeof(self->buffer)) {
        self->tail = 0;
    }

    self->read_bytes_so_far += 1;

    return byte;
}

static int file_reader__read(struct file_reader *self, int size, file_reader_error error_handler) {
    if (file_reader__available(self) < size) {
        error_handler("in 'file_reader__read': cannot read size much", FILE_READER_ERROR_WARN);
        size = file_reader__available(self);
    }

    byte  buffer[sizeof(self->buffer)];
    int   read_amount = read(self->fd, buffer, size);
    if (read_amount == -1) {
        perror("read");
        error_handler("in 'file_reader__read'", FILE_READER_ERROR_FATAL);
    }

    for (int buffer_index = 0; buffer_index < read_amount; ++buffer_index) {
        file_reader_store_byte(self, buffer[buffer_index]);
    }

    return read_amount;
}

void file_reader__read_byte(struct file_reader* self, void* out, file_reader_error error_handler) {
    if (file_reader__eof_reached(self)) {
        error_handler("in 'file_reader__read_byte': already reached end of file, nothing to read", FILE_READER_ERROR_FATAL);
        return ;
    }

    if (file_reader__size(self) == 0) {
        file_reader__read(self, file_reader__available(self), error_handler);
    }

    *(byte*)out = file_reader_read_byte(self);

    if (file_reader__read(self, 1, error_handler) == 0) {
        self->eof = true;
    }
}

void file_reader__read_word(struct file_reader* self, void* out, file_reader_error error_handler) {
    file_reader__read_byte(self, (byte*)out, error_handler);
    file_reader__read_byte(self, (byte*)out + 1, error_handler);
}

uint32_t file_reader__read_bytes_so_far(struct file_reader* self) {
    return self->read_bytes_so_far;
}
