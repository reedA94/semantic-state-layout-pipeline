#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 56

#define START_STATE 14

#define DEAD_STATE 2

#define NUM_CLASSES 14

#define NUM_TOKENS 39

static const char *CLASS_NAMES[14] = {
    "NONACC::KEYWORD_PREFIX",
    "DEAD",
    "ACC::WHITESPACE",
    "ACC::DELIM",
    "NONACC::STRING_ESCAPE",
    "ACC::OPERATOR",
    "ACC::IDENT",
    "NONACC::START",
    "ACC::KEYWORD",
    "NONACC::NUMBER_PREFIX",
    "ACC::COMMENT",
    "ACC::NUMBER",
    "NONACC::STRING_BODY",
    "ACC::STRING",
};

static const char *TOKEN_NAMES[39] = {
    "AMP",
    "ANDAND",
    "ARROW",
    "ASSIGN",
    "BANG",
    "COMMA",
    "COMMENT_LINE",
    "EQEQ",
    "FLOAT",
    "GE",
    "GT",
    "IDENT",
    "INT",
    "KW_FOR",
    "KW_IF",
    "KW_INT",
    "KW_RETURN",
    "KW_WHILE",
    "LBRACE",
    "LBRACK",
    "LE",
    "LPAR",
    "LT",
    "MINUS",
    "MINUSMINUS",
    "NEQ",
    "NO_TOKEN",
    "OROR",
    "PIPE",
    "PLUS",
    "PLUSPLUS",
    "RBRACE",
    "RBRACK",
    "RPAR",
    "SEMI",
    "SLASH",
    "STAR",
    "STRING",
    "WS",
};

static const int CLASS_ACCEPTING[14] = {
    0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1,
};

static const int CLASS_DEAD[14] = {
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[56] = {
    26, 26, 26, 38, 26, 26, 34, 26, 2, 30, 11, 5, 26, 7, 26, 14,
    25, 26, 26, 26, 19, 21, 26, 3, 26, 10, 22, 6, 23, 27, 36, 8,
    13, 26, 35, 32, 26, 26, 15, 9, 37, 26, 4, 26, 24, 33, 29, 1,
    18, 31, 20, 12, 28, 17, 0, 16,
};

static const int DEAD_OF_STATE[56] = {
    0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

static const int CLASS_OF_STATE[56] = {
    0, 0, 1, 2, 0, 0, 3, 4, 5, 5, 6, 3, 0, 5, 7, 8,
    5, 0, 0, 0, 3, 3, 0, 5, 9, 5, 5, 10, 5, 5, 5, 11,
    8, 0, 5, 3, 0, 12, 8, 5, 13, 0, 5, 0, 5, 3, 5, 5,
    3, 3, 5, 11, 5, 8, 5, 8,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    return CLASS_OF_STATE[s];
}
