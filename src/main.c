#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Vec.h"
#include "Str.h"

#define MAX_CHARS 30

char *string_calloc(size_t);
void print_str_contents(Str*);

int main()
{
    char *line = malloc(MAX_CHARS);
    Str c = Str_value(MAX_CHARS);

    while((line = fgets(line, MAX_CHARS, stdin)) != NULL) {
        Str_append(&c, line);

        if(strchr(line, '\n') != NULL) {
            print_str_contents(&c);
            Str_drop(&c);
            c = Str_value(MAX_CHARS);
        }

    }
    free(line);
    Str_drop(&c);
    return EXIT_SUCCESS;
}

void print_str_contents(Str* string) {
    printf("%s", Str_cstr(string));
}
