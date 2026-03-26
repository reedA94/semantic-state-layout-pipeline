#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 30

#define START_STATE 14

#define DEAD_STATE 29

#define NUM_CLASSES 2

#define NUM_TOKENS 13

static const char *CLASS_NAMES[2] = {
    "ACCEPTING",
    "NONACC_OR_DEAD",
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
    1, 0,
};

static const int CLASS_DEAD[2] = {
    0, 1,
};

static const int TOKEN_OF_STATE[30] = {
    12, 10, 7, 7, 7, 11, 2, 6, 3, 8, 4, 9, 0, 1, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
};

static const int DEAD_OF_STATE[30] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    if (s <= 13) return 0;
    return 1;
}
