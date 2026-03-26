#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 66

#define START_STATE 33

#define DEAD_STATE 0

#define NUM_CLASSES 8

#define NUM_TOKENS 31

static const char *CLASS_NAMES[8] = {
    "DEAD",
    "NONACC::KEYWORD_PREFIX",
    "NONACC::NUMBER_PREFIX",
    "NONACC::START",
    "NONACC::STRING_BODY",
    "NONACC::STRING_ESCAPE",
    "ACC::OPERATIONAL::rb1",
    "ACC::OPERATIONAL::rb0",
};

static const char *TOKEN_NAMES[31] = {
    "COMMA",
    "DOT",
    "EQ",
    "FLOAT",
    "GE",
    "GT",
    "IDENT",
    "INT",
    "KW_AND",
    "KW_FROM",
    "KW_INSERT",
    "KW_INTO",
    "KW_OR",
    "KW_SELECT",
    "KW_SET",
    "KW_UPDATE",
    "KW_VALUES",
    "KW_WHERE",
    "LE",
    "LPAR",
    "LT",
    "MINUS",
    "NEQ",
    "NO_TOKEN",
    "PLUS",
    "RPAR",
    "SEMI",
    "SLASH",
    "STAR",
    "STRING",
    "WS",
};

static const int CLASS_ACCEPTING[8] = {
    0, 0, 0, 0, 0, 0, 1, 1,
};

static const int CLASS_DEAD[8] = {
    1, 0, 0, 0, 0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[66] = {
    23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
    23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
    23, 23, 23, 23, 6, 7, 3, 20, 5, 13, 9, 17, 8, 12, 10, 11,
    16, 15, 14, 30, 29, 2, 18, 4, 22, 24, 21, 28, 27, 0, 26, 19,
    25, 1,
};

static const int DEAD_OF_STATE[66] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    if (s <= 0) {
        return 0;
    } else {
        if (s <= 35) {
            if (s <= 32) {
                if (s <= 31) {
                    return 1;
                } else {
                    return 2;
                }
            } else {
                if (s <= 33) {
                    return 3;
                } else {
                    if (s <= 34) {
                        return 4;
                    } else {
                        return 5;
                    }
                }
            }
        } else {
            if (s <= 50) {
                return 6;
            } else {
                return 7;
            }
        }
    }
}

