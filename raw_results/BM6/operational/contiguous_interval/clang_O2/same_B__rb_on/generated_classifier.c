#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 56

#define START_STATE 51

#define DEAD_STATE 28

#define NUM_CLASSES 8

#define NUM_TOKENS 28

static const char *CLASS_NAMES[8] = {
    "ACC::OPERATIONAL::rb0",
    "ACC::OPERATIONAL::rb1",
    "DEAD",
    "NONACC::KEYWORD_PREFIX",
    "NONACC::NUMBER_PREFIX",
    "NONACC::START",
    "NONACC::STRING_BODY",
    "NONACC::STRING_ESCAPE",
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

static const int CLASS_ACCEPTING[8] = {
    1, 1, 0, 0, 0, 0, 0, 0,
};

static const int CLASS_DEAD[8] = {
    0, 0, 1, 0, 0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[56] = {
    27, 26, 26, 2, 5, 0, 21, 19, 25, 24, 1, 3, 18, 23, 17, 22,
    7, 8, 6, 4, 10, 9, 13, 11, 16, 14, 12, 15, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20,
};

static const int DEAD_OF_STATE[56] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    if (s <= 15) return 0;
    if (s <= 27) return 1;
    if (s <= 28) return 2;
    if (s <= 49) return 3;
    if (s <= 50) return 4;
    if (s <= 51) return 5;
    if (s <= 53) return 6;
    return 7;
}
