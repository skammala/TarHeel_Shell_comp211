#include "Parser.h"

static Node* parse_pipe(Scanner*);
static Node* parse_command(Scanner*);

Node* parse(Scanner *scanner)
{
    Token next = Scanner_peek(scanner);
    switch (next.type) {
        case PIPE_NODE:
            return parse_pipe(scanner);
        case WORD_TOKEN:
            return parse_command(scanner);
        default:
            return ErrorNode_new("Expected Command or |");
    }
}

static Node* parse_pipe(Scanner *scanner) {
    Token next = Scanner_next(scanner);
    next = Scanner_peek(scanner);
    if (next.type == WORD_TOKEN) {
        return parse(scanner);
    } else {
        return ErrorNode_new("Expected word after '|'");
    }
}

static Node* parse_command(Scanner *scanner) {
    Token next = Scanner_next(scanner);
    StrVec words = StrVec_value(1);
    StrVec_push(&words, next.lexeme);
    while (Scanner_has_next(scanner)) {
        Token peek = Scanner_peek(scanner);
        if (peek.type == WORD_TOKEN) {
            next = Scanner_next(scanner);
            StrVec_push(&words, next.lexeme);
        } else if (peek.type == PIPE_TOKEN) {
            return PipeNode_new(CommandNode_new(words), parse(scanner)); 
        } else {
            break;
        }
    }
    return CommandNode_new(words);
}
