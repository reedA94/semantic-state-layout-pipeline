#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 17

#define START_STATE 14

#define DEAD_STATE 3

#define NUM_CLASSES 17

#define NUM_TOKENS 16

static const char *CLASS_NAMES[17] = {
    "ACC::NUMBER::tok=INT::aux=NUMBER::prio0::rb1",
    "ACC::DELIM::tok=SEMI::aux=DELIM::prio0::rb0",
    "ACC::OPERATOR::tok=ASSIGN::aux=OP::prio0::rb1",
    "DEAD",
    "ACC::DELIM::tok=LPAR::aux=DELIM::prio0::rb0",
    "ACC::IDENT::tok=IDENT::aux=WORD::prio0::rb1",
    "ACC::WHITESPACE::tok=WS::aux=WS::prio0::rb0",
    "ACC::OPERATOR::tok=PLUS::aux=OP::prio0::rb0",
    "ACC::DELIM::tok=RPAR::aux=DELIM::prio0::rb0",
    "ACC::DELIM::tok=COMMA::aux=DELIM::prio0::rb0",
    "ACC::OPERATOR::tok=MINUS::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=STAR::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=SLASH::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=EQEQ::aux=OP::prio0::rb0",
    "NONACC::START",
    "ACC::DELIM::tok=RBRACE::aux=DELIM::prio0::rb0",
    "ACC::DELIM::tok=LBRACE::aux=DELIM::prio0::rb0",
};

static const char *TOKEN_NAMES[16] = {
    "ASSIGN",
    "COMMA",
    "EQEQ",
    "IDENT",
    "INT",
    "LBRACE",
    "LPAR",
    "MINUS",
    "NO_TOKEN",
    "PLUS",
    "RBRACE",
    "RPAR",
    "SEMI",
    "SLASH",
    "STAR",
    "WS",
};

static const int CLASS_ACCEPTING[17] = {
    1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1,
    1,
};

static const int CLASS_DEAD[17] = {
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,
};

static const int TOKEN_OF_STATE[17] = {
    4, 12, 0, 8, 6, 3, 15, 9, 11, 1, 7, 14, 13, 2, 8, 10,
    5,
};

static const int DEAD_OF_STATE[17] = {
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,
};

static const int CLASS_OF_STATE[17] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    16,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    return CLASS_OF_STATE[s];
}
