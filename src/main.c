#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Vec.h"
#include "Str.h"

#define MAX_CHARS 30

char *string_calloc(size_t);
char *string_realloc(char*, size_t);
void print_str_contents(Str*);

int main()
{
    char *line = string_calloc(MAX_CHARS);
    char *first_idx = line;
    size_t capacity = MAX_CHARS;
    while((line = fgets(line, MAX_CHARS, stdin)) != NULL) {
        if(strchr(line, '\n') != NULL) {
            line = first_idx;
            Str c = Str_value(20);
            Str_append(&c, line);
            print_str_contents(&c);
            Str_drop(&c);
            free(line);
            line = string_calloc(MAX_CHARS);
            first_idx = line;
        } else {
            line = first_idx;
            string_realloc(line, (capacity *= 2));
            line = strchr(line, '\0');
        }
    }
    free(line);
    first_idx = NULL;
    return EXIT_SUCCESS;
}

char *string_calloc(size_t nitems) {
    char *string = calloc(nitems, sizeof(char));
    if(string == NULL) {
        fprintf(stderr, "Out of memory");
        exit(EXIT_FAILURE);
    }
    return string;
}

char *string_realloc(char *string, size_t new_capacity) {
    string = realloc(string, new_capacity * sizeof(char));
    if(string == NULL) {
        fprintf(stderr, "Out of memory");
        exit(EXIT_FAILURE);
    }
    return string;
}

void print_str_contents(Str* string) {
    for(size_t i = 0; i < Str_length(string); ++i) {
        printf("%c", Str_get(string, i));
    }
}
