#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 14

#define START_STATE 10

#define DEAD_STATE 13

#define NUM_CLASSES 14

#define NUM_TOKENS 11

static const char *CLASS_NAMES[14] = {
    "ACC::WHITESPACE::tok=WS::aux=WS::prio0::rb0",
    "ACC::PUNCT::tok=RBRACK::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=LBRACK::aux=PUNCT::prio0::rb0",
    "NONACC::STRING_BODY",
    "ACC::PUNCT::tok=EQ::aux=PUNCT::prio0::rb0",
    "ACC::IDENT::tok=IDENT::aux=IDENT::prio0::rb1",
    "ACC::VALUE::tok=BARE_VALUE::aux=VALUE::prio0::rb1",
    "ACC::STRING::tok=STRING::aux=STRING::prio0::rb0",
    "ACC::COMMENT::tok=COMMENT::aux=COMMENT::prio0::rb0",
    "ACC::NUMBER::tok=NUMBER::aux=NUMBER::prio0::rb1",
    "NONACC::START",
    "ACC::PUNCT::tok=DOT::aux=PUNCT::prio0::rb0",
    "NONACC::STRING_ESCAPE",
    "DEAD",
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
    1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0,
};

static const int CLASS_DEAD[14] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
};

static const int TOKEN_OF_STATE[14] = {
    10, 8, 5, 6, 3, 4, 0, 9, 1, 7, 6, 2, 6, 6,
};

static const int DEAD_OF_STATE[14] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
};

static const int CLASS_OF_STATE[14] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    return CLASS_OF_STATE[s];
}
