#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 13

#define START_STATE 10

#define DEAD_STATE 11

#define NUM_CLASSES 9

#define NUM_TOKENS 10

static const char *CLASS_NAMES[9] = {
    "ACC::WHITESPACE",
    "NONACC::STRING_ESCAPE",
    "NONACC::STRING_BODY",
    "ACC::TAG_PUNCT",
    "ACC::STRING",
    "ACC::TEXT",
    "ACC::NAME",
    "NONACC::START",
    "DEAD",
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
    1, 0, 0, 1, 1, 1, 1, 0, 0,
};

static const int CLASS_DEAD[9] = {
    0, 0, 0, 0, 0, 0, 0, 0, 1,
};

static const int TOKEN_OF_STATE[13] = {
    9, 6, 6, 2, 0, 8, 3, 1, 5, 4, 6, 6, 7,
};

static const int DEAD_OF_STATE[13] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
};

static const int CLASS_OF_STATE[13] = {
    0, 1, 2, 3, 4, 5, 3, 3, 6, 3, 7, 8, 3,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    return CLASS_OF_STATE[s];
}
