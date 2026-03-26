#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 56

#define START_STATE 23

#define DEAD_STATE 0

#define NUM_CLASSES 7

#define NUM_TOKENS 28

static const char *CLASS_NAMES[7] = {
    "DEAD",
    "NONACC::KEYWORD_PREFIX",
    "NONACC::NUMBER_PREFIX",
    "NONACC::START",
    "NONACC::STRING_BODY",
    "NONACC::STRING_ESCAPE",
    "ACC::OPERATIONAL",
};

static const char *TOKEN_NAMES[28] = {
    "COLON",
    "COMMA",
    "COMMENT",
    "DOT",
    "EQ",
    "EQEQ",
    "FLOAT",
    "IDENT",
    "INT",
    "KW_CLASS",
    "KW_DEF",
    "KW_ELSE",
    "KW_FOR",
    "KW_IF",
    "KW_IMPORT",
    "KW_IN",
    "KW_RETURN",
    "LBRACK",
    "LPAR",
    "MINUS",
    "NO_TOKEN",
    "PLUS",
    "RBRACK",
    "RPAR",
    "SLASH",
    "STAR",
    "STRING",
    "WS",
};

static const int CLASS_ACCEPTING[7] = {
    0, 0, 0, 0, 0, 0, 1,
};

static const int CLASS_DEAD[7] = {
    1, 0, 0, 0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[56] = {
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 7, 8, 6, 27,
    26, 26, 2, 4, 5, 0, 21, 19, 25, 24, 1, 3, 18, 23, 17, 22,
    10, 9, 13, 11, 16, 14, 12, 15,
};

static const int DEAD_OF_STATE[56] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    if (s <= 0) {
        return 0;
    } else {
        if (s <= 27) {
            if (s <= 22) {
                if (s <= 21) {
                    return 1;
                } else {
                    return 2;
                }
            } else {
                if (s <= 23) {
                    return 3;
                } else {
                    if (s <= 25) {
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

