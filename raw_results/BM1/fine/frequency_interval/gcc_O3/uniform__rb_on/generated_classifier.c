#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 17

#define START_STATE 16

#define DEAD_STATE 1

#define NUM_CLASSES 17

#define NUM_TOKENS 16

static const char *CLASS_NAMES[17] = {
    "ACC::IDENT::tok=IDENT::aux=WORD::prio0::rb1",
    "DEAD",
    "ACC::WHITESPACE::tok=WS::aux=WS::prio0::rb0",
    "ACC::NUMBER::tok=INT::aux=NUMBER::prio0::rb1",
    "ACC::DELIM::tok=SEMI::aux=DELIM::prio0::rb0",
    "ACC::OPERATOR::tok=ASSIGN::aux=OP::prio0::rb1",
    "ACC::DELIM::tok=COMMA::aux=DELIM::prio0::rb0",
    "ACC::DELIM::tok=LPAR::aux=DELIM::prio0::rb0",
    "ACC::DELIM::tok=RPAR::aux=DELIM::prio0::rb0",
    "ACC::OPERATOR::tok=MINUS::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=SLASH::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=EQEQ::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=STAR::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=PLUS::aux=OP::prio0::rb0",
    "ACC::DELIM::tok=LBRACE::aux=DELIM::prio0::rb0",
    "ACC::DELIM::tok=RBRACE::aux=DELIM::prio0::rb0",
    "NONACC::START",
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
    1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    0,
};

static const int CLASS_DEAD[17] = {
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,
};

static const int TOKEN_OF_STATE[17] = {
    3, 8, 15, 4, 12, 0, 1, 6, 11, 7, 13, 2, 14, 9, 5, 10,
    8,
};

static const int DEAD_OF_STATE[17] = {
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,
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
    if (s <= 13) return 13;
    if (s <= 14) return 14;
    if (s <= 15) return 15;
    return 16;
}
