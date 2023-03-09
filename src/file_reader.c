#include "file_reader.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

void file_reader__create(struct file_reader* self, const char* filename, file_reader_error error_handler) {
    self->available = sizeof(self->buffer);
    self->head      = 0;
    self->tail      = 0;
    self->fd        = open(filename, O_RDONLY);
    self->eof       = false;

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

static void file_reader_unread_byte(struct file_reader* self, byte byte) {
    if (self->tail == 0) {
        self->tail = sizeof(self->buffer) - 1;
    } else {
        --self->tail;
    }

    --self->available;

    self->buffer[self->tail] = byte;
}

static byte file_reader_read_byte(struct file_reader* self) {
    byte byte = self->buffer[self->tail++];
    ++self->available;

    if (self->tail == sizeof(self->buffer)) {
        self->tail = 0;
    }

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

void file_reader__read_byte(struct file_reader* self, byte* out, file_reader_error error_handler) {
    if (file_reader__eof_reached(self)) {
        error_handler("in 'file_reader__read_byte': already reached end of file, nothing to read", FILE_READER_ERROR_FATAL);
        return ;
    }

    if (file_reader__size(self) == 0) {
        file_reader__read(self, file_reader__available(self), error_handler);
    }

    *out = file_reader_read_byte(self);

    if (file_reader__read(self, 1, error_handler) == 0) {
        self->eof = true;
    }
}

bool file_reader__read_byte_opt(struct file_reader* self, byte* out, file_reader_error error_handler) {
    if (file_reader__size(self) == 0) {
        file_reader__read(self, file_reader__available(self), error_handler);
    }

    bool ret = false;
    if (file_reader__size(self)) {
        *out = file_reader_read_byte(self);
        ret = true;
    }

    if (file_reader__read(self, 1, error_handler) == 0) {
        self->eof = true;
    }

    return ret;
}

void file_reader__unread_byte(struct file_reader* self, byte c, file_reader_error error_handler) {
    if (file_reader__available(self) == 0) {
        error_handler("in 'file_reader__unread_byte': not enough space left to store byte back", FILE_READER_ERROR_FATAL);
        return ;
    }

    file_reader_unread_byte(self, c);
}