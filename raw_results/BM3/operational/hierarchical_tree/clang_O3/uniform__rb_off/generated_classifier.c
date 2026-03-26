#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 30

#define START_STATE 13

#define DEAD_STATE 0

#define NUM_CLASSES 7

#define NUM_TOKENS 13

static const char *CLASS_NAMES[7] = {
    "DEAD",
    "NONACC::LITERAL_PREFIX",
    "NONACC::NUMBER_PREFIX",
    "NONACC::START",
    "NONACC::STRING_BODY",
    "NONACC::STRING_ESCAPE",
    "ACC::OPERATIONAL",
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

static const int CLASS_ACCEPTING[7] = {
    0, 0, 0, 0, 0, 0, 1,
};

static const int CLASS_DEAD[7] = {
    1, 0, 0, 0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[30] = {
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    12, 10, 7, 7, 7, 11, 2, 6, 3, 8, 4, 9, 0, 1,
};

static const int DEAD_OF_STATE[30] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    if (s <= 0) {
        return 0;
    } else {
        if (s <= 15) {
            if (s <= 12) {
                if (s <= 10) {
                    return 1;
                } else {
                    return 2;
                }
            } else {
                if (s <= 13) {
                    return 3;
                } else {
                    if (s <= 14) {
                        return 4;
                    } else {
                        return 5;
                    }
                }
            }
        } else {
            return 6;
        }
    }
}

