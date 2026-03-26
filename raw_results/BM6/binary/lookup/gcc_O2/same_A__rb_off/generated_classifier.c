#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 56

#define START_STATE 14

#define DEAD_STATE 2

#define NUM_CLASSES 2

#define NUM_TOKENS 28

static const char *CLASS_NAMES[2] = {
    "ACCEPTING",
    "NONACC_OR_DEAD",
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
    1, 0,
};

static const int CLASS_DEAD[2] = {
    0, 1,
};

static const int TOKEN_OF_STATE[56] = {
    12, 20, 20, 27, 16, 20, 13, 20, 23, 1, 7, 20, 11, 2, 20, 20,
    5, 20, 20, 20, 9, 20, 20, 26, 20, 19, 0, 20, 3, 20, 10, 6,
    20, 20, 20, 20, 14, 20, 20, 25, 26, 20, 4, 20, 18, 20, 24, 22,
    20, 20, 21, 8, 20, 20, 17, 15,
};

static const int DEAD_OF_STATE[56] = {
    0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

static const int CLASS_OF_STATE[56] = {
    0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1,
    0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0,
    1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0,
    1, 1, 0, 0, 1, 1, 0, 0,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    return CLASS_OF_STATE[s];
}
