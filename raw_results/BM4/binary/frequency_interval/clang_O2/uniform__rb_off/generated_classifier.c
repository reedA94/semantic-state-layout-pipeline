#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 13

#define START_STATE 9

#define DEAD_STATE 12

#define NUM_CLASSES 2

#define NUM_TOKENS 10

static const char *CLASS_NAMES[2] = {
    "ACCEPTING",
    "NONACC_OR_DEAD",
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

static const int CLASS_ACCEPTING[2] = {
    1, 0,
};

static const int CLASS_DEAD[2] = {
    0, 1,
};

static const int TOKEN_OF_STATE[13] = {
    9, 8, 3, 4, 2, 7, 1, 5, 0, 6, 6, 6, 6,
};

static const int DEAD_OF_STATE[13] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    if (s <= 8) return 0;
    return 1;
}
