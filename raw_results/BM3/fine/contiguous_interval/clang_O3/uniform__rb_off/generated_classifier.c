#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 30

#define START_STATE 27

#define DEAD_STATE 14

#define NUM_CLASSES 18

#define NUM_TOKENS 13

static const char *CLASS_NAMES[18] = {
    "ACC::LITERAL::tok=FALSE::aux=LITERAL_PREFIX::prio1",
    "ACC::LITERAL::tok=NULL::aux=LITERAL_PREFIX::prio1",
    "ACC::LITERAL::tok=TRUE::aux=LITERAL_PREFIX::prio1",
    "ACC::NUMBER::tok=NUMBER::aux=NUMBER::prio0",
    "ACC::PUNCT::tok=COLON::aux=PUNCT::prio0",
    "ACC::PUNCT::tok=COMMA::aux=PUNCT::prio0",
    "ACC::PUNCT::tok=LBRACE::aux=PUNCT::prio0",
    "ACC::PUNCT::tok=LBRACK::aux=PUNCT::prio0",
    "ACC::PUNCT::tok=RBRACE::aux=PUNCT::prio0",
    "ACC::PUNCT::tok=RBRACK::aux=PUNCT::prio0",
    "ACC::STRING::tok=STRING::aux=STRING::prio0",
    "ACC::WHITESPACE::tok=WS::aux=WS::prio0",
    "DEAD",
    "NONACC::LITERAL_PREFIX",
    "NONACC::NUMBER_PREFIX",
    "NONACC::START",
    "NONACC::STRING_BODY",
    "NONACC::STRING_ESCAPE",
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
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0,
};

static const int CLASS_DEAD[18] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
    0, 0,
};

static const int TOKEN_OF_STATE[30] = {
    2, 6, 11, 7, 7, 7, 0, 1, 3, 4, 8, 9, 10, 12, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
};

static const int DEAD_OF_STATE[30] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    if (s <= 0) return 0;
    if (s <= 1) return 1;
    if (s <= 2) return 2;
    if (s <= 5) return 3;
    if (s <= 6) return 4;
    if (s <= 7) return 5;
    if (s <= 8) return 6;
    if (s <= 9) return 7;
    if (s <= 10) return 8;
    if (s <= 11) return 9;
    if (s <= 12) return 10;
    if (s <= 13) return 11;
    if (s <= 14) return 12;
    if (s <= 24) return 13;
    if (s <= 26) return 14;
    if (s <= 27) return 15;
    if (s <= 28) return 16;
    return 17;
}
