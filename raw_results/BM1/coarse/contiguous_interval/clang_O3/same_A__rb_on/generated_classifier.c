#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 17

#define START_STATE 16

#define DEAD_STATE 15

#define NUM_CLASSES 8

#define NUM_TOKENS 16

static const char *CLASS_NAMES[8] = {
    "ACC::DELIM::rb0",
    "ACC::IDENT::rb1",
    "ACC::NUMBER::rb1",
    "ACC::OPERATOR::rb0",
    "ACC::OPERATOR::rb1",
    "ACC::WHITESPACE::rb0",
    "DEAD",
    "NONACC::START",
};

static const char *TOKEN_NAMES[16] = {
    "ASSIGN",
    "COMMA",
    "EQEQ",
    "IDENT",
    "INT",
    "LBRACE",
    "LPAR",
    "MINUS",
    "NO_TOKEN",
    "PLUS",
    "RBRACE",
    "RPAR",
    "SEMI",
    "SLASH",
    "STAR",
    "WS",
};

static const int CLASS_ACCEPTING[8] = {
    1, 1, 1, 1, 1, 1, 0, 0,
};

static const int CLASS_DEAD[8] = {
    0, 0, 0, 0, 0, 0, 1, 0,
};

static const int TOKEN_OF_STATE[17] = {
    6, 11, 5, 10, 12, 1, 3, 4, 2, 9, 7, 14, 13, 0, 15, 8,
    8,
};

static const int DEAD_OF_STATE[17] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    0,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    if (s <= 5) return 0;
    if (s <= 6) return 1;
    if (s <= 7) return 2;
    if (s <= 12) return 3;
    if (s <= 13) return 4;
    if (s <= 14) return 5;
    if (s <= 15) return 6;
    return 7;
}
