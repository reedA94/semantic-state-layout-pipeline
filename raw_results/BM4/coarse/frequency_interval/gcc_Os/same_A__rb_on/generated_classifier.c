#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 13

#define START_STATE 10

#define DEAD_STATE 2

#define NUM_CLASSES 9

#define NUM_TOKENS 10

static const char *CLASS_NAMES[9] = {
    "ACC::TEXT::rb0",
    "ACC::NAME::rb1",
    "DEAD",
    "ACC::TAG_PUNCT::rb0",
    "ACC::WHITESPACE::rb0",
    "ACC::STRING::rb0",
    "NONACC::START",
    "NONACC::STRING_BODY",
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

static const int CLASS_ACCEPTING[9] = {
    1, 1, 0, 1, 1, 1, 0, 0, 0,
};

static const int CLASS_DEAD[9] = {
    0, 0, 1, 0, 0, 0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[13] = {
    8, 5, 6, 3, 4, 2, 7, 1, 9, 0, 6, 6, 6,
};

static const int DEAD_OF_STATE[13] = {
    0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    if (s <= 0) return 0;
    if (s <= 1) return 1;
    if (s <= 2) return 2;
    if (s <= 7) return 3;
    if (s <= 8) return 4;
    if (s <= 9) return 5;
    if (s <= 10) return 6;
    if (s <= 11) return 7;
    return 8;
}
