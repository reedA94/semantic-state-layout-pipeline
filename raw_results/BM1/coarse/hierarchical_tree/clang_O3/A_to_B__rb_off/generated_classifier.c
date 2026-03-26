#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 17

#define START_STATE 1

#define DEAD_STATE 0

#define NUM_CLASSES 7

#define NUM_TOKENS 16

static const char *CLASS_NAMES[7] = {
    "DEAD",
    "NONACC::START",
    "ACC::DELIM",
    "ACC::IDENT",
    "ACC::NUMBER",
    "ACC::OPERATOR",
    "ACC::WHITESPACE",
};

static const char *TOKEN_NAMES[16] = {
    "ASSIGN",
    "COMMA",
    "EQEQ",
    "IDENT",
    "INT",
    "LBRACE",
    "LPAR",
    "MINUS",
    "NO_TOKEN",
    "PLUS",
    "RBRACE",
    "RPAR",
    "SEMI",
    "SLASH",
    "STAR",
    "WS",
};

static const int CLASS_ACCEPTING[7] = {
    0, 0, 1, 1, 1, 1, 1,
};

static const int CLASS_DEAD[7] = {
    1, 0, 0, 0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[17] = {
    8, 8, 6, 11, 5, 10, 12, 1, 3, 4, 0, 2, 9, 7, 14, 13,
    15,
};

static const int DEAD_OF_STATE[17] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    if (s <= 0) {
        return 0;
    } else {
        if (s <= 1) {
            return 1;
        } else {
            if (s <= 8) {
                if (s <= 7) {
                    return 2;
                } else {
                    return 3;
                }
            } else {
                if (s <= 9) {
                    return 4;
                } else {
                    if (s <= 15) {
                        return 5;
                    } else {
                        return 6;
                    }
                }
            }
        }
    }
}

