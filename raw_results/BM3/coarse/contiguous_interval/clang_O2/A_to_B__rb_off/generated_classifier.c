#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 30

#define START_STATE 27

#define DEAD_STATE 14

#define NUM_CLASSES 11

#define NUM_TOKENS 13

static const char *CLASS_NAMES[11] = {
    "ACC::LITERAL",
    "ACC::NUMBER",
    "ACC::PUNCT",
    "ACC::STRING",
    "ACC::WHITESPACE",
    "DEAD",
    "NONACC::LITERAL_PREFIX",
    "NONACC::NUMBER_PREFIX",
    "NONACC::START",
    "NONACC::STRING_BODY",
    "NONACC::STRING_ESCAPE",
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

static const int CLASS_ACCEPTING[11] = {
    1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
};

static const int CLASS_DEAD[11] = {
    0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[30] = {
    11, 2, 6, 7, 7, 7, 3, 8, 4, 9, 0, 1, 10, 12, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
};

static const int DEAD_OF_STATE[30] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    if (s <= 2) return 0;
    if (s <= 5) return 1;
    if (s <= 11) return 2;
    if (s <= 12) return 3;
    if (s <= 13) return 4;
    if (s <= 14) return 5;
    if (s <= 24) return 6;
    if (s <= 26) return 7;
    if (s <= 27) return 8;
    if (s <= 28) return 9;
    return 10;
}
