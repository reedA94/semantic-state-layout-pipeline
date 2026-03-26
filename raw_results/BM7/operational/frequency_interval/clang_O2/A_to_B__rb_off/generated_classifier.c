#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 14

#define START_STATE 11

#define DEAD_STATE 10

#define NUM_CLASSES 5

#define NUM_TOKENS 11

static const char *CLASS_NAMES[5] = {
    "ACC::OPERATIONAL",
    "DEAD",
    "NONACC::START",
    "NONACC::STRING_BODY",
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

static const int CLASS_ACCEPTING[5] = {
    1, 0, 0, 0, 0,
};

static const int CLASS_DEAD[5] = {
    0, 1, 0, 0, 0,
};

static const int TOKEN_OF_STATE[14] = {
    10, 4, 0, 7, 9, 1, 5, 8, 3, 2, 6, 6, 6, 6,
};

static const int DEAD_OF_STATE[14] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    if (s <= 9) return 0;
    if (s <= 10) return 1;
    if (s <= 11) return 2;
    if (s <= 12) return 3;
    return 4;
}
