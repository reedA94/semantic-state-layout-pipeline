#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 66

#define START_STATE 20

#define DEAD_STATE 47

#define NUM_CLASSES 14

#define NUM_TOKENS 31

static const char *CLASS_NAMES[14] = {
    "NONACC::KEYWORD_PREFIX",
    "NONACC::STRING_ESCAPE",
    "ACC::PUNCT::rb0",
    "ACC::KEYWORD::rb1",
    "ACC::OPERATOR::rb1",
    "NONACC::NUMBER_PREFIX",
    "ACC::OPERATOR::rb0",
    "ACC::NUMBER::rb1",
    "NONACC::START",
    "ACC::IDENT::rb1",
    "NONACC::STRING_BODY",
    "DEAD",
    "ACC::STRING::rb0",
    "ACC::WHITESPACE::rb0",
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
    0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1,
};

static const int CLASS_DEAD[14] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
};

static const int TOKEN_OF_STATE[66] = {
    23, 23, 23, 23, 23, 23, 23, 1, 8, 23, 9, 11, 15, 5, 23, 20,
    23, 13, 18, 7, 23, 16, 6, 23, 23, 22, 24, 28, 12, 23, 23, 26,
    23, 23, 23, 23, 23, 19, 3, 23, 23, 2, 17, 23, 23, 23, 23, 23,
    27, 29, 23, 4, 23, 25, 23, 0, 14, 23, 23, 23, 23, 23, 21, 30,
    23, 10,
};

static const int DEAD_OF_STATE[66] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0,
};

static const int CLASS_OF_STATE[66] = {
    0, 0, 0, 1, 0, 0, 0, 2, 3, 0, 3, 3, 3, 4, 5, 4,
    0, 3, 6, 7, 8, 3, 9, 0, 0, 6, 6, 6, 3, 0, 0, 2,
    0, 0, 0, 0, 0, 2, 7, 0, 0, 6, 3, 0, 0, 0, 10, 11,
    6, 12, 0, 6, 0, 2, 0, 2, 3, 0, 0, 0, 0, 0, 6, 13,
    0, 3,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    return CLASS_OF_STATE[s];
}
