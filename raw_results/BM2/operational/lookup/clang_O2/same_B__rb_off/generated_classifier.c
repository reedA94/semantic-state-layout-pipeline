#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 56

#define START_STATE 14

#define DEAD_STATE 2

#define NUM_CLASSES 7

#define NUM_TOKENS 39

static const char *CLASS_NAMES[7] = {
    "NONACC::KEYWORD_PREFIX",
    "DEAD",
    "ACC::OPERATIONAL",
    "NONACC::STRING_ESCAPE",
    "NONACC::START",
    "NONACC::NUMBER_PREFIX",
    "NONACC::STRING_BODY",
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

static const int CLASS_ACCEPTING[7] = {
    0, 0, 1, 0, 0, 0, 0,
};

static const int CLASS_DEAD[7] = {
    0, 1, 0, 0, 0, 0, 0,
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
    0, 0, 1, 2, 0, 0, 2, 3, 2, 2, 2, 2, 0, 2, 4, 2,
    2, 0, 0, 0, 2, 2, 0, 2, 5, 2, 2, 2, 2, 2, 2, 2,
    2, 0, 2, 2, 0, 6, 2, 2, 2, 0, 2, 0, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    return CLASS_OF_STATE[s];
}
