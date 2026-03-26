#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 30

#define START_STATE 29

#define DEAD_STATE 1

#define NUM_CLASSES 8

#define NUM_TOKENS 13

static const char *CLASS_NAMES[8] = {
    "NONACC::STRING_BODY",
    "DEAD",
    "ACC::OPERATIONAL::rb0",
    "ACC::OPERATIONAL::rb1",
    "NONACC::LITERAL_PREFIX",
    "NONACC::NUMBER_PREFIX",
    "NONACC::STRING_ESCAPE",
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

static const int CLASS_ACCEPTING[8] = {
    0, 0, 1, 1, 0, 0, 0, 0,
};

static const int CLASS_DEAD[8] = {
    0, 1, 0, 0, 0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[30] = {
    5, 5, 12, 10, 3, 8, 4, 9, 0, 1, 7, 7, 7, 11, 2, 6,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
};

static const int DEAD_OF_STATE[30] = {
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    if (s <= 0) return 0;
    if (s <= 1) return 1;
    if (s <= 9) return 2;
    if (s <= 15) return 3;
    if (s <= 25) return 4;
    if (s <= 27) return 5;
    if (s <= 28) return 6;
    return 7;
}
