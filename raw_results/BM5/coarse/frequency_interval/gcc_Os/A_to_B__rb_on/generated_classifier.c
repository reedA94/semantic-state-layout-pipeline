#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 66

#define START_STATE 64

#define DEAD_STATE 1

#define NUM_CLASSES 14

#define NUM_TOKENS 31

static const char *CLASS_NAMES[14] = {
    "ACC::IDENT::rb1",
    "DEAD",
    "ACC::WHITESPACE::rb0",
    "ACC::PUNCT::rb0",
    "NONACC::STRING_BODY",
    "ACC::NUMBER::rb1",
    "NONACC::KEYWORD_PREFIX",
    "ACC::OPERATOR::rb0",
    "ACC::STRING::rb0",
    "ACC::OPERATOR::rb1",
    "ACC::KEYWORD::rb1",
    "NONACC::NUMBER_PREFIX",
    "NONACC::START",
    "NONACC::STRING_ESCAPE",
};

static const char *TOKEN_NAMES[31] = {
    "COMMA",
    "DOT",
    "EQ",
    "FLOAT",
    "GE",
    "GT",
    "IDENT",
    "INT",
    "KW_AND",
    "KW_FROM",
    "KW_INSERT",
    "KW_INTO",
    "KW_OR",
    "KW_SELECT",
    "KW_SET",
    "KW_UPDATE",
    "KW_VALUES",
    "KW_WHERE",
    "LE",
    "LPAR",
    "LT",
    "MINUS",
    "NEQ",
    "NO_TOKEN",
    "PLUS",
    "RPAR",
    "SEMI",
    "SLASH",
    "STAR",
    "STRING",
    "WS",
};

static const int CLASS_ACCEPTING[14] = {
    1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0,
};

static const int CLASS_DEAD[14] = {
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[66] = {
    6, 23, 30, 0, 26, 19, 25, 1, 23, 7, 3, 23, 23, 23, 23, 23,
    23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
    23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 2, 18, 4, 22, 24, 21,
    28, 27, 29, 20, 5, 13, 9, 17, 8, 12, 10, 11, 16, 15, 14, 23,
    23, 23,
};

static const int DEAD_OF_STATE[66] = {
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    if (s <= 0) return 0;
    if (s <= 1) return 1;
    if (s <= 2) return 2;
    if (s <= 7) return 3;
    if (s <= 8) return 4;
    if (s <= 10) return 5;
    if (s <= 41) return 6;
    if (s <= 49) return 7;
    if (s <= 50) return 8;
    if (s <= 52) return 9;
    if (s <= 62) return 10;
    if (s <= 63) return 11;
    if (s <= 64) return 12;
    return 13;
}
