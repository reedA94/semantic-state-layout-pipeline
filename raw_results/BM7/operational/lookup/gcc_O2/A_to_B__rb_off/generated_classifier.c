#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 14

#define START_STATE 10

#define DEAD_STATE 13

#define NUM_CLASSES 5

#define NUM_TOKENS 11

static const char *CLASS_NAMES[5] = {
    "ACC::OPERATIONAL",
    "NONACC::STRING_BODY",
    "NONACC::START",
    "NONACC::STRING_ESCAPE",
    "DEAD",
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
    0, 0, 0, 0, 1,
};

static const int TOKEN_OF_STATE[14] = {
    10, 8, 5, 6, 3, 4, 0, 9, 1, 7, 6, 2, 6, 6,
};

static const int DEAD_OF_STATE[14] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
};

static const int CLASS_OF_STATE[14] = {
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 2, 0, 3, 4,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    return CLASS_OF_STATE[s];
}
