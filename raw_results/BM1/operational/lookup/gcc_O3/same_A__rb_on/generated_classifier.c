#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 17

#define START_STATE 14

#define DEAD_STATE 3

#define NUM_CLASSES 4

#define NUM_TOKENS 16

static const char *CLASS_NAMES[4] = {
    "ACC::OPERATIONAL::rb1",
    "ACC::OPERATIONAL::rb0",
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

static const int CLASS_ACCEPTING[4] = {
    1, 1, 0, 0,
};

static const int CLASS_DEAD[4] = {
    0, 0, 1, 0,
};

static const int TOKEN_OF_STATE[17] = {
    4, 12, 0, 8, 6, 3, 15, 9, 11, 1, 7, 14, 13, 2, 8, 10,
    5,
};

static const int DEAD_OF_STATE[17] = {
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,
};

static const int CLASS_OF_STATE[17] = {
    0, 1, 0, 2, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1,
    1,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    return CLASS_OF_STATE[s];
}
