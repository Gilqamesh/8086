#include "file_reader.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

bool file_reader__create(struct file_reader* self, const char* filename) {
    self->available = sizeof(self->buffer);
    self->head      = 0;
    self->tail      = 0;
    self->fd        = open(filename, O_RDONLY);

    return self->fd != -1;
}

void file_reader__destroy(struct file_reader* self) {
    if (self->fd != -1) {
        close(self->fd);
    }
}

int file_reader__size(struct file_reader* self) {
    return sizeof(self->buffer) - self->available;
}

int file_reader__available(struct file_reader *self) {
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

    return byte;
}

int file_reader__read(struct file_reader *self, int size, file_reader_error error_handler) {
    if (file_reader__available(self) < size) {
        error_handler("file reader: full, can't read");
        return 0;
    }

    byte  buffer[1024];
    int   read_amount = read(self->fd, buffer, size);
    if (read_amount == -1) {
        perror("read");
        error_handler("in file reader");
    }

    for (int buffer_index = 0; buffer_index < read_amount; ++buffer_index) {
        file_reader_store_byte(self, buffer[buffer_index]);
    }

    // can be -1
    return read_amount;
}

bool file_reader__eat_byte(struct file_reader* self, byte* out) {
    if (file_reader__size(self) == 0) {
        return false;
    }

    *out = file_reader_read_byte(self);
    return true;
}

bool file_reader__ensure_byte(struct file_reader* self, byte* out, file_reader_error error_handler) {
    if (file_reader__size(self) == 0) {
        int read_amount = file_reader__read(self, 1, error_handler);
        if (read_amount == -1) {
            return false;
        }
        if (read_amount == 0) {
            // nothing more to read
            return false;
        }
    }

    return file_reader__eat_byte(self, out);
}
