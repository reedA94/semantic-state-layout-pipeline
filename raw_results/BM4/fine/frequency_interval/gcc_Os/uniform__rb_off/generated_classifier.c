#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 13

#define START_STATE 11

#define DEAD_STATE 2

#define NUM_CLASSES 13

#define NUM_TOKENS 10

static const char *CLASS_NAMES[13] = {
    "ACC::TEXT::tok=TEXT::aux=TEXT::prio0",
    "ACC::NAME::tok=NAME::aux=NAME::prio0",
    "DEAD",
    "NONACC::STRING_BODY",
    "ACC::TAG_PUNCT::tok=LT::aux=TAG::prio0",
    "ACC::WHITESPACE::tok=WS::aux=WS::prio0",
    "ACC::TAG_PUNCT::tok=GT::aux=TAG::prio0",
    "ACC::TAG_PUNCT::tok=LT_SLASH::aux=TAG::prio0",
    "ACC::STRING::tok=ATTR_STRING::aux=STRING::prio0",
    "ACC::TAG_PUNCT::tok=EQ::aux=TAG::prio0",
    "ACC::TAG_PUNCT::tok=SLASH_GT::aux=TAG::prio0",
    "NONACC::START",
    "NONACC::STRING_ESCAPE",
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
    1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0,
};

static const int CLASS_DEAD[13] = {
    0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[13] = {
    8, 5, 6, 6, 3, 9, 2, 4, 0, 1, 7, 6, 6,
};

static const int DEAD_OF_STATE[13] = {
    0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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
    return 12;
}
