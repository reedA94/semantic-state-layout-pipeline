#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 56

#define START_STATE 0

#define DEAD_STATE 27

#define NUM_CLASSES 2

#define NUM_TOKENS 28

static const char *CLASS_NAMES[2] = {
    "NONACC_OR_DEAD",
    "ACCEPTING",
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

static const int CLASS_ACCEPTING[2] = {
    0, 1,
};

static const int CLASS_DEAD[2] = {
    1, 0,
};

static const int TOKEN_OF_STATE[56] = {
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 7, 8, 6, 27,
    26, 26, 2, 4, 5, 0, 21, 19, 25, 24, 1, 3, 18, 23, 17, 22,
    10, 9, 13, 11, 16, 14, 12, 15,
};

static const int DEAD_OF_STATE[56] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    if (s <= 27) {
        return 0;
    } else {
        return 1;
    }
}

