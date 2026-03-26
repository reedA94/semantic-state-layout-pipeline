#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 30

#define START_STATE 5

#define DEAD_STATE 8

#define NUM_CLASSES 11

#define NUM_TOKENS 13

static const char *CLASS_NAMES[11] = {
    "NONACC::STRING_ESCAPE",
    "NONACC::LITERAL_PREFIX",
    "ACC::LITERAL::rb1",
    "NONACC::NUMBER_PREFIX",
    "NONACC::START",
    "ACC::NUMBER::rb1",
    "ACC::PUNCT::rb0",
    "DEAD",
    "NONACC::STRING_BODY",
    "ACC::STRING::rb0",
    "ACC::WHITESPACE::rb0",
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
    0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1,
};

static const int CLASS_DEAD[11] = {
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
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
    0, 1, 2, 1, 3, 4, 5, 6, 7, 1, 1, 6, 1, 8, 1, 9,
    6, 5, 1, 2, 5, 6, 1, 1, 3, 10, 2, 6, 1, 6,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    return CLASS_OF_STATE[s];
}
