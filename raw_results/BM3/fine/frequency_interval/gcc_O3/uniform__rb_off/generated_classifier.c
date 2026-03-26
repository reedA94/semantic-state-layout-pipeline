#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 30

#define START_STATE 29

#define DEAD_STATE 1

#define NUM_CLASSES 18

#define NUM_TOKENS 13

static const char *CLASS_NAMES[18] = {
    "NONACC::STRING_BODY",
    "DEAD",
    "ACC::WHITESPACE::tok=WS::aux=WS::prio0",
    "ACC::NUMBER::tok=NUMBER::aux=NUMBER::prio0",
    "ACC::STRING::tok=STRING::aux=STRING::prio0",
    "ACC::PUNCT::tok=COMMA::aux=PUNCT::prio0",
    "NONACC::LITERAL_PREFIX",
    "ACC::PUNCT::tok=COLON::aux=PUNCT::prio0",
    "NONACC::NUMBER_PREFIX",
    "ACC::PUNCT::tok=LBRACE::aux=PUNCT::prio0",
    "ACC::PUNCT::tok=RBRACE::aux=PUNCT::prio0",
    "NONACC::STRING_ESCAPE",
    "ACC::PUNCT::tok=LBRACK::aux=PUNCT::prio0",
    "ACC::PUNCT::tok=RBRACK::aux=PUNCT::prio0",
    "ACC::LITERAL::tok=FALSE::aux=LITERAL_PREFIX::prio1",
    "ACC::LITERAL::tok=TRUE::aux=LITERAL_PREFIX::prio1",
    "ACC::LITERAL::tok=NULL::aux=LITERAL_PREFIX::prio1",
    "NONACC::START",
};

static const char *TOKEN_NAMES[13] = {
    "COLON",
    "COMMA",
    "FALSE",
    "LBRACE",
    "LBRACK",
    "NO_TOKEN",
    "NULL",
    "NUMBER",
    "RBRACE",
    "RBRACK",
    "STRING",
    "TRUE",
    "WS",
};

static const int CLASS_ACCEPTING[18] = {
    0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1,
    1, 0,
};

static const int CLASS_DEAD[18] = {
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0,
};

static const int TOKEN_OF_STATE[30] = {
    5, 5, 12, 7, 7, 7, 10, 1, 5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 0, 5, 5, 3, 8, 5, 4, 9, 2, 11, 6, 5,
};

static const int DEAD_OF_STATE[30] = {
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    if (s <= 0) return 0;
    if (s <= 1) return 1;
    if (s <= 2) return 2;
    if (s <= 5) return 3;
    if (s <= 6) return 4;
    if (s <= 7) return 5;
    if (s <= 17) return 6;
    if (s <= 18) return 7;
    if (s <= 20) return 8;
    if (s <= 21) return 9;
    if (s <= 22) return 10;
    if (s <= 23) return 11;
    if (s <= 24) return 12;
    if (s <= 25) return 13;
    if (s <= 26) return 14;
    if (s <= 27) return 15;
    if (s <= 28) return 16;
    return 17;
}
