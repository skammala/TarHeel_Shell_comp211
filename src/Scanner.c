#include <ctype.h>

#include "Scanner.h"

static TokenType find_type(char);
static Token scan_next(CharItr*);
static Str getStr(CharItr*);
static Token create_token(TokenType, CharItr*);

Scanner Scanner_value(CharItr char_itr)
{
    Token next = scan_next(&char_itr);

    Scanner itr = {
        char_itr,
        next
    };

    return itr;
}

bool Scanner_has_next(const Scanner *self)
{
    return (self->next.type != END_TOKEN);
}

Token Scanner_peek(const Scanner *self)
{
    return self->next;

}

Token Scanner_next(Scanner *self)
{
    if (!Scanner_has_next(self)) {
        Token end = {
            END_TOKEN,
            Str_from("")
        };
        return end;
    } else {
        Token current_token = self->next;
        self->next = scan_next(&self->char_itr);
        return current_token;
    }
}

static TokenType find_type(char next_char) {
    if (next_char == '|') {
        return PIPE_TOKEN;
    } else if (next_char < 0 || next_char == '\0') {
        return END_TOKEN;
    } else if (next_char != ' ' && next_char != '\t' && next_char != '\n') {
        return WORD_TOKEN;
    }
    return -2; 
}

static Token scan_next(CharItr *char_itr) {
    char next;
    while (CharItr_has_next(char_itr)) {
        next = CharItr_peek(char_itr);
        if (find_type(next) == PIPE_TOKEN) {
            next = CharItr_next(char_itr);
            return create_token(PIPE_TOKEN, char_itr);
        } else if (find_type(next) == WORD_TOKEN) {
            return create_token(WORD_TOKEN, char_itr);
        } else {
            next = CharItr_next(char_itr);
        }
    }
    return create_token(END_TOKEN, char_itr);
}

static Str getStr(CharItr *char_itr) {
    Str word = Str_value(1);
    char peek, next;
    while (CharItr_has_next(char_itr)) {
        peek = CharItr_peek(char_itr);
        if (find_type(peek) == WORD_TOKEN) {
            next = CharItr_next(char_itr);
            Str_set(&word, Str_length(&word), next);
        } else {
            return word;
        }
    } 
    return word;
}

static Token create_token(TokenType type, CharItr *char_itr) {
    if (type == PIPE_TOKEN) {
        Token token = {
            PIPE_TOKEN,
            Str_from("|")
        };
        return token;
    } else if (type == WORD_TOKEN) {
        Token token = {
            WORD_TOKEN,
            getStr(char_itr)
        };
        return token;
    } else {
        Token token = {
            END_TOKEN,
            Str_from("")
        };
        return token;
    }
}
