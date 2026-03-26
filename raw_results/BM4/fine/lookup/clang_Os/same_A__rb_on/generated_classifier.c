#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 13

#define START_STATE 10

#define DEAD_STATE 11

#define NUM_CLASSES 13

#define NUM_TOKENS 10

static const char *CLASS_NAMES[13] = {
    "ACC::WHITESPACE::tok=WS::aux=WS::prio0::rb0",
    "NONACC::STRING_ESCAPE",
    "NONACC::STRING_BODY",
    "ACC::TAG_PUNCT::tok=GT::aux=TAG::prio0::rb0",
    "ACC::STRING::tok=ATTR_STRING::aux=STRING::prio0::rb0",
    "ACC::TEXT::tok=TEXT::aux=TEXT::prio0::rb0",
    "ACC::TAG_PUNCT::tok=LT::aux=TAG::prio0::rb0",
    "ACC::TAG_PUNCT::tok=EQ::aux=TAG::prio0::rb0",
    "ACC::NAME::tok=NAME::aux=NAME::prio0::rb1",
    "ACC::TAG_PUNCT::tok=LT_SLASH::aux=TAG::prio0::rb0",
    "NONACC::START",
    "DEAD",
    "ACC::TAG_PUNCT::tok=SLASH_GT::aux=TAG::prio0::rb0",
};

static const char *TOKEN_NAMES[10] = {
    "ATTR_STRING",
    "EQ",
    "GT",
    "LT",
    "LT_SLASH",
    "NAME",
    "NO_TOKEN",
    "SLASH_GT",
    "TEXT",
    "WS",
};

static const int CLASS_ACCEPTING[13] = {
    1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1,
};

static const int CLASS_DEAD[13] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
};

static const int TOKEN_OF_STATE[13] = {
    9, 6, 6, 2, 0, 8, 3, 1, 5, 4, 6, 6, 7,
};

static const int DEAD_OF_STATE[13] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
};

static const int CLASS_OF_STATE[13] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    return CLASS_OF_STATE[s];
}
