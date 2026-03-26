#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 56

#define START_STATE 55

#define DEAD_STATE 38

#define NUM_CLASSES 7

#define NUM_TOKENS 39

static const char *CLASS_NAMES[7] = {
    "ACC::OPERATIONAL",
    "DEAD",
    "NONACC::STRING_BODY",
    "NONACC::KEYWORD_PREFIX",
    "NONACC::NUMBER_PREFIX",
    "NONACC::STRING_ESCAPE",
    "NONACC::START",
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
    1, 0, 0, 0, 0, 0, 0,
};

static const int CLASS_DEAD[7] = {
    0, 1, 0, 0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[56] = {
    11, 12, 8, 38, 37, 35, 6, 3, 7, 4, 25, 22, 20, 10, 9, 29,
    30, 23, 24, 2, 0, 1, 28, 27, 36, 21, 33, 18, 31, 19, 32, 34,
    5, 14, 15, 13, 17, 16, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
    26, 26, 26, 26, 26, 26, 26, 26,
};

static const int DEAD_OF_STATE[56] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    if (s <= 37) return 0;
    if (s <= 38) return 1;
    if (s <= 39) return 2;
    if (s <= 52) return 3;
    if (s <= 53) return 4;
    if (s <= 54) return 5;
    return 6;
}
