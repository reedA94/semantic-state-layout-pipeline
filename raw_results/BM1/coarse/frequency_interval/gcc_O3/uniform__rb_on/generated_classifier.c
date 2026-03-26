#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 17

#define START_STATE 16

#define DEAD_STATE 1

#define NUM_CLASSES 8

#define NUM_TOKENS 16

static const char *CLASS_NAMES[8] = {
    "ACC::IDENT::rb1",
    "DEAD",
    "ACC::WHITESPACE::rb0",
    "ACC::DELIM::rb0",
    "ACC::NUMBER::rb1",
    "ACC::OPERATOR::rb1",
    "ACC::OPERATOR::rb0",
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
    1, 0, 1, 1, 1, 1, 1, 0,
};

static const int CLASS_DEAD[8] = {
    0, 1, 0, 0, 0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[17] = {
    3, 8, 15, 6, 11, 5, 10, 12, 1, 4, 0, 2, 9, 7, 14, 13,
    8,
};

static const int DEAD_OF_STATE[17] = {
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    if (s <= 0) return 0;
    if (s <= 1) return 1;
    if (s <= 2) return 2;
    if (s <= 8) return 3;
    if (s <= 9) return 4;
    if (s <= 10) return 5;
    if (s <= 15) return 6;
    return 7;
}
