#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 66

#define START_STATE 63

#define DEAD_STATE 30

#define NUM_CLASSES 36

#define NUM_TOKENS 31

static const char *CLASS_NAMES[36] = {
    "ACC::IDENT::tok=IDENT::aux=IDENT::prio0::rb1",
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
    "ACC::NUMBER::tok=INT::aux=NUMBER::prio0::rb1",
    "ACC::OPERATOR::tok=EQ::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=GE::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=GT::aux=OP::prio0::rb1",
    "ACC::OPERATOR::tok=LE::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=LT::aux=OP::prio0::rb1",
    "ACC::OPERATOR::tok=MINUS::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=NEQ::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=PLUS::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=SLASH::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=STAR::aux=OP::prio0::rb0",
    "ACC::PUNCT::tok=COMMA::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=DOT::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=LPAR::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=RPAR::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=SEMI::aux=PUNCT::prio0::rb0",
    "ACC::STRING::tok=STRING::aux=STRING::prio0::rb0",
    "ACC::WHITESPACE::tok=WS::aux=WS::prio0::rb0",
    "DEAD",
    "NONACC::KEYWORD_PREFIX",
    "NONACC::NUMBER_PREFIX",
    "NONACC::START",
    "NONACC::STRING_BODY",
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
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
    0, 0, 0, 0,
};

static const int CLASS_DEAD[36] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
    0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[66] = {
    6, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 3, 7, 2, 4, 5,
    18, 20, 21, 22, 24, 27, 28, 0, 1, 19, 25, 26, 29, 30, 23, 23,
    23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
    23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
    23, 23,
};

static const int DEAD_OF_STATE[66] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
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
    if (s <= 16) return 16;
    if (s <= 17) return 17;
    if (s <= 18) return 18;
    if (s <= 19) return 19;
    if (s <= 20) return 20;
    if (s <= 21) return 21;
    if (s <= 22) return 22;
    if (s <= 23) return 23;
    if (s <= 24) return 24;
    if (s <= 25) return 25;
    if (s <= 26) return 26;
    if (s <= 27) return 27;
    if (s <= 28) return 28;
    if (s <= 29) return 29;
    if (s <= 30) return 30;
    if (s <= 61) return 31;
    if (s <= 62) return 32;
    if (s <= 63) return 33;
    if (s <= 64) return 34;
    return 35;
}
