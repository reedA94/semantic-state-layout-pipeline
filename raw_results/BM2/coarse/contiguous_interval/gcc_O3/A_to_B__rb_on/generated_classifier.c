#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 56

#define START_STATE 53

#define DEAD_STATE 38

#define NUM_CLASSES 15

#define NUM_TOKENS 39

static const char *CLASS_NAMES[15] = {
    "ACC::COMMENT::rb0",
    "ACC::DELIM::rb0",
    "ACC::IDENT::rb1",
    "ACC::KEYWORD::rb1",
    "ACC::NUMBER::rb1",
    "ACC::OPERATOR::rb0",
    "ACC::OPERATOR::rb1",
    "ACC::STRING::rb0",
    "ACC::WHITESPACE::rb0",
    "DEAD",
    "NONACC::KEYWORD_PREFIX",
    "NONACC::NUMBER_PREFIX",
    "NONACC::START",
    "NONACC::STRING_BODY",
    "NONACC::STRING_ESCAPE",
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
    1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
};

static const int CLASS_DEAD[15] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[56] = {
    6, 21, 33, 18, 31, 19, 32, 34, 5, 11, 14, 15, 13, 17, 16, 12,
    8, 7, 25, 20, 9, 30, 24, 2, 1, 27, 36, 35, 3, 4, 22, 10,
    29, 23, 0, 28, 37, 38, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
    26, 26, 26, 26, 26, 26, 26, 26,
};

static const int DEAD_OF_STATE[56] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    if (s <= 0) return 0;
    if (s <= 8) return 1;
    if (s <= 9) return 2;
    if (s <= 14) return 3;
    if (s <= 16) return 4;
    if (s <= 26) return 5;
    if (s <= 35) return 6;
    if (s <= 36) return 7;
    if (s <= 37) return 8;
    if (s <= 38) return 9;
    if (s <= 51) return 10;
    if (s <= 52) return 11;
    if (s <= 53) return 12;
    if (s <= 54) return 13;
    return 14;
}
