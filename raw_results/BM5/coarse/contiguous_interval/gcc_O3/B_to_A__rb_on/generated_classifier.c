#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 66

#define START_STATE 63

#define DEAD_STATE 30

#define NUM_CLASSES 14

#define NUM_TOKENS 31

static const char *CLASS_NAMES[14] = {
    "ACC::IDENT::rb1",
    "ACC::KEYWORD::rb1",
    "ACC::NUMBER::rb1",
    "ACC::OPERATOR::rb0",
    "ACC::OPERATOR::rb1",
    "ACC::PUNCT::rb0",
    "ACC::STRING::rb0",
    "ACC::WHITESPACE::rb0",
    "DEAD",
    "NONACC::KEYWORD_PREFIX",
    "NONACC::NUMBER_PREFIX",
    "NONACC::START",
    "NONACC::STRING_BODY",
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
    1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
};

static const int CLASS_DEAD[14] = {
    0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[66] = {
    6, 13, 9, 17, 8, 12, 10, 11, 16, 15, 14, 7, 3, 2, 18, 4,
    22, 24, 21, 28, 27, 20, 5, 0, 26, 19, 25, 1, 29, 30, 23, 23,
    23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
    23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
    23, 23,
};

static const int DEAD_OF_STATE[66] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    if (s <= 0) return 0;
    if (s <= 10) return 1;
    if (s <= 12) return 2;
    if (s <= 20) return 3;
    if (s <= 22) return 4;
    if (s <= 27) return 5;
    if (s <= 28) return 6;
    if (s <= 29) return 7;
    if (s <= 30) return 8;
    if (s <= 61) return 9;
    if (s <= 62) return 10;
    if (s <= 63) return 11;
    if (s <= 64) return 12;
    return 13;
}
