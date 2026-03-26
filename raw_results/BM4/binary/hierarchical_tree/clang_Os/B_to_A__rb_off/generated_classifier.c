#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 13

#define START_STATE 0

#define DEAD_STATE 3

#define NUM_CLASSES 2

#define NUM_TOKENS 10

static const char *CLASS_NAMES[2] = {
    "NONACC_OR_DEAD",
    "ACCEPTING",
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
    0, 1,
};

static const int CLASS_DEAD[2] = {
    1, 0,
};

static const int TOKEN_OF_STATE[13] = {
    6, 6, 6, 6, 9, 8, 3, 4, 2, 7, 1, 5, 0,
};

static const int DEAD_OF_STATE[13] = {
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    if (s <= 3) {
        return 0;
    } else {
        return 1;
    }
}

