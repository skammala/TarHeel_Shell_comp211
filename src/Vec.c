#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "Guards.h"

#include "Vec.h"

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
/*
   void Vec_splice(Vec *self, size_t index, size_t delete_count, const void *items, size_t insert_count) {
   if(((index < self->length) && (delete_count <= (self->length - index))) || ((self->length == 0) && (index == 0) && (delete_count == 0))) {
   size_t new_buffer_length = self->length+(insert_count-delete_count);
   void *new_buffer = calloc(new_buffer_length, self->item_size); 
   OOM_GUARD(new_buffer, __FILE__, __LINE__);

   if(new_buffer_length == 0) {
   free(self->buffer);
   self->buffer = new_buffer;
   self->length = 0;
   self->capacity = 0;
   return;
   }

   size_t orig_array_idx = index + delete_count;
   for(size_t i=0; i<index; ++i) {
   memcpy(new_buffer + (i * self->item_size), Vec_ref(self, i), self->item_size);
   }
   for(size_t i=0; i<insert_count; ++i) {
   memcpy(new_buffer + ((i + index) * self->item_size), items + (i * self->item_size), self->item_size);
   }
   for(size_t i = index+insert_count; i < new_buffer_length; ++i) {
   memcpy(new_buffer + (i * self->item_size), Vec_ref(self, orig_array_idx++), self->item_size);
   }
   free(self->buffer);
   self->buffer = new_buffer;
   self->length = new_buffer_length;
   self->capacity = new_buffer_length;

   } else { 
   fprintf(stderr, "%s:%d - Out of Bounds", __FILE__, __LINE__);
   exit(EXIT_FAILURE);
   }
   }
   */

void Vec_splice(Vec *self, size_t index, size_t delete_count, const void *items, size_t insert_count) {
    if(((index < self->length) && (delete_count <= (self->length - index))) || ((self->length == 0) && (index == 0) && (delete_count == 0))) {
        size_t original_length = self->length;
        if(insert_count > delete_count) {
            _ensure_capacity(self, self->length + (insert_count - delete_count));
            self->length += (insert_count - delete_count);
        }
        for(size_t i = index + delete_count; i < original_length; ++i) {
            memcpy(Vec_ref(self, i - delete_count + insert_count), Vec_ref(self, i), self->item_size);
        }
        for(size_t i = 0; i < insert_count; ++i) {
            memcpy(Vec_ref(self, index + i), items + (i * self->item_size), self->item_size);
        }
        if(insert_count <= delete_count) {
            self->length += (insert_count - delete_count);
        }
    } else {
        fprintf(stderr, "%s:%d - Out of Bounds", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
}

static void _ensure_capacity(Vec *self, size_t n) {
    if(n > self->capacity) {
        size_t new_capacity = n * 2;
        self->buffer = realloc(self->buffer, new_capacity * self->item_size);
        OOM_GUARD(self->buffer, __FILE__, __LINE__);
        self->capacity = new_capacity;
    }
}
