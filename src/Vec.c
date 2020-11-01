#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "Guards.h"

#include "Vec.h"

static void* _get_pointer(const Vec*, size_t);
static void _ensure_capacity(Vec*, size_t);

/* Constructor / Destructor */

Vec Vec_value(size_t capacity, size_t item_size)
{
    Vec vec = {
        item_size,
        0,
        capacity,
        calloc(capacity, item_size)
    };
    OOM_GUARD(vec.buffer, __FILE__, __LINE__);
    return vec;
}

void Vec_drop(Vec *self)
{
    free(self->buffer);
    self->buffer = NULL;
    self->capacity = 0;
    self->length = 0;
}

/* Accessors */

size_t Vec_length(const Vec *self)
{
    return self->length;
}

void* Vec_ref(const Vec *self, size_t index)
{
    if (index < self->length) {
        return self->buffer + (index * self->item_size);
    } else {
        fprintf(stderr, "%s:%d - Out of Bounds", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
}

void Vec_get(const Vec *self, size_t index, void *out) {
    if(index < self->length) {
        memcpy(out, Vec_ref(self, index), self->item_size);
    } else { 
        fprintf(stderr, "%s:%d - Out of Bounds", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
}

void Vec_set(Vec *self, size_t index, const void *value) {
    if(index < self->length) {
        memcpy(Vec_ref(self, index), value, self->item_size);
    } else if(index == self->length) {
        _ensure_capacity(self, index+1);
        memcpy(self->buffer + (index * self->item_size), value, self->item_size);
        (self->length)++;
    } else {
        fprintf(stderr, "%s:%d - Out of Bounds", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
}

bool Vec_equals(const Vec *self, const Vec *other) {
    if(self->length == other->length) {
        for(size_t i=0; i<self->length; ++i) {
            if(memcmp(Vec_ref(self, i), Vec_ref(other, i), self->item_size) != 0) {
                return false;
            }
        }
        return true;
    } else {
        return false;
    }
}

void Vec_splice(Vec *self, size_t index, size_t delete_count, const void *items, size_t insert_count) {
    if((index + delete_count) <= self->length) {
        _ensure_capacity(self, self->length + (insert_count - delete_count));
        memcpy(_get_pointer(self, index + insert_count), _get_pointer(self, index + delete_count), self->item_size * (self->length - (index + delete_count)));
        memcpy(_get_pointer(self, index), items, self->item_size * insert_count);
        self->length += (insert_count - delete_count);
    } else {
        fprintf(stderr, "%s:%d - Out of Bounds", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
}

static void* _get_pointer(const Vec *self, size_t index) {
    return self->buffer + (index * self->item_size);
}

static void _ensure_capacity(Vec *self, size_t n) {
    if(n > self->capacity) {
        size_t new_capacity = n * 2;
        self->buffer = realloc(self->buffer, new_capacity * self->item_size);
        OOM_GUARD(self->buffer, __FILE__, __LINE__);
        self->capacity = new_capacity;
    }
}
