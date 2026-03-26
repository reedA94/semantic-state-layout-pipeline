#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 13

#define START_STATE 1

#define DEAD_STATE 0

#define NUM_CLASSES 9

#define NUM_TOKENS 10

static const char *CLASS_NAMES[9] = {
    "DEAD",
    "NONACC::START",
    "NONACC::STRING_BODY",
    "NONACC::STRING_ESCAPE",
    "ACC::NAME::rb1",
    "ACC::STRING::rb0",
    "ACC::TAG_PUNCT::rb0",
    "ACC::TEXT::rb0",
    "ACC::WHITESPACE::rb0",
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
    0, 0, 0, 0, 1, 1, 1, 1, 1,
};

static const int CLASS_DEAD[9] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[13] = {
    6, 6, 6, 6, 5, 0, 3, 4, 2, 7, 1, 8, 9,
};

static const int DEAD_OF_STATE[13] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    if (s <= 0) {
        return 0;
    } else {
        if (s <= 3) {
            if (s <= 1) {
                return 1;
            } else {
                if (s <= 2) {
                    return 2;
                } else {
                    return 3;
                }
            }
        } else {
            if (s <= 5) {
                if (s <= 4) {
                    return 4;
                } else {
                    return 5;
                }
            } else {
                if (s <= 10) {
                    return 6;
                } else {
                    if (s <= 11) {
                        return 7;
                    } else {
                        return 8;
                    }
                }
            }
        }
    }
}

