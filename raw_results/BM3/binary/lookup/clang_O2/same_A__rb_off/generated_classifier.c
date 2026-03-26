#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 30

#define START_STATE 5

#define DEAD_STATE 8

#define NUM_CLASSES 2

#define NUM_TOKENS 13

static const char *CLASS_NAMES[2] = {
    "NONACC_OR_DEAD",
    "ACCEPTING",
};

static const char *TOKEN_NAMES[13] = {
    "COLON",
    "COMMA",
    "FALSE",
    "LBRACE",
    "LBRACK",
    "NO_TOKEN",
    "NULL",
    "NUMBER",
    "RBRACE",
    "RBRACK",
    "STRING",
    "TRUE",
    "WS",
};

static const int CLASS_ACCEPTING[2] = {
    0, 1,
};

static const int CLASS_DEAD[2] = {
    1, 0,
};

static const int TOKEN_OF_STATE[30] = {
    5, 5, 2, 5, 5, 5, 7, 9, 5, 5, 5, 4, 5, 5, 5, 10,
    3, 7, 5, 6, 7, 1, 5, 5, 5, 12, 11, 8, 5, 0,
};

static const int DEAD_OF_STATE[30] = {
    0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static const int CLASS_OF_STATE[30] = {
    0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1,
    1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    return CLASS_OF_STATE[s];
}
