#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 56

#define START_STATE 55

#define DEAD_STATE 0

#define NUM_CLASSES 15

#define NUM_TOKENS 39

static const char *CLASS_NAMES[15] = {
    "DEAD",
    "ACC::IDENT::rb1",
    "ACC::WHITESPACE::rb0",
    "ACC::DELIM::rb0",
    "ACC::COMMENT::rb0",
    "ACC::NUMBER::rb1",
    "ACC::OPERATOR::rb1",
    "NONACC::KEYWORD_PREFIX",
    "NONACC::STRING_BODY",
    "NONACC::NUMBER_PREFIX",
    "ACC::OPERATOR::rb0",
    "ACC::STRING::rb0",
    "NONACC::STRING_ESCAPE",
    "ACC::KEYWORD::rb1",
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

static const int CLASS_ACCEPTING[15] = {
    0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0,
};

static const int CLASS_DEAD[15] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[56] = {
    26, 11, 38, 21, 33, 18, 31, 19, 32, 34, 5, 6, 12, 8, 35, 3,
    4, 22, 10, 29, 23, 0, 28, 26, 26, 26, 26, 26, 26, 26, 26, 26,
    26, 26, 26, 26, 26, 26, 7, 25, 20, 9, 30, 24, 2, 1, 27, 36,
    37, 26, 14, 15, 13, 17, 16, 26,
};

static const int DEAD_OF_STATE[56] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    if (s <= 0) return 0;
    if (s <= 1) return 1;
    if (s <= 2) return 2;
    if (s <= 10) return 3;
    if (s <= 11) return 4;
    if (s <= 13) return 5;
    if (s <= 22) return 6;
    if (s <= 35) return 7;
    if (s <= 36) return 8;
    if (s <= 37) return 9;
    if (s <= 47) return 10;
    if (s <= 48) return 11;
    if (s <= 49) return 12;
    if (s <= 54) return 13;
    return 14;
}
