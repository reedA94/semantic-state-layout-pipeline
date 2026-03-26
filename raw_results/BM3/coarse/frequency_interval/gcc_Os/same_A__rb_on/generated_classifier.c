#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 30

#define START_STATE 29

#define DEAD_STATE 1

#define NUM_CLASSES 11

#define NUM_TOKENS 13

static const char *CLASS_NAMES[11] = {
    "NONACC::STRING_BODY",
    "DEAD",
    "ACC::PUNCT::rb0",
    "ACC::NUMBER::rb1",
    "ACC::STRING::rb0",
    "NONACC::LITERAL_PREFIX",
    "NONACC::NUMBER_PREFIX",
    "NONACC::STRING_ESCAPE",
    "ACC::LITERAL::rb1",
    "ACC::WHITESPACE::rb0",
    "NONACC::START",
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
    0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0,
};

static const int CLASS_DEAD[11] = {
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[30] = {
    5, 5, 3, 8, 4, 9, 0, 1, 7, 7, 7, 10, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 11, 2, 6, 12, 5,
};

static const int DEAD_OF_STATE[30] = {
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    if (s <= 0) return 0;
    if (s <= 1) return 1;
    if (s <= 7) return 2;
    if (s <= 10) return 3;
    if (s <= 11) return 4;
    if (s <= 21) return 5;
    if (s <= 23) return 6;
    if (s <= 24) return 7;
    if (s <= 27) return 8;
    if (s <= 28) return 9;
    return 10;
}
