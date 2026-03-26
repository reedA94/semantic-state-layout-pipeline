#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 66

#define START_STATE 64

#define DEAD_STATE 1

#define NUM_CLASSES 36

#define NUM_TOKENS 31

static const char *CLASS_NAMES[36] = {
    "ACC::IDENT::tok=IDENT::aux=IDENT::prio0::rb1",
    "DEAD",
    "ACC::WHITESPACE::tok=WS::aux=WS::prio0::rb0",
    "NONACC::STRING_BODY",
    "ACC::NUMBER::tok=INT::aux=NUMBER::prio0::rb1",
    "NONACC::KEYWORD_PREFIX",
    "ACC::PUNCT::tok=SEMI::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=COMMA::aux=PUNCT::prio0::rb0",
    "ACC::OPERATOR::tok=EQ::aux=OP::prio0::rb0",
    "ACC::STRING::tok=STRING::aux=STRING::prio0::rb0",
    "ACC::PUNCT::tok=LPAR::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=RPAR::aux=PUNCT::prio0::rb0",
    "ACC::OPERATOR::tok=GE::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=GT::aux=OP::prio0::rb1",
    "ACC::KEYWORD::tok=KW_AND::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_FROM::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_INSERT::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_INTO::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_OR::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_SELECT::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_SET::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_UPDATE::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_VALUES::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_WHERE::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::NUMBER::tok=FLOAT::aux=NUMBER::prio0::rb1",
    "ACC::OPERATOR::tok=LE::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=LT::aux=OP::prio0::rb1",
    "ACC::OPERATOR::tok=MINUS::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=NEQ::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=PLUS::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=SLASH::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=STAR::aux=OP::prio0::rb0",
    "ACC::PUNCT::tok=DOT::aux=PUNCT::prio0::rb0",
    "NONACC::NUMBER_PREFIX",
    "NONACC::START",
    "NONACC::STRING_ESCAPE",
};

static const char *TOKEN_NAMES[31] = {
    "COMMA",
    "DOT",
    "EQ",
    "FLOAT",
    "GE",
    "GT",
    "IDENT",
    "INT",
    "KW_AND",
    "KW_FROM",
    "KW_INSERT",
    "KW_INTO",
    "KW_OR",
    "KW_SELECT",
    "KW_SET",
    "KW_UPDATE",
    "KW_VALUES",
    "KW_WHERE",
    "LE",
    "LPAR",
    "LT",
    "MINUS",
    "NEQ",
    "NO_TOKEN",
    "PLUS",
    "RPAR",
    "SEMI",
    "SLASH",
    "STAR",
    "STRING",
    "WS",
};

static const int CLASS_ACCEPTING[36] = {
    1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0,
};

static const int CLASS_DEAD[36] = {
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[66] = {
    6, 23, 30, 23, 7, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
    23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
    23, 23, 23, 23, 26, 0, 2, 29, 19, 25, 4, 5, 8, 9, 10, 11,
    12, 13, 14, 15, 16, 17, 3, 18, 20, 21, 22, 24, 27, 28, 1, 23,
    23, 23,
};

static const int DEAD_OF_STATE[66] = {
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    if (s <= 0) return 0;
    if (s <= 1) return 1;
    if (s <= 2) return 2;
    if (s <= 3) return 3;
    if (s <= 4) return 4;
    if (s <= 35) return 5;
    if (s <= 36) return 6;
    if (s <= 37) return 7;
    if (s <= 38) return 8;
    if (s <= 39) return 9;
    if (s <= 40) return 10;
    if (s <= 41) return 11;
    if (s <= 42) return 12;
    if (s <= 43) return 13;
    if (s <= 44) return 14;
    if (s <= 45) return 15;
    if (s <= 46) return 16;
    if (s <= 47) return 17;
    if (s <= 48) return 18;
    if (s <= 49) return 19;
    if (s <= 50) return 20;
    if (s <= 51) return 21;
    if (s <= 52) return 22;
    if (s <= 53) return 23;
    if (s <= 54) return 24;
    if (s <= 55) return 25;
    if (s <= 56) return 26;
    if (s <= 57) return 27;
    if (s <= 58) return 28;
    if (s <= 59) return 29;
    if (s <= 60) return 30;
    if (s <= 61) return 31;
    if (s <= 62) return 32;
    if (s <= 63) return 33;
    if (s <= 64) return 34;
    return 35;
}
