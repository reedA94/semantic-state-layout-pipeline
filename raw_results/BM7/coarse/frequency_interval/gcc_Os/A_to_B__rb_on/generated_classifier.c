#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 14

#define START_STATE 11

#define DEAD_STATE 2

#define NUM_CLASSES 11

#define NUM_TOKENS 11

static const char *CLASS_NAMES[11] = {
    "ACC::VALUE::rb1",
    "ACC::IDENT::rb1",
    "DEAD",
    "ACC::COMMENT::rb0",
    "ACC::WHITESPACE::rb0",
    "ACC::PUNCT::rb0",
    "ACC::NUMBER::rb1",
    "ACC::STRING::rb0",
    "NONACC::START",
    "NONACC::STRING_BODY",
    "NONACC::STRING_ESCAPE",
};

static const char *TOKEN_NAMES[11] = {
    "BARE_VALUE",
    "COMMENT",
    "DOT",
    "EQ",
    "IDENT",
    "LBRACK",
    "NO_TOKEN",
    "NUMBER",
    "RBRACK",
    "STRING",
    "WS",
};

static const int CLASS_ACCEPTING[11] = {
    1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0,
};

static const int CLASS_DEAD[11] = {
    0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[14] = {
    0, 4, 6, 1, 10, 5, 8, 3, 2, 7, 9, 6, 6, 6,
};

static const int DEAD_OF_STATE[14] = {
    0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    if (s <= 0) return 0;
    if (s <= 1) return 1;
    if (s <= 2) return 2;
    if (s <= 3) return 3;
    if (s <= 4) return 4;
    if (s <= 8) return 5;
    if (s <= 9) return 6;
    if (s <= 10) return 7;
    if (s <= 11) return 8;
    if (s <= 12) return 9;
    return 10;
}
