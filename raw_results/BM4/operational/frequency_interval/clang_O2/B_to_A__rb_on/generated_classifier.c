#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 13

#define START_STATE 11

#define DEAD_STATE 10

#define NUM_CLASSES 6

#define NUM_TOKENS 10

static const char *CLASS_NAMES[6] = {
    "ACC::OPERATIONAL::rb0",
    "ACC::OPERATIONAL::rb1",
    "NONACC::STRING_BODY",
    "DEAD",
    "NONACC::START",
    "NONACC::STRING_ESCAPE",
};

static const char *TOKEN_NAMES[10] = {
    "ATTR_STRING",
    "EQ",
    "GT",
    "LT",
    "LT_SLASH",
    "NAME",
    "NO_TOKEN",
    "SLASH_GT",
    "TEXT",
    "WS",
};

static const int CLASS_ACCEPTING[6] = {
    1, 1, 0, 0, 0, 0,
};

static const int CLASS_DEAD[6] = {
    0, 0, 0, 1, 0, 0,
};

static const int TOKEN_OF_STATE[13] = {
    9, 8, 3, 4, 2, 7, 1, 0, 5, 6, 6, 6, 6,
};

static const int DEAD_OF_STATE[13] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    if (s <= 7) return 0;
    if (s <= 8) return 1;
    if (s <= 9) return 2;
    if (s <= 10) return 3;
    if (s <= 11) return 4;
    return 5;
}
