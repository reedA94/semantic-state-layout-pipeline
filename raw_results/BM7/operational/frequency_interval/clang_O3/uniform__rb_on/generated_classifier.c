#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 14

#define START_STATE 12

#define DEAD_STATE 10

#define NUM_CLASSES 6

#define NUM_TOKENS 11

static const char *CLASS_NAMES[6] = {
    "ACC::OPERATIONAL::rb1",
    "ACC::OPERATIONAL::rb0",
    "DEAD",
    "NONACC::STRING_BODY",
    "NONACC::START",
    "NONACC::STRING_ESCAPE",
};

static const char *TOKEN_NAMES[11] = {
    "BARE_VALUE",
    "COMMENT",
    "DOT",
    "EQ",
    "IDENT",
    "LBRACK",
    "NO_TOKEN",
    "NUMBER",
    "RBRACK",
    "STRING",
    "WS",
};

static const int CLASS_ACCEPTING[6] = {
    1, 1, 0, 0, 0, 0,
};

static const int CLASS_DEAD[6] = {
    0, 0, 1, 0, 0, 0,
};

static const int TOKEN_OF_STATE[14] = {
    4, 0, 7, 10, 9, 1, 5, 8, 3, 2, 6, 6, 6, 6,
};

static const int DEAD_OF_STATE[14] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    if (s <= 2) return 0;
    if (s <= 9) return 1;
    if (s <= 10) return 2;
    if (s <= 11) return 3;
    if (s <= 12) return 4;
    return 5;
}
