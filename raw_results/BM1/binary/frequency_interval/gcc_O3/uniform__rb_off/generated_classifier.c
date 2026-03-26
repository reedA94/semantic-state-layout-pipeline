#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 17

#define START_STATE 15

#define DEAD_STATE 16

#define NUM_CLASSES 2

#define NUM_TOKENS 16

static const char *CLASS_NAMES[2] = {
    "ACCEPTING",
    "NONACC_OR_DEAD",
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

static const int CLASS_ACCEPTING[2] = {
    1, 0,
};

static const int CLASS_DEAD[2] = {
    0, 1,
};

static const int TOKEN_OF_STATE[17] = {
    3, 4, 0, 2, 9, 7, 14, 13, 15, 6, 11, 5, 10, 12, 1, 8,
    8,
};

static const int DEAD_OF_STATE[17] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    if (s <= 14) return 0;
    return 1;
}
