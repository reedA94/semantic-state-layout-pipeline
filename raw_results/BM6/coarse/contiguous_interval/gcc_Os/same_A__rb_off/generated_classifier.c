#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 56

#define START_STATE 51

#define DEAD_STATE 28

#define NUM_CLASSES 14

#define NUM_TOKENS 28

static const char *CLASS_NAMES[14] = {
    "ACC::COMMENT",
    "ACC::IDENT",
    "ACC::KEYWORD",
    "ACC::NUMBER",
    "ACC::OPERATOR",
    "ACC::PUNCT",
    "ACC::STRING",
    "ACC::WHITESPACE",
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

static const int CLASS_ACCEPTING[14] = {
    1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
};

static const int CLASS_DEAD[14] = {
    0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[56] = {
    2, 7, 10, 9, 13, 11, 16, 14, 12, 15, 8, 6, 4, 5, 21, 19,
    25, 24, 0, 1, 3, 18, 23, 17, 22, 26, 26, 27, 20, 20, 20, 20,
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
    if (s <= 0) return 0;
    if (s <= 1) return 1;
    if (s <= 9) return 2;
    if (s <= 11) return 3;
    if (s <= 17) return 4;
    if (s <= 24) return 5;
    if (s <= 26) return 6;
    if (s <= 27) return 7;
    if (s <= 28) return 8;
    if (s <= 49) return 9;
    if (s <= 50) return 10;
    if (s <= 51) return 11;
    if (s <= 53) return 12;
    return 13;
}
