#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 14

#define START_STATE 12

#define DEAD_STATE 2

#define NUM_CLASSES 14

#define NUM_TOKENS 11

static const char *CLASS_NAMES[14] = {
    "ACC::IDENT::tok=IDENT::aux=IDENT::prio0::rb1",
    "ACC::VALUE::tok=BARE_VALUE::aux=VALUE::prio0::rb1",
    "DEAD",
    "ACC::COMMENT::tok=COMMENT::aux=COMMENT::prio0::rb0",
    "ACC::WHITESPACE::tok=WS::aux=WS::prio0::rb0",
    "ACC::PUNCT::tok=EQ::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=LBRACK::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=RBRACK::aux=PUNCT::prio0::rb0",
    "NONACC::STRING_BODY",
    "ACC::STRING::tok=STRING::aux=STRING::prio0::rb0",
    "ACC::NUMBER::tok=NUMBER::aux=NUMBER::prio0::rb1",
    "ACC::PUNCT::tok=DOT::aux=PUNCT::prio0::rb0",
    "NONACC::START",
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

static const int CLASS_ACCEPTING[14] = {
    1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0,
};

static const int CLASS_DEAD[14] = {
    0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[14] = {
    4, 0, 6, 1, 10, 3, 5, 8, 6, 9, 7, 2, 6, 6,
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
    if (s <= 5) return 5;
    if (s <= 6) return 6;
    if (s <= 7) return 7;
    if (s <= 8) return 8;
    if (s <= 9) return 9;
    if (s <= 10) return 10;
    if (s <= 11) return 11;
    if (s <= 12) return 12;
    return 13;
}
