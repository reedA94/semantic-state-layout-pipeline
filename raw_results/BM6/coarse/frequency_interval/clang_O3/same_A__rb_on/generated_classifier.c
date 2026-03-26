#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 56

#define START_STATE 53

#define DEAD_STATE 1

#define NUM_CLASSES 15

#define NUM_TOKENS 28

static const char *CLASS_NAMES[15] = {
    "ACC::IDENT::rb1",
    "DEAD",
    "ACC::WHITESPACE::rb0",
    "ACC::COMMENT::rb0",
    "NONACC::KEYWORD_PREFIX",
    "ACC::PUNCT::rb0",
    "NONACC::STRING_BODY",
    "ACC::OPERATOR::rb1",
    "ACC::NUMBER::rb1",
    "ACC::OPERATOR::rb0",
    "ACC::STRING::rb0",
    "ACC::KEYWORD::rb1",
    "NONACC::NUMBER_PREFIX",
    "NONACC::START",
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

static const int CLASS_ACCEPTING[15] = {
    1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0,
};

static const int CLASS_DEAD[15] = {
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[56] = {
    7, 20, 27, 2, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 0, 1, 3, 18, 23, 17, 22,
    20, 20, 4, 8, 6, 5, 21, 19, 25, 24, 26, 26, 10, 9, 13, 11,
    16, 14, 12, 15, 20, 20, 20, 20,
};

static const int DEAD_OF_STATE[56] = {
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    if (s <= 0) return 0;
    if (s <= 1) return 1;
    if (s <= 2) return 2;
    if (s <= 3) return 3;
    if (s <= 24) return 4;
    if (s <= 31) return 5;
    if (s <= 33) return 6;
    if (s <= 34) return 7;
    if (s <= 36) return 8;
    if (s <= 41) return 9;
    if (s <= 43) return 10;
    if (s <= 51) return 11;
    if (s <= 52) return 12;
    if (s <= 53) return 13;
    return 14;
}
