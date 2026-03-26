#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 30

#define START_STATE 5

#define DEAD_STATE 8

#define NUM_CLASSES 18

#define NUM_TOKENS 13

static const char *CLASS_NAMES[18] = {
    "NONACC::STRING_ESCAPE",
    "NONACC::LITERAL_PREFIX",
    "ACC::LITERAL::tok=FALSE::aux=LITERAL_PREFIX::prio1",
    "NONACC::NUMBER_PREFIX",
    "NONACC::START",
    "ACC::NUMBER::tok=NUMBER::aux=NUMBER::prio0",
    "ACC::PUNCT::tok=RBRACK::aux=PUNCT::prio0",
    "DEAD",
    "ACC::PUNCT::tok=LBRACK::aux=PUNCT::prio0",
    "NONACC::STRING_BODY",
    "ACC::STRING::tok=STRING::aux=STRING::prio0",
    "ACC::PUNCT::tok=LBRACE::aux=PUNCT::prio0",
    "ACC::LITERAL::tok=NULL::aux=LITERAL_PREFIX::prio1",
    "ACC::PUNCT::tok=COMMA::aux=PUNCT::prio0",
    "ACC::WHITESPACE::tok=WS::aux=WS::prio0",
    "ACC::LITERAL::tok=TRUE::aux=LITERAL_PREFIX::prio1",
    "ACC::PUNCT::tok=RBRACE::aux=PUNCT::prio0",
    "ACC::PUNCT::tok=COLON::aux=PUNCT::prio0",
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
    0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1,
    1, 1,
};

static const int CLASS_DEAD[18] = {
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0,
};

static const int TOKEN_OF_STATE[30] = {
    5, 5, 2, 5, 5, 5, 7, 9, 5, 5, 5, 4, 5, 5, 5, 10,
    3, 7, 5, 6, 7, 1, 5, 5, 5, 12, 11, 8, 5, 0,
};

static const int DEAD_OF_STATE[30] = {
    0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static const int CLASS_OF_STATE[30] = {
    0, 1, 2, 1, 3, 4, 5, 6, 7, 1, 1, 8, 1, 9, 1, 10,
    11, 5, 1, 12, 5, 13, 1, 1, 3, 14, 15, 16, 1, 17,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    return CLASS_OF_STATE[s];
}
